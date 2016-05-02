#include "Assembler.h"
#include "VirtualMachine.h"

using namespace std;

int main (int argc, char *argv[]){
	Assembler as;
	VirtualMachine vm;

	string fileName;
	string oFileName;
	
	if (argc==2){
		as.compile(argv[1]);
	} else if (argc==1){
		cout << "Enter file name: ";
		cin >> fileName;
	
		as.compile(fileName);
	
		oFileName = fileName.substr(0,fileName.length()-2) + ".o";
	}

	vm.run(argv[1]);

	return 0;
}
