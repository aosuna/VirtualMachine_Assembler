/* *********************** 
* Assembler.h
*Written by Aaron Chamberlain & Adrian Osuna April 2016 at CSUSB
*The header of the Assembler class which compiles a made-up assembly language
* into an object file of integers to be run in the virtual machine
*************************/

#include <bitset>
#include <string>
#include <type_traits>

//type safety check for integral types.
#define IS_INTEGRAL(T) typename std::enable_if< std::is_integral<T>::value >::type* = 0

//function to convert a int or char byte into a bitset for copying.
template<class T>
std::bitset<8> intToBinString(T byte, IS_INTEGRAL(T)){
	std::bitset<sizeof(T) * CHAR_BIT> bs(byte);
	return bs;
}

/*Class Assembler should be instantiated only once */
class Assembler{
public:
	Assembler(); //default constructor

	//data modifier functions
	void clearInstruction(); //func to set all bits to 0
	void setOpcode(std::string instruction);
	void setDestinationReg(std::string instruction);
	void setImmediate(std::string instruction);
	void setSourceReg(std::string instruction);
	void setAddress(std::string instruction);

	//data access functions
	std::bitset<16> getInstruction();
	std::string compile(std::string inFileName);

private:
	std::bitset<16> test_instruction;
};


//default constructor
Assembler::Assembler(){
}; 

/* ----------------------------------Implementation---------------------------------*/

//enumerated type for switch statement/human readable form (non-member)
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

//hash function that looks for the opcode and returns the corresponding enum (non-member)
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

//function to clear the entire contents of our instruction to avoid dynamic allocation
void Assembler::clearInstruction(){
	for (int i=0; i<=15; i++){
		test_instruction[i] = 0;
	}
}

//function to set the bits corresponding to the opcode [15:11]
void Assembler::setOpcode(std::string instruction){
	switch(hashit(instruction)){
		case load: //load & loadi = 00000 so change nothing
			setAddress(instruction);
			break;
		case store: //store = 00001 so change only bit 11
			test_instruction[11] = 1;
			setAddress(instruction);
			break;
		case add: //add = 00010 so change only bit 12
			test_instruction[12] = 1;
			setSourceReg(instruction);
			break;	
		case addc: //add = 00011 so change bits 12-11
			test_instruction[11] = 1;
			test_instruction[12] = 1;
			setSourceReg(instruction);
			break;			
		case sub: //sub = 00100 so change only bit 13
			test_instruction[13] = 1;
			setSourceReg(instruction);
			break;	
		case subc: //subc = 00101 so change only bit 13&11
			test_instruction[13] = 1;
			test_instruction[11] = 1;
			setSourceReg(instruction);
			break;				
		case eand: //and = 00110 so change only bit 13&12
			test_instruction[13] = 1;
			test_instruction[12] = 1;
			setSourceReg(instruction);
			break;			
		case exor: //xor = 00111 so change only bits 13-11
			test_instruction[13] = 1;
			test_instruction[12] = 1;
			test_instruction[11] = 1;
			setSourceReg(instruction);
			break;			
		case ecompl: //compl = 01000 so change only bit 14
			test_instruction[14] = 1;
			break;		
		case shl: //shl = 01001 so change only bits 14&11
			test_instruction[14] = 1;
			test_instruction[11] = 1;
			break;				
		case shla: //shla = 01010 so change only bit 14&12
			test_instruction[14] = 1;
			test_instruction[12] = 1;
			break;
		case shr: //shr = 01011 so change only bits 14,12,11
			test_instruction[14] = 1;
			test_instruction[12] = 1;
			test_instruction[11] = 1;
			break;	
		case shra: //shra = 01100 so change only bits 14-13
			test_instruction[14] = 1;
			test_instruction[13] = 1;
			break;								
		case compr: //compr = 01101 so change only bit 14-13,11
			test_instruction[14] = 1;
			test_instruction[13] = 1;
			test_instruction[11] = 1;
			setSourceReg(instruction);
			break;			
		case getstat: //getstat = 01110 so change only bits 14-12
			test_instruction[14] = 1;
			test_instruction[13] = 1;
			test_instruction[12] = 1;
			break;			
		case putstat: //putstat = 01111 so change only bits 14-11
			test_instruction[14] = 1;
			test_instruction[13] = 1;
			test_instruction[12] = 1;
			test_instruction[11] = 1;
			break;			
		case jump: //jump = 10000 so change only bit 15
			test_instruction[15] = 1;
			setAddress(instruction);
			break;	
		case jumpl: //jumpl = 10001 so change only bits 15,11
			test_instruction[15] = 1;
			test_instruction[11] = 1;
			setAddress(instruction);
			break;				
		case jumpe: //jumpe = 10010 so change only bit 15,12
			test_instruction[15] = 1;
			test_instruction[12] = 1;
			setAddress(instruction);
			break;		
		case jumpg: //jumpg = 10011 so change only bit 15,12-11
			test_instruction[15] = 1;
			test_instruction[12] = 1;
			test_instruction[11] = 1;
			setAddress(instruction);
			break;	
		case call: //call = 10100 so change only bit 15,13
			test_instruction[15] = 1;
			test_instruction[13] = 1;
			setAddress(instruction);
			break;		
		case ereturn: //return = 10101 so change only bit 15,13,11
			test_instruction[15] = 1;
			test_instruction[13] = 1;
			test_instruction[11] = 1;
			break;			
		case read: //read = 10110 so change only bit 15,13,12
			test_instruction[15] = 1;
			test_instruction[13] = 1;
			test_instruction[12] = 1;
			break;		
		case write: //write = 10111 so change only bit 15,13-11
			test_instruction[15] = 1;
			test_instruction[13] = 1;
			test_instruction[12] = 1;
			test_instruction[11] = 1;
			break;
		case halt: //halt = 11000 so change only bit 15-14
			test_instruction[15] = 1;
			test_instruction[14] = 1;
			break;				
		case noop: //noop = 11001 so change only bit 15-14,11
			test_instruction[15] = 1;
			test_instruction[14] = 1;
			test_instruction[11] = 1;
			break;				
	}
}

