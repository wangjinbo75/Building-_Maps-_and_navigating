#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace zbar;
//using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cam     = NULL;
    MainWindow::QrSize  = 7.0;   //设置二维码的尺寸cm
    VideoTimer   = new QTimer(this);
    scanner = new ImageScanner();
    scanner->set_config(zbar::ZBAR_NONE,zbar::ZBAR_CFG_ENABLE, 1);

    cam = cvCreateCameraCapture(0);//打开摄像头，从摄像头中获取视频  // 0 摄像头设备号
    /*信号和槽*/
    connect(ui->StartAndStop, SIGNAL(on_StartAndStop_clicked()), this, SLOT(on_StartAndStop_clicked()));
    connect(VideoTimer, SIGNAL(timeout()), this, SLOT( GetOneFrameAndDetectQR()));  // 时间到，读取当前摄像头信息

    value = 0;//初始化
    VideoTimer->start(33);              // 开始计时，超时则发出timeout()信号
}

MainWindow::~MainWindow()
{
    cvReleaseCapture(&cam);//释放内存；
    delete ui;
}

//void MainWindow::on_pushButton_clicked()
//{
//    QString fileName=QFileDialog::getOpenFileName(this,tr("Open Image"), ".",tr("Image Files(*.png *.jpg *.jpeg)"));
//    image=cv::imread(fileName.toStdString());
//    QImage img = QImage((const unsigned char*)(image.data),
//                        image.cols, image.rows, QImage::Format_RGB888); //设定图像大小自适应label窗口的大小
//    img = img.scaled(ui->label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    ui->label->setPixmap(QPixmap::fromImage(img));
//}

/*
*2018-12-18
* fish
* 功能： 自动刷新
* 输入参数：
* 输出参数：
* 备注：
*/
void MainWindow::paintEvent(QPaintEvent *)
{

}
/*
 *
 * 2018-12-17
 * fish
 * 功能：按键 触发 事件timer
 * 输入参数：
 * 输出参数：
 * 备注：
 *
 */
void MainWindow::on_StartAndStop_clicked()
{
    if(ui->StartAndStop->text() == "Stop")
    {
        QString S = "Start";
        ui->StartAndStop->setText(S);
        VideoTimer->stop();  //暂停视频
    }
    else if(ui->StartAndStop->text() == "Start")
    {
        QString S = "Stop";
        ui->StartAndStop->setText(S);
        VideoTimer->start(33); //开始视频
    }
    else
    {
        ;
    }
}
/*
 * 2018-12-17
 * fish
 * 输入参数： 一帧图像
 * 输出参数： 识别二维码
 * 备注：
 *
 */
void MainWindow::GetOneFrameAndDetectQR()
{
    frame = cvQueryFrame(cam);// 从摄像头中抓取并返回每一帧
//    cvSaveImage("frame0.jpg",frame);
//    cv::Mat imageGray = cv::imread("frame0.jpg", 1);

//      IplImage* frame0  = cvCloneImage(frame);  //浅拷贝一下 内存会飙升
    cv::Mat InitialImage = cv::cvarrToMat(frame);  //cv MAT
//      cv::Mat partion_img(imageGray, cv::Rect(0, 0, 640,480));
//      cv::Mat resize(partion_img, imageGray,cv::Size(320, 0), 0, 0, cv::INTER_CUBIC);

    cv::Mat RGB2GRAYImg;
    cv::Mat DisplayImg;

    cv::cvtColor(InitialImage,RGB2GRAYImg,CV_BGR2GRAY);   //   转换成灰色 检查二维吗
    cv::cvtColor(InitialImage,DisplayImg,CV_BGR2RGB);    //    过滤颜色


    int width  = RGB2GRAYImg.cols;
    int height = RGB2GRAYImg.rows;
    uchar *raw = (uchar *) RGB2GRAYImg.data;
    Image imageZbar(width, height, "Y800", raw, width * height);
    scanner->scan(imageZbar); //掃描條碼

    Image::SymbolIterator symbol = imageZbar.symbol_begin();
    if (imageZbar.symbol_begin() == imageZbar.symbol_end())
    {
          cout << "width = "<< width <<"\n" ;
          cout << "rows = "<<  height<<"\n" ;
          cout << "data = "<<  &raw  <<"\n" ;
          cout << "Fail! Please check the picture again!" << endl;
    }

    for (int i=0; symbol != imageZbar.symbol_end(); ++symbol,i++)
    {
         cout<<"symbol ="<< i<<"\n";
        string type = symbol->get_type_name();
        string code = symbol->get_data();
        cout << "type：" << endl << type << endl << endl;
        cout << "code：" << endl << code << endl << endl;

       //获取定位点个数
        int pointCount = symbol->get_location_size();

        int max_loc_x = symbol->get_location_x(0);
        int max_loc_y = symbol->get_location_y(0);
        int min_loc_x = symbol->get_location_x(0);
        int min_loc_y = symbol->get_location_y(0);

        int thickness = 2;
                //遍历所有定位点
        for (int i = 0; i < pointCount; i++) {

                int cur_loc_x = symbol->get_location_x(i);
                int cur_loc_y = symbol->get_location_y(i);
                int next_loc_x = symbol->get_location_x((i + 1) % pointCount);
                int next_loc_y = symbol->get_location_y((i + 1) % pointCount);
                cout << '(' << cur_loc_x << ',' << cur_loc_y << ")" << endl;
                //转换全局变量
                MainWindow::GetCoordinateX[i] =cur_loc_x;
                MainWindow::GetCoordinateY[i] =cur_loc_y;

                //画线
                cv::line(DisplayImg, cv::Point(cur_loc_x, cur_loc_y), cv::Point(next_loc_x, next_loc_y), cv::Scalar(0,255,0),
                     thickness);

                if (cur_loc_x > max_loc_x) {
                    max_loc_x = cur_loc_x;
                }
                if (cur_loc_y > max_loc_y) {
                    max_loc_y = cur_loc_y;
                }
                if (cur_loc_x < min_loc_x) {
                    min_loc_x = cur_loc_x;
                }
                if (cur_loc_y < min_loc_y) {
                    min_loc_y = cur_loc_y;
                }
         }
        //内容
          cv::putText(DisplayImg, code, cv::Point(min_loc_x, min_loc_y),
                          cv::FONT_HERSHEY_SIMPLEX,
                          0.5, cv::Scalar(255,0,0),1);
//         value =QRDistance( GetCoordinateX , GetCoordinateY , DisplayImg);
        cout << min_loc_x << "," << min_loc_y << "," << max_loc_x << "," << max_loc_y << endl;
       //算出点的坐标
       double *DisXYZ;
       DisXYZ = QRDistance(MainWindow::GetCoordinateX,MainWindow::GetCoordinateY,DisplayImg,MainWindow::QrSize );

       string t = "";
       stringstream ss;
       ss<<"z= "<< DisXYZ[2] <<" a= "<<DisXYZ[0]<<" b= "<<DisXYZ[1];
       t = ss.str();
        //cv::String test = cv::String((int)z);
        //S =  QString::number(z,10,5);//同int
//                  cv::putText(DisplayImg, t, cv::Point(min_loc_x, min_loc_y),
//                                  cv::FONT_HERSHEY_SIMPLEX,
//                                  0.5, cv::Scalar(255,0,0),1);

    }
    //QImage image((const uchar*)image1.data, image1.cols, image1.rows,QImage::Format_Indexed8);   //  灰度图
    QImage image((const uchar*)DisplayImg.data, DisplayImg.cols, DisplayImg.rows,QImage::Format_RGB888);
    ui->VideoWindows->setPixmap(QPixmap::fromImage(image));  // 将图片显示到label上

    //test
    S = QString::number(state, 10);
    ui->Test_Label->setText(S);
    state++;
}

