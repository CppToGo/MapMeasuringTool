#include "aimwindow.h"
#include <QApplication>
#include <mil.h>
#if WIN32
#include <windows.h>
#endif
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    AimWindow* w = AimWindow::getInstance();
//    w->showMaximized();
//#if WIN32
//    SetWindowPos((HWND)w->winId(), HWND_TOPMOST,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE ); //使用win32 API 来让窗口置顶
//#endif
    mil * test = new mil();

    return a.exec();
}
