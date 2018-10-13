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
#include <unistd.h>
#include <functional>
//#include <chrono>         // std::chrono::seconds

std::thread::id  main_thread_id = std::this_thread::get_id();
class test_thread{
	public:
		test_thread(int m ) :n(m),testThread_( std::bind(&test_thread::print,this) )
		{
			std::cout << n <<"\n";
		}
		void print(void)
		{
			std::cout << n <<"\n";
		}
	private:
		int n;
		std::thread testThread_;
};

void thread_A()
{
//	using namespace std::literals::chrono_literals;

	std::cout << " thread ...\n" ;
	std::thread::id  thread_A_id = std::this_thread::get_id();
	std::cout << "thread_A_id  = "<< thread_A_id <<"\n";

}
int main(int argc, char *argv[])
{
	std::cout << "main_thread_id  = "<< main_thread_id <<"\n";
	
	
	std::thread work1(thread_A);

	work1.join();
	
	std::cin.get(); 

	test_thread test(8);
	sleep(1);  // 秒

	char *p = new char[10];
	printf("sizeof(p) = %lu \n",sizeof(p) );
	
	for(int i = 0; i < 7 ; i++ )
	{
		p[i] = i;
		printf("p[%d] = %d \n", i, p[i]);
	}	
	delete []p;

	std::cout << "completed ... \n";
	return 0;
}

