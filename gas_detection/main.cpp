/**************************
FileName: Serial_send_rev.cpp
Time: 2018年 10月 09日 星期二 14:38:51 CST
Author: fish
Description: 
	1.  不想用rosserial, h文件与CPP文件分开
	2.  	发十六进制
others:
		sleep(1);    //秒
		usleep(500*1000); //500毫秒
**************************/
#include <iostream>
#include <sstream>
#include "Serial_send_rev.h" 
void sensor_data(unsigned char ReadBuf[],int BufSize)
{
	char GasNames[] = {0x03,0x04,0x05};
	/* 状态值 */
	for(int i = 3,j = 1; i<	(BufSize-7) ; i+=7,j++)
	{
		unsigned char status;
		char BitsFlags = true;
		status =ReadBuf[i];
		printf("\e[36m\e[1m连接号 %d  status : \e[0m",j);

		switch(status) {
			case 0x00 : printf("\e[32m\e[1m正常\e[0m\n");
				    break;
			case 0x01 : printf("\e[33m\e[1m低报\e[0m\n");
				    break;
			case 0x02 : printf("\e[31m\e[1m高报\e[0m\n");
				    break;
			case 0x04 : printf("\e[36m\e[1m无数据\e[0m\n");
				    BitsFlags = false;
				    break;
			case 0x06 : printf("传感器故障\n");
				    BitsFlags = false;
				    break;
		}
		/*小数位*/
		unsigned char bits;
		bits  = ReadBuf[i+4];
		if(BitsFlags == true)
		{
			/*浓度值*/
			int a[3] ={0};
			int b[3] ={0};
			for(int n=0;n<3;n++) {
				b[n] = (  ReadBuf[i+1+n] & 0x0f );
				a[n] = ( (ReadBuf[i+1+n] & 0xf0) >> 4 );
			}
			float sum = a[0]*100000+b[0]*10000+a[1]*1000+b[1]*100+a[2]*10+b[2];
			//	printf("ReadBuf[i+3] = %x \n",ReadBuf[i+3]);
			//	printf("high4[2] = %d , low4[2]= %d \n",a2,a1);
			//	printf("a3 = %d \n",a1+a2*10);
			//float sum = a6*100000+a5*10000+a4*1000+a3*100+a2*10+a1;
			switch(bits) {
				case 0x00 : printf("	  无小数 \n");
							printf("	  浓度值 : %f \n",sum);
					    break;
				case 0x01 : printf("	  1位小数 \n");
							printf("	  浓度值 : %f \n",sum/10);
					    break;
				case 0x02 : printf("	  2位小数 \n");
							printf("	  浓度值 : %f \n",sum/100);
					    break;
				case 0x03 : printf("	  3位小数 \n");
							printf("	  浓度值 : %f \n",sum/1000);
					    break;
				case 0x04 : printf("      4位小数 \n");
					    break;
			}
		
		//printf("	    浓度值 : %x%x%x \n",ReadBuf[i+1],ReadBuf[i+2],ReadBuf[i+3]);
		}
		/*气体名称*/
		unsigned char GasName;
		GasName  = ReadBuf[i+5];
		switch(GasName) {
			case 0x01 : printf("	  可燃气体 \n");
				    break;
			case 0x04 : printf(" 	  氢气 \n");
				    break;
			case 0x05 : printf("	  甲烷 \n");
				    break;
		}
		/*气体单位*/
		unsigned char GasUnit;
		GasUnit  = ReadBuf[i+6];
		switch(GasUnit) {
			case 0x00 : printf("	  LEL \n");
				    break;
			case 0x01 : printf("	  VOL\n");
				    break;
			case 0x02 : printf("	  ppm\n");
				    break;
			case 0x03 : printf("	  ppb\n");
				    break;
		}

	}



}
int main(int argc, char *argv[])
{

	int  fd;
	// 01 03 03 00 00 1C 44 47
	//01 03 03 80 00 0C 44 63
	//char buff[] = "abc";
	std::string str; 
	unsigned char buff1[] = {0x01,0x03,0x03,0x80,0x00,0x0C,0x44,0x63};
	unsigned char buff2[] ={0x01,0x03,0x03,0x00,0x00,0x1C,0x44,0x47};

	Serial m("/dev/ttyUSB0",9600,8,'n',1);


	char * a ;
	char a1[] ="abc";
	a = a1;
	std::string str1 = a ;
	std::cout << "str1 = "<<str1<<std::endl;
	//char recv1[8];

	char * p = new char[10];
	printf("sizeof(p) = %lu \n",sizeof(p) );
	delete [] p;


    
	long int i = 1000000 ;
	while(i--)
	{	
		//if(sizeof(buff1) == m.write_data(buff1,sizeof(buff1)) ) 
		if(sizeof(buff2) == m.write_data(buff2,sizeof(buff2)) ) 
		{
			printf("count: %li write yes ! \n",i);
		}
		usleep(500*1000); //500毫秒
		int BufSize  = 33;
		m.read_data(BufSize);
		sensor_data(m.Readbuf,BufSize);

	}
	m.close_port();
	return 0;

}



