/*
 * 2018-12-5
 * fish
 * 输入参数： 二维码 x y 的坐标， 一帧图像, 实际二维码的大小
 * 输出参数： 二维码距离 摄像头的距离
 * 备注：    返回值必须全局变量  通过返回传入数组的指针的方式  直接返回距离值
 *
 */
double *MainWindow::QRDistance(int *GetCoordinateX,int *GetCoordinateY,cv::Mat grayFrame,double QrSize)
{
    int X[4];
    int Y[4];
    double hViewAngle = 0.74;
    double qrSize = QrSize;
    for(int i=0;i<4; i++ )
    {
        X[i] = GetCoordinateX[i];
        Y[i] = GetCoordinateY[i];
    }

    double leftH = Y[1] - Y[0]; //左边沿纵向差
    double rightH = Y[2] - Y[3];  //右边沿纵向差
    double leftW = abs(X[0] - X[1]);//左边沿横向差
    double rightW = abs(X[2] - X[3]);//右边沿横向差

    cout << "1  : \n";
    cout << leftH << " " << rightH << endl;
    cout << leftW << " " << rightW << endl;
//    //必须保证0点高于1点，3点高于2点
   if (leftH < 0 || rightH < 0)
         cout<<"必须保证1点高于0点，3点高于2点"<<endl;
//        return 0;

//    //不能太倾斜
//    if (max( leftW / leftH, rightW / rightH) > QRLOCATION_INCLINATION_THRESHOLD)
//           return false;
    //上下视角一半的正切值，因为一直要用，所以先计算出来

    double tanHalfView = tan(hViewAngle / 2);
    double leftLen = sqrt(leftH * leftH + leftW * leftW);
    double rightLen = sqrt(rightH * rightH + rightW * rightW);

    cout << "2  : \n";
    cout << leftLen << " " << rightLen << endl;
    cout << tanHalfView << "\n" ;
    //左边沿的深度
    int height = grayFrame.rows;
    int width = grayFrame.cols;
    double leftZ = height * qrSize / tanHalfView / 2 / leftLen;//右边沿的深度
    double rightZ = height * qrSize / tanHalfView / 2 / rightLen; //得到中心点的深度

    cout << "3  : \n";
     cout << height << " " << width << endl;
    cout << leftZ << " " << rightZ << endl;

    double z = (leftZ + rightZ) / 2;
    //计算b的正弦值
    double sinB = (leftZ - rightZ) / qrSize;//数值计算
    //ROS_INFO("sinB = %f, leftZ = %f, rightZ = %f, qrSize = %f\n", sinB, leftZ, rightZ, qrSize);
//    if (sinB > 1)
//        return false;
    //得到b
    double b = asin(sinB);
    //两条对角线的系数和偏移
    double k1 = (Y[2] - Y[0]) / (X[2] - X[0]);
    double b1 = (X[2] * X[0] - X[0] * Y[2]) / (X[2] - X[0]);
    double k2 = (Y[3] - Y[1]) / (X[3] - X[1]);
    double b2 = (X[3] * Y[1] - X[1] * Y[3]) / (X[3] - X[1]);
    //两条对角线交点的X坐标
    double crossX = -(b1 - b2) / (k1 - k2);
    //计算a的正切值
    double tanA = tanHalfView * (2 * crossX - width) / height;
    //得到a
    double a = atan(tanA);


    MainWindow::DisXYZ[0] = a;
    MainWindow::DisXYZ[1] = b;
    MainWindow::DisXYZ[2] = z;

    cout<<"a= "<<a <<" b= "<<b<<" z= "<<z<<"\n";

    return MainWindow::DisXYZ;
}
