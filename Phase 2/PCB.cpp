#include "PCB.h"

using namespace std;

PCB::PCB(){ 
//default constructer, need to initialize values when a new PCB is created
	reg = vector<int> (REG_FILE_SIZE);
	pc = 0;
	limit = 0;
	base = 0;
	sp = 256;
	state = "new"; //all new PCB states get sent to readyQ
}

