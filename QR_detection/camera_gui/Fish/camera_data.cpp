#include "camera_data.h"
Camera::Camera()
{
   fd = -1;
   buffers = NULL;
   n_buffers = 0;
   index = -1;
   fps = 5 ;
   force_format = 1;


   CameraInit();
   v4l2_grab();
   StartCapturing();
   //yuyv_to_rgb888();
   //memset(rgb_buffer , 0 ,sizeof(rgb_buffer));
   
}
Camera::~Camera()
{
   StopCapturing();
   CloseCamera();
}
int Camera::CameraInit()
{
    if (( fd = open(FILE_VIDEO, O_RDWR)) == -1)                 /**打开摄像头**/        
    {
       std::cout<<RED<< "Error opening V4L interface"<<RESET<<"\n";
       return (false);
    }
    if ( ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1 )   /**设置设备属性**/
    {
        std::cout<<RED<< "Error opening device: unable to query device."<<RESET<<"\n";
        return (false);
    }
    else                                                       /*读video_capability中信息**/
    {
        printf("\tdriver:\t\t%s\n",cap.driver);
        printf("\tcard:\t\t%s\n",cap.card);
        printf("\tbus_info: \t%s\n",cap.bus_info);
        printf("\tversion: \t%d\n",cap.version);
        printf("\tcapabilities: \t%x\n",cap.capabilities);
        if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE) 
        {
            printf("Device %s: supports capture.\n",FILE_VIDEO);
        }
        if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING) 
        {
            printf("Device %s: supports streaming.\n",FILE_VIDEO);
        }
    }
    /*列举摄像头所支持像素格式*/
    fmtdesc.index =0;
    fmtdesc.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf("Support format:\n");
    while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
    {
        printf("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
        fmtdesc.index++;
    }
    /*设置像素格式*/

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (force_format == 2){
        fmt.fmt.pix.width       = 640;
        fmt.fmt.pix.height      = 480;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_H264;
        fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

        //每行像素所占的 byte 数
        unsigned int min;
        min = fmt.fmt.pix.width * 2;
        if (fmt.fmt.pix.bytesperline < min)  fmt.fmt.pix.bytesperline = min;

	    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
        if (fmt.fmt.pix.sizeimage < min)    fmt.fmt.pix.sizeimage = min;


    }
	if(force_format == 1){
		fmt.fmt.pix.width	= 640;
		fmt.fmt.pix.height	= 480;
		fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
		fmt.fmt.pix.field	= V4L2_FIELD_INTERLACED;
        printf("V4L2_PIX_FMT_YUYV \n");
		}
    
    // Set the data format
    if(ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
    {
        printf("Unable to set format\n");
        return False;
    }
    // Get the data format
    /*了确保设置的格式作用到摄像头上，再通过命令VIDIOC_G_FMT将摄像头设置读取回来。*/
    if(ioctl(fd, VIDIOC_G_FMT, &fmt) == -1)
    {
        printf("Unable to get format\n");
        return False;
    } 
    {
        printf("fmt.type:\t\t%d\n",fmt.type);
        printf("\tpix.pixelformat:\t%c%c%c%c\n",fmt.fmt.pix.pixelformat & 0xFF, (fmt.fmt.pix.pixelformat >> 8) & 0xFF,(fmt.fmt.pix.pixelformat >> 16) & 0xFF, (fmt.fmt.pix.pixelformat >> 24) & 0xFF);
        printf("\tpix.height:\t%d\n",fmt.fmt.pix.height);
        printf("\tpix.width:\t\t%d\n",fmt.fmt.pix.width);
        printf("\tpix.field:\t\t%d\n",fmt.fmt.pix.field);
    }
    //set fps
     memset(&setfps, 0, sizeof(setfps) );
     setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
     setfps.parm.capture.timeperframe.numerator = 1;
     setfps.parm.capture.timeperframe.denominator = fps;
     if (ioctl(fd, VIDIOC_G_PARM, &setfps) < 0) 
	{
		printf("Cannot set frame rate");
	}
	//fprintf(stderr, "denominator=%d\n", setfps.parm.capture.timeperframe.denominator);
    //fprintf(stderr, "numerator=%d\n", setfps.parm.capture.timeperframe.numerator);
    
    std::cout<<GREEN<<"1. Camera init is OK"<<RESET<<"\n";

    return True;
}
int Camera::v4l2_grab()
{
    memset( &req, 0, sizeof(req) );
    /*获取每个缓存的信息，并mmap到用户空间。定义结构体*/
    //request for 4 buffers  申请缓存区
    req.count   = 4 ;
    req.type    =V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory  =V4L2_MEMORY_MMAP;
    // Initiate memory mapping or user pointer I/O
    if(ioctl(fd,VIDIOC_REQBUFS,&req)==-1)
    {
        printf("request for buffers error\n");
    }
    //mmap for buffers  来存储mmap后的地址信息 // Memory allocate
     /**内存映射MMAP 及定义一个结构体来映射每个缓冲帧**/
    buffers = (struct buffer*)calloc (req.count, sizeof (*buffers));
    if (!buffers) 
    {
        printf ("Out of memory\n");
        return(False);
    }

    for (n_buffers = 0; n_buffers < req.count; n_buffers++) 
    {
        memset( &buf, 0, sizeof(buf) );
        buf.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory  = V4L2_MEMORY_MMAP;
        buf.index   = n_buffers;
        //query buffers /*将申请到的帧缓冲映射到用户空间*/
        if (ioctl (fd, VIDIOC_QUERYBUF, &buf) == -1)
        {
            printf("query buffer error\n");
            return(False);
        }
        buffers[n_buffers].length = buf.length;
        //map
        buffers[n_buffers].start = mmap(NULL,
                                        buf.length,
                                        PROT_READ |PROT_WRITE, 
                                        MAP_SHARED, 
                                        fd, buf.m.offset);
        if (buffers[n_buffers].start == MAP_FAILED)
        {
            printf("buffer map error\n");
            return(False);
        } 
    }
    std::cout<<GREEN<<"2. Set Camera OK"<<RESET<<"\n";
    return true;
}
int Camera::StartCapturing()
{
    //queue   /* 将申请到的帧缓冲全部加入队列 */
     for (n_buffers = 0; n_buffers < req.count; n_buffers++)
     {
         memset( &buf, 0, sizeof(buf) );
         buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		 buf.memory = V4L2_MEMORY_MMAP;
         buf.index  = n_buffers;
         if(ioctl(fd, VIDIOC_QBUF, &buf) == -1)
         {
             printf("VIDIOC_QBUF error\n");
             return false;
         }
        // printf("Frame buffer %d: address=0x%x, length=%d\n", 
        //n_buffers,(unsigned int )buffers[n_buffers].start,buffers[n_buffers].length);
     } 

    /*开始采集  开始录制  Start capturing  fmtdesc.type VIDIOC_STREAMON*/
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if( ioctl (fd,VIDIOC_STREAMON, &type) == -1 )
    {
        printf("VIDIOC_STREAMON error\n");
        return  false;
    } 

    /*  Read buffer   CapturePhotos == 1 */
//    if ( force_format == 2 && ioctl(fd, VIDIOC_DQBUF, &buf) == -1 )
//    {
//        printf("StartCapturing VIDIOC_DQBUF error\n");
//        return  false;
//    }

    std::cout<<GREEN<<"3. StartCapturing OK"<<RESET<<"\n";
    return true;
}
int Camera::yuyv_to_rgb888()
{
    /* YUYV就是422形式，简单来说就是，两个像素点P1、P2本应该有Y1、U1、V1和Y2、U2、V2这六个分量*/
    int           i,j;
    unsigned char y1,y2,u,v;
    int r1,g1,b1,r2,g2,b2;
    char * pointer; 
    pointer = (char *) buffers[0].start;
    
    for(i=0;i<IMAGEHEIGHT;i++)
    {
        for(j=0;j<IMAGEWIDTH/2;j++)
        {
            y1 = *( pointer + (i*IMAGEWIDTH/2+j)*4);
            u  = *( pointer + (i*IMAGEWIDTH/2+j)*4 + 1);
            y2 = *( pointer + (i*IMAGEWIDTH/2+j)*4 + 2);
            v  = *( pointer + (i*IMAGEWIDTH/2+j)*4 + 3);
            
            r1 = y1 + 1.042*(v-128);
            g1 = y1 - 0.34414*(u-128) - 0.71414*(v-128);
            b1 = y1 + 1.772*(u-128);
            
            r2 = y2 + 1.042*(v-128);
            g2 = y2 - 0.34414*(u-128) - 0.71414*(v-128);
            b2 = y2 + 1.772*(u-128);
                            
            r1 = (r1>255)? 255: ((r1<0)? 0: r1);
            b1 = (b1>255)? 255: ((b1<0)? 0: b1);
            g1 = (g1>255)? 255: ((g1<0)? 0: g1);
            r2 = (r2>255)? 255: ((r2<0)? 0: r2);
            b2 = (b2>255)? 255: ((b2<0)? 0: b2);
            g2 = (g2>255)? 255: ((g2<0)? 0: g2);
                
            *(frame_buffer + ((IMAGEHEIGHT-1-i)*(IMAGEWIDTH/2)+j)*6    ) = (unsigned char)b1;
            *(frame_buffer + ((IMAGEHEIGHT-1-i)*(IMAGEWIDTH/2)+j)*6 + 1) = (unsigned char)g1;
            *(frame_buffer + ((IMAGEHEIGHT-1-i)*(IMAGEWIDTH/2)+j)*6 + 2) = (unsigned char)r1;
            *(frame_buffer + ((IMAGEHEIGHT-1-i)*(IMAGEWIDTH/2)+j)*6 + 3) = (unsigned char)b2;
            *(frame_buffer + ((IMAGEHEIGHT-1-i)*(IMAGEWIDTH/2)+j)*6 + 4) = (unsigned char)g2;
            *(frame_buffer + ((IMAGEHEIGHT-1-i)*(IMAGEWIDTH/2)+j)*6 + 5) = (unsigned char)r2;

        }
    } 
    //std::cout<<GREEN<<"3. yuyv_to_rgb888():Change to RGB  OK "<<RESET<<"\n";
    return true;
}
int Camera::CapturePhotos()
{
    /*yuv格式照片*/
    // FILE * fp2 = fopen(YUV, "wb");
    // fwrite(buffers[0].start, 640*480*2, 1, fp2);
    // std::cout<<GREEN<<BMP<< "save BMP OK"<< RESET<<"\n";
    // fclose(fp2);
   
    int size = IMAGEWIDTH * IMAGEHEIGHT * 3 ; // 每个像素点3个字节     * sizeof(char)
    FILE * fp1 = fopen(BMP, "wb");

    if(!fp1 )
	{
		printf("open error\n");
		return false;
	}
  
    BITMAPFILEHEADER   bf;    // 位图第一部分，文件信息  
    BITMAPINFOHEADER   bi;   // 位图第二部分，数据信息 

   //Set BITMAPFILEHEADER
    bf.bfType = 0x4d42;
    bf.bfSize = 54 + size;
    // bf.bfSize = size 
    //           + sizeof( BITMAPFILEHEADER ) // first section size    
    //           + sizeof( BITMAPINFOHEADER )  ;// second section size    ; //54 + bi.biSizeImage;     
	bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits   = 54;//sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER )  ;
    //printf("BitMapFileHeader = %ld BitMapInfoHeader = %ld \n", sizeof( bf ),sizeof( bi )); 
   
   //Set BITMAPINFOHEADER
	bi.biSize = 40;
	bi.biWidth = IMAGEWIDTH;
	bi.biHeight = IMAGEHEIGHT;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biCompression = 0;
	bi.biSizeImage = size;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
     
     fwrite(&bf, 14, 1, fp1);
     fwrite(&bi, 40, 1, fp1);    
     fwrite(frame_buffer, bi.biSizeImage, 1, fp1);
    //  fwrite(&bf.bfType, 8, 1,fp1);   
    //  fwrite(&bf.bfReserved2, sizeof(bf.bfReserved2), 1, fp1);    
    //  fwrite(&bf.bfOffBits, sizeof(bf.bfOffBits), 1, fp1);    
    //  fwrite(&bi,sizeof( BITMAPINFOHEADER ), 1, fp1);    
    //  fwrite(frame_buffer, size, 1, fp1);
    if(NULL != fp1)
    {
        fclose(fp1);
        std::cout<<GREEN<<BMP<< "save BMP OK"<< RESET<<"\n";
        fp1 = NULL;
    }

    return true;
}
int Camera::ReadFrame(FILE * fp)
{
    /*read_frame*/
     memset(&(buf), 0 ,sizeof(buf));
     buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	 buf.memory = V4L2_MEMORY_MMAP;
     if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1)
     {
        printf("VIDIOC_DQBUF error\n");
        return  false;  
     }
     /* write to file */
     assert(buf.index < n_buffers);
     fwrite(buffers[buf.index].start, buf.bytesused, 1, fp);
     
     /* write to stdout */

    //  if( write(1,buffers[buf.index].start, buf.bytesused)  == -1 )
    //  {
    //     printf(" write error\n");
    //     return  false;  
    //  }
    if ( ioctl(fd, VIDIOC_QBUF, &buf)  == -1)
    {
        printf(" VIDIOC_QBUF2 error\n");
        return  false;  
    }
    
    return true;
}
int Camera::CaptureVideo()
 {
    //创建文件
    int count = 300; // 30为一帧 300 /30 = 10 秒
    FILE *fp = fopen("123.mp4","ab");
    while(count--)
     {
        for (;; ) 
         {
			fd_set fds;
			struct timeval tv;
			int r;

			FD_ZERO(&fds);
			FD_SET(fd, &fds);
            
            r = select(fd + 1, &fds, NULL, NULL, &tv);
			/* Timeout. */
			tv.tv_sec = 2;
			tv.tv_usec = 0;

            if (0 == r) {
				fprintf(stderr, "select timeout\n");
			}

            if ( ReadFrame(fp) )  {	
                break;
            } 
         }
     }
     fflush(fp);
     fclose(fp);
     std::cout<<GREEN<<"4. ReadVideo"<<RESET<<"\n";
     return true;
 }
