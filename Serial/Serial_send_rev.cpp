/**************************

FileName: Serial_send_rev.cpp
Time: 2018年 10月 09日 星期二 14:38:51 CST
Author: fish
Description:
	1.  不想用rosserial, h文件与CPP文件分开
	2.  接收到数据类型，unsigned char * , c++ 不能返回数组
	3.  使用new 的时候，需要 动态内存 sizeof(数组名）  为字符的大小
	4.  使用了 Readbuf[]  接收数据  全局
	5.  竟然很奇怪，只能一次性读20个字节 缓存给的太小
others:

 **************************/
#include "Serial_send_rev.h" 

#define MAX_FILE_SIZE 300*1024

Serial::Serial(std::string port,int nSpeed,int nBits,char nEvent, int nStop)
{
	Start = clock();
	
	fd = open_port(port,0,0); 
	set_opt(fd, nSpeed, nBits, nEvent, nStop);
	//recv_thread.join();
	//read_data.detach();
	//send_thread.detach();
	sleep(1);
	printf("serial \n");	
}
Serial::~Serial()
{
	close_port();
	End =clock()-Start;
	std::cout <<GREEN<<"Time consume : "<<(long int)End/CLOCKS_PER_SEC<<RESET<<"\n";
	printf("~serial \n");	
}
/**
* 设置串口数据，校验位,速率，停止位
* @param nBits 类型 int数据位 取值 位7或8
* @param nEvent 类型 char 校验类型 取值N ,E, O,,S
* @param mSpeed 类型 int 速率 取值 2400,4800,9600,115200
* @param mStop 类型 int 停止位 取值1 或者 2
*/
int Serial::set_opt(int fd,int nSpeed,int nBits,char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	/* 保留原串口的属性 */
	if(tcgetattr(fd,&oldtio) != 0)
	{
		perror("SetupSerial 1");
		return -1;
	}
	bzero(&newtio,sizeof( newtio ) );
	newtio.c_cflag |= CLOCAL | CREAD;	/*CREAD 开启串行数据接收，CLOCAL并打开本地连接模式*/
	newtio.c_cflag &= ~CSIZE;	/*设置数据位*/
	/*数据位选择*/
	switch(nBits)
	{
		case 7: newtio.c_cflag |= CS7;
			break;
		case 8: newtio.c_cflag |= CS8;
			break;
	}
	/*设置奇偶校验位*/
	switch (nEvent)
	{
		/*奇校验*/
		case 'o':
		case 'O': newtio.c_cflag |= PARENB;
			  newtio.c_cflag |= (INPCK | ISTRIP);
			  newtio.c_cflag |= PARODD;
			  break;
			  /*偶校验*/
		case 'e':
		case 'E': newtio.c_cflag |= PARENB;
			  newtio.c_cflag |= (INPCK | ISTRIP);
			  newtio.c_cflag &= ~PARODD;
			  break;
			  /*无校验*/
		case 'n':
		case 'N': newtio.c_cflag &= ~PARENB;
			  break;
	}
	/* 设置波特率*/
	switch(nSpeed)
	{
		case 2400: cfsetispeed(&newtio, B2400);
			   cfsetospeed(&newtio, B2400);
			   break;
		case 4800: cfsetispeed(&newtio, B4800);
			   cfsetospeed(&newtio, B4800);
			   break;
		case 9600: cfsetispeed(&newtio, B9600);
			   cfsetospeed(&newtio, B9600);
			   break;
		case 115200: cfsetispeed(&newtio, B115200);
			     cfsetospeed(&newtio, B115200);
			     break;
		default:   cfsetispeed(&newtio, B9600);
			   cfsetospeed(&newtio, B9600);
			   break;
	}
	/*设置停止位*/
	if(nStop == 1)
	{
		newtio.c_cflag &= ~CSTOPB;	
	}
	else if( nStop == 2)
	{
		newtio.c_cflag &= CSTOPB;	
	}
	newtio.c_cc[VTIME] = 0.0;
	newtio.c_cc[VMIN] = 0.0;
	tcflush(fd,TCIFLUSH);
	/*激活配置使其生效*/
	if((tcsetattr(fd,TCSANOW,&newtio)) != 0)
	{
		perror("com set error") ;
		return -1;
	}
	printf("set done ! \n");
	return 0;
}
int Serial::open_port(std::string port, int fd , int comport)
{	
	long vdisable;
	if (comport == 0)
	{
		fd=open(port.c_str(),O_RDWR | O_NOCTTY | O_NDELAY);
		if(fd == -1) 
		{
			perror("Can't Open Serial Port");
			return (-1);
		}
		else
		{
		 	printf("\e[31m\e[1m success:SerialPort%s open \e[0m\n",port.c_str());
		}
	}
	if(fcntl(fd,F_SETFL,0) < 0)
	{
		printf("fcntl failed! \n");
	}
	else
	{
		printf("fcntl =%d\n ",fcntl(fd,F_SETFL,0));
	}
	if(isatty(STDIN_FILENO) == 0) 
	{
		printf("standard input is not a terminal device \n");
	}
	else
	{
		printf("isatty success! \n");
	}
	printf("fd-open= %d\n",fd);
	return fd;
}
void Serial::close_port()
{
	close(fd);
//	printf("close(fd)\n") ;
}

int Serial::write_data(unsigned char *buff,int size)
{
	if(buff == NULL || size == 0 ||  size >MAX_FILE_SIZE )
	{
		printf("buff error\n");
		return 0;	
	}
	int writeSize = write(fd,buff,size);
	//printf("writedata size: %i \r\n",writeSize);
	//close_port();
	//sleep(0.6); //1s
	return writeSize;

}
void Serial::read_data(int RecvSize)
{
//	std::thread::id  thread_A_id = std::this_thread::get_id();
//	std::cout <<BLUE<<"recv_thread = "<< thread_A_id <<RESET<<"\n";
	
	int result;
	int RealReadSize = RecvSize;
	
	//unsigned char *readBuf = (unsigned char *) malloc(iRealReadSize);  //c语言用动态内存分配
	unsigned char *readBuf = new unsigned char[RealReadSize];	     //new c++  动态内存分配
	
	bzero(readBuf,RealReadSize);
	bzero(Readbuf,RealReadSize);
	
	//printf( "sizeof(readBuf) = %lu sizeof(ReadBuf) = %lu   \n" ,sizeof(readBuf),sizeof(Readbuf) );

	while( (result = read(fd,readBuf,RealReadSize)) >0)
	{
		printf("result = %d \n", result);
		for(int i=0 ;i < result; i++)
		{    
		//	printf("hex recv = %x\n",readBuf[i]);
			Readbuf[i]= readBuf[i];	
		}
	}
	//readBufShow = readBuf;
	for(int i=0 ;i < 17;i++)
	{    
		//printf("hex recv = %x\n",Readbuf[i]);
	}
	
	delete []readBuf;

	//return readBuf;
}























