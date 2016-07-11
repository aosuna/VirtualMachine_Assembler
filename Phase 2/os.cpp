#include "os.h"
#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>

using namespace std;

os::os(){
	OSContextSwitchTime = 0;
	OSOperatingTime = 0;
	//default constructor
}

void os::VMReturnStatus(){
cout << "*************************************** VM RETURN STATUS ***********************************\n";
cout << "value of vm "<< bitset<32>(vm.sr) << endl;
	saveToPCB();

	int tempSR = vm.sr;
	tempSR = tempSR >> 5;
	tempSR &= 0b111;
	cout << "return status is " << tempSR << endl;
	OSOperatingTime = OSOperatingTime + vm.clock;
	running->CPUTime = running->CPUTime + vm.clock;

	switch(tempSR){
		case 0:
			running->state = "ready";
			running->CPUTime = running->CPUTime + 15;
			readyQ.push(running);
			cout << "time slice" << endl;
			break;
		case 1:
			running->state = "terminated";
			cout << "size of readyQ: " << readyQ.size() << endl;
			cout << "halt was called"  << endl;
			break;
		case 2:
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
		case 6: //read
			cout << "\nRead Operation from VM Return" << endl;
			OSOperatingTime = OSOperatingTime + 1;
			running->state = "waiting";
			running->interruptTime = OSOperatingTime + 28;
			running->CPUTime = running->CPUTime + 1;
			running->IOTime = running->IOTime + 27;
			
			cout << "		running->readPCB function called" << endl;
			cout << "		sending to read \n";

			running->readPCB();
			
			waitQ.push(running);
			
			//check if there is something in waitQ
			running = waitQ.back();
			
			cout << "		read file in file: " << running->infile << endl;
			
			break;
		case 7: //write
			cout << "\nWrite Operation from VM Return" << endl;
			OSOperatingTime = OSOperatingTime + 1;
			running->state = "waiting";
			running->interruptTime = OSOperatingTime + 28;
			running->turnAroundTime = running->turnAroundTime + running->CPUTime;
			running->CPUTime = running->CPUTime + 1;
			running->IOTime = running->IOTime + 27;
			
			cout << "		pcb.write function called" << endl;
			running->writePCB();

			
			waitQ.push(running);
			
			//check that PCB was sent to waitQ
			running = waitQ.back();
			
			cout << "		waitQ out file: " << running->outfile << endl;
			
			break;
		default:
			cout << "Went to default" << endl;
		break;
	}

}

void os::saveToPCB(){
	cout << "********** Will save register to PCB **************" << endl;
	cout << "process save state is: " << running->sfile << endl;

	cout << "vm.sp value is: " << vm.sp << endl;
	
	running->pc = vm.pc;
	running->r[0] = vm.r[0];
	running->r[1] = vm.r[1];
	running->r[2] = vm.r[2];
	running->r[3] = vm.r[3];
	running->sr = vm.sr;
	running->sp = vm.sp;
	running->CPUTime = running->CPUTime + vm.clock;

	cout << "	running pc: "<< running->pc << endl;
	cout << "	running cpu: "<< running->CPUTime << endl;
	cout << "	running r[0]: "<< running->r[0] << endl;
	cout << "	running r[1]: "<< running->r[1] << endl;
	cout << "	running r[2]: "<< running->r[2] << endl;
	cout << "	running r[3]: "<< running->r[3] << endl;
	cout << "	running sr: " << running->sr << endl;
	cout << " 	running sp: " << running->sp << endl;
	
	if(running->sp < 256){
		
		ofstream saveSTFile(running->stfile.c_str());

		cout << "			save SP in VM has values <-----------------\n";
		cout << "			save SP in VM has values <-----------------\n";
		cout << "			save SP in VM has values <-----------------\n";
		cout << "			save SP in VM has values <-----------------\n";
		cout << "			save SP in VM has values <-----------------\n";


		if(!saveSTFile.is_open()){
			cout << running->stfile << "failed to open. \n";
		}
		else{
			for(int i = vm.sp; i < 256; i++){
				saveSTFile << vm.mem[i] << endl;
				//count++; // used to get stack size
			}
		}
		saveSTFile.close();
	}
	
}

