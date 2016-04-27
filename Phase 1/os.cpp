/* *********************** 
* os.cpp
*Written by Aaron Chamberlain & Adrian Osuna April 2016 at CSUSB
*The header of the Assembler class which compiles a made-up assembly language
* into an object file of integers to be run in the virtual machine
*************************/

#include "Assembler.h"
#include "VirtualMachine.h"

using namespace std;

int main(int argc, char *argv[]){
	Assembler as;
	VirtualMachine vm;
	as.compile( argv[1] );
	vm.run(argv[1]);
}