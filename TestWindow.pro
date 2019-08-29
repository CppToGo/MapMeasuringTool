#-------------------------------------------------
#
# Project created by QtCreator 2019-08-27T20:07:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestWindow
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
        main.cpp \
        aimwindow.cpp \
        win32hook.cpp \
    mil.cpp

HEADERS += \
        aimwindow.h \
        win32hook.h \
    mil.h

FORMS += \
        aimwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icon/favicon128_128.ico \
    icon/favicon16_16.ico \
    icon/favicon32_32.ico \
    icon/favicon48_48.ico \
    icon/favicon64_64.ico

RC_ICONS =  icon/favicon128_128.ico