void os::restoreToVM(){

	vm.base = running->base;
	vm.limit = running->limit;
	vm.pc = running->pc;
	vm.oFile = running->ofile;
	vm.inFile = running->infile;
	vm.outFile = running->outfile;
	vm.r[0] = running->r[0];
	vm.r[1] = running->r[1];
	vm.r[2] = running->r[2];
	vm.r[3] = running->r[3];
	vm.sr = running->sr;
	vm.sp = running->sp;

	cout << "********** Restoring values from PCB to VM **************" << endl;
	cout << "process return name is: " << running->sfile << endl;

	cout << "running base PCB: " << running->base << endl;
	cout << "running limit PCB: "<< vm.limit << endl;
	cout << "running pc PCB: " << running->pc << endl;
	//cout << "current running clock PCB: " << running->clock << endl;
	cout << "running ofile PCB: " << running->ofile << endl;
	cout << "running infile PCB: " << running->infile << endl;
	cout << "running outfile PCB: " << running->outfile << endl;
	cout << "running r[0] PCB: " << running->r[0] << endl;
	cout << "running r[1] PCB: " << running->r[1] << endl;
	cout << "running r[2] PCB: " << running->r[2] << endl;
	cout << "running r[3] PCB: " << running->r[3] << endl;
	cout << "running sr PCB: " << running->sr << endl;
	cout << "running sp PCB: " << running->sp << endl;

	
	if(running->sp < 256){
		ifstream restoreSTFile(running->stfile.c_str());

		cout << "			resotre	SP in VM has values <-----------------\n";
		cout << "			restore	SP in VM has values <-----------------\n";
		cout << "			restore	SP in VM has values <-----------------\n";
		cout << "			restore	SP in VM has values <-----------------\n";

		int info;
		string rline;
		int tempSP = running->sp;
		int count = 0;
		//running->readSTFile.open(running->stfile.c_str());
		
		if(restoreSTFile.is_open()){
		cout << "line in .st \n";
			while(restoreSTFile.good()){
				getline(restoreSTFile, rline);
				if(rline == ""){
					cout << "no line read" << endl;
					continue;
				}else{
				
					cout << "             " << rline << endl;
					
					stringstream convert(rline);
					convert >> info;
					vm.mem[tempSP] = info;
					tempSP++;
					count++;
				}
			}
		}else{
			cout << running->stfile << "failed to open. \n";
		}
		restoreSTFile.close();

		//gets largest stack size written to file
		if (running->stackSize == 0){
				running->stackSize = count; //setting max stack size 
		}
		else if( count > running->stackSize){
			running->stackSize = count;
		}
	}
}

