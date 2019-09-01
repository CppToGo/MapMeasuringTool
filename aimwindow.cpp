#include "aimwindow.h"
#include "ui_aimwindow.h"
#include "win32hook.h"
#include <QCursor>
#include <QBitmap>
#include <QPolygonF>

extern HHOOK keyHook;
AimWindow* AimWindow::m_Instance = nullptr ;

AimWindow::AimWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AimWindow),
    m_isPressLB(false),
    m_isPressRB(false),
    m_pen(new QPen(QColor(255,0,0,150) , 3 , Qt::SolidLine , Qt::FlatCap, Qt::MiterJoin)),
    m_line(new QLineF(0,0,0,0)),
    m_normalLine(new QLineF(0,0,0,-100)),
    m_ruler(100),
    m_config(new Config()),
    m_isUsingTool(true)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );//无边框  //置顶，还是只有换成win32的API
    setAttribute(Qt::WA_TranslucentBackground );//透明背景
    setMouseTracking(false);
    QBitmap bitmap = QBitmap(QCoreApplication::applicationDirPath() + "/icon/Cursor_cross.png");//光标加载
    QBitmap bitmap_mask = QBitmap(QCoreApplication::applicationDirPath() + "/icon/Cursor_cross_mask.png");
    setCursor(QCursor(bitmap , bitmap_mask, -1, -1));

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
    delete m_config;
    delete m_normalLine;
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
        m_normalLine->setLine(m_line->x1() , m_line->y1() ,  m_line->x1() , m_line->y1() - m_line->length()); //设置法线
        update();
    }
    if (e->button() == Qt::RightButton){

    }
}
void AimWindow::keyPressEvent(QKeyEvent *e){
    qDebug()<<  e->key() << VK_F1 ;
    if (e->key() == m_config->getConfigValue("KeyBoard", "33mRuler").toInt()){
        m_ruler =  m_line->length() * 100 / 33 ;
    }
    if (e->key() == m_config->getConfigValue("KeyBoard", "100mRuler").toInt()){
        m_ruler =  m_line->length() ;
    }
    if (e->key() == m_config->getConfigValue("KeyBoard", "300mRuler").toInt()){
        m_ruler =  m_line->length() * 100 / 300 ;
    }
    if (e->key() == m_config->getConfigValue("KeyBoard", "900mRuler").toInt()){
        m_ruler =  m_line->length() * 100 / 900 ;
    }
    update();
}

void AimWindow::paintEvent(QPaintEvent *e){
    //计算出所需数据
    double Dis = m_line->length() / m_ruler * 100.0 ;
    double Ang = this->countAngle() ;
    double Mil =  m_config->getMil(Dis);



    QPainter painter(this);
    painter.save();
    painter.fillRect(this->rect() , QColor(255,255,255,m_alpha));
    painter.setPen(*m_pen);
    painter.drawLine(this->width() - 100 - m_ruler , this->height()-50 ,this->width() - 100 , this->height() -50);
    if(m_isUsingTool && m_line->length() > 0  ){
        painter.drawLine(*m_line);
        //计算三角形的三个点
        QLineF v = m_line->unitVector();
        v.setLength(15);
        //qDebug()<< v.length();
        v.translate(m_line->dx() - v.dx() , m_line->dy() - v.dy());
        QLineF n = v.normalVector();
        //n.setLength(n.length() *0.5);
        n.translate(n.dx() / -2.0 , n.dy() / -2.0);
        QPolygonF arrow = QPolygonF();
        arrow.append(m_line->p2());
        arrow.append(n.p2());
        arrow.append(n.p1());
        arrow.append(m_line->p2());
        QPainterPath path ;
        path.addPolygon(arrow);
        painter.setBrush(QBrush(QColor(m_pen->color())));
        painter.drawPath(path);
    }
    painter.restore();

    painter.save();
    painter.setFont(QFont("Arial", 10));
    painter.setPen(QColor(255, 0 ,0));
    painter.drawText(this->width() - 200  , this->height()-  65 , "距离 = " + QString::number(Dis) + " m");
    painter.drawText(this->width() - 200  , this->height()-  80 , "方位 = " + QString::number(Ang) + " °");
    painter.drawText(this->width() - 200  , this->height()-  95 , "密位 = " + QString::number(Mil) +" mil");
    painter.drawText(this->width() - 200  , this->height()-  35 , "标尺 = " + QString::number(m_ruler) + " px" );
    painter.restore();

    if (m_isPressRB){
        painter.save();
        painter.setFont(QFont("Arial Black", 10));
        painter.setPen(QColor(0, 0 ,255,150));
        //if you contributed for this project, you can put your name here.
        painter.drawText(0,10,"Made by Johnny_焦尼");
        painter.restore();
    }
}

double AimWindow::getDistance(){
    return  m_line->length();
}

double AimWindow::countAngle(){
    //qDebug()<< m_line->dx() << m_line->dy();
    if ( qFuzzyIsNull(m_line->length())){
        return 0;
    }
    double vMul = (m_line->dx() * m_normalLine->dx() + m_line->dy() * m_normalLine->dy());
    double cosa = vMul /(m_line->length() * m_line->length());
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

void AimWindow::setAlphaValue(){ //设置透明背景
    m_alpha = m_alpha > 0 ? 0 : m_minAlpha;
    m_isUsingTool = !m_isUsingTool ;
    update();
}




















