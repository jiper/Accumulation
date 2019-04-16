
TARGET = $(notdir $(CURDIR))

main:main.o
	echo &(TARGET)
	g++ -o main main.o
	
main.o:main.cpp
	g++ -c main.cpp

