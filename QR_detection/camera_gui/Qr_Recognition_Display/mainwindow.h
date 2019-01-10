#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <sstream>

#include <boost/limits.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/videoio.hpp>

#include <zbar.h>
#include <zbar/QZBar.h>


#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>

//二维码倾斜阈值
#define QRLOCATION_INCLINATION_THRESHOLD  0.1

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);//
    ~MainWindow();
   //全局变量
    static double DisX;
    double QrSize    ; //实际二维码尺寸


    //摄像头坐标
    int GetCoordinateX[4];
    int GetCoordinateY[4];
    double DisXYZ[3];

public slots:
    void paintEvent(QPaintEvent *);
    void on_StartAndStop_clicked();
    void GetOneFrameAndDetectQR();

private:
    Ui::MainWindow *ui;
    cv::Mat image;
    cv::VideoCapture *Cam;

    QTimer    *VideoTimer;   //时间
    CvCapture *cam;     // 视频获取结构， 用来作为视频获取函数的一个参数
    IplImage  *frame;   //申请IplImage类型指针，就是申请内存空间来存放每一帧图像
    IplImage  *input;
    zbar::ImageScanner *scanner;  //zbar
    QString S;
    int state;
    int value;
    QPushButton *aButton;

    double * QRDistance(int * GetCoordinateX,int *GetCoordinateY,cv::Mat grayFrame,double QrSize);

};

//#if defined(__cplusplus)
//}
//#endif

#endif // MAINWINDOW_H
