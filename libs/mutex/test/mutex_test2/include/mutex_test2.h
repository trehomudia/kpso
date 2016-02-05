#include <QtTest>
#include <QtCore>

#ifndef MUTEX_TEST1_H_
#define MUTEX_TEST1_H_

class testDate: public QObject
{
  Q_OBJECT
  private slots:
    void testValidity();
    void testMonth();
};

#endif