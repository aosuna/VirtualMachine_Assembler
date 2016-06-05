#include "VirtualMachine.h"

VirtualMachine::VirtualMachine(){
		r = vector<int> (REG_FILE_SIZE); //set limit to 4 registers
		mem = vector<int> (MEM_SIZE); //set memory size to 256
		oFile =""; // read o file from assembler
        inFile = ""; //reads a .in file for read()
        outFile = "";	//output a file
		
		//initialize the status registers to 0

		pc = 0;		//program counter
		ir = 0;		//instruction register
		sr = 0;		//status register
		sp = 256;	//stack pointer
		clock = 0;	//clock counter
		base = 0; 	//base register 
		limit = 0;	//limit register
			
		//populate map instruction with the address to the class with its opcode
		OPInstruc[0] = &VirtualMachine::load;
		OPInstruc[1] = &VirtualMachine::store;
		OPInstruc[2] = &VirtualMachine::add;
		OPInstruc[3] = &VirtualMachine::addc;
		OPInstruc[4] = &VirtualMachine::sub;
		OPInstruc[5] = &VirtualMachine::subc;
		OPInstruc[6] = &VirtualMachine::and_;
		OPInstruc[7] = &VirtualMachine::xor_;
		OPInstruc[8] = &VirtualMachine::compl_;
		OPInstruc[9] = &VirtualMachine::shl;
		OPInstruc[10] = &VirtualMachine::shla;
		OPInstruc[11] = &VirtualMachine::shr;
		OPInstruc[12] = &VirtualMachine::shra;
		OPInstruc[13] = &VirtualMachine::compr;
		OPInstruc[14] = &VirtualMachine::getstat;
		OPInstruc[15] = &VirtualMachine::putstat;
		OPInstruc[16] = &VirtualMachine::jump;
		OPInstruc[17] = &VirtualMachine::jumpl;
		OPInstruc[18] = &VirtualMachine::jumpe;
		OPInstruc[19] = &VirtualMachine::jumpg;
		OPInstruc[20] = &VirtualMachine::call;
		OPInstruc[21] = &VirtualMachine::return_;
        OPInstruc[22] = &VirtualMachine::read;
		OPInstruc[23] = &VirtualMachine::write;
        OPInstruc[24] = &VirtualMachine::halt;
        OPInstruc[25] = &VirtualMachine::noop;		
}

void VirtualMachine::run(){
	isRunning = true;
	clock = 0;

	//clear vm return status;
	sr &= 0b0000011111;
	cout << "status register in run" << sr << endl;
	cout << "in vm run\n";
	cout << "infile initialize: " << inFile << "\n";
	cout << "outfile initialize: " << outFile << "\n";
	cout << "vm.limit: " << limit << "\n";
	cout << "vm.base: " << base << "\n";
	cout << "vm.pc: " << pc << "\n";

		//run program for the base and limit of each process loaded into memory
	while(isRunning){
		cout << "Process running: " << oFile << endl;
			//look at each instruction in memory
			ir = mem[pc];
			//increase program counter
			pc++;
			//set the Int value of instruction to the instruction register
			instr.i = ir;
			//compare IR to Opcode 
			(this->*OPInstruc[instr.f1.OP])();
			
			//check that number of clock ticks for executed programs is less than 15 
			//if clock ticks is 15 or more strop executions, os needs to check the sr 
			if(clock > 15){
				sr &= 0b0000011111;
				isRunning = false;
			}
	}
}

void VirtualMachine::load(){
	//check if load indirect before going through function
	//look at union format2 to see if it follows format
	//else go to loadi
	
	//check I bit
	if(instr.f2.I == 0){
		//check if loading out of range address
		if(instr.f2.ADDR + base < base || base + instr.f2.ADDR > limit){
			cout << "Address loaded is out of range." << endl;
			//set[7:5] 010 out of bound
			sr |= 0b01000000;
			isRunning = false;
		}
		else{
			clock += 4;

			//set destination register to content in memory address
			//need to add base of prog start to make sure that prog store witin proper limit
			r[instr.f2.RD] = mem[instr.f2.ADDR + base];
		}
	}
	else{
		loadi();
	}
}

