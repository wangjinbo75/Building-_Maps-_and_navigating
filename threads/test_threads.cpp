/**************************
FileName: test_threads.cpp
Time: 2018年 10月 12日 星期五 09:24:59 CST
Author: fish
Description: 	
	1.想练习一下，c++ 线程
	2.2018年 10月 17日 星期三 09:30:59 CST 出现了线程不同步 
others:
	char *p = new char[10];
        printf("sizeof(p) = %lu \n",sizeof(p) );
        
        for(int i = 0; i < 7 ; i++ )
        {
        p[i] = i;
        printf("p[%d] = %d \n", i, p[i]);
        }	
        delete []p;
        //,testThread_( std::bind(&test_thread::print,this,"fish") )
        testThread_.join();
        std::thread::id  testThread_id = std::this_thread::get_id();
        if(main_thread_id == testThread_id )
        {
        std::cout << "This is the main thread. \n";
        }
        else 
        {
        std::cout << "testThread_id 3 = "<< testThread_id <<"\n";
        }
**************************/
#include<iostream>
#include<thread>
#include <unistd.h>
#include <functional>
//#include <chrono>         // std::chrono::seconds
#include <sys/time.h>
#include <mutex>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

std::thread::id  main_thread_id = std::this_thread::get_id();
std::mutex mtx;

class test_thread
{
	public :
		test_thread(int m ) :n(m),testThread_( std::bind(&test_thread::print,this) )
		{
	//		testThread_.join();
	
		}
		~test_thread()
		{
			std::cout << "~test_thread() "<<"\n";
		}
		void print(void)
		{
			std::cout << "print(void) ="<< n <<"\n";
			std::thread::id  testThread_id = std::this_thread::get_id();
			if(main_thread_id == testThread_id )
			{
				std::cout << "This is the main thread. \n";
			}
		
			else 
			{
				std::cout << "class testThread_id 3 = "<< testThread_id <<"\n";
			}
		}
		void print1(const char *print)
		{
			std::cout << "print ="<< print <<"\n";
			std::thread::id  testThread_id = std::this_thread::get_id();
			if(main_thread_id == testThread_id )
			{
				std::cout << "This is the main thread. \n";
			}
			else 
			{
				std::cout << "class testThread_id 4 = "<< testThread_id <<"\n";
			}
		}
		std::thread testThread_1()
		{
			return std::thread(&test_thread::print1,this,"fly");
		}
	private:
		int n;
		std::thread testThread_;

};

long int get_current_time()
{
	struct timeval  tv; //
	struct timezone tz;
	struct timeval  ts;
	gettimeofday(&tv,NULL);
	//print("%ld.%ld \n", tv.tv_sec, tv.tv_usec / 1000);

	//std::cout <<GREEN<<"second: is "<<tv.tv_sec<<RESET<<"\n";
	//std::cout <<GREEN<<"millisecond: is "<<tv.tv_sec*1000+tv.tv_usec/1000<<RESET<<"\n";
	//std::cout <<GREEN<<"microsecond: is "<<tv.tv_sec*1000000+tv.tv_usec<<RESET<<"\n";
	return tv.tv_sec*1000000+ tv.tv_usec ;//微秒
}
void thread_A(const char *what)
{
	//	using namespace std::literals::chrono_literals;
	mtx.lock();
	std::thread::id  thread_A_id = std::this_thread::get_id();
	std::cout << "thread_A_id 1 = "<< thread_A_id <<"\n";
	std::cout << "	what = "<< what <<"\n";
	std::cout << "thread_A : ";
	for(int i = 0; i < 10 ;i++)
	{
		std::cout<<YELLOW<< i <<RESET;
		sleep(1);
	}
	std::cout << "\n";
	mtx.unlock();

}
void thread_B(const char *what)
{
	mtx.lock();
//	using namespace std::literals::chrono_literals;
	std::thread::id  thread_B_id = std::this_thread::get_id();
	std::cout << "thread_B_id 2 = "<< thread_B_id <<"\n";
	std::cout << "	what = "<< what <<"\n";
	mtx.unlock();
}
int main(int argc, char *argv[])
{
	std::cout << "main_thread_id 0 = "<< main_thread_id <<"\n";
	long int t0,t1,t2,t3;
	t0 = get_current_time();
	sleep(1);  // 秒
/*
	函数的thread
	join  阻塞模式 等我执行完 其他才能执行 （thread id 在主函数一样）
	detach 非阻塞模式    （thread id 在 主函数一样 ）
*/	
	std::thread work1(&thread_A,"work1");
	work1.join();
	t1 = get_current_time();
	std::cout <<GREEN<<"use time:  "<<(double)(t1-t0)/1000000<<"s"<<RESET<<"\n";
	
	std::thread work2(&thread_B,"work2");
	work2.join();
	t2 = get_current_time();
//	sleep(1);  // 秒
	std::cout <<GREEN<<"use time:  "<<(double)(t2-t1)/1000000<<"s"<<RESET<<"\n";
	
//	std::cin.get();   //等待ENTER 按下
//	test_thread test(8);
//	test.testThread_.detach();

/*
	C++11 std::thread在类的成员函数中的使用
	见了好几种用法
	1.创建一个 new 类的对象 然后去指向这个函数,thread_ID
*/

	test_thread *w = new test_thread(8);
	std::thread tw1 = w->testThread_1();
	tw1.join();
	t3 = get_current_time();
	sleep(1);  // 秒
	std::cout <<GREEN<<"use time:  "<<(double)(t3-t2)/1000000<<"s"<<RESET<<"\n";
//	std::thread work2(&test_thread::print1,test);
//	work2.join();
	
//	sleep(50);  // 秒
	std::cout << "completed ... \n";
	return 0;
}

