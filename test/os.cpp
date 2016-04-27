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
	//remove the last char from the file name
	fileName.pop_back()
	//add o to end of file
	oFileName = fileName + "o";
	vm.run(fileName);

	return 0;
}
