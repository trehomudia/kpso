#ifndef DECLARATION_H
#define DECLARATION_H

#include <QVector>

////QList<int> cashList = QList<int>() << 10 << 10 << 20 << 40 << 80 << 160 << 320 << 640 << 1280 << 2560 << 5120 << 10240 << 20480;
////QList<int> cashList = QList<int>() << 20 << 20 << 40 << 80 << 160 << 320 << 640 << 1280 << 2560 << 5120 << 10240 << 20480 << 40960;
////QList<int> cashList = QList<int>() << 10 << 10 << 20 << 40 << 80 << 160 << 320 << 640 << 1280 << 2560 << 5120 << 10240 << 20480 << 40960;

//const QList<int> cashList = QList<int>() << 10 << 10 << 20 << 34 << 58 << 98 << 167 << 284 << 483 << 820 << 1395 << 2371 << 4031 << 6854 << 11652 << 19807;

////QList<int> cashList = QList<int>() << 50 << 50 << 100 << 170 << 290 << 490 << 835 << 1420 << 2415 << 4100 << 6975 << 11855 << 20155 << 34270 << 58260 << 99035;
////QList<int> cashList3 = QList<int>() << 30 << 200 << 1200 << 7200;

const QList<int> cashListParity = QList<int>() << 10 << 10 << 20 << 34 << 58 << 98 << 167 << 284 << 483 << 820 << 1395 << 2371 << 4031 << 6854 << 11652;
const QList<int> cashListNoParity = QList<int>() << 10 << 50 << 300 << 1800 << 10800;
const int NO_PARITY = 0;
const int PARITY = 1;
const int NO_PARITY_LIMIT = 5;
const int PARITY_LIMIT = 15;

const double PARITY_KOEF = 2.5;
const double NO_PARITY_KOEF = 1.2;
const double WIN_KOEF = 1.4;
const double SUM15_KOEF = 1.2;
const double SUM25_KOEF = 1.7;
const double SUM35_KOEF = 2.7;

#endif //DECLARATION_H
