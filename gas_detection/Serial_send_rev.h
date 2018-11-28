/**************************
FileName: Serial_send_rev.cpp
Time: 2018年 10月 09日 星期二 14:38:51 CST
Author: fish
Description:
others:
**************************/
#ifndef SERIAL_SEND_REV_H
#define SERIAL_SEND_REV_H

#include <stdio.h>	/*标准输入输出的定义*/
#include <stdlib.h>	/*标准函数库定义*/
#include <unistd.h>	/*UNIX 标准函数定义*/
#include <fcntl.h>	/*文件控制定义*/   
#include <termios.h>    /* POSIX中断控制定义*/ 
#include <errno.h> 	/*错误号定义*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>

#include <thread>
#include <vector>
#include <iostream>                                                         
#include <string>
#include <functional>

#include <string.h>
#include <time.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

#define  BUFFERSIZE 36	//缓存大小
class Serial {
	public:
		unsigned char Readbuf[BUFFERSIZE];
		
		Serial(std::string port,int nSpeed,int nBits,char nEvent, int nStop)  ;
		~Serial();
		int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
		int open_port(std::string port,int fd, int comport);
		void close_port();
		int write_data(unsigned char *buff,int size);
		void read_data(int RecvSize);
	private:
		int fd;
		std::thread recv_thread;                                                       
		std::thread send_thread;
		clock_t Start,End;
};

#endif
