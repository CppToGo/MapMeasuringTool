#ifndef AIMWINDOW_H
#define AIMWINDOW_H

#include <QWidget>
#include <QPen>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainter>
#include <math.h>
#include <QtDebug>
#include <config.h>
#include <knnopencv.h>
#include <websocket.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <httpserver.h>
#include <commonfunc.h>

#define Pi 3.1415926

namespace Ui {
class AimWindow;
}



class AimWindow : public QWidget
{
    Q_OBJECT

public:
    ~AimWindow();
    static AimWindow* getInstance();
    static void delInstance();
    void setAlphaValue();
    double getDistance();
    double countAngle();
    void paintArrowLine(QPainter& painter, QLineF& line);
private:
    explicit AimWindow(QWidget *parent = nullptr);

    Ui::AimWindow *ui;
    bool m_isPressLB;
    bool m_isPressRB;
    QPen * m_pen ;
    QLineF *m_line ;
    QLineF *m_normalLine;
    double m_ruler;
    double m_Old_Ruler;
    char m_alpha;
    char m_minAlpha;
    static AimWindow* m_Instance ;
    Config * m_config;
    KnnOpencv *m_knn ;
    bool m_isUsingTool ;
    bool m_AutoStatu_len ;
    bool m_AutoStatu_type ;
    bool m_Auto_lock;
    QHostAddress m_IPAddress;
    websocket* m_websocketServer;
    HttpServer* m_httpServer;
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *e);
    void paintEvent(QPaintEvent *e);
    void hookOpen();
    void hookClose();
};
#endif // AIMWINDOW_H
