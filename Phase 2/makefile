os: Assembler.o VirtualMachine.o
	g++ -std=c++14 os.cpp Assembler.o VirtualMachine.o -o os
	
Assembler.o: Assembler.cpp Assembler.h
	g++ -std=c++14 -c Assembler.cpp
	
VirtualMachine.o: VirtualMachine.cpp VirtualMachine.h
	g++ -std=c++14 -c VirtualMachine.cpp
	
clean: 
	rm *.o os