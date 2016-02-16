#include "hockey_manager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  CHockeyManager w;
  w.show();
  w.Do();
  return a.exec();
}
