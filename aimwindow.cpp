#include "aimwindow.h"
#include "ui_aimwindow.h"
#include "win32hook.h"
#include <QDir>
extern HHOOK keyHook;
AimWindow* AimWindow::m_Instance = nullptr ;

AimWindow::AimWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AimWindow),
    m_isPressLB(false),
    m_isPressRB(false),
    m_pen(new QPen(QColor(255,0,0,150) , 3 , Qt::SolidLine , Qt::RoundCap, Qt::RoundJoin)),
    m_line(new QLine(0,0,0,0)),
    m_ruler(100),
    m_distance(0),
    m_mil(new Mil())
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );//无边框  //置顶，还是只有换成win32的API
    setAttribute(Qt::WA_TranslucentBackground );//透明背景

    setMouseTracking(false);
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
    delete m_mil;
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
        m_distance = countDistance();
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
        m_distance = countDistance();
        update();
    }
    if (e->button() == Qt::RightButton){

    }
}
void AimWindow::keyPressEvent(QKeyEvent *e){
//    if (e->key() == Qt::Key_Space){
//        m_alpha = m_alpha > 0 ? 0 : m_minAlpha;
//        update();
//    }
}

void AimWindow::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    painter.save();
    painter.fillRect(this->rect() , QColor(255,255,255,m_alpha));
    painter.setPen(*m_pen);
    painter.drawLine(this->width() - 100 - m_ruler , this->height()-50 ,this->width() - 100 , this->height() -50);
    painter.drawLine(*m_line);
    painter.restore();

    painter.save();
    painter.setFont(QFont("Arial Rounded MT Bold", 10));
    painter.setPen(QColor(255, 0 ,0));
    painter.drawText(this->width() - 200   ,  this->height() - 35, "（100m）标尺 = " + QString::number(m_ruler) + " px" );
    painter.drawText(m_line->p2().x() + 10 , m_line->p2().y()+15  , "距离 = " + QString::number(m_distance / m_ruler * 100) + " m");
    painter.drawText(m_line->p2().x() + 10 , m_line->p2().y()     , "方位 = " + QString::number(this->countAngle()) + " °");
    painter.drawText(m_line->p2().x() + 10 , m_line->p2().y()-15  , "密位 = " + QString::number(m_mil->getMil(m_distance / m_ruler * 100)) +" mil");
    painter.restore();

    if (m_isPressRB){
        painter.save();
        painter.setFont(QFont("Arial Rounded MT Bold", 10));
        painter.setPen(QColor(0, 0 ,255,150));
        //if you contributed for this project, you can put your name here.
        painter.drawText(0,10,"Made by Johnny_焦尼");
        painter.restore();
    }
}

double AimWindow::countDistance(){
    return sqrt(m_line->dx() * m_line->dx() + m_line->dy() * m_line->dy());
}

double AimWindow::countAngle(){
    //qDebug()<< m_line->dx() << m_line->dy();
    QLine line0(m_line->x1() , m_line->y1() ,  m_line->x1() , m_line->y1() - 100);
    double vMul = (m_line->dx() * line0.dx() + m_line->dy() * line0.dy());
    double cosa = vMul /(m_distance * 100);
    // double sina = m_line->dx() / sqrt((m_line->dx()*m_line->dx() + m_line->dy() * m_line->dy()));
    //qDebug() << cosa << vMul ;
    double radian = acos(cosa);
    double angle = radian * 180 / Pi ;
    if (m_line->dx() < 0){
       angle = 360 -  angle;
    }
    return  angle ;
}


void AimWindow::wheelEvent(QWheelEvent *e){
    if (e->delta() > 0){
        if ( e->modifiers() & Qt::ShiftModifier){
            m_ruler += 0.1 ;
          }else {
            m_ruler += 5 ;
          }
    }else if (e->delta() < 0 && m_ruler > 1 ){
        if (e->modifiers() & Qt::ShiftModifier){
            m_ruler -= 0.1 ;
          }else{
            m_ruler -= 5 ;
          }
    }
    if(m_ruler <=0){
        m_ruler = 1;
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




















