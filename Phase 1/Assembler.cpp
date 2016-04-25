//read the entire file and verify that integrity (one command per line,valid commands)
//create the same number of bitsets as there are instructions.
//loop n=#lines in file
	//function to set opcode [15:11]
	//function to set RD [10:9]
	//set immediate bit [8] (search for i at end of instruction)
	//set either RS[7:6] or ADDR[7:0] based off value of i (0 or 1 respectively.)
#include "Assembler.h"
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
			myOutput << line << "\n"; //output the original assmb. instruction
			//convert to the bitset version and output
			file.setOpcode(line);
			file.setDestinationReg(line);
			file.setImmediate(line);
			file.setSourceReg(line);
			//setAddr is called in setImm. and explicitly in exception opcodes

			myOutput << file.getInstruction() << "\n";
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