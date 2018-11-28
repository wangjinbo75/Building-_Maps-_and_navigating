#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QPixmap>
#include <QImage>
#include <QPainter>

//#include <image.h>
char yuvfilename[11] = {'r','c','q','0','0','0','.','y','u','v','\0'};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
       ui->setupUi(this);
      // CapturePhotos();  抓怕照片  记得设置 force_format = 1 ;
       vd    = new Camera();
       frame = new QImage(vd->frame_buffer,640,480,QImage::Format_RGB888);
       //state = 1;
        S = "0";
        state= 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete vd;
    delete frame;
}
void MainWindow::paintEvent(QPaintEvent *)
{

    value = vd->GetFrame(&vd->buffer_pointer,&vd->len);
    value = vd->yuyv_to_rgb888();
    frame->loadFromData(vd->frame_buffer,640*480 * 3);
    ui->label->setPixmap(QPixmap::fromImage(*frame,Qt::AutoColor));
    value=vd->FreeFrame();

    //test
    S = QString::number(state, 10);
    ui->test_label->setText(S);
    state++;
    if(state == 333 )
    {

        value= vd->StopCapturing();
        value= vd->CloseCamera();
    }
    //    if(state == 1)
    //    {
    //        H264file  = fopen(yuvfilename,"wb+");
    //        state =2 ;

    //    }
}
void  MainWindow::pushButton_clicked()
{

}
int MainWindow::CapturePhotos()
{
    QPixmap pix;  //用来显示图片
    QByteArray aa ;

    BITMAPFILEHEADER   bf;
    BITMAPINFOHEADER   bi;
    //Set BITMAPINFOHEADER
    bi.biSize = 40;
    bi.biWidth = IMAGEWIDTH;
    bi.biHeight = IMAGEHEIGHT;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = 0;
    bi.biSizeImage = IMAGEWIDTH*IMAGEHEIGHT*3;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;


    //Set BITMAPFILEHEADER
    bf.bfType = 0x4d42;
    bf.bfSize = 54 + bi.biSizeImage;
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits = 54;

    Camera Cam;

    aa.append((char *)&bf,14);
    aa.append((char *)&bi,40);
    aa.append((char *)Cam.frame_buffer,640*480*3);
    pix.loadFromData(aa);
    ui->label->setPixmap(pix);

    Cam.CloseCamera();
    return true;
}
