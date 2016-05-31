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
private:
	list<PCB *> jobs; //create a list of object PCB
	queue<PCB *> readyQ, waitQ; //create a queue of object PCB
	PCB * running; //create a pointer to PCB  

	void saveToPCB();   //save status of VM to PCB
	void restoreToVM(); //send PCB to VM to execute
	void loadMemory();


public:
	void start();
	os();

};


#endif