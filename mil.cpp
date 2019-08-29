#include "mil.h"
#include <QDebug>

mil::mil()
{
  QSettings * config = new QSettings("Mil.ini", QSettings::IniFormat);
  for (int i = MIN_DISTANCE ; i <= MAX_DISTANCE ; i +=25 ){
      m_milList.append(new QPoint(i , config->value("/MIL/"+ QString::number(i)).toInt()));
    }
  foreach (QPoint* key, m_milList) {
     qDebug()<< key->rx() << key->ry() ;
    }
  delete config;
  config = nullptr ;
}

mil::~mil(){

}

int mil::getMil(int A1, int B1, int A2, int B2 , int distance){
  return (distance - A1)*(B2 - B1)/(A2 - A1) + B1 ;
}
