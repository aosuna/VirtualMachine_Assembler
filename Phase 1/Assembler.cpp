//read the entire file and verify that integrity (one command per line,valid commands)
//create the same number of bitsets as there are instructions.
//loop n=#lines in file
	//function to set opcode [15:11]
	//function to set RD [10:9]
	//set immediate bit [8] (search for i at end of instruction)
	//set either RS[7:6] or ADDR[7:0] based off value of i (0 or 1 respectively.)
#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <type_traits>

//type safety check for integral types.
#define IS_INTEGRAL(T) typename std::enable_if< std::is_integral<T>::value >::type* = 0

//enumerated type for switch statement/human readable form
enum opcode {
	load, 
	store, 
	add,
	addc,
	sub,
	subc,
	eand, //and is reserved word
	exor, //xor is reserved word
	ecompl, //xor is reserved word
	shl,
	shla,
	shr,
	shra,
	compr,
	getstat,
	putstat,
	jump,
	jumpl,
	jumpe,
	jumpg,
	call,
	ereturn, //return is reserved word
	read,
	write,
	halt,
	noop
};

//hash function that looks for the opcode and returns the corresponding enum
opcode hashit (std::string const& inString){
	std::size_t position;

	position = inString.find("load");
	if (position!=std::string::npos) return load; //return the enum value of load
	position = inString.find("store");
	if (position!=std::string::npos) return store;
	position = inString.find("addc"); //test before add to ensure logical possibility
	if (position!=std::string::npos) return addc;
	position = inString.find("add");
	if (position!=std::string::npos) return add;
	position = inString.find("subc"); //test before sub to ensure logical possibility
	if (position!=std::string::npos) return subc;
	position = inString.find("sub");
	if (position!=std::string::npos) return sub;
	position = inString.find("and");
	if (position!=std::string::npos) return eand;
	position = inString.find("xor");
	if (position!=std::string::npos) return exor;
	position = inString.find("compl");
	if (position!=std::string::npos) return ecompl;
	position = inString.find("shla"); //test before shl to ensure logical possibility
	if (position!=std::string::npos) return shla;
	position = inString.find("shl");
	if (position!=std::string::npos) return shl;
	position = inString.find("shra"); //test before shr to ensure logical possibility
	if (position!=std::string::npos) return shra;
	position = inString.find("shr");
	if (position!=std::string::npos) return shr;
	position = inString.find("compr");
	if (position!=std::string::npos) return compr;
	position = inString.find("getstat");
	if (position!=std::string::npos) return getstat;
	position = inString.find("putstat");
	if (position!=std::string::npos) return putstat;
	position = inString.find("jumpg");
	if (position!=std::string::npos) return jumpg;
	position = inString.find("jumpe");
	if (position!=std::string::npos) return jumpe;
	position = inString.find("jumpl");
	if (position!=std::string::npos) return jumpl;
	position = inString.find("jump");
	if (position!=std::string::npos) return jump;
	position = inString.find("call");
	if (position!=std::string::npos) return call;
	position = inString.find("return");
	if (position!=std::string::npos) return ereturn;
	position = inString.find("read");
	if (position!=std::string::npos) return read;
	position = inString.find("write");
	if (position!=std::string::npos) return write;
	position = inString.find("halt");
	if (position!=std::string::npos) return halt;
	position = inString.find("noop");
	if (position!=std::string::npos) return noop;

	return load; //must return something bc function is non-void, treat as 0
}

