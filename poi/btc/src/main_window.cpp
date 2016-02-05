#include "main_window.h"
#include "utils.h"
#include "indicator/indicator.h"	// test!

CBtcMainWindow::CBtcMainWindow(QWidget *parent) : QMainWindow(parent), Ui::MainWindow()
{
	setupUi(this);

	AddConnects();
	m_3dPlot->resize(300,300);
	m_3dPlot->show();
}

CBtcMainWindow::~CBtcMainWindow(){}

void CBtcMainWindow::AddConnects()
{

	connect(m_actionLoadFile,	SIGNAL(triggered()),			this, SLOT(OnFileOpen()));
	// элементы управления параметрами расчета априорной информации:
	connect(m_spbxWinSize1,		SIGNAL(EditingFinished(int)),	this, SLOT(OnWin1Changed(int)));
	connect(m_spbxStep1,		SIGNAL(EditingFinished(int)),	this, SLOT(OnStep1Changed(int)));
	connect(m_btnFixedWin,		SIGNAL(clicked()),				this, SLOT(OnCountFixedWin()));
	connect(m_btnFixedStep,		SIGNAL(clicked()),				this, SLOT(OnCountFixedStep()));
	connect(m_btnCalcQuality,	SIGNAL(clicked()),				this, SLOT(OnCount()));

	// элементы управления индикаторами (realtime)
	connect(m_spbxMa1Win,		SIGNAL(EditingFinished(int)),	this, SLOT(OnMa1WinChanged(int)));
	connect(m_spbxMa1Step,		SIGNAL(EditingFinished(int)),	this, SLOT(OnMa1StepChanged(int)));
	connect(m_spbxMr1Win,		SIGNAL(EditingFinished(int)),	this, SLOT(OnMr1WinChanged(int)));
	connect(m_spbxMr1Step,		SIGNAL(EditingFinished(int)),	this, SLOT(OnMr1StepChanged(int)));
	
}

void CBtcMainWindow::OnFileOpen()
{
	m_samples = GetPriceSamplesFromCsvFile(";");

	qSort(m_samples);

	m_pricePlot->SetData(m_samples);
	//test:
	m_indicatorPlot->SetData(m_samples);

	int win = m_spbxWinSize1->value();
	int step = m_spbxStep1->value();
	if(win != 0 && step != 0)
		m_pricePlot->SetMovingAverageIndicator(win, step);

}

void CBtcMainWindow::OnWin1Changed(int win)
{
	//m_pricePlot->SetMovingRegressionIndicator(win, 1);
	int step = m_spbxStep1->value();
	m_pricePlot->SetMovingAverageIndicator(win, step);
}

void CBtcMainWindow::OnStep1Changed(int step)
{
	int win = m_spbxWinSize1->value();
	m_pricePlot->SetMovingAverageIndicator(win, step);
}

void CBtcMainWindow::OnCountFixedWin()
{
	if(m_samples.empty())
		return;

	int winWidth	= m_spbxWinConst->value();
	int stepStart	= m_spbxStepStart->value();
	int stepEnd		= m_spbxStepEnd->value();
	int stepStep	= m_spbxStepIncrement->value();

	QVector< QPair< double, double > > stepQualityDep = GetStepQualityDependence(m_samples, winWidth, stepStart, stepEnd, stepStep);

	m_fixedWinPlot->SetData(stepQualityDep);

	//test:
	std::vector< FuncPoint > stepQualityDepStd = ConvertQVectorQPairToStd(stepQualityDep);
	CIndicator ind(stepQualityDepStd);
	std::vector< FuncPoint > ma1 = ind.GetMovingAverage(10, 1);

	CIndicator ind2(ma1);
	std::vector< FuncPoint > ma2 = ind2.GetMovingAverage(10, 1);

	m_fixedWinPlot->SetAddData(ConvertStdVectorStdPairToQt(ma2));

}

void CBtcMainWindow::OnCountFixedStep()
{
	if(m_samples.empty())
		return;

	int step		= m_spbxStepConst->value();
	int winStart	= m_spbxWinStart->value();
	int winEnd		= m_spbxWinEnd->value();
	int winStep		= m_spbxWinIncrement->value();

	QVector< QPair< double, double > > winQualityDep = GetWinQualityDependence(m_samples, step, winStart, winEnd, winStep);

	m_fixedStepPlot->SetData(winQualityDep);
}

void CBtcMainWindow::OnCount()
{
	if(m_samples.empty())
		return;

	int winStart	= m_winStart->value();
	int winEnd		= m_winEnd->value();
	int winStep		= m_winStep->value();
	int stepStart	= m_stepStart->value();
	int stepEnd		= m_stepEnd->value();
	int stepStep	= m_stepStep->value();

	QMap< QPair< int, int >, double > quality = GetQualityDependence(m_samples, winStart, winEnd, winStep, stepStart, stepEnd, stepStep);

	int nx = (winEnd - winStart) / winStep + 1;
	int ny = (stepEnd - stepStart) / stepStep + 1;

	m_3dPlot->SetData(quality, nx, ny, winStep, stepStep);

}


void CBtcMainWindow::OnMa1WinChanged(int win)
{
	int step = m_spbxMa1Step->value();
	m_indicatorPlot->SetRealTimeMovingAverageIndicator(win, step);
}


void CBtcMainWindow::OnMa1StepChanged(int step)
{
	int win = m_spbxMa1Win->value();
	m_indicatorPlot->SetRealTimeMovingAverageIndicator(win, step);
}

void CBtcMainWindow::OnMr1WinChanged(int win)
{
	int step = m_spbxMr1Step->value();
	m_indicatorPlot->SetRealTimeMovingRegressionIndicator(win, step);
}


void CBtcMainWindow::OnMr1StepChanged(int step)
{
	int win = m_spbxMr1Win->value();
	m_indicatorPlot->SetRealTimeMovingRegressionIndicator(win, step);
}