void VirtualMachine::loadi(){
	//load CONST to the destination register
	clock += 1;
	//load a constant in the destination register
	
	r[instr.f3.RD] = (instr.f3.CONST);
	//if negative sign extend
		if( r[instr.f3.RD] & 0b1000000000000000 ){
		//sign extend to 32 bits to make negative
			r[instr.f3.RD] = r[instr.f3.RD] | 0b11111111111111110000000000000000;
														 	
		}
}

void VirtualMachine::store(){
	//Set the contents of destination register to location in memory
	//check if memory location out of bounds
	if(instr.f2.ADDR + base < base || instr.f2.ADDR + base > limit){
		cout << "Address store is out of range" << endl;
		//set[7:5] 010 out of bound
		sr |= 0b01000000;
		isRunning = false;
	}
	else{
	//set memory to content of destination register
	//need to add base of prog start to make sure that prog store witin proper limit
	mem[instr.f2.ADDR + base] = r[instr.f1.RD];
	clock += 4;
	}	
}

void VirtualMachine::add(){
	clock += 1;
	
	//check if I bit
	if (instr.f1.I == 0){
		//temp result
		int resultSum = 0;
		resultSum = (r[instr.f1.RD] + r[instr.f1.RS]);
		
		//check overflow for two negative numbers
		if( r[instr.f1.RD] < 0 && r[instr.f1.RS] < 0 && resultSum > 0){
			//set overflow 
			sr |= 0b10000;
		}
		//if RD, RS positive and result negative 
		else if(r[instr.f1.RD] > 0 && r[instr.f1.RS] > 0 && resultSum < 0){
			//set overflow
			sr |= 0b10000;
		}
		//set temp result to destination register
		r[instr.f1.RD] = resultSum;
		
		//check for carry
		if(r[instr.f1.RD] & 0b10000000000000000){
			sr |= 0b1;
		}
		else{
			sr &= 0b0;
		}

		//check if value is negative for, mask value to have all 1's and out put negative
		if(r[instr.f1.RD] & 0b1000000000000000){
		//sign extend to 32 bits to make negative
			r[instr.f1.RD] = r[instr.f1.RD] | 0b11111111111111110000000000000000;
		}
	}
	else{
		addi();
	}
}

void VirtualMachine::addi(){
	clock += 1;
	//temp result
	int resultSum = 0;
	resultSum = (r[instr.f3.RD] + instr.f3.CONST);
	
	//if RD, CONST negative and result positive
	if( r[instr.f3.RD] < 0 && instr.f3.CONST < 0 && resultSum > 0){
		//set overflow
		sr |= 0b10000;
	}
	//if RD, CONST positive and result negative
	if(r[instr.f3.RD] > 0 && instr.f3.CONST > 0 && resultSum < 0 ){
		//set overflow
		sr |= 0b10000;
	}
	//set destination register to temp result
	r[instr.f3.RD] = resultSum;
	
	//set carry
	if(r[instr.f3.RD] & 0b10000000000000000){
		sr |= 0b1;
	}
	else{
		sr &= 0b11110;
	}
	
	//if negative sign extend
	if( r[instr.f3.RD] & 0b1000000000000000 ){
	//sign extend to 32 bits to make negative
		r[instr.f3.RD] = r[instr.f3.RD] | 0b11111111111111110000000000000000;
	}
}

