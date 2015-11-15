#include "storage.h"
#include <QSet>
#include <QFile>
#include <QTextStream>
#include <QDir>

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
        if (!championats[champName][i].GetSeasons().contains(match.season))
          championats[champName][i].GetSeasons().insert(match.season, Season());

        championats[champName][i].GetSeasons()[match.season] << match;
      }
    }
  }
}

QMap<QString, Championat> CStorage::ReadFiles(int confidentialSeasons)
{
  QMap<QString, Championat> championats;
  foreach(QString champ, GetChampNames())
    championats.insert(champ, Championat());

  foreach(QString champName, championats.keys())
    foreach(QString fileName, GetFileNames(champName, confidentialSeasons))
      FormTeams(ReadFile(fileName), champName, championats);

  return championats;
}

QStringList CStorage::GetFileNames(const QString& champName, int confidentialSeasons)
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

  if (0 == confidentialSeasons)
    return list;
  else
    return list.mid(list.count() - confidentialSeasons, confidentialSeasons);
}

QStringList CStorage::GetChampNames()
{
  QStringList list;
  list << "russia";
//  list << "england";
//  list << "spain";
//  list << "italy";
//  list << "germany";
//  list << "france";
  return list;
}

bool CStorage::RateIsEmpty(QString champName)
{
  QDir dir(QDir::current());
  dir.cd(QString("../kpso"));

  while(!dir.cd(QString("rates")))
    dir.mkdir(QString("rates"));

  while(!dir.cd(champName))
    dir.mkdir(champName);

  QString fileName("../kpso/rates/%1/draw.csv");
  fileName = fileName.arg(champName);
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    if (file.open(QIODevice::WriteOnly))
    {
      file.close();
      return true;
    }
  }

  QString str;
  QTextStream in(&file);
  while (!in.atEnd())
  {
    str += in.readLine();
  }

  file.close();

  if (str.isEmpty())
    return true;
  else
    return false;
}

Season CStorage::ReadFile(QString fileName)
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

void CStorage::AddTeam(CTeam team, QString champName, int type)
{
  QString fileName("../kpso/rates/%1/%2.csv");
  fileName = fileName.arg(champName);
  if (PARITY == type)
    fileName = fileName.arg("draw");

  QFile file(fileName);
  if (!file.open(QIODevice::Append))
    return;

  QTextStream out(&file);
  out << team.GetName() << QString(",") << team.CashParityPosition() << "\n";

  file.close();
}

int CStorage::ReadCurrentRate(CTeam team, QString champName, int type)
{
  QString fileName("../kpso/rates/%1/%2.csv");
  fileName = fileName.arg(champName);
  if (PARITY == type)
    fileName = fileName.arg("draw");

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return -1;

  int numCash = -1;
  QTextStream in(&file);
  while (!in.atEnd())
  {
    QString str = in.readLine();
    if (str.contains(team.GetName()))
    {
      QStringList list = str.split(",");
      numCash = list.last().toInt();
    }
  }

  file.close();
  return numCash;
}
