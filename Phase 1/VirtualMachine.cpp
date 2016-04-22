#include "VirtualMachine.h"

VirtualMachine::VirtualMachine(){
		r = vector<int> (REG_FILE_SIZE); //set limit to 4 registers
		mem = vector<int> (MEM_SIZE); //set memory size to 256
			
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

void VirtualMachine::setCarry(){
	int checkCarry = 0;
	
	checkCarry = r[instruction.f1.RD] & 0b10000000000000000;
	checkCarry = checkCarry >> 16;
		
	//set carry bit
	if(checkCarry == 1){
	//if carry bit is 1 them remove
		sr -= 1;
	}
	else{
	//if carry is 0 then add 1
		sr += 1;
	}
}

void VirtualMachine::run(string fileName){
	
	pc = 0;		//program counter
    ir = 0;		//instruction register
    sr = 0;		//status register
	sp = 256;		//stack pointer
    clock = 0;	//clock counter
    base = 0; 	//base register 
    limit = 0;	//limit register
	
	fstream oFile;
	oFile.open(fileName.c_str(), ios::in);
	
	if (!oFile.is_open()) {
		cout << oFile << " failed to open." << endl;
		exit(1);
	} 
	string line;
	getline(oFile, line);
	int limitSize = 0;
	
	while (!oFile.eof()) {
	//get line convert from string to int and store in memory location
		mem[limitSize++] = stoi(line.c_str());
		getline(oFile, line);
	}
	//set size of limit to the total size of the program
	limit = limitSize;
	while(pc <= limit){
		//look at each instruction in memory
		ir = mem[pc++];
		//set the Int value of instruction to the instruction register
		instruction.i = ir;
		//compare IR to Opcode 
		if (this->*OPInstruc[instruction.f1.OP])();
	}
	
	
}

void VirtualMachine::load(){
	//check if load indirect before going through function
	//look at union format2 to see if it follows format
	//else go to loadi
	clock += 4;
	
	if(instruction.f2.I == 0){
		if(instruction.f2.ADDR < base || instruction.f2.ADDR > limit){
			cout << "Address loaded is out of range." << endl;
			exit(1);
		}else{
			r[instruction.f2.RD] = mem[instruction.f2.ADDR];
		}
	}
	else{
		loadi();
	}
	
}

void VirtualMachine::loadi(){
	//load CONST to the destination register
	clock += 1;
	
	r[instruction.f3.RD] = instruction.f3.CONST;
}

void VirtualMachine::store(){
	//Set the contents of destination register to location in memory
	
	if(instruction.f2.ADDR < base || instruction.f2.ADDR > limit){
		cout << "Address store is out of range" << endl;
	}
	mem[instruction.f2.ADDR] = r[instruction.f2.RD];
	clock += 4;
}

void VirtualMachine::add(){
	clock += 1;
	
	int resultSum = 0;
	resultSum = r[instruction.f1.RD] + r[instruction.f1.RS];
	if (instruction.f1.I == 0){
		//check overflow for two negative numbers
		if( r[instruction.f1.RD] < 0 && r[instruction.f1.RS] < 0 && resultSum > 0){
			sr |= 0b010000;
		}
		else if(r[instruction.f1.RD] > 0 && r[instruction.f1.RS] > 0 && resultSum > 0){
			sr |= 0b010000;
		}
		r[instruction.f1.RD] = resultSum;
		
		//check bit if bit 17 is 1
		setCarry();
	}
	else{
		addi();
	}
}

void VirtualMachine::addi(){
	clock += 1;
	
	//use format3 to add a CONST to Destination Register
	resultSum = r[instruction.f3.RD] + instruction.f3.CONST;
	if( r[instruction.f3.RD] < 0 && instruction.f3.CONST < 0 && resultSum > 0){
		//set overflow
		sr |= 0b010000;
	}
	if(r[instruction.f3.RD > 0 && instruction.f3.CONST > 0 && resultSum < 0 ){
		sr |= 0b010000;
	}
	r[instruction.f3.RD] = resultSum;
	setCarry();
}

void VirtualMachine::addc(){
	clock += 1;
	
	if(instruction.f1.I == 0){
		int carry = sr & 0b1;
		int resultSum = 0;
		resultSum = r[instruction.f1.RD] + r[instruction.f1.RS] + carry;
		if(r[instruction.f1.RD] < 0 && r[instruction.f1.RS] < 0 && resultSum  > 0){
			sr |= 0b010000;
		}
		else if(r[instruction.f1.RD] > 0= && r[instruction.f1.RS] >= 0 && carry >= 0 && resultSum  < 0){
			sr |= 0b010000;
		}
		r[instruction.f1.RD] = resultSum;
		setCarry();
	}
	else{
		addci();
	}
}

void VirtualMachine::addci(){
	clock += 1;
	int carry = sr & 0b1;
	int resultSum = 0;
	resultSum = r[instruction.f3.RD] + instruction.f3.CONST + carry;
	
	if(r[instruction.f3.RD] < 0 && instruction.f3.CONST < 0 && resultSum > 0){
		sr |= 0b010000;
	}
	else if(r[instruction.f3.RD] >= 0 && instruction.f3.CONST >= 0 && resultSum > 0){
		sr |= 0b010000;
	}
	
	r[instruction.f3.RD] = resultSum;
	checkCarry();
}

void VirtualMachine::sub(){
	clock += 1;
	
	if(instruction.f1.I == 0){
		int resultSub = 0;
		resultSub = r[instruction.f1.RD] - r[instruction.f1.RS];
		//overflow detection
		if(r[instruction.f1.RD] < 0 && (~r[instruction.f1.RS] + 1) < 0 && resultSub >= 0){
			sr |= 0b010000;
		}
		else if(r[instruction.f1.RD] >= 0 && (~r[instruction.f1.RS] + 1) >= 0 && resultSub < 0){
			sr |= 0b010000;
		}
		
		r[instruction.f1.RD] = resultSub;
		checkCarry();
	}
	else{
		subi();
	}
}

void VirtualMachine::subi(){
	clock += 1;
	int resultSub = r[instruction.f3.RD] - instruction.f3.CONST;
		//overflow detection
	if(r[instruction.f3.RD] < 0 && (~instruction.f3.CONST + 1) < 0 && resultSub >= 0){
		sr |= 0b010000;
	}
	else if(r[instruction.f3.RD] >= 0 && (~instruction.f3.CONST + 1) >= 0 && resultSub < 0){
		sr |= 0b010000;
	}
	
}

void VirtualMachine::subc(){
	clock += 1;
	if(instruction.f1.I == 0){
		int carry = sr & 0b1;
		int resultSub = 0;
		resultSub = r[instruction.f1.RD] - r[instruction.f1.RS] - carry;
		if(r[instruction.f1.RD] < 0 && (~r[instruction.f1.RS] + 1) < 0 && resultSub >= 0){
			sr |= 0b010000;
		}
		else if(r[instruction.f1.RD] >= 0 && (~r[instruction.f1.RS] + 1) >= 0 && resultSub < 0){
			sr |= 0b010000;
		}
		
		r[instruction.f1.RD] = resultSub;
		checkCarry();
	}
	else{
		subci();
	}
}

void VirtualMachine::subci(){
	clock += 1;
	
	int carry = sr & 0b1;
	int resultSub = 0;
	resultSub = r[instruction.f3.RD] - instruction.f3.CONST - carry;
	
	if(r[instruction.f3.RD] < 0 && (~instruction.f3.CONST + 1) < 0 && resultSub >= 0){
			sr |= 0b010000;
	}
	else if(r[instruction.f3.RD] >= 0 && (~instruction.f3.CONST + 1) >= 0 && resultSub < 0){
			sr |= 0b010000;
	}
	
	r[instruction.f3.RD] = resultSub;
	checkCarry();
}

void VirtualMachine::and_(){
	clock += 1;
	
	if(instruction.f1.I == 0){
		r[instruction.f1.RD] = r[instruction.f1.RD] & r[instruction.f1.RS];
	}
	else{
		andi();
	}
}

void VirtualMachine::andi(){
	clock += 1;
	
	r[instruction.f3.RD] = r[instruction.f3.RD] & instruction.f3.CONST;
}

void VirtualMachine::xor_(){
	clock += 1;
	if(instruction.f1.I == 0){
		r[instruction.f1.RD] = r[instruction.f1.RD] ^ r[instruction.f2.RS];
	}
	else{
		xori();
	}
}

void VirtualMachine::xori(){
	clock += 1;
	r[instruction.f3.RD] = r[instruction.f3.RD] ^ instruction.f3.CONST;
}

void VirtualMachine::compl_(){
	clock += 1;
	r[instruction.f1.RD] = ~r[instruction.f1.RD];
}

void VirtualMachine::shl(){
	clock += 1;
	
	r[instruction.f1.RD] = r[instruction.f1.RD] << 1;
	checkCarry();
}

void VirtualMachine::shla(){
	clock += 1;
	
	
}

void VirtualMachine::shr(){
	clock += 1;
	r[instruction.f1.RD] = r[instruction.f1.RD >> 1;
	checkCarry();
}

void VirtualMachine::shra(){
	clock += 1;
}

void VirtualMachine::compr(){
	clock += 1;
	
}

void VirtualMachine::compri(){
	clock += 1;
}

void VirtualMachine::getstat(){
	clock += 1;
	r[instruction.f1.RD] = sr;
	
}

void VirtualMachine::putstat(){
	clock += 1;
	sr = r[instruction.f1.RD];
}

void VirtualMachine::jump(){
	
	if(instruction.f2.ADDR >= limit || instruction.f2.ADDR < base){
		cout << "Jump out of range." << endl;
		exit(2);
	}
	else{
		clock += 1;
		pc = instruction.f2.ADDR;
	}
}

void VirtualMachine::jumpl(){
	if(instruction.f2.ADDR > limit || instruction.f2.ADDR < base){
		cout << "Jumpl out of range." << endl;
		exit(3);
	}
	else{
		clock += 1;
		if(sr & 0b001000){
			pc = instruction.f2.ADDR;
		}
	}
}

void VirtualMachine::jumpe(){
	if(instruction.f2.ADDR > limit || instruction.f2.ADDR < base){
		cout << "Jumpe out of range." << endl;
		exit(4);
	}
	else{
		clock += 1;
		if(sr & 0b0100){
			pc = instruction.f2.ADDR;
	}
}

void VirtualMachine::jumpg(){
	if(instruction.f2.ADDR > limit || instruction.f2.ADDR < base){
		cout << "Jumpg out of range." << endl;
		exit(5);
	}
	else{
		clock += 1;
		if(sr & 0b010){
			pc = instruction.f2.ADDR;
		}
	}
}

void VirtualMachine::call(){
	clock += 4;
	if(sp < limit + 6){
		cout << "Stack Overflow" << endl;
		exit(6);
	}
	
	mem[--sp] = sr;
	mem[--sp] = r[0];
	mem[--sp] = r[1];
	mem[--sp] = r[2];
	mem[--sp] = r[3];
	mem[--sp] = pc;
	
	pc = instruction.f2.ADDR;
}

void VirtualMachine::return_(){
	clock += 4;
	
	pc = mem[sp++];
	r[3] = mem[sp++];
	r[2] = mem[sp++];
	r[1] = mem[sp++];
	r[0] = mem[sp++];
	sr = mem[sp++];
}

void VirtualMachine::read(){
	clock += 28;
	
	fstream inFile;
	inFile.open(fileName.c_str(), ios::in);
	
	if (!inFile.is_open()) {
		cout << inFile << " failed to open." << endl;
		exit(7);
	}
	
	string line;
	getline(inFile, line);
	inFile.close();
	
	int input = stoi(line.c_str());
	r[instruction.f1.RD] = input;
}

void VirtualMachine::write(){
	clock += 28;
}

void VirtualMachine::halt(){
	clock += 1;
	exit();
}

void VirtualMachine::noop(){
	clock += 1;
}
