#include "os.h"

using namespace std;

os::os(){
	//default constructor
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

				while(asCode.good()) {
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

				//set new limit to program
				sysLimit = count;
				p->base = sysBase;
				p->limit = sysLimit;

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
	} else{
		cout << "progs" << " failed to open \n";
	}
	//set limit to total count of instructions loaded into mem
	vm.limit = count;


/**********************************************delete*down**************************************************/
	//set the limit for the whole number of instructions
	
	cout << "\n\n\n Content in whole memory\n";
	//
	for(int i = 0; i <= vm.limit; i++){
		cout << vm.mem[i] << endl;
	}
	cout << "total number of instructions in memory are: " << vm.limit << endl;

	//check list<PCB *> jobs to see if values are populated
	cout << "\nContent inside each job \n";
	list<PCB *>::iterator it;
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
	}
/**********************************************delete*up**************************************************/
};

int main(){
	os main;
	main.start();
	
	return 0;
}
