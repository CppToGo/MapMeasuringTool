#ifndef MIL_H
#define MIL_H
#define MAX_DISTANCE 1250
#define MIN_DISTANCE  50

#include <QSettings>
#include <QStringList>
#include <QList>

class Config
{
public:
    Config();
    ~Config();

    qreal getMil(qreal distance);
    QVariant getConfigValue(QString key);
    QVariant getConfigValue(QString Session , QString key);
    QRect getAutoNumerArea();
    QRect getAutoLineArea();
private:
    qreal countMil(QPointF* A, QPointF *B  , qreal distance);
    QList<QPointF *> m_milList;
    QSettings * m_config ;
};

#endif // MIL_H
