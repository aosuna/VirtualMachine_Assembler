#ifndef OS_H
#define OS_H 
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <list>
#include <string>
#include <sstream>
#include <cstdlib>
#include "VirtualMachine.h"
#include "Assembler.h"
#include "PCB.h"

using namespace std;

class os{

	Assembler as;
	VirtualMachine vm;

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