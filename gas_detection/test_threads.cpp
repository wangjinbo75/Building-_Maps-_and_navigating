/**************************
FileName: test_threads.cpp
Time: 2018年 10月 12日 星期五 09:24:59 CST
Author: fish
Description: 	
	1.想练习一下，c++ 线程
others:
**************************/
#include<iostream>
#include<thread>

void thread_1()
{
	std::cout << " thread ...\n" ;
}


int main(int argc, char *argv[])
{
	std::thread work1(thread_1);
	std::cin >> get(); 
	work1.join();
	return 0;
}

