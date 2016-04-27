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
	
	fileName.pop_back();
	oFileName = fileName + "o";
	cout << "The name of the file being opened is: " << oFileName << "\n";
	vm.run(oFileName);

	return 0;
}
