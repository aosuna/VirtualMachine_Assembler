#include "Assembler.h"
#include "VirtualMachine.h"

using namespace std;

int main (int argc, char *argv[]){
	Assembler as;
	VirtualMachine vm;

	string fileName;
	string oFileName;
	cout << "Enter file name: ";
	cin >> fileName;
	
	as.compile(fileName);
	
	fileName.pop_back()
	oFileName = fileName + "o";
	vm.run(fileName);

	return 0;
}
