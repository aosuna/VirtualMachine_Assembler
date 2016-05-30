os: Assembler.o VirtualMachine.o PCB.o
	g++ -std=c++14 os.cpp Assembler.o VirtualMachine.o PCB.o -o os
	
Assembler.o: Assembler.cpp Assembler.h
	g++ -std=c++14 -c Assembler.cpp
	
VirtualMachine.o: VirtualMachine.cpp VirtualMachine.h
	g++ -std=c++14 -c VirtualMachine.cpp

PCB.o: PCB.cpp PCB.h
	g++ -std=c++14 -c PCB.cpp
	
clean: 
	rm *.o os