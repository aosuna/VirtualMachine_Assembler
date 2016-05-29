#ifndef OS_H
#define OS_H 

#include <queue>
#include <list>
#include "VirtualMachine.h"
#include "Assembler.h"
#include "PCB.h"

using namespace std;

class os{

	Assembler as;
	VirtualMachine vm;
private:
	list<PCB *> jobs;
	queue<PCB *> readQ, waitQ;
	PCB * running;

	void returnVM();	//send PCB to OS
	void sendVM();		//send PCB to Virtual Machine
	void errStatus();	//return error status


public:
	void start();
	os();

};


#endif