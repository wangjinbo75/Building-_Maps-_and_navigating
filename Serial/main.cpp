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

	int  fd;
	// 01 03 03 00 00 1C 44 47
	char buff[] = "abc";
	Serial m("/dev/ttyUSB0",9600,8,'O',1);

	std::cout <<"buff_size = " << sizeof(buff) << std::endl;
	
	while(1){

	std::cout<< "write_data " <<m.write_data(buff,sizeof(buff))<<std::endl;
		
		if(sizeof(buff) == m.write_data(buff,sizeof(buff)) ) 
		{
			printf("write yes");
		}
		/*
		char * ret = m.read_data(fd,6);
		if( ret == NULL ) 
		{
			printf("ret == NULL\n");
		}
		*/
	}
	m.close_port();
	return 0;
}



