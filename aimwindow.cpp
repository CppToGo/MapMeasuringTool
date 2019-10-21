#include "aimwindow.h"
#include "ui_aimwindow.h"
#include "win32hook.h"
#include <QCursor>
#include <QBitmap>
#include <QPolygonF>
#include <QDesktopWidget>

extern HHOOK keyHook;
AimWindow* AimWindow::m_Instance = nullptr ;

AimWindow::AimWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AimWindow),
    m_isPressLB(false),
    m_isPressRB(false),
    m_line(new QLineF(0,0,0,0)),
    m_normalLine(new QLineF(0,0,0,-100)),
    m_ruler(100),
    m_config(new Config()),
    m_knn(new KnnOpencv()),
    m_isUsingTool(true),
    m_AutoStatu_len(true),
    m_AutoStatu_type(true),
    m_Auto_lock(false)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );//无边框  //置顶，还是只有换成win32的API
    setAttribute(Qt::WA_TranslucentBackground );//透明背景
    setMouseTracking(false);
    QBitmap bitmap = QBitmap(QCoreApplication::applicationDirPath() + "/icon/Cursor_cross.png");//光标加载
    QBitmap bitmap_mask = QBitmap(QCoreApplication::applicationDirPath() + "/icon/Cursor_cross_mask.png");
    setCursor(QCursor(bitmap , bitmap_mask, -1, -1));
    m_pen = new QPen(QColor(255,0,0,150) , m_config->getConfigValue("Paint", "LineWidth").toInt() , Qt::SolidLine , Qt::FlatCap, Qt::MiterJoin);

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
    delete m_knn;
    hookClose();
    qDebug()<< "quit ~AimWindow";
}

