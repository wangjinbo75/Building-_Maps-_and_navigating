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
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>

#include <thread>
#include <vector>
#include <iostream>                                                         
#include <string>

#include <string.h>
class Serial {
	public:
		Serial(std::string port,int nSpeed,int nBits,char nEvent, int nStop);
		~Serial();
		int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
		int open_port(std::string port,int fd, int comport);
		void close_port();
		int write_data(char *buff,int size);
		char * read_data(int readSize);
	private:
		int fd;
	
};

#endif
