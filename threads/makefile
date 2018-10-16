

CC=clang++
CXXFLAGS=clang++  -std=c++11 -c  	#-Wall -pedantic #CXXFLAGS 表示用于 C++ 编译器的选项
LDFLAGS=-pthread			#有线程 必须加上  在生成的代码中支持POSIX线程

all: 	output

output:	 test_threads.o
	$(CC)  $(LDFLAGS)  $^  -o $@
test_threads.o: test_threads.cpp  
	$(CXXFLAGS)  $(LDFLAGS)  $^  -o $@

.PHONY:
	clean
clean:
	rm -rf *.o
