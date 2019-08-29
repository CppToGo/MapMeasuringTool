#ifndef MIL_H
#define MIL_H
#define MAX_DISTANCE 1250
#define MIN_DISTANCE  50

#include <QSettings>
#include <QStringList>
#include <QList>

class Mil
{
public:
    Mil();
    ~Mil();

    int getMil(double distance);
private:
    int countMil(QPoint* A, QPoint *B  , int distance);
    QList<QPoint *> m_milList;
};

#endif // MIL_H
