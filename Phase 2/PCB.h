/* *********************** 
PCB.h
Written by Aaron Chamberlain & Adrian Osuna May 2016 at CSUSB
The PCB is the status container of running processes within the OS.
Important values are read/written to the PCB to keep track of state, including:
Process Related: pc,r[0]-r[3],sr,sp,base,limit,process name,fstreams
Accounting Info: CPU (VM), Waiting, Turnaround, I/O Time(s), & Max Stack Size
All Accounting plus VM Utilization & Throughput must appear at end of .out file
*************************/

#ifndef PCB_H
#define PCB_H 

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

class PCB{

private:    
	//get registers from vm in order to store their state and contents
    static const int REG_FILE_SIZE = 4;
	vector<int> r;
	int pc;		//program counter
    int ir;		//instruction register
    int sr;		//status register
    int sp;		//stack pointer
    int clock;	//clock counter
    int base; 	//base register
    int limit;	//limit register size of the instructions

    int CPUTime; //read and write each take 1 CPU clock tick and 28 I/O clock ticks
    int waitingTime;
    int turnAroundTime;
    int IOTime;
    int stackSize;
    int contextSwitchTime;
    int idleTime;

    ofstream writeFile;
    ifstream readFile;

    string sfile, ofile, stfile, infile, outfile; //hold the names of the filename + .s, .o, .st, .in, .out 
	
	string state;//hold the state of the process: new, running, waiting, ready, terminated

    void readPCB(); // take in sr and read in value to reg
    void writePCB(); //write to file

public:

    PCB(); //default constructor

    friend class os;
    
};

#endif