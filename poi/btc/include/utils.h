/**
	@file utils.h 
	@defgroup utils утилиты
    @{ 
    
     
*/
#ifndef UTILS_H
#define UTILS_H

#include <QVector>
#include <QPair>
#include <QString>
#include <vector>
#include <map>
#include <QDoubleSpinBox>
#include <QMap>
#include <QPair>


typedef std::pair< int, int >				IndexInterval;
typedef std::pair< double, double >			FuncPoint;
typedef std::pair< FuncPoint, FuncPoint >	FuncInterval;

struct IncreasePercentResult
{
	int		n;								//!< количество интервалов возрастания
	int		dt;								//!< разница в секундах между концом последнего интервала и началом первого
	double	increasePercent;				//!< общее увеличение без учета комиссии
	double	increasePercentWithComission;	//!< общее увеличение с учетом комиссии
};

/** @brief очень полезная функция
*/
QVector< QPair< double, double > > GetPriceSamplesFromCsvFile(const QString separator = ";");

std::vector< FuncPoint > ConvertQVectorQPairToStd(QVector< QPair< double, double > > &qVectorQPair);

QVector< QPair< double, double > > ConvertStdVectorStdPairToQt(std::vector< FuncPoint > &stdVectorStdPair);

IncreasePercentResult GetIncreasePercent( std::vector< FuncInterval > &increaseIntervalVec, double comissionPercent = 0.0 );

/**	@brief "ускоренный" вариант без промежуточных данных */
IncreasePercentResult GetIncreasePercent( std::vector< FuncPoint > &dependence, int winWidth, int step, double comissionPercent = 0.0 );


std::vector< FuncInterval > GetIncreaseIntervals(std::multimap< FuncPoint , bool > &localExtremums);

std::multimap< FuncPoint, bool > GetSourceLocalExtremums(std::vector< FuncPoint > &sourceDependence,
																		std::multimap< FuncPoint, bool > &localExtremums);

std::multimap< FuncPoint, bool > GetLocalExtremums(std::vector< FuncPoint > &dependence);

std::multimap< IndexInterval, bool > GetLocalExtremumIndexes(std::vector< double > &samples);

QVector< QPair< double, double > >	GetStepQualityDependence(QVector< QPair< double, double > > &dep, int winWidth, int stepStart, int stepEnd, int stepStep);
QVector< QPair< double, double > >	GetWinQualityDependence(QVector< QPair< double, double > > &dep, int step, int winStart, int winEnd, int winStep);
QMap< QPair< int, int >, double >	GetQualityDependence(QVector< QPair< double, double > > &dep, int winStart, int winEnd, int winStep, int stepStart, int stepEnd, int stepStep);

class CDoubleSpinBox : public QDoubleSpinBox
{
  Q_OBJECT
public:
  CDoubleSpinBox(QWidget * parent = 0);   //!< Конструктор.
  virtual ~CDoubleSpinBox() {}            //!< Деструктор.

signals:
  void EditingFinished(double);

private slots:
  void Edit();

private:
	double val;
};

class CSpinBox : public QSpinBox
{
  Q_OBJECT
public:
  CSpinBox(QWidget * parent = 0); //!< Конструктор.
  virtual ~CSpinBox() {}          //!< Деструктор.

signals:
  void EditingFinished(int);

private slots:
  void Edit();

private:
	int val;
};


/**	@} */

#endif // UTILS_H