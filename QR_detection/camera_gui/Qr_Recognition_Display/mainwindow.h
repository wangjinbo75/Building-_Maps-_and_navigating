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
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_start_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat image;
    cv::VideoCapture *Cam;
   // cv::Mat frame;
    int GetCoordinateX[4];
    int GetCoordinateY[4];


    QTimer    *timer;
    CvCapture *cam;// 视频获取结构， 用来作为视频获取函数的一个参数
    IplImage  *frame;//申请IplImage类型指针，就是申请内存空间来存放每一帧图像
    IplImage  *input;
    zbar::ImageScanner *scanner;  //zbar
    QString S;
    int state;
    int value;
    //摄像头上下视角
    int QRDistance(int GetCoordinateX[4],int  GetCoordinateY[4],cv::Mat grayFrame); //  得到四个坐标 结算出来剧摄像头距离


};

//#if defined(__cplusplus)
//}
//#endif

#endif // MAINWINDOW_H
