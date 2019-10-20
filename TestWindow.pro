#-------------------------------------------------
#
# Project created by QtCreator 2019-08-27T20:07:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SuperMeasringTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        commonfunc.cpp \
        config.cpp \
        knnopencv.cpp \
        main.cpp \
        aimwindow.cpp \
        win32hook.cpp

HEADERS += \
        aimwindow.h \
        commonfunc.h \
        config.h \
        knnopencv.h \
        win32hook.h

FORMS += \
        aimwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

OPENCV_PATH=E:\opencv-4.1.1\win_Build\install

win32 {
        INCLUDEPATH     += $${OPENCV_PATH}\include

        CONFIG(debug , debug | release):{
                LIBS            += $${OPENCV_PATH}\x64\mingw\bin\libopencv_*.dll
        } else:CONFIG(release , debug | release):{
                LIBS            += -L$${OPENCV_PATH}\x64\mingw\bin \
                                   -llibopencv_core411 \
                                   -llibopencv_highgui411 \
                                   -llibopencv_imgcodecs411 \
                                   -llibopencv_imgproc411 \
                                   -llibopencv_features2d411 \
                                   -llibopencv_calib3d411
       }
}

DISTFILES += \
    icon/favicon128_128.ico \

RC_ICONS =  icon/favicon128_128.ico


