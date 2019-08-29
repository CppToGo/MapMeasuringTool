#include "aimwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AimWindow* w = AimWindow::getInstance();
    w->showMaximized();

    return a.exec();
}
