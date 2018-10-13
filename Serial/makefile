#this is the makefile


#BIN=prog

#SRC=$(wildcard *.cpp)
#OBJ=$(SRC:%.cpp=%.o)

#all: $(OBJ)
#	$(CXX) -o $(BIN) $^

#%.o: %.c
#	$(CXX) $@ -c $<

#clean:
#	rm -f *.o
#	rm $(BIN)

CC=clang++
CFlAGS=-c -Wl			#表示用于 C 编译器的选项  CFLAGS： 指定头文件（.h文件）的路径
CXXFLAGS=clang++  -std=c++11 -c  	#-Wall -pedantic #CXXFLAGS 表示用于 C++ 编译器的选项
LDFLAGS=-pthread		#有线程 必须加上


all: 	output

output:	main.o serial.o
	$(CC)  $(LDFLAGS) main.o  serial.o  -o  output
main.o:  main.cpp 
	$(CXXFLAGS) $(LDFLAGS) main.cpp 
serial.o: Serial_send_rev.cpp  Serial_send_rev.h
	$(CXXFLAGS) $(LDFLAGS) Serial_send_rev.cpp  -o serial.o
clean:
	rm -rf *.o output
