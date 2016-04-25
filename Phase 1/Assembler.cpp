/* *********************** 
* Assembler.cpp
*Written by Aaron Chamberlain & Adrian Osuna April 2016 at CSUSB
*The implementation of the Assembler class which compiles a made up assembly language
* into an object file of integers to be run in the virtual machine
*************************/
#include "Assembler.h"
#include <iostream>
#include <fstream>

int main(){
		//get user input to the assembly file
	std::cout << "Input the file you wish to load: ";
	std::string inFileName;
	std::cin >> inFileName;

	//create a string that will be the output file name
	std::string outFileName = inFileName.substr(0,inFileName.length()-2);
	outFileName = outFileName + ".o";

	//open the input & output file streams
	std::ofstream myOutput (outFileName);
	std::ifstream myAssembly (inFileName);
	std::string line;

	Assembler file;

	if (myAssembly.is_open() && myOutput.is_open()){
		while (getline(myAssembly,line)){
			file.clearInstruction();
			//myOutput << line << "\n"; //DEBUG: output the original assmb. instruction
			//convert to the bitset version and output
			file.setOpcode(line);
			file.setDestinationReg(line);
			file.setImmediate(line);
			//setAddr is called in setImm. and explicitly in exception opcodes

			//myOutput << file.getInstruction() << "\n"; //DEBUG: Output the binary format of comp. instr.
			myOutput << file.getInstruction().to_ulong() << "\n";
			//one final check to make sure it was set correctly
			std::cout << "The instruction has been changed to: " << file.getInstruction() << ".\n";
		}
	}

	//good manners: close the file stream
	myAssembly.close();
	myOutput.close();

	return 0;
}