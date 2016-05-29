/* *********************** 
* Assembler.h
*Written by Aaron Chamberlain & Adrian Osuna April 2016 at CSUSB
*The header of the Assembler class which compiles a made-up assembly language
* into an object file of integers to be run in the virtual machine
*************************/
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <type_traits>
#include <climits>

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

#endif