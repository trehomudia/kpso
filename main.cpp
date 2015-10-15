#include "footbol_manager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  CFootbolManager w;
  w.show();
  w.Do();
  return a.exec();
}
