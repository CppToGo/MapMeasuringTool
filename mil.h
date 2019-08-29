#ifndef MIL_H
#define MIL_H
#define MAX_DISTANCE 1250
#define MIN_DISTANCE  50

#include <QSettings>
#include <QStringList>
#include <QList>

class mil
{
public:
  mil();
  ~mil();
  int getMil(int A1 , int B1, int A2, int B2 , int distance);
private:
  QList<QPoint *> m_milList;
};

#endif // MIL_H
