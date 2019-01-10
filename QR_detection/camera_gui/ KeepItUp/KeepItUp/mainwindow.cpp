#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cam = cvCreateCameraCapture(0);//打开摄像头，从摄像头中获取视频

    QR = new QRLocation();//调用Class类的构造方法构造一个Class 对象其名字为x.
    QR->init(0.74,true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
int MainWindow::ScanImage()
{
  return true;
}
