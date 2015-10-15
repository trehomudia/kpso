#include "team.h"
#include <QSet>
#include <QFile>
#include <QTextStream>
#include <QPair>

CTeam::CTeam(const QString& name)
  : m_name(name)
{}

CTeam::CTeam()
{}

CTeam::~CTeam()
{}

int CTeam::GetNoParity()
{
  QVector<int> v = noParityes;
  qSort(v);
  return v.last();
}

int CTeam::GetParity()
{
  QVector<int> v = parityes;
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

void CTeam::FormParity()
{
  int parity = 0;
  foreach(CMatch match, SelectTeamData(seasons))
  {
    if(match.score == 0)
    {
      parity++;
    }
    else
    {
      if (parity != 0)
        parityes << parity;
      parity = 0;
    }
  }
}

void CTeam::FormNoParity()
{
  int noParity = 0;
  foreach(CMatch match, SelectTeamData(seasons))
  {
    if(match.score != 0)
    {
      noParity++;
    }
    else
    {
      if (noParity != 0)
        noParityes << noParity;
      noParity = 0;
    }
  }
}

void CTeam::FormData()
{
  FormParity();
  FormNoParity();
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

  int seasonNum = fileName.remove("../statistica/seasons/data").remove(".csv").toInt();

  QTextStream in(&file);
  Season season;
  while (!in.atEnd())
  {
    QStringList line = in.readLine().split(",");

    CMatch match1;
    match1.season = seasonNum;
    match1.tur = line[0].toInt();
    match1.name = line[1];
    if (line[3].toInt() > line[4].toInt())
      match1.score = 1;
    else if (line[3].toInt() == line[4].toInt())
      match1.score = 0;
    else
      match1.score = -1;
    season << match1;

    CMatch match2;
    match2.season = seasonNum;
    match2.tur = line[0].toInt();
    match2.name = line[2];
    if (line[3].toInt() > line[4].toInt())
      match2.score = -1;
    else if (line[3].toInt() == line[4].toInt())
      match2.score = 0;
    else
      match2.score = 1;
    season << match2;
  }
  file.close();
  return season;
}