void VirtualMachine::addc(){
	clock += 1;
	
	//check I bit
	if(instr.f1.I == 0){
		//set carry = sr[0]
		int carry = sr & 0b1;
		//temp result
		int resultSum = 0;
		resultSum = (r[instr.f1.RD] + r[instr.f1.RS] + carry);
		
		//check if RD and RS are negative and if result is postive
		if(r[instr.f1.RD] < 0 && r[instr.f1.RS] < 0 && resultSum  > 0){
			//set overflow
			sr |= 0b10000;
		}
		else if(r[instr.f1.RD] >= 0 && r[instr.f1.RS] >= 0 && carry >= 0 && resultSum  < 0){
			//set for overflow
			sr |= 0b10000;
		}
		r[instr.f1.RD] = resultSum;
		
		//set carry
		if(r[instr.f1.RD] & 0b10000000000000000){
			sr |= 0b1;
		}
		else{
			sr &= 0b11110;
		}
		//if negative sign extend
		if( r[instr.f1.RD] & 0b1000000000000000 ){
		//sign extend to 32 bits to make negative
			r[instr.f1.RD] = r[instr.f1.RD] | 0b11111111111111110000000000000000;
		}
	}
	else{
		addci();
	}
}

void VirtualMachine::addci(){
	clock += 1;
	//set carry = sr[0]
	int carry = sr & 0b1;
	//temp result
	int resultSum = 0;
	resultSum = (r[instr.f3.RD] + instr.f3.CONST + carry) & 0b11111111;
	
	//if result rd is neg and const is neg result is positive set overflow
	if(r[instr.f3.RD] < 0 && instr.f3.CONST < 0 && resultSum > 0){
		//set overflow
		sr |= 0b010000;
	}
	//if rd and CONST pos and result is pos then set overflow
	else if(r[instr.f3.RD] >= 0 && instr.f3.CONST >= 0 && resultSum < 0){
		sr |= 0b010000;
	}
	
	r[instr.f3.RD] = resultSum;
	
	//set carry
	if(r[instr.f3.RD] & 0b10000000000000000){
		sr |= 0b1;
	}
	else{
		sr &= 0b11110;
	}
	
}

void VirtualMachine::sub(){
	clock += 1;
	
	if(instr.f1.I == 0){
		int resultSub = 0;
		resultSub = (r[instr.f1.RD] - r[instr.f1.RS]);
		
		//if rd and NOT(rs) are neg and result is pos then set overflow
		if(r[instr.f1.RD] < 0 && (~r[instr.f1.RS] + 1) < 0 && resultSub >= 0){
			//set overflow
			sr |= 0b010000;
		}
		//if rd and NOT(rs) are pos and result is neg set overflow
		else if(r[instr.f1.RD] >= 0 && (~r[instr.f1.RS] + 1) >= 0 && resultSub < 0){
			//set overflow
			sr |= 0b010000;
		}
		
		r[instr.f1.RD] = resultSub;
		
		//set carry
		if(r[instr.f1.RD] & 0b10000000000000000){
			sr |= 0b1;
		}
		else{
			sr &= 0b11111110;
		}
		
		//if negative sign extend
		if( r[instr.f1.RD] & 0b1000000000000000 ){
			r[instr.f1.RD] = r[instr.f1.RD] | 0b11111111111111111000000000000000;
		}
	}
	else{
		subi();
	}
}

void VirtualMachine::subi(){
	clock += 1;
	int resultSub = 0;
	resultSub = (r[instr.f3.RD] - instr.f3.CONST);
	
	//if rd and NOT(CONST) are neg and result is pos then set overflow
	if(r[instr.f3.RD] < 0 && (~instr.f3.CONST + 1) < 0 && resultSub >= 0){
		//set overflow
		sr |= 0b010000;
	}
	//if rd and NOT(CONST) are postive and result is neg then set overflow
	else if(r[instr.f3.RD] >= 0 && (~instr.f3.CONST + 1) >= 0 && resultSub < 0){
		//set overflow
		sr |= 0b010000;
	}
	
	r[instr.f3.RD] = resultSub;
	
	//set carry
	if(r[instr.f3.RD] & 0b10000000000000000){
			sr |= 0b1;
		}
		else{
			sr &= 0b0;
		}
	//if value is negative, sign extend 
	if( r[instr.f3.RD] & 0b1000000000000000 ){
		r[instr.f3.RD] = r[instr.f3.RD] | 0b11111111111111110000000000000000;
	}
}

