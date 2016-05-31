#include "os.h"

using namespace std;

os::os(){
	OSContextSwitchTime = 0;
	OSOperatingTime = 0;
	//default constructor
}

void os::VMReturnStatus(){
	int tempSR = vm.sr;
	tempSR = tempSR >> 5;
	tempSR &= 0b111;

	switch(tempSR){
			case 0:
			saveToPCB();
			running->state = "ready";
			readyQ.push(running);
			cout << "time slice" << endl;
			break;
		case 1:
			running->state = "terminated";
			running->clock = running->clock + vm.clock;
			//vm.clock = running->clock; //used for testing output of clock in vm.
			//vm.writeClock();
			cout << "halt was called"  << endl;
			break;
		case 2:
			running->clock = running->clock + vm.clock;
			running->state = "terminated";
			cout << "Out of bound called" << endl;
			break;
		case 3:
			running->state = "terminated";
			cout << "Stack Overflow" << endl;
			break;
		case 4:
			running->state = "terminated";
			cout << "Stack Underflow" << endl;
			break;
		case 5:
			running->state = "terminated";
			cout << "Invalid OpCode" << endl;
			break;
		case 6:
			running->state = "waiting";
			running->IOTime = vm.clock + 28;
			saveToPCB();
			waitQ.push(running);
			running->CPUTime = running->CPUTime + 1;
			running->IOTime = running_IOTime + 28;
			PCB.readPCB();
			cout << "Read Operation" << endl;
			break;
		case 7:
			running->state = "waiting";
			running->IOTime = vm.clock + 28;
			saveToPCB();
			waitQ.push(running);
			running->CPUTime = running->CPUTime + 1;
			running->IOTime = running_IOTime + 28;
			PCB.writePCB();
			cout << "Write Operation" << endl;
			break;
		default:
			cout << "Went to default" << endl;
		break;
	}

}

void os::saveToPCB(){
	cout << "********** Will save register to PCB **************" << endl;
	cout << "process save state is: " << running->sfile << endl;

	cout << "vm.pc: "<< vm.pc << endl;
	cout << "vm.clock: "<< vm.clock << endl;
	cout << "vm.r[0]: "<< vm.r[0] << endl;
	cout << "vm.r[1]: "<< vm.r[1] << endl;
	cout << "vm.r[2]: "<< vm.r[2] << endl;
	cout << "vm.r[3]: "<< vm.r[3] << endl;
	cout << "vm.sr: "<< vm.sr << endl;

	running->pc = vm.pc;
	running->clock = running->clock + vm.clock; //cpu time
	running->r[0] = vm.r[0];
	running->r[1] = vm.r[1];
	running->r[2] = vm.r[2];
	running->r[3] = vm.r[3];
	running->sr = vm.sr;
}

void os::restoreToVM(){

	cout << "********** Restoring values from PCB to VM **************" << endl;
	cout << "process return name is: " << running->sfile << endl;

	cout << "running base PCB: " << running->base << endl;
	cout << "running limit PCB: "<< vm.limit << endl;
	cout << "running pc PCB: " << running->pc << endl;
	cout << "current running clock PCB: " << running->clock << endl;
	cout << "running ofile PCB: " << running->ofile << endl;
	cout << "running infile PCB: " << running->infile << endl;
	cout << "running outfile PCB: " << running->outfile << endl;
	cout << "running r[0] PCB: " << running->r[0] << endl;
	cout << "running r[1] PCB: " << running->r[1] << endl;
	cout << "running r[2] PCB: " << running->r[2] << endl;
	cout << "running r[3] PCB: " << running->r[3] << endl;
	cout << "running sr PCB: " << running->sr << endl;


	vm.base = running->base;
	vm.limit = running->limit;
	vm.pc = running->pc;
	vm.clock = running->clock;
	vm.oFile = running->ofile;
	vm.inFile = running->infile;
	vm.outFile = running->outfile;
	vm.r[0] = running->r[0];
	vm.r[1] = running->r[1];
	vm.r[2] = running->r[2];
	vm.r[3] = running->r[3];
	vm.sr = running->sr;
}

