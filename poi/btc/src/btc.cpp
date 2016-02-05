#include <QApplication>
#include "main_window.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	CBtcMainWindow *mw = new CBtcMainWindow();

	mw->show();

	return app.exec();
}