void AimWindow::mousePressEvent(QMouseEvent *e){
    if (e->button() == Qt::RightButton){
        m_isPressRB = !m_isPressRB ;
//        QPixmap pix = QGuiApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId(),1721,884,150,10);
//        double ruler = m_knn->GetLineLength(pix);
//        if ( ruler > 0 ){
//            m_ruler = ruler;
//        }
    }
    if (e->button() == Qt::LeftButton){
        m_isPressLB = !m_isPressLB ;

        if (!m_Auto_lock){
            m_AutoStatu_len = true ;
            m_AutoStatu_type = true ;
            QPixmap pix = QGuiApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId());
            //qDebug() << m_config->getAutoLineArea()  << m_config->getAutoNumerArea() ;
            QPixmap Ruler_len = pix.copy(m_config->getAutoLineArea()) , Ruler_type = pix.copy(m_config->getAutoNumerArea());
            double ruler_len = m_knn->GetLineLength(Ruler_len);
            int ruler_type = m_knn->GetRuleType(Ruler_type);
            switch (ruler_type) {
            case 33:
                ruler_len *= 3 ;
                break;
            case 100:
                break;
            case 300:
                ruler_len /= 3 ;
                break;
            case 900:
                ruler_len /= 9 ;
                break;
            default:
                m_AutoStatu_type = false ;
                break;
            }
            if (ruler_len > 0 ){
                m_ruler = ruler_len;
            }else{
                 m_AutoStatu_len = false ;
            }

            m_Auto_lock = true;//自动识别加锁，防止每次点击都会自动识别，只有在下一次进入使用时，才会在 setAlphaValue（） 方法中恢复
            m_Old_Ruler = ruler_len ;
        }

        m_line->setP1(e->pos());
        m_normalLine->setLine(m_line->x1() , m_line->y1() ,  m_line->x1() , m_line->y1() - m_line->length()); //设置法线
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
    //qDebug()<<  e->key() << m_config->getConfigValue("KeyBoard", "100mRuler").toInt() ;
    if (!m_AutoStatu_len){
        if (m_line->length() < 30){ //不能设置太小，否则会造成配置低的电脑卡顿
            return ;
        }
        if (e->key() == m_config->getConfigValue("KeyBoard", "KeyRuler1").toInt()){
            m_ruler =  m_line->length() * 100 / m_config->getConfigValue("KeyBoard", "Ruler1").toDouble() ;
        }
        if (e->key() == m_config->getConfigValue("KeyBoard", "KeyRuler2").toInt()){
            m_ruler =  m_line->length() * 100 / m_config->getConfigValue("KeyBoard", "Ruler2").toDouble() ;
        }
        if (e->key() == m_config->getConfigValue("KeyBoard", "KeyRuler3").toInt()){
            m_ruler =  m_line->length() * 100 / m_config->getConfigValue("KeyBoard", "Ruler3").toDouble() ;
        }
    }else{
        switch (e->key()) {
        case 49:
            m_ruler = m_Old_Ruler ;
            break;
        case 51:
            m_ruler = m_Old_Ruler / 3;
            break;
        case 57:
            m_ruler = m_Old_Ruler / 9;
            break;
        default:
            break;
        }

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
    if (m_config->getConfigValue("Paint","isPaintRuler").toBool()){
        painter.drawLine(this->width() - 100 - m_ruler , this->height()-50 ,this->width() - 100 , this->height() -50);
    }
    if((m_isUsingTool | m_config->getConfigValue("Paint", "isPaintLine").toBool() )&& m_line->length() > 0  ){
        paintArrowLine(painter, *m_line);
//        painter.save();
        painter.setPen(QPen(m_pen->color(),m_pen->width(), Qt::DashLine , m_pen->capStyle(), m_pen->joinStyle()));
        if(m_config->getConfigValue("Paint","isPaintMaxCircle").toBool()){
            painter.drawEllipse(m_line->p1(),m_line->length(), m_line->length());
        }
        if(m_config->getConfigValue("Paint","isPaint100Circle").toBool()){
            for ( int i = 1 ; i <= (int)(Dis / 100) ; i ++ ){
                painter.drawText(m_line->p1().x() + m_ruler * i + 5 ,m_line->p1().y(), QString::number(i));
                painter.drawText(m_line->p1().x() - m_ruler * i - 10 ,m_line->p1().y(), QString::number(i));
                painter.drawText(m_line->p1().x() ,m_line->p1().y() + m_ruler * i + 11, QString::number(i));
                painter.drawText(m_line->p1().x() ,m_line->p1().y() - m_ruler * i - 5, QString::number(i));
                painter.drawEllipse(m_line->p1() , m_ruler * i,m_ruler * i);
            }
        }
//        painter.restore();
    }
    painter.restore();

    painter.save();
    painter.setFont(QFont(m_config->getConfigValue("Paint","FontStyle").toString(), m_config->getConfigValue("Paint","FontSize").toInt()));
    painter.setPen(QColor(255, 0 ,0));
    if (m_config->getConfigValue("Paint" , "isInfoFollowArrow").toBool()){
        painter.drawText(m_line->p2().x()  , m_line->p2().y() - m_config->getConfigValue("Paint","FontSize").toInt() - 5, "距离 = " + QString::number(Dis) + " m");
        painter.drawText(m_line->p2().x()  , m_line->p2().y()    , "方位 = " + QString::number(Ang) + " °");
        painter.drawText(m_line->p2().x()  , m_line->p2().y() + m_config->getConfigValue("Paint","FontSize").toInt() + 5, "密位 = " + QString::number(Mil) + " mil");
    }else{
        painter.drawText(this->width() - 260  , this->height()-  m_config->getConfigValue("Paint","FontSize").toInt() - 65 , "距离 = " + QString::number(Dis) + " m");
        painter.drawText(this->width() - 260  , this->height()-  m_config->getConfigValue("Paint","FontSize").toInt()*2 - 70  , "方位 = " + QString::number(Ang) + " °");
        painter.drawText(this->width() - 260 , this->height()-  m_config->getConfigValue("Paint","FontSize").toInt()*3 - 75  , "密位 = " + QString::number(Mil) + " mil");
    }
    QString text ;
    //painter.drawText(this->width() - 250  , this->height() -  50 + m_config->getConfigValue("Paint","FontSize").toInt() + 35 ,  "标尺 = " + QString::number(m_ruler) + " px" );
    painter.drawText(this->width() - 260  , this->height() - m_config->getConfigValue("Paint","FontSize").toInt() - 50 ,QString("Auto= %1").arg(m_AutoStatu_len? (m_AutoStatu_type?"正常":"快捷"):"手动"));
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

void AimWindow::paintArrowLine(QPainter &painter, QLineF &line){
    painter.drawLine(line);
    QLineF v = line.unitVector();
    v.setLength(10);
    //qDebug()<< v.length();
    v.translate(line.dx() - v.dx() , line.dy() - v.dy());
    QLineF n = v.normalVector();
    //n.setLength(n.length() *0.5);
    n.translate(n.dx() / -2.0 , n.dy() / -2.0);
    QPolygonF arrow = QPolygonF();
    arrow.append(line.p2());
    arrow.append(n.p2());
    arrow.append(n.p1());
    arrow.append(line.p2());
    QPainterPath path ;
    path.addPolygon(arrow);
    painter.save();
    painter.setBrush(QBrush(QColor(m_pen->color())));
    painter.drawPath(path);
    painter.restore();
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
    if(m_ruler <=30){
        m_ruler = 30;
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
    m_Auto_lock = false ; //解除自动识别锁
    update();
}




















