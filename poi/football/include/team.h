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

  QVector<int> NoParityesCommon() {return noParityesCommon;}
  int PointsCommon() {return pointsCommon;}
  int Differince() {return differince;}

  void SetConcurents(QVector<QString> concurents) {m_concurents = concurents;}
  QVector<QString> Concurents() {return m_concurents;}
  void ExchengeConcurents(QString concurent, QString distConcurent);

  QVector<CMatch>& GetSeasons() {return m_seasons;}
  int CashParityPosition() {return cashParityPosition;}

  void CropMatches(int num);

  void Rename(const QString& newName);

private:
  QString m_name;
  int cashParityPosition;
  int pointsCommon;
  int differince;
  QVector<int> noParityesCommon;
  QVector<QString> m_concurents;
  QVector<CMatch> m_seasons;
//  QMap<int, Season> m_seasons;
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
