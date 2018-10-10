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
#include <unistd.h>	/*UNIX 标准函数定义*/
#include <stdlib.h>	/*标准函数库定义*/
#include <fcntl.h>	/*文件控制定义*/   
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h> 
#include <fstream>
#include <thread>
#include <vector>
#include <iostream>                                                         
#include <string.h>
class Serial {
	private:
	public:
		int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
		int open_port(int fd, int comport);
		void close_port(int fd);
		int write_data(int fd,char *buff,int size);
		char * read_data(int fd,int readSize);
	
};

#endif
