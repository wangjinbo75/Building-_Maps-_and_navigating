#ifndef MAINWINDOW_H
#define MAINWINDOW_H


//不停闪退

#include <QMainWindow>
#include "camera_data.h"
#define  WIDTH    640
#define  HEIGHT   480
namespace Ui {
class MainWindow;
class Camera;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int CapturePhotos();
   // int convert_yuv_to_rgb_buffer();
private:
    Ui::MainWindow *ui;


    QImage *frame;
    Camera * vd;
    int value;
    int state;
    FILE   * file;
    QString S;
    
public slots:              //槽意味着任何对象都可将信号与之相连接
    void pushButton_clicked();
    void paintEvent(QPaintEvent *);

};

#endif // MAINWINDOW_H
