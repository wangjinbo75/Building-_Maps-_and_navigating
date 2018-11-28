

CC=clang++
CXXFLAGS=clang++  -std=c++11 -c  	#-Wall -pedantic #CXXFLAGS 表示用于 C++ 编译器的选项
LDFLAGS=-pthread			#有线程 必须加上  在生成的代码中支持POSIX线程

all: 	output

output:	main.o camera.o
	$(CC)  $(LDFLAGS) main.o  camera.o  -o  output
main.o: main.cpp  camera_data.h
	$(CXXFLAGS)  $(LDFLAGS)  main.cpp 
camera.o: camera_data.cpp  camera_data.h
	$(CXXFLAGS) $(LDFLAGS) camera_data.cpp  -o camera.o

.PHONY:
	clean
clean:
	rm -rf *.o