void os::closePCBFiles(){
	list<PCB *>::iterator iter;
	
	for(iter = jobs.begin(); iter != jobs.end(); iter++){
		if((*iter)->writeFile.is_open()){
			(*iter)->writeFile.close();
		}
		if((*iter)->readFile.is_open()){
			(*iter)->readFile.close();
		}
	}
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
				
				//open files for each PCB
				p->readFile.open(p->infile.c_str());
				p->writeFile.open(p->outfile.c_str());
				
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
	
///////////////////////////////////////////////////////// RUNNING PROCESSES /////////////////////////////////////////////////////////////////
	
	//use readyQ to run process
		while(!readyQ.empty() || !waitQ.empty()){
			cout << "\n\nexecuting processes, readyQ and waitQ are not empty\n\n";
			
			//check that running is pointing to a PCB if pass then run process
			//if fail there is a process in waitQ
			cout << "checking that readyQ is not empty\n";
			cout << "readyQ size " << readyQ.size() << endl;
			cout << "waitQ size " << waitQ.size() << endl;

			if(!readyQ.empty()){
			cout << "	****readQ is not empty****\n";
				running = readyQ.front();
				running->state = "running";
				cout << "running process: " <<  running->sfile << endl; 
				readyQ.pop();
				restoreToVM();
				vm.run();

				running->contextSwitchTime = running->contextSwitchTime + 5;
				OSOperatingTime = OSOperatingTime + 5;

				//OSContextSwitchTime = OSContextSwitchTime + 5;
				
				//update all nonterminated processes with context switch time
				/*list<PCB *>::iterator it;
				for(it = jobs.begin(); it != jobs.end(); it++){
					if((*it)->state == "terminated"){
						continue;
					}
					else{
						(*it)->contextSwitchTime = (*it)->contextSwitchTime + 5;
					}
				}*/
				//add the number of clock tick of the running process as wait time for all proceses in readyQ
				list<PCB *>::iterator it;
				for(it = jobs.begin(); it != jobs.end(); it++){
					if((*it)->state == "ready"){
						(*it)->waitingTime = (*it)->waitingTime + vm.clock;
					}
				}
			}
			cout << "virtual machine stack pointer before sending to VMReturnStatus: " << vm.sp << endl;
			//check if process is in waitQ is less than total OS time
			//if it is waiting completed, change state to ready and send to readyQ
			cout << "checking waitQ is not empty and waitQ interrupt time is less than OS master time\n";
			if(!waitQ.empty()){
				
			cout << "waitQ not empty, checking waitQ time\n";
				while(waitQ.front()->interruptTime <= OSOperatingTime){
				cout << "wait Q was not empty  and its interrupt time less then master time\n";
				cout << "process in waitQ " << waitQ.front()->sfile << endl;
					PCB * waiting;
					waiting = waitQ.front();
					waitQ.pop();
					waiting->state = "ready";
					readyQ.push(waiting);
					if(waitQ.empty()){
						break;
					}
		
					waiting = NULL;
				}
				//waiting = NULL;
			}

			//Check if all processes are in waitQ
			cout << "checking if readyQ is empty and waitQ is not empty then send a process to readyQ\n";
			if(readyQ.empty() && !waitQ.empty()){
				int tempidle = 0;
			cout << "------------------ all processes are in waitQ --------------------\n";
				PCB * pwaiting;
				pwaiting = waitQ.front(); //waiting process
				pwaiting->state = "ready";
				waitQ.pop();
				readyQ.push(pwaiting);
				tempidle = pwaiting->interruptTime - OSOperatingTime;
				OSOperatingTime = OSOperatingTime + tempidle;
				pwaiting->idleTime = pwaiting->idleTime + tempidle;
				pwaiting = NULL;
			} // end all processes in waitQ, sent one process to execute in ready state
			
			if(running != NULL){
				cout << "virtual machine stack pointer before sending to VMReturnStatus: " << vm.sp << endl;
				VMReturnStatus(); //will send to waitQ, readyQ, or terminiate process

				running = NULL;
			}
			
		}

	list<PCB *>::iterator it;
	int osidelTime = 0;
	double sumOfJobs = 0.0;
	for(it = jobs.begin(); it != jobs.end(); it++){
		ofstream writeTiming((*it)->outfile.c_str(), ios::app);
		if(writeTiming.is_open()){
			writeTiming << "\n";
			writeTiming << "Process Name: " << (*it)->sfile << "\n\n";
			writeTiming << "Process Time: " << (*it)->CPUTime << endl;
			writeTiming << "Waiting Time: " << (*it)->waitingTime << endl;
			writeTiming << "Turn Around Time: " << (*it)->turnAroundTime << endl;
			writeTiming << "I/O Time: " << (*it)->IOTime << endl;
			if((*it)->idleTime < 0){
				(*it)->idleTime = 0;
			}
			writeTiming << "Idle Time: " << (*it)->idleTime << endl;
			writeTiming << "Stack Size: " << (*it)->stackSize << endl;
			//writeTiming << "Context Switch Time: " << (*it)->contextSwitchTime << endl;
			writeTiming << "Interrupt Time: " << (*it)->interruptTime << endl;
		}
		writeTiming.close();
		osidelTime = osidelTime + (*it)->idleTime;
		sumOfJobs = sumOfJobs + (*it)->CPUTime;
	}



	double systemTime = osidelTime + OSContextSwitchTime;
	double cpuUtil = sumOfJobs / OSOperatingTime;
	string osfile = "os.out";
	
	ofstream writeSystemTime(osfile.c_str());
	if(writeSystemTime.is_open()){
		writeSystemTime << "System Information\n\n";
		writeSystemTime << "System Time: " << systemTime << endl;
		writeSystemTime << "CPU Utilization: " << cpuUtil << endl;
	}
	
};

int main(){
	os system;
	system.start();
	
	return 0;
}
