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
	
	fileName.;
	
	oFileName = fileName.substr(0,inFileName.length()-2) + ".o";
	
	cout << "The name of the file being opened is: " << oFileName << endl;
	vm.run(oFileName);

	return 0;
}
