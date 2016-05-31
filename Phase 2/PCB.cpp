#include "PCB.h"

using namespace std;

PCB::PCB(){ 
//default constructer, need to initialize values when a new PCB is created
	r = vector<int> (REG_FILE_SIZE);
	pc = 0;
	limit = 0;
	base = 0;
	sp = 256;
	clock = 0;
	r[0] = 0;
	r[1] = 0;
	r[2] = 0;
	r[3] = 0;
	CPUTime = 0; 		//read and write each take 1 CPU clock tick and 27 I/O clock ticks
    waitingTime = 0; 	//time spent in readyQ
    turnAroundTime = 0;	//
    IOTime = 0;			//
    stackSize = 0;
    contextSwitchTime = 0;
    idleTime = 0;
	
	state = "new"; //all new PCB states get sent to readyQ
}

void PCB::readPCB(){
	int input;
	int rd =  rd & 0b11; //mask destination register
	int tempReg = tempReg & 0b1111111111; // mask temp reg
	tempReg = sr; 	//set temp value to get destination register
	rd = tempReg >> 8; // shift right to get the destination register
	
	readFile(infile.c_str());

	if (readFile.is_open()) {
		
		readin >> input; //casting value in .in file to type int
		r[rd] = input; //set value from .in to destination register

		//readFile.close();
	}
	else{
		cout << infile << " failed to open." << endl;
		exit(1);
	}
}

void PCB::writePCB(){
	int rd =  rd & 0b11; //mask destination register
	int tempReg = tempReg & 0b1111111111; // mask temp reg
	tempReg = sr; 	//set temp value to get destination register
	rd = tempReg >> 8; // shift right to get the destination register
	
	writeFile(outfile.c_str(), ios::app);

	if (writeFile.is_open()) {
		string line; 
		
		writeFile << r[rd] << endl;

		writeFile.close();
	}
	else{
		cout << infile << " failed to open." << endl;
		exit(1);
	}
}