// int Camera::CaptureJpeg(int width,int height)
// { 
//     struct jpeg_compress_struct cinfo ;
//     struct jpeg_error_mgr jerr ;
//     JSAMPROW  row_pointer[1] ;
//     int row_stride ;
//     char *buf=NULL ;
//     int x ;
 
//     FILE *fptr_jpg = fopen (JPEG,"wb");//注意这里为什么用fopen而不用open
//     if(fptr_jpg==NULL)
//     {
//         printf("Encoder:open file failed!/n") ;
//         return false;
//     }
 
//     cinfo.err = jpeg_std_error(&jerr);
//     jpeg_create_compress(&cinfo);
//     jpeg_stdio_dest(&cinfo, fptr_jpg);
 
//     cinfo.image_width = width;
//     cinfo.image_height = height;
//     cinfo.input_components = 3;
//     cinfo.in_color_space = JCS_RGB;
 
//     jpeg_set_defaults(&cinfo);
 
 
//     jpeg_set_quality(&cinfo, 80,TRUE);
 
 
//     jpeg_start_compress(&cinfo, TRUE);
 
//     row_stride = width * 3;
//     buf=(char *) malloc(row_stride) ;
//     row_pointer[0] = (JSAMPROW) buf;
//     while (cinfo.next_scanline < height)
//     {
//      for (x = 0; x < row_stride; x+=3)
//     {
//         buf[x]   = frame_buffer[x];
//         buf[x+1] = frame_buffer[x+1];
//         buf[x+2] = frame_buffer[x+2];
    