//set the destination register bits [10:9], which can only be 0,1,2, or 3
void Assembler::setDestinationReg(std::string instruction){
	std::size_t position;

	//handle the exceptions that don't set the dest. reg.
	position = instruction.find("jump");
	if (position!=std::string::npos){
		return; //do nothing
	}
	position = instruction.find("call");
	if (position!=std::string::npos){
		return; //do nothing
	}

	position = instruction.find(' ');
	position++; //get next character after first space

	char RD = instruction[position];
	int iRD = RD - '0';

	if (iRD == 1){
		test_instruction[9] = 1;
	} else if (iRD == 2){
		test_instruction[10] = 1;
	} else if (iRD == 3){
		test_instruction[10] = 1;
		test_instruction[9] = 1;
	} else {
		//throw an error because we don't have that many registers
	}
}

//function to set the Immediate bit: by design, none of the opcodes contain 'i' unless it 
//is an immediate instruction so simply set bit 8 dependent on whether 'i' is found
void Assembler::setImmediate(std::string instruction){
	std::size_t position;

	//all imm. instr. end w/ the character i with exception of jump(l/e/g),call & store
	position = instruction.find("i");
	if (position!=std::string::npos){
		//write will pass this condition but we don't want it, so return
		position = instruction.find("write");
		if (position!=std::string::npos){
			return;
		} else {	
			test_instruction[8] = 1; //if found, set to I to 1
			setAddress(instruction);
		}
	} else {
		test_instruction[8] = 0; //explicity set to 0 to avoid problems.
	}	

	//set i for exceptions jump(l/e/g) & call
	position = instruction.find("jump");
	if (position!=std::string::npos){
		test_instruction[8] = 1;
	}
	position = instruction.find("call");
	if (position!=std::string::npos){
		test_instruction[8] = 1;
	}
	position = instruction.find("store");
	if (position!=std::string::npos){
		test_instruction[8] = 1;
	}
}

//set the source register [7:6] which can only be 0,1,2,3
void Assembler::setSourceReg(std::string instruction){
	std::size_t position;

	position = instruction.rfind(' ');
	position++; //get next character after second space

	char RS = instruction[position];
	int iRS = RS - '0';

	if (iRS == 1){
		test_instruction[6] = 1;
	} else if (iRS == 2){
		test_instruction[7] = 1;
	} else if (iRS == 3){
		test_instruction[7] = 1;
		test_instruction[6] = 1;
	} else {
		//throw an error because we don't have that many registers
	}
}


//function to set either the address or const. address is 0-->256 and const is -128-->0-->128
void Assembler::setAddress(std::string instruction){
	std::size_t position;

	//handle exception: jump(l/e/g) & call only take one parameter of ADDR
	position = instruction.find("jump");
	if (position!=std::string::npos){
		position = instruction.find(' '); //if jump is found, then get val after first space
	} else {
		position = instruction.rfind(' '); //else get val after second space
	}
	position++; //get next character after space

	std::string address = instruction.substr(position, instruction.length());
	std::string::size_type sz;
	int iaddress = std::stoi(address,&sz);

	std::bitset<8> constAddress;
	if (iaddress<0){ //the value is a const, can be neg, set to signed char
		signed char byte = iaddress;
		constAddress = intToBinString(byte);
	} else { //the value is either an address, set to unsigned char
		unsigned char byte = iaddress;
		constAddress = intToBinString(byte);
	}
	//for each of the 8 bits in constant/address loop through and assign to test_instruction
	for (int i=0; i<=7; i++){
		test_instruction[i] = constAddress[i];
	}
}

std::bitset<16> Assembler::getInstruction(){
	return test_instruction;
}