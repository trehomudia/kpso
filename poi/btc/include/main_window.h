#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include "ui_btc.h"

class CBtcMainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

public:

	CBtcMainWindow(QWidget *parent = 0);

	~CBtcMainWindow();

public slots:

	void OnFileOpen();

	void OnWin1Changed(int win);
	void OnStep1Changed(int step);

	void OnCountFixedWin();
	void OnCountFixedStep();
	void OnCount();


	// для исследования индикаторов:
	void OnMa1WinChanged(int win);
	void OnMa1StepChanged(int step);
	void OnMr1WinChanged(int win);
	void OnMr1StepChanged(int step);

private:

	void AddConnects();

	QVector< QPair< double, double > > m_samples;


};

#endif // MAIN_WINDOW_H