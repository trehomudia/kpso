#ifndef TEAM_H
#define TEAM_H

#include "ui_championat_widget.h"
#include <QString>
#include <QStandardItemModel>
#include <QDateTime>

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

class CTableViewWd;

struct CMatch
{
  QDate date;
  int season;
//  int tur;
  QString name;
  int point;
  QString opponent;
  int difference;
};

typedef QVector<CMatch> Season;

class CTeam
{
public:
  CTeam(const QString& name);
  CTeam();
  ~CTeam();

  void SetName(const QString& name);
  QString GetName();
  void FormData();
  void FormDataCommon();

  void FindCurrentCashParity();
  void SetCurrentCashParity(int numCash);

  QVector<int> NoParityesCommon() {return noParityesCommon;}
  int PointsCommon() {return pointsCommon;}
  int Differince() {return differince;}

  void SetConcurents(QVector<QString> concurents) {m_concurents = concurents;}
  QVector<QString> Concurents() {return m_concurents;}
  void ExchengeConcurents(QString concurent, QString distConcurent);

  QMap<int, Season>& GetSeasons() {return m_seasons;}
  int CashParityPosition() {return cashParityPosition;}

private:
  QString m_name;
  int cashParityPosition;
  int pointsCommon;
  int differince;
  QVector<int> noParityesCommon;
  QVector<QString> m_concurents;
  QMap<int, Season> m_seasons;
};

Season SelectTeamData(const QMap<int, Season>& data);

class CStandardItemModel : public QStandardItemModel
{
  typedef QStandardItemModel Base;
  Q_OBJECT

 public:
  CStandardItemModel(QObject * parent = 0);
  virtual ~CStandardItemModel(){}
  void SetView(CTableViewWd* v);
  CTableViewWd* GetView();
  void SetColumns(const QVector<QString>& names);
//  void PrintCommonResult(CTeam team, int numPrint, int typeOutcome);
  void PrintCommonResult(QString team, int numPrint, int value);

private:
  CTableViewWd* view;
};

int GetCash(int num, int type);

class CWidget : public QWidget
{
  Q_OBJECT
  typedef QWidget Base;

public:
  CWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
  CStandardItemModel* TableCommon();
  CStandardItemModel* TableResult();

signals:
  void doubleClicked(const QModelIndex& index);
  void clicked();
  void AllChecked(bool);

private:
  Ui::championatWidget ui;
};

#endif //TEAM_H
