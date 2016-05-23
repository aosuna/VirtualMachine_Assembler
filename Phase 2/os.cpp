#include "os.h"

using namespace std;

int main (int argc, char *argv[]){
	Assembler as;
	VirtualMachine vm;

	string fileName;
	
	if (argc>=2){ //compile the files the user specified
		for (int i=argc; i>1; i--){
			as.compile(argv[i-1]); //ex: 2 files, argc=3, runs for 

			vm.run(argv[i-1]); //now run it
		}
	} else if (argc==1){ //compile all *.s files in the directory
		system("ls *.s > progs");

		ifstream progLS ("progs");
		string line;
		int progNumber = 1; //number of progs in folder

		if (progLS.is_open()){
			while(getline(progLS, line)){ //open progs and compile all items
				cout << line << endl;
				fileName = line;
				as.compile(fileName);
				vm.run(fileName);

				progNumber++;
			}
			cout << "The number of programs in the folder is: " << progNumber << endl;
		}

		//good manners: close the file stream
		progLS.close();
	}

	return 0;
}
