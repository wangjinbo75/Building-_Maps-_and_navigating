/**************************

FileName: Serial_send_rev.cpp
Time: 2018年 10月 09日 星期二 14:38:51 CST
Author: fish
Description:
	1.  不想用rosserial, h文件与CPP文件分开
others:

 **************************/
#include "Serial_send_rev.h" 
#define MAX_FILE_SIZE 300*1024

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
int Serial::open_port(int fd , int comport)
{	
	long vdisable;
	if (comport == 0)
	{
		fd=open("/dev/ttyUSB0",O_RDWR | O_NOCTTY | O_NDELAY);
		if(fd == -1) 
		{
			perror("Can't Open Serial Port");
			return (-1);
		}
		else
		{
			printf("open ttyUSB0 ....\n");
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
void Serial::close_port(int fd)
{
	close(fd);
	printf("close(fd)") ;
}

int Serial::write_data(int fd,char *buff,int size)
{
	if(buff == NULL || size == 0 ||  size >MAX_FILE_SIZE )
	{
		return 0;	
	}
	int writeSize = write(fd,buff,size);
	close_port(fd);
	if(writeSize != size) {
		return 0;
	}
	return writeSize;

}
char * Serial::read_data(int fd,int readSize)
{
	int result;
	int iRealReadSize = readSize;
	char *readBuf = (char *) malloc(iRealReadSize);
	if (NULL == readBuf) {
		printf("[readBuf] : malloc error\n");
		return 0;
	}
	memset(readBuf,'\0',iRealReadSize); 
	result = read(fd,readBuf,iRealReadSize); 
	if(result != iRealReadSize )
	{
		printf("[ReadBuf]:mtd read error\r\n");
		free(readBuf);
		readBuf = NULL ;
		close_port(fd);	
	}
	printf("[ReadBuf]:end to read  readbuf = %s\r\n", readBuf);
	close(fd);
	return readBuf;
}























