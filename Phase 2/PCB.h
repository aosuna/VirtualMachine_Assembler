#ifndef PCB_H
#define PCB_H 

#include <vector>

using namespace std;

class PCB{
	//get registers from vm in order to store their state and contents
    static const int REG_FILE_SIZE = 4;
	vector<int> reg;
	int pc;		//program counter
    int ir;		//instruction register
    int sr;		//status register
    int sp;		//stack pointer
    int clock;	//clock counter
    int base; 	//base register
    int limit;	//limit register size of the instructions

    int CPUTime; //read and write each take 1 CPU clock tick and 27 I/O clock ticks
    int waitingTime;
    int turnAroundTime;
    int IOTime;
    int stackSize;

	friend class os;
};

#endif