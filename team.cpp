#include "team.h"
#include <QSet>
#include <QPair>

CTeam::CTeam(const QString& name)
  : m_name(name)
  , cashParityPosition(0)
  , pointsCommon(0)
{}

CTeam::CTeam()
{}

CTeam::~CTeam()
{}

void CTeam::SetName(const QString &name)
{
  m_name = name;
}

QString CTeam::GetName()
{
  return m_name;
}

void CTeam::FormPointCommon()
{
  pointsCommon = 0;
  foreach(CMatch match, SelectTeamData(GetSeasons()))
  {
    pointsCommon += match.point;
  }
}

void CTeam::FormData()
{
  FormPointCommon();
}

void CTeam::FindCurrentCashParity()
{
  QVector<int> matchs;
  foreach(int season, m_seasons.keys())
    foreach(CMatch match, m_seasons.value(season))
      matchs << match.point;

  for(int i = matchs.count() - 1; i >= 0; --i)
  {
    if(matchs.value(i) != PARITY)
      cashParityPosition++;
    else
      i = 0;
  }
}

void CTeam::SetCurrentCashParity(int numCash)
{
  cashParityPosition = numCash;
}

int CTeam::CurrentCash(int type)
{
  if (PARITY == type)
    return cashList.value(cashParityPosition);

  return -1;
}

void CTeam::FormDataCommon()
{
  int noParity = 0;
  foreach(CMatch match, SelectTeamData(GetSeasons()))
  {
    if (m_concurents.contains(match.opponent))
    {
      if(match.point != 1)
      {
        noParity++;
      }
      else
      {
        noParityesCommon << noParity;
        noParity = 0;
      }
    }
  }
}

int CTeam::GetResult(int typeResult)
{
  return 0;
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

void CStandardItemModel::PrintCommonResult(CTeam team, int numPrint, int typeOutcome)
{
  for(int i = 0; i < columnCount(); ++i)
    if (team.GetName() == horizontalHeaderItem(i)->data(Qt::DisplayRole).toString())
      setItem(numPrint, i, new QStandardItem(QString::number(team.CurrentCash(typeOutcome))));
}