void os::start(){
	
	string line, instrLine;
	int tempOP;
	int count = 0;
	
	int sysBase = 0, sysLimit = 0;

	list<PCB *> jobs;


	system("ls *.s > progs");
	
	ifstream sFiles("progs");

/**********************************************delete*down**************************************************/
	cout << "vm.limit: " << vm.limit << endl; //check that vm limit is 0
/**********************************************delete*up**************************************************/

	if(sFiles.is_open()){ //open list of .s files (opens progs file created)
	cout << "file is open \n";
		while(sFiles.good() ){ //check until the end of file 
			getline(sFiles, line);
			if(line == ""){ // condition set if reads blank line then continue DO NOTHING
				continue;
			}else{
				cout << "\n line: " << line << endl; // if line exist sent to assembler to convert to .o
				as.compile(line);
				
				PCB * p = new PCB; // create a new PCB for the process 
				jobs.push_back(p); //store the new PCB into list<PCB *> jobs;

				p->state = "ready";//set the state to ready for the new process being loaded
				readyQ.push(p);

				//take the file names and store it into PCB
				string oline = line.substr(0, line.length()-2) + ".o";
				string stline = line.substr(0, line.length()-2) + ".st";
				string iline = line.substr(0, line.length()-2) + ".in"; 
				string outline = line.substr(0, line.length()-2) + ".out";

				p->sfile = line;
				p->ofile = oline;
				p->stfile = stline;
				p->infile = iline;
				p->outfile = outline;
				
				//store values into vm.mem
				ifstream asCode(oline.c_str() );

/**********************************************delete*down**************************************************/
				cout << "new file open asCode \n";
/**********************************************delete*up**************************************************/

				while(asCode.good()) { //store .o file into vm.mem
						getline(asCode, instrLine);
						//if line is blank then move on and DO NOTHING
						if(instrLine == ""){
							continue;
						}else{
							//take line in file and convert to int
							stringstream convert(instrLine);
							convert >> tempOP;
							//store value into memory
							vm.mem[count] = tempOP;

/**********************************************delete*down**************************************************/
							cout << instrLine << endl;
/**********************************************delete*up****************************************************/

							count++;
						}
				} //end while loop where .o file is being stored into vm.mem
				asCode.close();
				//set the limit of each process in PCB
				if(sysLimit == 0){ 
					//first set of instructions loaded then set base to 0
					sysBase = 0;
				}else{ 
				//new set of instructions loaded then set base to prev limit
				//since mem begins its count from 0 need to subtract 1 from count
					sysBase = sysLimit;
				}

				//set base and limit for each PCB
				sysLimit = count;
				p->base = sysBase;
				p->limit = sysLimit;
				p->pc = p->base;

/**********************************************delete*down**************************************************/
				cout << "base for progcess is: " << sysBase << endl;
				cout << "end of instruction for (limit for process) " << line << " : " << sysLimit << endl;
				cout << "cout is: " << count << endl;
				cout << "\n\n Contents of Mem begining from: " << sysBase << "\n";
				for(int i = sysBase; i <= count; i++) {
					cout << vm.mem[i] << endl;
				}
				
				//vm.limit = count;
				cout << "\nbase is : " << sysBase;
				cout << "\nlimit is : " << sysLimit << "\n";
/**********************************************delete*up**************************************************/
			}

		} //end while loop where progs is being loaded and assembled
		sFiles.close();
	} 
	else{
		cout << "progs" << " failed to open \n";
	}
	//set total limit, has limit of all instructions in memory
	vm.limit = count;

/**********************************************delete*down**************************************************/	
	cout << "\n\n\n Content in whole memory\n";
	//
	for(int i = 0; i <= vm.limit; i++){
		cout << vm.mem[i] << endl;
	}
	cout << "total number of instructions in memory are: " << vm.limit << endl;
	
	cout << "\nContent inside each job using readyQ:\n";
	/**********************************************delete*up**************************************************/
	//use readyQ to run process
		while(!readyQ.empty() || !waitQ.empty()){

			running = readyQ.front();
			running->state = "running";
			cout << "running process: " <<  running->sfile << endl; 
			readyQ.pop();

			restoreToVM();
			vm.run();

			OSOperatingTime = OSOperatingTime + 5;
			OSContextSwitchTime = OSContextSwitchTime + 5;

			//update all nonterminated processes with context switch time
			list<PCB *>::iterator it;
			for(it = jobs.begin(); it != jobs.end(); it++){
				if((*it)->state == "terminated"){
					continue;
				}
				else{
					(*it)->contextSwitchTime = (*it)->contextSwitchTime + 5;
				}
			}
			//add the number of clock tick of the running process as wait time for all proceses in readyQ
			for(it = jobs.begin(); it != jobs.end(); it++){
				if((*it)->state == "ready"){
					(*it)->waitingTime = (*it)->waitingTime + vm.clock;
				}
				else{
					continue;
				}
			}

			//check if process is in waitQ is less than total OS time
			//if it is waiting completed, change state to ready and send to readyQ
			if(!waitQ.empty()){
				while(waitQ.front()->IOTime <= OSOperatingTime){
					PCB * waiting = waitQ.front();
					waitQ.pop();
					waiting->state = "ready";
					readyQ.push(waiting);
					if(waitQ.size() == 0){
						break;
					}
				}
			}

			VMReturnStatus(); //will send to waitQ, readyQ, or terminiate process
			

		}

/**********************************************delete*down**************************************************/
	
	//check list<PCB *> jobs to see if values are populated
	/*list<PCB *>::iterator it;
	for(it = jobs.begin(); it != jobs.end(); it++){
		cout << "Process name: ";
		cout << (*it)->sfile << "\n";
		cout << "  " << (*it)->base << "\n";
		cout << "  " << (*it)->limit << "\n";
		cout << "  " << (*it)->state << "\n";
		vm.oFile = (*it)->ofile;
		vm.inFile = (*it)->infile;
		vm.outFile = (*it)->outfile;
		vm.base = (*it)->base;
		vm.limit = (*it)->limit;
		vm.pc = (*it)->base;
		vm.clock = (*it)->clock;
		vm.run();
		vm.isRunning = true;
	}*/
/**********************************************delete*up**************************************************/
};

int main(){
	os system;
	system.start();
	
	return 0;
}