void VirtualMachine::subc(){
	clock += 1;
	
	if(instr.f1.I == 0){
		int carry = sr & 0b1;
		int resultSub = 0;
		resultSub = (r[instr.f1.RD] - r[instr.f1.RS] - carry);
		
		//if rd and NOT(rs) are neg and result is pos then set overflow
		if(r[instr.f1.RD] < 0 && (~r[instr.f1.RS] + 1) < 0 && resultSub >= 0){
			sr |= 0b010000;
		}
		//if rd and NOT(rs) positive and result is neg then set overflow
		else if(r[instr.f1.RD] >= 0 && (~r[instr.f1.RS] + 1) >= 0 && resultSub < 0){
			sr |= 0b010000;
		}
		
		r[instr.f1.RD] = resultSub;
		//set carry
		if(r[instr.f1.RD] & 0b10000000000000000){
			sr |= 0b1;
		}
		else{
			sr &= 0b0;
		}
		//if value is negative sign extend
		if( r[instr.f3.RD] & 0b1000000000000000 ){
			r[instr.f3.RD] = r[instr.f3.RD] | 0b11111111111111110000000000000000;
		}
	}
	else{
		subci();
	}
}

void VirtualMachine::subci(){
	clock += 1;
	
	int carry = sr & 0b1;
	int resultSub = 0;
	resultSub = r[instr.f3.RD] - instr.f3.CONST - carry;
	
	//if rd and NOT(CONST) neg and result is pos then set overflow
	if(r[instr.f3.RD] < 0 && (~instr.f3.CONST + 1) < 0 && resultSub >= 0){
			sr |= 0b010000;
	}
	//if rd and NOT(CONST) pos and result is neg then set overflow 
	else if(r[instr.f3.RD] >= 0 && (~instr.f3.CONST + 1) >= 0 && resultSub < 0){
			sr |= 0b010000;
	}
	
	r[instr.f3.RD] = resultSub;
	//set carry
		if(r[instr.f1.RD] & 0b10000000000000000){
			sr |= 0b1;
		}
		else{
			sr &= 0b0;
		}
		//if value is negative sign extend
		if( r[instr.f3.RD] & 0b1000000000000000 ){
			r[instr.f3.RD] = r[instr.f3.RD] | 0b11111111111111110000000000000000;
		}
}

void VirtualMachine::and_(){
	clock += 1;
	
	if(instr.f1.I == 0){
		//and rd with rs
		r[instr.f1.RD] = r[instr.f1.RD] & r[instr.f1.RS];
	}
	else{
		andi();
	}
}

void VirtualMachine::andi(){
	clock += 1;
	//and rd with const
	r[instr.f3.RD] = r[instr.f3.RD] & instr.f3.CONST;
}

void VirtualMachine::xor_(){
	clock += 1;
	if(instr.f1.I == 0){
		//xor rd with rs
		r[instr.f1.RD] = r[instr.f1.RD] ^ r[instr.f1.RS];
	}
	else{
		xori();
	}
}

void VirtualMachine::xori(){
	clock += 1;
	//xor rd with const
	r[instr.f3.RD] = r[instr.f3.RD] ^ instr.f3.CONST;
}

void VirtualMachine::compl_(){
	clock += 1;
	//take compl of rd 
	r[instr.f1.RD] = ~r[instr.f1.RD];
}

void VirtualMachine::shl(){
	clock += 1;
	//adds zero to LSB to shift left
	//if MSB has 1 shift left then set carry
	if(r[instr.f1.RD] & 0b10000000000000000){
		sr = sr | 0b1;
		r[instr.f1.RD] = r[instr.f1.RD] << 1;
	}
	else{
		//if MSB has 0 then shift left, no carry
		r[instr.f1.RD] = r[instr.f1.RD] << 1;
	}
}

