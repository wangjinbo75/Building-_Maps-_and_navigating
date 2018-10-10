/**************************

FileName: Serial_send_rev.cpp
Time: 2018年 10月 09日 星期二 14:38:51 CST
Author: fish
Description:
	1.  不想用rosserial, h文件与CPP文件分开
others:

**************************/
#include <iostream>
#include "Serial_send_rev.h" 

int main(int argc, char *argv[])
{

	int fd;
	Serial m;
	fd = m.open_port(0,0);
	m.set_opt(fd,9600,8,'O',1);

	m.close_port(fd);
	return 0;
}



