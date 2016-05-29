#include "os.h"

//#include "PCB.h"
using namespace std;

os::os(){

}

void os::start(){
	
	string line, instrLine;
	int tempOP;
	int count = 0;
	vm.limit = 0;
	vm.base = 0;
	
	system("ls *.s > progs");
	
	ifstream sFiles("progs");
	
	cout << "vm.limit: " << vm.limit << endl;
	
	if(sFiles.is_open()){
	cout << "file is open \n";
		while(sFiles.good() ){
			getline(sFiles, line);
			if(line == ""){
				continue;
			}else{
				cout << "\n line: " << line << endl;
				as.compile(line);
				
				cout << "after as.compile \n";
				string oline = line.substr(0, line.length()-2) + ".o";
				ifstream asCode(oline.c_str() );
				cout << "new file open asCode \n";
				while(asCode.good()) {
						getline(asCode, instrLine);
						if(instrLine == ""){
							continue;
						}else{
							stringstream convert(instrLine);
							convert >> tempOP;
							vm.mem[count] = tempOP;
							cout << instrLine << endl;
							count++;
						}
				}
				asCode.close();
				if(vm.limit == 0){ 
					//first set of instructions loaded then set base to 0
					vm.base = 0;
				}else{ 
				//new set of instructions loaded then set base to prev limit
				vm.base = vm.limit -1;
				}
				//set new limit to program
				vm.limit = count;
				cout << "vm.limit is: " << vm.base << endl;
				cout << "end of instruction for " << line << " : " << vm.limit << endl;
				cout << "cout is: " << count << endl;
				cout << "\n\n\n Contents of Mem: \n";
				for(int i = vm.base; i < count; i++) {
					
					cout << vm.mem[i] << endl;
				}
				
				vm.limit = count;
				cout << "\nvm.base is : " << vm.base << endl;
				cout << "\nvm.limit is : " << vm.limit << endl;
			}
		}
		sFiles.close();
	} else{
		cout << "progs" << " failed to open \n";
	}
	//set limit to total count of instructions loaded into mem
	vm.limit = count;
	cout << "\n\n\n out of loop\n";
	for(int i = 0; i < vm.limit; i++){
		cout << vm.mem[i] << endl;
	}
	
	cout << "total number of instructions: " << vm.limit << endl;
};

int main(){
	os main;
	main.start();
	
	return 0;
}