//     }
//     jpeg_write_scanlines (&cinfo, row_pointer, 1);//critical
//     //frame_buffer += (unsigned char)row_stride;
//     }
 
//     jpeg_finish_compress(& cinfo);
//     fclose(fptr_jpg);
//     jpeg_destroy_compress(&cinfo);
//     free(buf) ;

//     return true ;
 
// }  

int Camera::GetFrame(unsigned char ** yuv_buffer_pointer, unsigned int *len)
{
    v4l2_buffer queue_buf;

    queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    queue_buf.memory = V4L2_MEMORY_MMAP;

    if(ioctl(fd, VIDIOC_DQBUF, &queue_buf) == -1) //将申请到的帧缓冲全部加入队列
    {
        printf("GetFrame  VIDIOC_DQBUF error!\n") ;
        return false;
    }
    *yuv_buffer_pointer = (unsigned char *)buffers[queue_buf.index].start;
    *len = buffers[queue_buf.index].length;
    index = queue_buf.index; // 每一帧的序号
    //cout<<queue_buf.m.offset<<endl;// 输出每一帧的缓冲地址

    return true;
}   
int Camera::FreeFrame()
{
    if(index != -1)
    {
        v4l2_buffer queue_buf;
        queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        queue_buf.memory = V4L2_MEMORY_MMAP;
        queue_buf.index = index;
        if(ioctl(fd, VIDIOC_QBUF, &queue_buf) == -1)  //将申请到的帧缓冲全部加入队列
        {
            printf("FreeFrame  VIDIOC_QBUF error!\n") ;
            return false;
        }
        return true;
    }
    return false;
} 
int Camera::CloseCamera()
{
      ioctl(fd, VIDIOC_STREAMOFF, &type);     
      if(fd != -1) 
      {
           close(fd);
           std::cout<<RED<<"CloseCamera"<<RESET<<"\n";
           return (True);
       }
       return (False);
}
int Camera::StopCapturing(void)
{
    //enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == ioctl(fd, VIDIOC_STREAMOFF, &type))
    {
    return false;
    }
    return true;
}
void Camera::CameraInfo()
{
    uint8_t search_times = 3;// 最多搜索次数
    std::string dev_camera;
    std::string dev_input;
    int fd; // 设备索引号
    /**v4l2中的设备属性结构体**/
    struct v4l2_capability cap;

    std::cout<<"Video devices!"<<std::endl;
   // std::cout<<"Please copy the right camera name to the launch file param: camera_name "<<std::endl;
    for(unsigned int i = 0 ; i < search_times ; i++)
    {
        /**生成摄像头设备索引号**/
        std::stringstream stream;
        stream<<i;
        /**打开摄像头**/
        fd = open(dev_camera.c_str(), O_RDWR);
        if (fd < 0)
        {
          //  std::cout<<"Camera Info: capture open failed!"<<std::endl;
        }
        else
        {
            dev_camera = "/dev/video" + stream.str();
            /**设置设备属性**/
            int ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
            if(ret < 0)
            {
                std::cout<<"Youibot Camera: ioctl querycap!\n";
            }else
            {
                const unsigned char* card = cap.card;
                std::cout <<GREEN<< "dev: " << dev_camera << " | name:" << card << std::endl;
            }
        }
    }

}
