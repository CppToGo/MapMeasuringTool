#include "config.h"
#include <QDebug>
#include <QCoreApplication>
Config::Config()
{
  m_config = new QSettings(QCoreApplication::applicationDirPath() +"/config/config.ini", QSettings::IniFormat);
  //迫击炮密位读取
  m_config->beginGroup("PJPMIL");
  for (int i = MIN_DISTANCE ; i <= MAX_DISTANCE ; i +=5 ){
        if (m_config->value(QString::number(i)).toDouble() > 0 ){
          m_PjpMilList.append(new QPointF(i , m_config->value(QString::number(i)).toDouble() ));
      }
    }
  m_config->endGroup();
  //车载迫击炮读取
  m_config->beginGroup("VPJPMIL");
  for (int i = MIN_DISTANCE ; i <= MAX_DISTANCE ; i +=5 ){
        if (m_config->value(QString::number(i)).toDouble() > 0 ){
          m_vPjpMilList.append(new QPointF(i , m_config->value(QString::number(i)).toDouble() ));
      }
    }
  m_config->endGroup();
  //多管火箭车密位读取
  m_config->beginGroup("HJCMIL");
  for (int i = MIN_DISTANCE ; i <= MAX_DISTANCE ; i +=5 ){
        if (m_config->value(QString::number(i)).toDouble() > 0 ){
          m_HjcMilList.append(new QPointF(i , m_config->value(QString::number(i)).toDouble() ));
      }
    }
  m_config->endGroup();
  //煤气罐密位读取（抛射）
  m_config->beginGroup("MQGMIL");
  for (int i = MIN_DISTANCE ; i <= MAX_DISTANCE ; i ++ ){
        if (m_config->value(QString::number(i)).toDouble() > 0 ){
          m_MqgMilList.append(new QPointF(i , m_config->value(QString::number(i)).toDouble() ));
      }
    }
  m_config->endGroup();

  //煤气罐密位读取（直射）
  m_config->beginGroup("MQGSMIL");
  for (int i = MIN_DISTANCE ; i <= MAX_DISTANCE ; i ++ ){
        if (m_config->value(QString::number(i)).toDouble() > 0 ){
          m_MqgSMilList.append(new QPointF(i , m_config->value(QString::number(i)).toDouble() ));
      }
    }
  m_config->endGroup();
  //qDebug()<< "config" << config->allKeys();
//    foreach (QPointF* key, m_milList) {
//            qDebug()<< key->rx() << "\t"<<  key->ry() ;
//    }

}

Config::~Config(){
    qDeleteAll(m_PjpMilList);
    delete m_config;
    m_config = nullptr ;
}

qreal Config::countMil(QPointF* A, QPointF *B , qreal distance){
  return (distance - A->rx())*(B->ry() - A->ry())/(B->rx() - A->rx()) + A->ry() ;
}

qreal Config::getPJPMil(qint16 Weapon, qreal distance){
    QList<QPointF *> temp ;
    switch (Weapon) {
    case PJP:
        temp = m_PjpMilList;
        break;
    case VPJP:
        temp = m_vPjpMilList;
        break;
    case HJC:
        temp = m_HjcMilList;
        break;
    case MQG:
        temp = m_MqgMilList;
        break;
    case MQGS:
        temp = m_MqgSMilList;
        break;
    }
    //qDebug()<< "m_mlist size" << m_milList.size() ;
    for(int i = 0 ; i < temp.size() - 1; i ++){
        if (temp.at(i)->rx() < distance && temp.at(i+1)->rx() > distance){
            return countMil(temp.at(i), temp.at(i+1), distance );
        }
        if (qFuzzyIsNull(temp.at(i)->rx() - distance)){
            return temp.at(i)->ry();
        }
        if (qFuzzyIsNull(temp.at(i+1)->rx() - distance)){
            return temp.at(i+1)->ry();
        }
    }
    return 0; //表示找不到密位
}

QVariant Config::getConfigValue(QString key){
    return m_config->value(key);
}

QVariant Config::getConfigValue(QString Session ,QString key){
    return m_config->value("/" + Session + "/" + key);
}


QRect Config::getAutoNumerArea(){
    m_config->beginGroup("Auto");
    QRect temp = QRect(getConfigValue("Number_X").toInt(),getConfigValue("Number_Y").toInt(),getConfigValue("Number_Width").toInt(),getConfigValue("Number_Height").toInt());
    m_config->endGroup();
    return temp;
}

QRect Config::getAutoLineArea(){
    m_config->beginGroup("Auto");
    QRect temp = QRect(getConfigValue("Line_X").toInt(),getConfigValue("Line_Y").toInt(),getConfigValue("Line_Width").toInt(),getConfigValue("Line_Height").toInt());
    m_config->endGroup();
    return  temp;
}














