#include<iostream>
#include"Serial_send_rev.h"

int main(int argc,char * argv[] )
{
	Serial m("/dev/ttyUSB0",9600,8,'n',1);
	
	int BufSize = 100;
		
	m.read_data(BufSize);

	std::cout << "ycm is good" <<"\n";
	
	return 0;
}
