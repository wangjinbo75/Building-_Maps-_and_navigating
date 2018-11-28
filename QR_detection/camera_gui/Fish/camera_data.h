#ifndef CAMERA_DATA_H__
#define CAMERA_DATA_H__


/*********************
   V4L2(Video For Linux Two) 是内核提供给应用程序访问音、视频驱动的统一接口。
   https://blog.csdn.net/qq_38880380/article/details/78604153 参考居多
    
    1. 灰度图像的话，缓冲区大小就是width*height.
    2. RGB图像的话，缓冲区大小是width*height*3，同时，像素排列顺序是RGB RGB RGB RGB…
  
   errors: 真的想捅自己几刀， typedef u_int16_t	WORD; 32位的编译器和64位int的int 大小不一样
   1128 : yuv  显示出来  H264

 **********************/
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>  //

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <unistd.h>	    /*UNIX 标准函数定义*/
#include <fcntl.h>  	/*文件控制定义*/              
#include <termios.h>    /* POSIX中断控制定义*/ 
#include <errno.h>      /*错误号定义*/

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <linux/videodev2.h>

extern "C" 
{
#include "jpeglib.h"
}

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */



#define FILE_VIDEO     "/dev/video0"
#define BMP      	"image_bmp.bmp"
#define YUV		    "/home/w/Building_Maps_and_navigating/QR_detection/image_yuv.yuv"
#define JPEG		"/home/w/Building_Maps_and_navigating/QR_detection/image_jpeg.jpeg"
#define MP4         "video.mp4"
#define  IMAGEWIDTH    640
#define  IMAGEHEIGHT   480
#define  False   0
#define  True     1



struct video_info
{
    
#ifdef DEBUG  

    enum v4l2_field field;
    uint32 bytes_per_line;
    uint32 size_image;
    enum v4l2_colorspace color_space;
    uint32 priv;
#endif   
};

typedef unsigned char   BYTE;
typedef u_int16_t	WORD;
typedef u_int32_t   DWORD;

typedef struct tagBITMAPFILEHEADER{
     WORD     bfType;                // the flag of bmp, value is "BM"
     DWORD    bfSize;                // size BMP file ,unit is bytes
     DWORD    bfReserved1;            // 0
     DWORD    bfReserved2;            // 0
     DWORD    bfOffBits;             // must be 54
}BITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER{
     DWORD    biSize;                // must be 0x28
     DWORD    biWidth;           //
     DWORD    biHeight;          //
     WORD     biPlanes;          // must be 1
     WORD     biBitCount;            //
     DWORD    biCompression;         //
     DWORD    biSizeImage;       //
     DWORD    biXPelsPerMeter;   //
     DWORD    biYPelsPerMeter;   //
     DWORD    biClrUsed;             //
     DWORD    biClrImportant;        //
}BITMAPINFOHEADER;

/*********************/
class Camera
{
    public:

       Camera(); 
       ~Camera();

       unsigned char frame_buffer[IMAGEWIDTH*IMAGEHEIGHT*3] ;
       unsigned char * buffer_pointer;
       unsigned int  len;

       int CloseCamera();
       int StopCapturing(void);

       int StartCapturing(void);
       int CapturePhotos();
       int CaptureJpeg(int width,int height);
       int CaptureVideo();
       void CameraInfo();
       /**将yuyv格式转化为rgb888格式**/
       int yuyv_to_rgb888(void);
       /**从缓冲区中取值**/
       int GetFrame(unsigned char **yuv_buffer_pointer, unsigned int *len);
       /**更新缓冲区**/
       int FreeFrame();
       
    private:

        int fd;                      /**< camera file descriptor */
        int index;
        int fps  ;
        int force_format ;     // 2 V4L2_PIX_FMT_H264
                                  // 1 V4L2_PIX_FMT_YUYV
        unsigned int n_buffers;
        struct v4l2_capability cap;     //功能
        struct v4l2_format fmt;         //格式
        struct v4l2_requestbuffers rb;  //存放读取到的数据缓存
        struct v4l2_buffer buf;         //缓冲区
        struct v4l2_fmtdesc fmtdesc;
        struct v4l2_streamparm setfps; 
        struct v4l2_requestbuffers req;
        enum v4l2_buf_type type;       

        struct buffer
        {
            void * start;
            unsigned int length;
        } * buffers;



       int CameraInit(void);
       /**配置摄像头**/
       int v4l2_grab(void);


       int ReadFrame(FILE * fp);





};

#endif
