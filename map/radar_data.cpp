#include"radar_data.h"
#include"Serial_send_rev.h"

/*构造函数*/
Radar_data::Radar_data()
{
	std::cout << "Radar_data" <<"\n";
}
/*析构函数*/
Radar_data::~Radar_data()
{
	std::cout << "~Radar_data" <<"\n";
}
/*
 * 功能：得到雷达数据
 *  
 * * 
 */
Radar_data::get_radar_data(unsigned char * get_data,int DataSize)
{
	unsigned char * ReadData = new unsigned char[DataSize];
	bzero(ReadData,DataSize);    
	ReadData = get_data;
	ReadData 
	
}
 
int main(int argc,char * argv[] )
{
	Serial m("/dev/ttyUSB0",9600,8,'n',1);
	
	int BufSize = 100;
		
	m.read_data(BufSize);

	std::cout << "ycm is good" <<"\n";
	
	return 0;
}
