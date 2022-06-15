#ifndef MIL_H
#define MIL_H
#define MAX_DISTANCE 1250
#define MIN_DISTANCE  50

#include <QSettings>
#include <QStringList>
#include <QList>
#include <commonfunc.h>

class Config
{
public:
    Config();
    ~Config();
    enum type{PJP=1, VPJP, HJC, MQG, MQGS, PJPSCATTER};
    qreal getPJPMil(qint16 Weapon, qreal distance);
    QVariant getConfigValue(QString key);
    QVariant getConfigValue(QString Session , QString key);
    QRect getAutoNumerArea();
    QRect getAutoLineArea();
private:
    qreal countMil(QPointF* A, QPointF *B  , qreal distance);
    QList<QPointF *> m_PjpMilList;
    QList<QPointF *> m_PjpScatterList;
    QList<QPointF *> m_vPjpMilList;
    QList<QPointF *> m_HjcMilList;
    QList<QPointF *> m_MqgMilList;
    QList<QPointF *> m_MqgSMilList;
    QSettings * m_config ;
};

#endif // MIL_H
