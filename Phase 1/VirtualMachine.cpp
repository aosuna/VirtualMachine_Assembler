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

void VirtualMachine::load(){
	//check if load indirect before going through function
	//look at union format2 to see if it follows format
	//else go to loadi
	clock += 1;
	pc++;
	if(instruction.f2.I == 0){
		r[instruction.f2.RD] = mem[instruction.f2.ADDR];
	}
	else{
		loadi();
	}
	
}

void VirtualMachine::loadi(){
	clock += 1;
	pc++;
	r[instruction.f3.RD] = instruction.f3.CONST;
}

void VirtualMachine::store(){
	clock += 1;
	pc++;
	mem[instruction.f2.ADDR] = r[instruction.f2.RD];
}

void VirtualMachine::add(){
	clock += 1;
	pc++;
	//need to check for overflow
	if (instruction.f1.I == 0){
		r[instruction.f1.RD] = r[instruction.f1.RD] + r[instruction.f1.RS];
	}
	else{
		addi();
	}
		
}

void VirtualMachine::addi(){
	//need to check for overflow
	clock += 1;
	pc++;
	//use format3 to add a CONST to Destination Register
	r[instruction.f3.RD] = r[instruction.f3.RD] + r[instruction.f3.CONST];
}

void VirtualMachine::addc(){
	
}

void VirtualMachine::addci(){
	
}

void VirtualMachine::sub(){
	
}

void VirtualMachine::subi(){
	
}

void VirtualMachine::subc(){
	
}

void VirtualMachine::subci(){
	
}

void VirtualMachine::and_(){
	
}

void VirtualMachine::andi(){
	
}

void VirtualMachine::xor_(){
	
}

void VirtualMachine::xori(){
	
}

void VirtualMachine::compl_(){
	
}

void VirtualMachine::shl(){
	
}

void VirtualMachine::shla(){
	
}

void VirtualMachine::shr(){
	
}

void VirtualMachine::shra(){
	
}

void VirtualMachine::compr(){
	
}

void VirtualMachine::compri(){
	
}

void VirtualMachine::getstat(){
	
}

void VirtualMachine::putstat(){
	
}

void VirtualMachine::jump(){
	
}

void VirtualMachine::jumpe(){
	
}

void VirtualMachine::jumpg(){
	
}

void VirtualMachine::call(){
	
}

void VirtualMachine::return_(){
	
}

void VirtualMachine::read(){
	
}


void VirtualMachine::write(){
	
}

void VirtualMachine::halt(){
	clock += 1;
	exit();
}

void VirtualMachine::noop(){
	clock += 1;
}
