#ifndef OS_H
#define OS_H 

#include <queue>
#include <list>
#include <iterator>
#include "VirtualMachine.h"
#include "Assembler.h"
#include "PCB.h"

using namespace std;

class os{

	Assembler as;
	VirtualMachine vm;
	PCB pcb;
private:
	list<PCB *> jobs; //create a list of object PCB
	queue<PCB *> readyQ, waitQ; //create a queue of object PCB
	PCB * running; //create a pointer to PCB  
	int OSContextSwitchTime; //total context switch time for all processes
	int OSOperatingTime;

	void saveToPCB();   	//save status of VM to PCB
	void restoreToVM(); 	//send PCB to VM to execute
	void VMReturnStatus(); 	//function to handle the return status code for VM
	void closePCBFiles();	//close all open files
	//void readPCB();

public:
	void start();
	os();

};


#endif