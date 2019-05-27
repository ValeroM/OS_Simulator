
program: main.o HD.o OS.o PCB.o ManageMem.o
	g++ main.o HD.o OS.o PCB.o ManageMem.o -o program

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

HD.o: HD.cpp
	g++ -std=c++11 -c HD.cpp

OS.o: OS.cpp
	g++ -std=c++11 -c OS.cpp

PCB.o: PCB.cpp
	g++ -std=c++11 -c PCB.cpp

ManageMem.o: ManageMem.cpp
	g++ -std=c++11 -c ManageMem.cpp

clean:
	rm *.o program