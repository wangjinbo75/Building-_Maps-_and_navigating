#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "header.h"

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
    int ScanImage();

private:
    Ui::MainWindow *ui;
    QRLocation *QR; // 创建类指针
    cv::VideoCapture *Cam;
    CvCapture *cam;     // 视频获取结构， 用来作为视频获取函数的一个参数
};

#endif // MAINWINDOW_H
