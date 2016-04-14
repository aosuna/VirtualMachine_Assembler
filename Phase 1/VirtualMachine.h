#ifndef VirtualMachine_H
#define VirtualMachine_H 
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <string>
#include <map>


using namespace std;

class format1 {
	public:
		unsigned UNUSED:6;
		unsigned RS:2;
		unsigned I:1;
		unsigned RD:2;
		unsigned OP:5;
	};

class format2 {
	public:
		unsigned ADDR:8;
		unsigned I:1;
		unsigned RD:2;
		unsigned OP:5;
	};


class format3 {
	public:
		int CONST:8;
		unsigned I:1;
		unsigned RD:2;
		unsigned OP:5;
	};

union instruction {
		int i;
		format1 f1;
		format2 f2;
		format3 f3;
	};

class VirtualMachine{
           private:
               static const int REG_FILE_SIZE = 4;
               static const int MEM_SIZE = 256;
			   typedef void (VirtualMachine::*FP)(int); //declare pointer to void function pass int value into void
               vector<int> r;	//registers use only 0-3
               vector<int> mem; //256 words of memory
               int pc;		//program counter
               int ir;		//instruction register
               int sr;		//status register
               int sp;		//stack pointer
               int clock;	//clock counter
               int base; 	//base register
               int limit;	//limit register
               //bool ALU; 	//arithmetic and logic unit NOT SURE IF NEEDED??	
               string inFile; //reads in file
               string outFile;	//output a file
			   map<int, FP> OPInstruc; //map instructions to function

           public:
            	VirtualMachine(); //constructor
				void run(string infile, string outfile); //execute the program init pc, ir, sr, clock, base, limit, here

            	//assembly functions
            	void load();	//r[RD] = mem[ADDR]
               	void loadi();	//r[RD] = CONST (a const value)
            	void store();	//mem[ADDR] = r[RD] memory = destination register
                void add();		//r[RD] = r[RD] + r[RS]
                void addi();	//r[RD] = r[RD] + CONST
                void addc();	//r[RD] = r[RD] + r[RS] + CARRY
                void addci();	//r[RD] = r[RD] + CONST + CARRY
                void sub();		//r[RD] = r[RD] - r[RS]
                void subi();	//r[RD] = r[RD] - CONST
                void subc();	//r[RD] = r[RD] - r[RS] - CARRY
                void subci();	//r[RD] = r[RD] - CONST - CARRY
                void and_();	//r[RD] = r[RD] AND r[RS]
                void andi();	//r[RD] = r[RD] AND CONST
                void xor_();	//r[RD] = r[RD] XOR r[RS]
                void xori();	//r[RD] = r[RD] XOR CONST
                void compl_();	//r[RD] = NOT r[RD]
                void shl();		//r[RD] = r[RD] << 1, shift-in-bit = 0
                void shla();	//shl arithmetic 
                void shr();		//r[RD] = r[RD] >> 1, shift-in-bit = 0
                void shra();	//shr arithmetic
                void compr();
                void compri();
                void getstat();	//r[RD] = SR
                void putstat();	//SR = r[RD]
                void jump();	//pc = ADDR
                void jumpl();	//if LESS == 1, pc = ADDR, else do nothing
                void jumpe();	//if EQUAL == 1, pc = ADDR, else do nothing
                void jumpg();	//if GREATER == 1, pc = ADDR, else do nothing
                void call();	//push VM status; pc = ADDR
                void return_();	//pop and restore VM status
                void read();	//read new content of r[RD] from .in file
                void write();	//write r[RD] into .out file
                void halt();	//halt execution exit()
                void noop();	//no operation 
				
};

#endif