void VirtualMachine::shla(){
	clock += 1;
	//check if content in RD is less then zero 
	//sign extend after shift
	if(r[instr.f1.RD] & 0b1000000000000000){
		//set carry
		sr = sr | 0b1;
		//shift left
		r[instr.f1.RD] = r[instr.f1.RD] << 1;
		//set signed bit to 1
		r[instr.f1.RD] = r[instr.f1.RD] | 0b11111111111111111000000000000000;
		
	}
	else{
	
		//rd is positive just shift right, no sign extend
		r[instr.f1.RD] = r[instr.f1.RD] << 1;
	}
}

void VirtualMachine::shr(){
	clock += 1;
	//adds zero to MSB to shift right
	//if LSB is 1 and shift right then add carry
		
	if(r[instr.f1.RD] & 0b1){
		//set Carry Right
		sr = sr | 0b1;
	}
	if(r[instr.f1.RD] & 0b1000000000000000){
			//value is negative then set sign extend
			r[instr.f1.RD] = r[instr.f1.RD] | 0b11111111111111111000000000000000;
			r[instr.f1.RD] = r[instr.f1.RD] >> 1;
		}
	else{
		//if LSB is not 1 then shift right no carry
		r[instr.f1.RD] = r[instr.f1.RD] >> 1;
	}
}

void VirtualMachine::shra(){
	clock += 1;
	
	//check if content in RD is less then zero 
	//sign extend after shift
	if(r[instr.f1.RD] & 0b1){
		//set Carry right
		sr = sr | 0b1;
	}
	if(r[instr.f1.RD] & 0b10000000000000000){
			//value is negative then set sign extend
			r[instr.f1.RD] = r[instr.f1.RD] | 0b11111111111111111000000000000000;
			r[instr.f1.RD] = r[instr.f1.RD] >> 1;
	}
	else{
		//rd is positive just shift right, no sign extend
		r[instr.f1.RD] = r[instr.f1.RD] >> 1;
	}
}

void VirtualMachine::compr(){
	clock += 1;
	if(instr.f1.I == 0){
		//if rd < rs
		if(r[instr.f1.RD] < r[instr.f1.RS]){
			//V L E G C
		   //16 8 4 2 0
			//reset L E G bits in sr
			sr &= 0b10001;
			//set Less bit to 1
			sr |= 0b01000; 
		}
		
		//if rd == rs
		else if (r[instr.f1.RD] == r[instr.f1.RS]){
			//reset L E G bits in sr
			sr &= 0b10001;
			//set EqualTO bit to 1
			sr |= 0b00100;
		}
		//rd is greater than rs
		else{
			//reset L E G bits in rs
			sr &= 0b10001;
			//set greater than bit to 1
			sr |= 0b00010;
		}
	}
	else{
		compri();
	}
	
}

void VirtualMachine::compri(){
	clock += 1;
	//sr &= 0b10001;
	if(r[instr.f1.RD] < instr.f3.CONST){
			//V L E G C
			//4 3 2 1 0
			//reset L E G bits in sr
			sr &= 0b10001;
			//set Less bit to 1
			sr |= 0b01000; 
		}
		//if rd == rs
	else if (r[instr.f1.RD] == instr.f3.CONST){
		//clear L E G bits in sr
		//sr &= 0b10001;
		//set EqualTO bit to 1
		sr |= 0b00100;
	}
	else{
		//rd is greater than rs
		//reset L E G bits in rs
		sr &= 0b10001;
		//set greater than bit to 1
		sr |= 0b00010;
	}
}

void VirtualMachine::getstat(){
	clock += 1;
	r[instr.f1.RD] = sr;	
}

void VirtualMachine::putstat(){
	clock += 1;
	sr = r[instr.f1.RD];
}

void VirtualMachine::jump(){
	
	if(instr.f2.ADDR + base >= limit || instr.f2.ADDR + base < base){
		cout << "Jump out of range." << endl;
		//set[7:5] 010 out of bound
		sr |= 0b01000000;
		isRunning = false;
	}
	else{
		clock += 1;
		pc = instr.f2.ADDR + base;
	}
}

