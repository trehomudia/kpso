#include "mutex_test2.h"
#include "mutex_main.h"

void testDate::testValidity()
{
  //QFETCH(MUTEX, pMutex);
  MUTEX* pMutex = new MUTEX;
  QCOMPARE(InitMutex(pMutex), 0);
  delete pMutex;
    // 11 March 1967
    //QDate date( 1967, 3, 11 );
    //QVERIFY( date.isValid() );
}

void testDate::testMonth()
{
    // 11 March 1967
    //QDate date;
    //date.setYMD( 1967, 3, 11 );
    //QCOMPARE( date.month(), 3 );
    //QCOMPARE( QDate::longMonthName(date.month()),
              //QString("March") );
}


QTEST_MAIN(testDate)
