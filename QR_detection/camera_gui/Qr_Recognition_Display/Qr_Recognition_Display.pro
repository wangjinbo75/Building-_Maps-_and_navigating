#-------------------------------------------------
#
# Project created by QtCreator 2018-12-02T12:57:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qr_Recognition_Display
TEMPLATE = app

INCLUDEPATH +=               /usr/local/include/opencv \
               /usr/local/include/opencv2 \
               /usr/local/include/zbar
#  opencv

LIBS += /usr/local/lib/libopencv_core.so.3.4
LIBS += /usr/local/lib/libopencv_highgui.so.3.4
LIBS += /usr/local/lib/libopencv_core.so.3.4
LIBS += /usr/local/lib/libopencv_highgui.so.3.4
LIBS += /usr/local/lib/libopencv_calib3d.so.3.4
LIBS += /usr/local/lib/libopencv_features2d.so.3.4
LIBS += /usr/local/lib/libopencv_flann.so.3.4
LIBS += /usr/local/lib/libopencv_imgcodecs.so.3.4
LIBS += /usr/local/lib/libopencv_imgproc.so.3.4
LIBS += /usr/local/lib/libopencv_ml.so.3.4
LIBS += /usr/local/lib/libopencv_objdetect.so.3.4
LIBS += /usr/local/lib/libopencv_video.so.3.4
LIBS += /usr/local/lib/libopencv_videoio.so.3.4
LIBS += /usr/local/lib/libopencv_shape.so.3.4
#  zbar
LIBS += -L /usr/local/lib -lzbar -lpng

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
