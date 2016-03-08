#ifndef TEAM_H
#define TEAM_H

#include "ui_championat_widget.h"
#include "declaration.h"
#include <QString>
#include <QStandardItemModel>

class CTableViewWd;

struct CMatch
{
  int season;
  int tur;
  QString name;
  int point;
  QString opponent;
  int difference;
  int sum;
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

//  QVector<int> NoParityesCommon() {return noParityesCommon;}
  int PointsCommon() {return pointsCommon;}
  int Differince() {return differince;}

  QVector<int>& NoWins() {return m_noWins;}
  QVector<int>& Parityes() {return m_parityes;}
  QVector<int>& NoSum15s() {return m_noSum15s;}
  QVector<int>& NoSum25s() {return m_noSum25s;}
  QVector<int>& NoSum35s() {return m_noSum35s;}

  void SetConcurents(QVector<QString> concurents, QMap<QString, int> concurentPositions) {m_concurents = concurents; m_concurentPositions = concurentPositions;}
  QVector<QString> Concurents() {return m_concurents;}
  void ExchengeConcurents(QString concurent, QString distConcurent);

  QVector<CMatch>& GetSeasons() {return m_seasons;}
  int CashParityPosition() {return cashParityPosition;}

  void CropMatches(int num);

  void Rename(const QString& newName);

private:
  void FormNoParity();
  void FormWin();
  void FormParity();
  void FormSum();

private:
  QString m_name;
  QVector<CMatch> m_seasons;
  QVector<QString> m_concurents;
  QMap<QString, int> m_concurentPositions;

  QVector<int> m_noParityes;
  int m_noParity;

  QVector<int> m_noWins;
  int m_noWin;

  QVector<int> m_parityes;
  int m_parity;

  QVector<int> m_noSum15s;
  int m_noSum15;

  QVector<int> m_noSum25s;
  int m_noSum25;

  QVector<int> m_noSum35s;
  int m_noSum35;

  int cashParityPosition;
  int pointsCommon;
  int differince;
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
