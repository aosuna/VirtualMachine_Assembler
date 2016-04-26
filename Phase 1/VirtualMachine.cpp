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
	
	checkCarry = r[instr.f1.RD] & 0b10000000000000000;
	checkCarry = checkCarry >> 16;
	
	//check carry bit in sr first
	if(sr & 0b1){
		//if sr[carry] is 1 then set to 0
		if(checkCarry == 1){
		sr &= 0b11110;
		}
	}
	else{
		if(checkCarry == 1)
		//if sr[carry] is 0 then add 1
		sr &= 0b11111;
	}
}

void VirtualMachine::run(string fileName){
	
	pc = 0;		//program counter
    ir = 0;		//instruction register
    sr = 0;		//status register
	sp = 256;	//stack pointer
    clock = 0;	//clock counter
    base = 0; 	//base register 
    limit = 0;	//limit register
	
	//cout << "Enter file name for VM to compile: "
	fileName = fileName + ".o";
	//cout << "File sent to VM run() " << fileName << endl;
	ifstream oFile ( fileName.c_str() );
	
	if ( oFile.is_open() ){
		string line;
		int limitSize = 0;
		int tempOP;
		
		while (!oFile.eof()) {
			//get line convert from string to int and store in memory location
			getline(oFile, line);
			//convert string to int by using stringstream and pass value to tempOP
			stringstream op(line);
			op >> tempOP;
			//load file line into memory
			mem[limitSize] = tempOP;
			//increase to next memory location
			limitSize++;

		}
		oFile.close();
		//set size of limit to the total size of the program
		limit = limitSize;
		while(pc <= limit){
			//look at each instruction in memory
			ir = mem[pc];
			//increase program counter
			pc++;
			//set the Int value of instruction to the instruction register
			instr.i = ir;
			//compare IR to Opcode 
			(this->*OPInstruc[instr.f1.OP])();
		}
	} 
	else{
		cout << oFile << " failed to open." << endl;
		exit(1);
	}
	
	//print clock
	//out used for printing clock under same fileName
	string out = "o.out";
	ofstream outFile (out.c_str());
	if(outFile.is_open()){
		outFile << "clock: " << clock << endl;
		outFile.close();
	}
	else{
		cout << outFile << " failed to open." << endl;
	}
	
}

