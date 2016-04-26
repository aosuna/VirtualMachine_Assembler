#include "Assembler.h"
#include "VirtualMachine.h"

int main (int argc, char *argv[]){
	//Assembler as;
	VirtualMachine vm;

	/*std::string fileName;
	cout << "Enter file name: ";
	cin >> fileName;

	std::string objectFile;
	objectFile = as.compile(fileName); */

	vm.run("fact.o");

	return 0;
}