void VirtualMachine::jumpl(){
	if(instr.f2.ADDR + base > limit || instr.f2.ADDR + base < base){
		cout << "Jumpl out of range." << endl;
		//set[7:5] 010 out of bound
		sr |= 0b01000000;
		isRunning = false;
	}
	else{
		clock += 1;
		//check less than bit
		if(sr & 0b1000){
			pc = instr.f2.ADDR + base;
		}
	}
}

void VirtualMachine::jumpe(){
	if(instr.f2.ADDR + base > limit || instr.f2.ADDR + base < base){
		cout << "Jumpe out of range." << endl;
		//set[7:5] 010 out of bound
		sr |= 0b01000000;
		isRunning = false;
	}
	else{
		clock += 1;
		//check equal to bit
		if(sr & 0b100){
			pc = instr.f2.ADDR + base;
		}
	}
}

void VirtualMachine::jumpg(){
	if(instr.f2.ADDR + base > limit || instr.f2.ADDR + base < base){
		cout << "Jumpg out of range." << endl;
		//set[7:5] 010 out of bound
		sr |= 0b01000000;
		isRunning = false;
	}
	else{
		clock += 1;
		//check greater than bit
		if(sr & 0b10){
			pc = instr.f2.ADDR + base;
		}
	}
}

void VirtualMachine::call(){
	clock += 4;
	cout << "--------- VM CALL ---------\n";
	if(sp < limit + 6){
		cout << "Stack Overflow" << endl;
		//set[7:5] 010 stack overflow
		sr |= 0b01100000;
		isRunning = false;
	}
	
	mem[--sp] = sr;
	mem[--sp] = r[0];
	mem[--sp] = r[1];
	mem[--sp] = r[2];
	mem[--sp] = r[3];
	mem[--sp] = pc;

	cout << "storing values into stack: \n";
	for (int i  = sp; i < 256; i++){
		cout << "	" << mem[sp] << endl;
	}

	
	pc = instr.f2.ADDR + base;
}

void VirtualMachine::return_(){
	//under flow if trying to pop status from mem[251]
	//if sp is at location 251 there is nothing to pop at location 256 or beyond 
	if(sp >= 256 ){
		cout << "Stack Underflow" << endl;
		sr |= 0b10000000;
		isRunning = false;
	}
	
	clock += 4;
	
	pc = mem[sp++];
	r[3] = mem[sp++];
	r[2] = mem[sp++];
	r[1] = mem[sp++];
	r[0] = mem[sp++];
	sr = mem[sp++];
}

void VirtualMachine::read(){
	cout << "***************** VM Read was called ******************\n";
	int tempsr = sr | 0b11000000; // mask the status register to 8 bits
	int tempReg = instr.f1.RD; // set temp to register wanting to write out
	
	cout << "register: " << instr.f1.RD << endl;
	tempReg = tempReg << 8;       // shift left to bits [9:8]
	sr = tempReg | 0b11000000 | tempsr;		  // put values together
	//sr &= 0b1111011111;			  // add VM Return status to bits [7:5]
	
	cout << "		sr in vm is: " << sr << '\n';

	isRunning = false;
}

void VirtualMachine::write(){
	cout << "***************** VM Write was called ******************\n";
	
	int tempsr = sr | 0b11100000; // mask the status register to 8 bits
	int tempReg = instr.f1.RD; // set temp to register wanting to write out
	tempReg = tempReg << 8;       // shift left to bits [9:8]
	sr = tempsr	| tempReg | 0b11100000;		  // put values together
	//sr &= 0b1111111111;			  // add VM Return status to bits [7:5]
	
	cout << "		sr in vm is: " << sr << '\n';

	isRunning = false;
}

void VirtualMachine::halt(){
	clock += 1;

	//set[7:5] 001 halt
	sr |= 0b00100000;
	
	isRunning = false;
}

void VirtualMachine::noop(){
	clock += 1;
}

/*int main(){
	VirtualMachine vm;
	
	string file;
	cout << "Enter file: ";
	cin >> file;
	
	vm.run(file);
}
*/