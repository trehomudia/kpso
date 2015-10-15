#include "statistica.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  statistica w;
  w.show();
  w.Do();
  return a.exec();
}
