#include "PCB.h"

using namespace std;

PCB::PCB(){ 
//default constructer, need to initialize values when a new PCB is created
	r = vector<int> (REG_FILE_SIZE);
	pc = 0;
	limit = 0;
	base = 0;
	sr = 0;
	sp = 256;
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
	interruptTime = 0;
	
	state = "new"; //all new PCB states get sent to readyQ
}

void PCB::readPCB(){ //handles read in VM, save value from file to register destination
	cout << "************************** READPCB ****************************************\n";

	int tempReg = sr; // mask temp reg
	int trd = tempReg >> 8; // shift right to get the destination register
	
	if(readFile.is_open()){
		//stringstream convert(infile);	
		readFile >> r[trd];
	}else {
		cout << infile << " failed to open\n";
		exit(1);
	}
	cout << "value stored to register: " << trd << endl;
	cout << "value in register is: " << r[trd] << endl;
}

void PCB::writePCB(){ //handles read in VM, write value from file to register destination
cout << "************************** WRITEPCB ****************************************\n"; 
cout << "write PCB called: \n";
cout << "write to file: " << outfile <<  "\n";

	int tempReg = sr; // mask temp reg
	int trd = tempReg >> 8; // shift right to get the destination register

	if (writeFile.is_open()) {
		//string line; 
		writeFile << r[trd] << endl;
	}
	else{
		cout << outfile << " failed to open." << endl;
		exit(1);
	}
}

