#include "aimwindow.h"
#include "ui_aimwindow.h"
#include "win32hook.h"

extern HHOOK keyHook;
AimWindow* AimWindow::m_Instance = nullptr ;

AimWindow::AimWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AimWindow),
    m_isPressLB(false),
    m_isPressRB(false),
    m_pen(new QPen()),
    m_line(new QLine(0,0,0,0)),
    m_ruler(50)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_pen->setColor(QColor(255,0,0,255));
    m_pen->setStyle(Qt::SolidLine);
    m_pen->setWidth(3);
#if WIN32
    m_minAlpha = 1;
#else
    m_minAlpha = 15;
#endif
    m_alpha = m_minAlpha;
    hookOpen(); //开启钩子
    qDebug()<< "start AimWindow()";
}

AimWindow::~AimWindow()
{
    delete ui;
    delete m_pen;
    delete m_line;
    hookClose();
    qDebug()<< "quit ~AimWindow";
}

void AimWindow::mousePressEvent(QMouseEvent *e){
    if (e->button() == Qt::RightButton){
        m_isPressRB = !m_isPressRB ;
    }
    if (e->button() == Qt::LeftButton){
        m_isPressLB = !m_isPressLB ;
        m_line->setP1(e->pos());
        update();
    }

}

void AimWindow::mouseReleaseEvent(QMouseEvent *e){
    if (e->button() == Qt::RightButton){
        m_isPressRB = !m_isPressRB ;
    }
    if (e->button() == Qt::LeftButton){
        m_isPressLB = !m_isPressLB ;
    }
}
void AimWindow::mouseMoveEvent(QMouseEvent *e){
    if (m_isPressLB){
        m_line->setP2(e->pos());
        update();
    }
    if (e->button() == Qt::RightButton){

    }
}
void AimWindow::keyPressEvent(QKeyEvent *e){
    if (e->key() == Qt::Key_Space){
        m_alpha = m_alpha > 0 ? 0 : m_minAlpha;
        update();
    }
}

void AimWindow::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    painter.fillRect(this->rect() , QColor(255,255,255,m_alpha));
    painter.setPen(*m_pen);
    painter.drawLine(this->width() - 100 - m_ruler , this->height()-100 ,this->width() - 100 , this->height() -100);
    painter.drawLine(*m_line);
    painter.drawText(m_line->p2().x() , m_line->p2().y()-10 ,  "* 33 = "  + QString::number(this->countDistance() * 33));
    painter.drawText(m_line->p2().x() , m_line->p2().y()    ,  "*100 = " + QString::number(this->countDistance() * 100));
    painter.drawText(m_line->p2().x() , m_line->p2().y()+10  , "*300 = " + QString::number(this->countDistance() * 300));
    painter.drawText(m_line->p2().x() , m_line->p2().y()+20  , "Angle= " + QString::number(this->countAngle()));
}

double AimWindow::countDistance(){
    double length = sqrt(m_line->dx() * m_line->dx() + m_line->dy() * m_line->dy()) / m_ruler ;
    return length;
}

double AimWindow::countAngle(){
    //qDebug()<< m_line->dx() << m_line->dy();
    QLine line0(m_line->x1() , m_line->y1() ,  m_line->x1() , m_line->y1() - 100);
    double vMul = (m_line->dx() * line0.dx() + m_line->dy() * line0.dy());
    double cosa = vMul /
            (sqrt(m_line->dx()*m_line->dx() + m_line->dy() * m_line->dy())* sqrt(line0.dx()*line0.dx() + line0.dy()*line0.dy()));
    // double sina = m_line->dx() / sqrt((m_line->dx()*m_line->dx() + m_line->dy() * m_line->dy()));
    qDebug() << cosa << vMul ;
    double radian = acos(cosa);
    double angle = radian * 180 / Pi ;
    if (m_line->dx() > 0){
       angle = 360 -  angle;
    }
    return  angle ;
}


void AimWindow::wheelEvent(QWheelEvent *e){
    if (e->delta() > 0 ){
        m_ruler ++ ;
    }else if (e->delta() < 0 && m_ruler > 0 ){
        m_ruler -- ;
    }
    update();
}

AimWindow* AimWindow::getInstance(){
    if (m_Instance == nullptr){
        m_Instance = new AimWindow();
    }
    return m_Instance ;
}

void AimWindow::delInstance(){
    if (m_Instance != nullptr){
        delete m_Instance ;
        m_Instance = nullptr;
    }
}

void AimWindow::hookOpen(){
    if (keyHook == nullptr){
        keyHook = loadKeyHook();
    }
}

void AimWindow::hookClose(){
    if (keyHook != nullptr){
        unLoadHook(keyHook);
    }
}

void AimWindow::setAlphaValue(){
    m_alpha = m_alpha > 0 ? 0 : m_minAlpha;
    update();
}
