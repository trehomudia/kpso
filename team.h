#ifndef TEAM_H
#define TEAM_H

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
  void FormParityAll();
  void FormNoParityAll();
  void FormPointCommon();
  void FormDataCommon();
  QMap<int, Season> seasons;
  QVector<int> Parityes() {return parityesAll;}
  QVector<int> NoParityes() {return noParityesAll;}
  QVector<int> NoParityesCommon() {return noParityesCommon;}
  int PointsCommon() {return pointsCommon;}
  void SetConcurents(QVector<QString> concurents) {m_concurents = concurents;}

private:
  int GetNoParity();
  int GetParity();
  QString m_name;
  int positionCommon;
  int pointsCommon;
  QVector<int> parityesAll;
  QVector<int> noParityesAll;
  QVector<int> noParityesCommon;
  QVector<QString> m_concurents;
};

const int NO_PARITY = 0;
const int PARITY = 1;

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
  void PrintCommonResult(CTeam team, int numPrint);

private:
  CTableViewWd* view;
};

Season ReadFile(QString fileName);
#endif //TEAM_H
