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
  int score;
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
  void FormParity();
  void FormNoParity();
  QMap<int, Season> seasons;

private:
  int GetNoParity();
  int GetParity();
  QString m_name;
  QVector<int> parityes;
  QVector<int> noParityes;
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
