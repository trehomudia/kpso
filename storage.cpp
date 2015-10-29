#include "storage.h"
#include "team.h"
#include <QSet>

CStorage::CStorage(QObject *parent) : QObject(parent){}

CStorage::~CStorage(){}

QStringList CStorage::GetTeamNames(const QString& champName, const QMap<QString, Championat>& championats)
{
  QSet<QString> teams;
  foreach(CTeam team, championats[champName])
    teams << team.GetName();

  return teams.toList();
}

void CStorage::FormTeams(Season season, const QString& champName, QMap<QString, Championat>& championats)
{
  foreach(CMatch match, season)
  {
    if (!GetTeamNames(champName, championats).contains(match.name))
    {
      CTeam team(match.name);
      championats[champName] << team;
    }
  }

  foreach(CMatch match, season)
  {
    for(int i = 0; i < championats[champName].count(); ++i)
    {
      if (championats[champName][i].GetName() == match.name)
      {
        if (!championats[champName][i].seasons.contains(match.season))
          championats[champName][i].seasons.insert(match.season, Season());

        championats[champName][i].seasons[match.season] << match;
      }
    }
  }
}

QMap<QString, Championat> CStorage::ReadFiles()
{
  QMap<QString, Championat> championats;
  foreach(QString champ, GetChampNames())
    championats.insert(champ, Championat());

  foreach(QString champName, championats.keys())
    foreach(QString fileName, GetFileNames(champName))
      FormTeams(ReadFile(fileName), champName, championats);

  return championats;
}

QStringList CStorage::GetFileNames(const QString& champName)
{
  QStringList list;
  if (champName == "russia")
  {
    list << "../kpso/data/russia/02.csv";
    list << "../kpso/data/russia/03.csv";
    list << "../kpso/data/russia/04.csv";
    list << "../kpso/data/russia/05.csv";
    list << "../kpso/data/russia/06.csv";
    list << "../kpso/data/russia/07.csv";
    list << "../kpso/data/russia/08.csv";
    list << "../kpso/data/russia/09.csv";
    list << "../kpso/data/russia/10.csv";
    list << "../kpso/data/russia/11.csv";
    list << "../kpso/data/russia/12.csv";
    list << "../kpso/data/russia/13.csv";
    list << "../kpso/data/russia/14.csv";
    list << "../kpso/data/russia/15.csv";
  }
  else if(champName == "england")
  {
    list << "../kpso/data/england/06.csv";
    list << "../kpso/data/england/07.csv";
    list << "../kpso/data/england/08.csv";
    list << "../kpso/data/england/09.csv";
    list << "../kpso/data/england/10.csv";
    list << "../kpso/data/england/11.csv";
    list << "../kpso/data/england/12.csv";
    list << "../kpso/data/england/13.csv";
    list << "../kpso/data/england/14.csv";
    list << "../kpso/data/england/15.csv";
  }
  else if(champName == "spain")
  {
    list << "../kpso/data/spain/06.csv";
    list << "../kpso/data/spain/07.csv";
    list << "../kpso/data/spain/08.csv";
    list << "../kpso/data/spain/09.csv";
    list << "../kpso/data/spain/10.csv";
    list << "../kpso/data/spain/11.csv";
    list << "../kpso/data/spain/12.csv";
    list << "../kpso/data/spain/13.csv";
    list << "../kpso/data/spain/14.csv";
    list << "../kpso/data/spain/15.csv";
  }
  else if(champName == "italy")
  {
    list << "../kpso/data/italy/06.csv";
    list << "../kpso/data/italy/07.csv";
    list << "../kpso/data/italy/08.csv";
    list << "../kpso/data/italy/09.csv";
    list << "../kpso/data/italy/10.csv";
    list << "../kpso/data/italy/11.csv";
    list << "../kpso/data/italy/12.csv";
    list << "../kpso/data/italy/13.csv";
    list << "../kpso/data/italy/14.csv";
    list << "../kpso/data/italy/15.csv";
  }
  else if(champName == "germany")
  {
    list << "../kpso/data/germany/06.csv";
    list << "../kpso/data/germany/07.csv";
    list << "../kpso/data/germany/08.csv";
    list << "../kpso/data/germany/09.csv";
    list << "../kpso/data/germany/10.csv";
    list << "../kpso/data/germany/11.csv";
    list << "../kpso/data/germany/12.csv";
    list << "../kpso/data/germany/13.csv";
    list << "../kpso/data/germany/14.csv";
    list << "../kpso/data/germany/15.csv";
  }
  else if(champName == "france")
  {
    list << "../kpso/data/france/06.csv";
    list << "../kpso/data/france/07.csv";
    list << "../kpso/data/france/08.csv";
    list << "../kpso/data/france/09.csv";
    list << "../kpso/data/france/10.csv";
    list << "../kpso/data/france/11.csv";
    list << "../kpso/data/france/12.csv";
    list << "../kpso/data/france/13.csv";
    list << "../kpso/data/france/14.csv";
    list << "../kpso/data/france/15.csv";
  }

  return list;
}

QStringList CStorage::GetChampNames()
{
  QStringList list;
  list << "russia";
  list << "england";
  list << "spain";
  list << "italy";
  list << "germany";
  list << "france";
  return list;
}
