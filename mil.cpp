#include "mil.h"
#include <QDebug>

Mil::Mil()
{
  QSettings * config = new QSettings("../Mil.ini", QSettings::IniFormat);
  config->beginGroup("MIL");
  //qDebug()<< "config" << config->allKeys();
  for (int i = MIN_DISTANCE ; i <= MAX_DISTANCE ; i ++ ){
        if (config->value(QString::number(i)).toInt() > 0 ){
          m_milList.append(new QPoint(i , config->value(QString::number(i)).toInt() ));
      }
    }
  config->endGroup();
  //qDebug()<< "config" << config->allKeys();
    foreach (QPoint* key, m_milList) {
            qDebug()<< key->rx() << "\t"<<  key->ry() ;
    }
    delete config;
    config = nullptr ;
}

Mil::~Mil(){
    qDeleteAll(m_milList);
}

int Mil::countMil(QPoint* A, QPoint *B , int distance){
  return (distance - A->rx())*(B->ry() - A->ry())/(B->rx() - A->rx()) + A->ry() ;
}

int Mil::getMil(double distance){
    qDebug()<< "m_mlist size" << m_milList.size() ;
    for(int i = 0 ; i < m_milList.size() - 1; i ++){
        if (m_milList.at(i)->rx() < distance && m_milList.at(i+1)->rx() > distance){
            return countMil(m_milList.at(i), m_milList.at(i+1), (int)distance );
        }
        if (m_milList.at(i)->rx() == (int)distance){
            return m_milList.at(i)->ry();
        }
        if (m_milList.at(i+1)->rx() == (int)distance){
            return m_milList.at(i+1)->ry();
        }
    }
    return 0; //表示找不到密位
}




















