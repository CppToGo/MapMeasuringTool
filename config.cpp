#include "config.h"
#include <QDebug>
#include <QCoreApplication>
Config::Config()
{
  m_config = new QSettings(QCoreApplication::applicationDirPath() +"/config/config.ini", QSettings::IniFormat);
  m_config->beginGroup("MIL");
  //qDebug()<< "config" << config->allKeys();
  for (int i = MIN_DISTANCE ; i <= MAX_DISTANCE ; i ++ ){
        if (m_config->value(QString::number(i)).toInt() > 0 ){
          m_milList.append(new QPointF(i , m_config->value(QString::number(i)).toDouble() ));
      }
    }
  m_config->endGroup();
  //qDebug()<< "config" << config->allKeys();
//    foreach (QPointF* key, m_milList) {
//            qDebug()<< key->rx() << "\t"<<  key->ry() ;
//    }

}

Config::~Config(){
    qDeleteAll(m_milList);
    delete m_config;
    m_config = nullptr ;
}

qreal Config::countMil(QPointF* A, QPointF *B , qreal distance){
  return (distance - A->rx())*(B->ry() - A->ry())/(B->rx() - A->rx()) + A->ry() ;
}

qreal Config::getMil(qreal distance){
    //qDebug()<< "m_mlist size" << m_milList.size() ;
    for(int i = 0 ; i < m_milList.size() - 1; i ++){
        if (m_milList.at(i)->rx() < distance && m_milList.at(i+1)->rx() > distance){
            return countMil(m_milList.at(i), m_milList.at(i+1), distance );
        }
        if (qFuzzyIsNull(m_milList.at(i)->rx() - distance)){
            return m_milList.at(i)->ry();
        }
        if (qFuzzyIsNull(m_milList.at(i+1)->rx() - distance)){
            return m_milList.at(i+1)->ry();
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

















