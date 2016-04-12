#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include "ui_btc.h"

class CBtcMainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

public:

/**	@brief Конструктор */
	CBtcMainWindow(QWidget *parent = 0);

/**	@brief Деструктор */
	~CBtcMainWindow();

public slots:

	void OnFileOpen();

	void OnWin1Changed(int win);
	void OnStep1Changed(int step);

	void OnCountFixedWin();
/**	@brief Расчет и отрисовка сглаживания зависимости ф-ции "качества" от шага при фиксированной ширине окна */
	void OnSmoothFixedWin();
	void OnCountFixedStep();
	void OnCount();



	// для исследования индикаторов:
	void OnMa1WinChanged(int win);
	void OnMa1StepChanged(int step);
	void OnMr1WinChanged(int win);
	void OnMr1StepChanged(int step);
	void OnMr2WinChanged(int win);

private:

	void AddConnects();

	QVector< QPair< double, double > > m_samples;

	QVector< QPair< double, double > > m_stepQualityDep; //!< зависимость "выигрыша" от шага при фиксированной ширине окна


};

#endif // MAIN_WINDOW_H