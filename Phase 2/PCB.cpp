#include "PCB.h"

using namespace std;

PCB::PCB(){
	reg = vector<int> (REG_FILE_SIZE);
	pc = 0;
	limit = 0;
	base = 0;
	sp = 256;
}

