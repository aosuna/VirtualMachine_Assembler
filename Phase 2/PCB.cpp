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
	
	state = "new"; //all new PCB states get sent to readyQ
}