//function to set the bits corresponding to the opcode [15:11]
std::bitset<16> setOpcode(std::string instruction, std::bitset<16> VMinstruction){
	switch(hashit(instruction)){
		case load: //load & loadi = 00000 so change nothing
			break;
		case store: //store = 00001 so change only bit 11
			VMinstruction[11] = 1;
			break;
		case add: //add = 00010 so change only bit 12
			VMinstruction[12] = 1;
			break;	
		case addc: //add = 00011 so change bits 12-11
			VMinstruction[11] = 1;
			VMinstruction[12] = 1;
			break;			
		case sub: //sub = 00100 so change only bit 13
			VMinstruction[13] = 1;
			break;	
		case subc: //subc = 00101 so change only bit 13&11
			VMinstruction[13] = 1;
			VMinstruction[11] = 1;
			break;				
		case eand: //and = 00110 so change only bit 13&12
			VMinstruction[13] = 1;
			VMinstruction[12] = 1;
			break;			
		case exor: //xor = 00111 so change only bits 13-11
			VMinstruction[13] = 1;
			VMinstruction[12] = 1;
			VMinstruction[11] = 1;
			break;			
		case ecompl: //compl = 01000 so change only bit 14
			VMinstruction[14] = 1;
			break;		
		case shl: //shl = 01001 so change only bits 14&11
			VMinstruction[14] = 1;
			VMinstruction[11] = 1;
			break;				
		case shla: //shla = 01010 so change only bit 14&12
			VMinstruction[14] = 1;
			VMinstruction[12] = 1;
			break;
		case shr: //shr = 01011 so change only bits 14,12,11
			VMinstruction[14] = 1;
			VMinstruction[12] = 1;
			VMinstruction[11] = 1;
			break;	
		case shra: //shra = 01100 so change only bits 14-13
			VMinstruction[14] = 1;
			VMinstruction[13] = 1;
			break;								
		case compr: //compr = 01101 so change only bit 14-13,11
			VMinstruction[14] = 1;
			VMinstruction[13] = 1;
			VMinstruction[11] = 1;
			break;			
		case getstat: //getstat = 01110 so change only bits 14-12
			VMinstruction[14] = 1;
			VMinstruction[13] = 1;
			VMinstruction[12] = 1;
			break;			
		case putstat: //putstat = 01111 so change only bits 14-11
			VMinstruction[14] = 1;
			VMinstruction[13] = 1;
			VMinstruction[12] = 1;
			VMinstruction[11] = 1;
			break;			
		case jump: //jump = 10000 so change only bit 15
			VMinstruction[15] = 1;
			break;	
		case jumpl: //jumpl = 10001 so change only bits 15,11
			VMinstruction[15] = 1;
			VMinstruction[11] = 1;
			break;				
		case jumpe: //jumpe = 10010 so change only bit 15,12
			VMinstruction[15] = 1;
			VMinstruction[12] = 1;
			break;		
		case jumpg: //jumpg = 10011 so change only bit 15,12-11
			VMinstruction[15] = 1;
			VMinstruction[12] = 1;
			VMinstruction[11] = 1;
			break;	
		case call: //call = 10100 so change only bit 15,13
			VMinstruction[15] = 1;
			VMinstruction[13] = 1;
			break;		
		case ereturn: //return = 10101 so change only bit 15,13,11
			VMinstruction[15] = 1;
			VMinstruction[13] = 1;
			VMinstruction[11] = 1;
			break;			
		case read: //read = 10110 so change only bit 15,13,12
			VMinstruction[15] = 1;
			VMinstruction[13] = 1;
			VMinstruction[12] = 1;
			break;		
		case write: //write = 10111 so change only bit 15,13-11
			VMinstruction[15] = 1;
			VMinstruction[13] = 1;
			VMinstruction[12] = 1;
			VMinstruction[11] = 1;
			break;
		case halt: //halt = 11000 so change only bit 15-14
			VMinstruction[15] = 1;
			VMinstruction[14] = 1;
			break;				
		case noop: //noop = 11001 so change only bit 15-14,11
			VMinstruction[15] = 1;
			VMinstruction[14] = 1;
			VMinstruction[11] = 1;
			break;				
	}

	return VMinstruction;
}

