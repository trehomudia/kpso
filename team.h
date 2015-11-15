#ifndef TEAM_H
#define TEAM_H

#include <QString>
#include <QStandardItemModel>

////QList<int> cashList = QList<int>() << 10 << 10 << 20 << 40 << 80 << 160 << 320 << 640 << 1280 << 2560 << 5120 << 10240 << 20480;
////QList<int> cashList = QList<int>() << 20 << 20 << 40 << 80 << 160 << 320 << 640 << 1280 << 2560 << 5120 << 10240 << 20480 << 40960;
////QList<int> cashList = QList<int>() << 10 << 10 << 20 << 40 << 80 << 160 << 320 << 640 << 1280 << 2560 << 5120 << 10240 << 20480 << 40960;

const QList<int> cashList = QList<int>() << 10 << 10 << 20 << 34 << 58 << 98 << 167 << 284 << 483 << 820 << 1395 << 2371 << 4031 << 6854 << 11652 << 19807;

////QList<int> cashList = QList<int>() << 50 << 50 << 100 << 170 << 290 << 490 << 835 << 1420 << 2415 << 4100 << 6975 << 11855 << 20155 << 34270 << 58260 << 99035;
////QList<int> cashList3 = QList<int>() << 30 << 200 << 1200 << 7200;

const int NO_PARITY = 0;
const int PARITY = 1;

class CTableViewWd;

struct CMatch
{
  int season;
  int tur;
  QString name;
  int point;
  QString opponent;
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
  int GetResult(int typeResult);
  void FormData();
  void FormPointCommon();
  void FormDataCommon();

  void FindCurrentCashParity();
  void SetCurrentCashParity(int numCash);
  int CurrentCash(int type);

  QVector<int> NoParityesCommon() {return noParityesCommon;}
  int PointsCommon() {return pointsCommon;}
  void SetConcurents(QVector<QString> concurents) {m_concurents = concurents;}
  QMap<int, Season>& GetSeasons() {return m_seasons;}
  int CashParityPosition() {return cashParityPosition;}

private:
  QString m_name;
  int cashParityPosition;
  int pointsCommon;
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
  void SetColumns(const QStringList& names);
  void PrintCommonResult(CTeam team, int numPrint, int typeOutcome);

private:
  CTableViewWd* view;
};

#endif //TEAM_H