void VirtualMachine::load(){
	//check if load indirect before going through function
	//look at union format2 to see if it follows format
	//else go to loadi
	clock += 4;
	
	//check I bit
	if(instr.f2.I == 0){
		//check if loading out of range address
		if(instr.f2.ADDR < base || instr.f2.ADDR > limit){
			cout << "Address loaded is out of range." << endl;
			exit(1);
		}
		else{
			//set destination register to content in memory address
			r[instr.f2.RD] = mem[instr.f2.ADDR];
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
	r[instr.f3.RD] = instr.f3.CONST;
}

void VirtualMachine::store(){
	//Set the contents of destination register to location in memory
	
	//check if memory location out of bounds
	if(instr.f2.ADDR < base || instr.f2.ADDR > limit){
		cout << "Address store is out of range" << endl;
	}
	//set memory to content of destination register
	mem[instr.f2.ADDR] = r[instr.f2.RD];
	clock += 4;
}

void VirtualMachine::add(){
	clock += 1;
	
	//check if I bit
	if (instr.f1.I == 0){
		//temp result
		int resultSum = 0;
		resultSum = r[instr.f1.RD] + r[instr.f1.RS];
		
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
		
		//check bit if bit 17 is 1
		setCarry();
	}
	else{
		addi();
	}
}

void VirtualMachine::addi(){
	clock += 1;
	//temp result
	int resultSum = 0;
	resultSum = r[instr.f3.RD] + instr.f3.CONST;
	
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
	setCarry();
}

void VirtualMachine::addc(){
	clock += 1;
	
	//check I bit
	if(instr.f1.I == 0){
		//set carry = sr[0]
		int carry = sr & 0b1;
		//temp result
		int resultSum = 0;
		resultSum = r[instr.f1.RD] + r[instr.f1.RS] + carry;
		
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
		setCarry();
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
	resultSum = r[instr.f3.RD] + instr.f3.CONST + carry;
	
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
	setCarry();
}

void VirtualMachine::sub(){
	clock += 1;
	
	if(instr.f1.I == 0){
		int resultSub = 0;
		resultSub = r[instr.f1.RD] - r[instr.f1.RS];
		
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
		setCarry();
	}
	else{
		subi();
	}
}

void VirtualMachine::subi(){
	clock += 1;
	int resultSub = 0;
	resultSub = r[instr.f3.RD] - instr.f3.CONST;
	
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
	setCarry();
}

void VirtualMachine::subc(){
	clock += 1;
	
	if(instr.f1.I == 0){
		int carry = sr & 0b1;
		int resultSub = 0;
		resultSub = r[instr.f1.RD] - r[instr.f1.RS] - carry;
		
		//if rd and NOT(rs) are neg and result is pos then set overflow
		if(r[instr.f1.RD] < 0 && (~r[instr.f1.RS] + 1) < 0 && resultSub >= 0){
			sr |= 0b010000;
		}
		//if rd and NOT(rs) positive and result is neg then set overflow
		else if(r[instr.f1.RD] >= 0 && (~r[instr.f1.RS] + 1) >= 0 && resultSub < 0){
			sr |= 0b010000;
		}
		
		r[instr.f1.RD] = resultSub;
		setCarry();
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
	setCarry();
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
	r[instr.f1.RD] = r[instr.f1.RD] << 1;
	setCarry();
}

void VirtualMachine::shla(){
	clock += 1;
	//check if content in RD is less then zero 
	//sign extend after shift
	if(r[instr.f1.RD] & 0b1000000000000000){
		//shift left
		r[instr.f1.RD] = r[instr.f1.RD] << 1;
		setCarry();
		//set signed bit to 1
		r[instr.f1.RD] |= 0b1000000000000000;
	}
	else{
		//rd is positive just shift right, no sign extend
		r[instr.f1.RD] = r[instr.f1.RD] << 1;
	}
}

void VirtualMachine::shr(){
	clock += 1;
	//adds zero to MSB to shift right
	r[instr.f1.RD] = r[instr.f1.RD] >> 1;
	setCarry();
}

void VirtualMachine::shra(){
	clock += 1;
	
	//check if content in RD is less then zero 
	//sign extend after shift
	if(r[instr.f1.RD] & 0b1){
		setCarry();
	}
	if(r[instr.f1.RD] & 0b1000000000000000){
		//shift left
		r[instr.f1.RD] = r[instr.f1.RD] >> 1;
		//set signed bit to 1
		r[instr.f1.RD] |= 0b1000000000000000;
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
			//4 3 2 1 0
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
		sr &= 0b10001;
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
	
	if(instr.f2.ADDR >= limit || instr.f2.ADDR < base){
		cout << "Jump out of range." << endl;
		exit(2);
	}
	else{
		clock += 1;
		pc = instr.f2.ADDR;
	}
}

void VirtualMachine::jumpl(){
	if(instr.f2.ADDR > limit || instr.f2.ADDR < base){
		cout << "Jumpl out of range." << endl;
		exit(3);
	}
	else{
		clock += 1;
		//check less than bit
		if(sr & 0b1000){
			pc = instr.f2.ADDR;
		}
	}
}

void VirtualMachine::jumpe(){
	if(instr.f2.ADDR > limit || instr.f2.ADDR < base){
		cout << "Jumpe out of range." << endl;
		exit(4);
	}
	else{
		clock += 1;
		//check equal to bit
		if(sr & 0b100){
			pc = instr.f2.ADDR;
		}
	}
}

void VirtualMachine::jumpg(){
	if(instr.f2.ADDR > limit || instr.f2.ADDR < base){
		cout << "Jumpg out of range." << endl;
		exit(5);
	}
	else{
		clock += 1;
		//check greater than bit
		if(sr & 0b10){
			pc = instr.f2.ADDR;
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
	
	pc = instr.f2.ADDR;
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
	cout << "please enter file name: ";
	cin >> inFile;
	//ifstream oFile ( fileName.c_str() );
	inFile = inFile + ".in";
	ifstream oFile(inFile.c_str());
	//oFile.open();
	
	if (oFile.is_open()) {
		string line;
		int input;
	
		getline(oFile, line);
		istringstream in(line.c_str());
		in >> input;
		r[instr.f1.RD] = input;
		oFile.close();
	}
	else{
		cout << oFile << " failed to open." << endl;
		exit(7);
	}
}

void VirtualMachine::write(){
	clock += 28;
	
	ofstream outFile;
	string out;

	cout << "Enter file to to print to: ";
	cin >> out;
	
	out = out + ".out";
	
	outFile.open( out.c_str() );

	outFile << r[instr.f1.RD] << endl;
	outFile.close();
}

void VirtualMachine::halt(){
	clock += 1;
	exit(8);
}

void VirtualMachine::noop(){
	clock += 1;
}