//function to set the Immediate bit: by design, none of the opcodes contain 'i' unless it 
//is an immediate instruction so simply set bit 8 dependent on whether 'i' is found
std::bitset<16> setImmediate(std::string instruction, std::bitset<16> VMinstruction){
	std::size_t position;

	position = instruction.find("i");
	if (position!=std::string::npos){
		VMinstruction[8] = 1; //if found, set to I to 1
	} else {
		VMinstruction[8] = 0; //explicity set to 0 to avoid problems.
	}	

	return VMinstruction;
}

//set the destination register bits [10:9], which can only be 0,1,2, or 3
std::bitset<16> setDestinationReg(std::string instruction, std::bitset<16> VMinstruction){
	std::size_t position;

	position = instruction.find(' ');
	position++; //get next character after first space

	char RD = instruction[position];
	int iRD = RD - '0';
	std::cout << "Destination reg is: " << iRD << ".\n";

	if (iRD == 1){
		VMinstruction[9] = 1;
	} else if (iRD == 2){
		VMinstruction[10] = 1;
	} else if (iRD == 3){
		VMinstruction[10] = 1;
		VMinstruction[9] = 1;
	} else {
		//throw an error because we don't have that many registers
	}

	return VMinstruction;
}

//set the source register [7:6] which can only be 0,1,2,3
std::bitset<16> setSourceReg(std::string instruction, std::bitset<16> VMinstruction){
	std::size_t position;

	position = instruction.rfind(' ');
	position++; //get next character after second space

	char RS = instruction[position];
	int iRS = RS - '0';
	std::cout << "Source reg is: " << iRS << ".\n";

	if (iRS == 1){
		VMinstruction[6] = 1;
	} else if (iRS == 2){
		VMinstruction[7] = 1;
	} else if (iRS == 3){
		VMinstruction[7] = 1;
		VMinstruction[6] = 1;
	} else {
		//throw an error because we don't have that many registers
	}

	return VMinstruction;
}

//function to convert a int or char byte into a bitset for copying.
template<class T>
std::bitset<8> intToBinString(T byte, IS_INTEGRAL(T)){
	std::bitset<sizeof(T) * CHAR_BIT> bs(byte);
	return bs;
}

//function to set either the address or const. address is 0-->256 and const is -128-->0-->128
std::bitset<16> setAddress(std::string instruction, std::bitset<16> VMinstruction){
	std::size_t position;

	position = instruction.rfind(' ');
	position++; //get next character after first space

	std::string address = instruction.substr(position, instruction.length());
	std::string::size_type sz;
	int iaddress = std::stoi(address,&sz);

	std::cout << "str = " << iaddress <<".\n";

	std::bitset<8> constAddress;
	if (iaddress<0){ //the value is a const, can be neg, set to signed char
		signed char byte = iaddress;
		constAddress = intToBinString(byte);
		std::cout << "The value of constAddress outside of if is: " << constAddress <<".\n";
	} else { //the value is either an address, set to unsigned char
		unsigned char byte = iaddress;
		constAddress = intToBinString(byte);
		std::cout << "The value of constAddress outside of if is: " << constAddress <<".\n";
	}
	//for each of the 8 bits in constant/address loop through and assign to VMinstruction
	for (int i=0; i<=7; i++){
		VMinstruction[i] = constAddress[i];
	}

	return VMinstruction;
}

int main(){
	std::bitset<16> test_instruction;
	std::cout << "Instruction: " << test_instruction << "\n";

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

	if (myAssembly.is_open() && myOutput.is_open()){
		while (getline(myAssembly,line)){
			myOutput << line << "\n"; //output the original ass instruction
			//convert to the bitset version and output
			test_instruction = setOpcode(line, test_instruction);
			test_instruction = setImmediate(line, test_instruction);
			test_instruction = setDestinationReg(line, test_instruction);
			test_instruction = setSourceReg(line, test_instruction);
			test_instruction = setAddress(line, test_instruction);

			myOutput << test_instruction << "\n";

			//one final check to make sure it was set correctly
			std::cout << "The instruction has been changed to: " << test_instruction << ".\n";
		}
	}

	//good manners: close the file stream
	myAssembly.close();
	myOutput.close();

	return 0;
}