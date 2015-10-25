#include "team.h"
#include <QSet>
#include <QFile>
#include <QTextStream>
#include <QPair>

CTeam::CTeam(const QString& name)
  : m_name(name)
  , positionCommon(-1)
  , pointsCommon(0)
{}

CTeam::CTeam()
{}

CTeam::~CTeam()
{}

int CTeam::GetNoParity()
{
  QVector<int> v = noParityesAll;
  qSort(v);
  return v.last();
}

int CTeam::GetParity()
{
  QVector<int> v = parityesAll;
  qSort(v);
  return v.last();
}

void CTeam::SetName(const QString &name)
{
  m_name = name;
}

QString CTeam::GetName()
{
  return m_name;
}

void CTeam::FormParityAll()
{
  int parity = 0;
  foreach(CMatch match, SelectTeamData(seasons))
  {
    if(match.point == 1)
    {
      parity++;
    }
    else
    {
      if (parity != 0)
        parityesAll << parity;
      parity = 0;
    }
  }
}

void CTeam::FormNoParityAll()
{
  int noParity = 0;
  foreach(CMatch match, SelectTeamData(seasons))
  {
    if(match.point != 1)
    {
      noParity++;
    }
    else
    {
      if (noParity != 0)
        noParityesAll << noParity;
      noParity = 0;
    }
  }
}

void CTeam::FormPointCommon()
{
  pointsCommon = 0;
  foreach(CMatch match, SelectTeamData(seasons))
  {
    pointsCommon += match.point;
  }
}

void CTeam::FormData()
{
  FormParityAll();
  FormNoParityAll();
  FormPointCommon();
}

void CTeam::FormDataCommon()
{
  int noParity = 0;
  foreach(CMatch match, SelectTeamData(seasons))
  {
    if (m_concurents.contains(match.opponent))
    {
      if(match.point != 1)
      {
        noParity++;
      }
      else
      {
        if (noParity != 0)
          noParityesCommon << noParity;
        noParity = 0;
      }
    }
  }
}

int CTeam::GetResult(int typeResult)
{
  if (typeResult == NO_PARITY)
    return GetNoParity();
  else if (typeResult == PARITY)
    return GetParity();
}

Season SelectTeamData(const QMap<int, Season>& data)
{
  QVector<CMatch> plays;
  foreach(int num, data.keys())
    foreach(CMatch match, data.value(num))
      plays << match;
  return plays;
}

CStandardItemModel::CStandardItemModel(QObject * parent)
  : Base(parent) {}

void CStandardItemModel::SetView(CTableViewWd* v)
{
  view = v;
}

CTableViewWd* CStandardItemModel::GetView()
{
  return view;
}

void CStandardItemModel::SetColumns(const QStringList &names)
{
  setColumnCount(names.count());
  setHorizontalHeaderLabels(names);
}

void CStandardItemModel::PrintCommonResult(CTeam team, int numPrint)
{
  for(int i = 0; i < columnCount(); ++i)
    if (team.GetName() == horizontalHeaderItem(i)->data(Qt::DisplayRole).toString())
      setItem(numPrint, i, new QStandardItem(QString::number(team.GetResult(numPrint))));
}

Season ReadFile(QString fileName)
{
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return Season();

  fileName.remove(".csv");
  int seasonNum = fileName.remove(0, fileName.size() - 2).toInt();

  QTextStream in(&file);
  Season season;
  while (!in.atEnd())
  {
    QString str = in.readLine();
    str.remove(" ");
    QStringList line = str.split(",");

    if (line[3] != "-" && line[4] != "-")
    {
      CMatch match1;
      match1.season = seasonNum;
      match1.tur = line[0].toInt();
      match1.name = line[1];
      match1.opponent = line[2];
      if (line[3].toInt() > line[4].toInt())
        match1.point = 3;
      else if (line[3].toInt() == line[4].toInt())
        match1.point = 1;
      else
        match1.point = 0;
      season << match1;

      CMatch match2;
      match2.season = seasonNum;
      match2.tur = line[0].toInt();
      match2.name = line[2];
      match2.opponent = line[1];
      if (line[3].toInt() > line[4].toInt())
        match2.point = 0;
      else if (line[3].toInt() == line[4].toInt())
        match2.point = 1;
      else
        match2.point = 3;
      season << match2;
    }
  }
  file.close();
  return season;
}
