#include "storage.h"
#include <QSet>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QApplication>

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
  QString str1 = QApplication::applicationDirPath();
  if (champName == "russia")
  {
    list << "../../skotina/poi/football/resource/data/russia/02.csv";
    list << "../../skotina/poi/football/resource/data/russia/03.csv";
    list << "../../skotina/poi/football/resource/data/russia/04.csv";
    list << "../../skotina/poi/football/resource/data/russia/05.csv";
    list << "../../skotina/poi/football/resource/data/russia/06.csv";
    list << "../../skotina/poi/football/resource/data/russia/07.csv";
    list << "../../skotina/poi/football/resource/data/russia/08.csv";
    list << "../../skotina/poi/football/resource/data/russia/09.csv";
    list << "../../skotina/poi/football/resource/data/russia/10.csv";
    list << "../../skotina/poi/football/resource/data/russia/11.csv";
    list << "../../skotina/poi/football/resource/data/russia/12.csv";
    list << "../../skotina/poi/football/resource/data/russia/13.csv";
    list << "../../skotina/poi/football/resource/data/russia/14.csv";
    list << "../../skotina/poi/football/resource/data/russia/15.csv";
    //list << "../../skotina/poi/football/resource/data/russia/14!.csv";
//    list << "../../skotina/poi/football/resource/data/russia/15!.csv";
//    list << "../../skotina/poi/football/resource/data/russia/15!!.csv";
  }
  else if(champName == "england")
  {
    list << "../../skotina/poi/football/resource/data/england/06.csv";
    list << "../../skotina/poi/football/resource/data/england/07.csv";
    list << "../../skotina/poi/football/resource/data/england/08.csv";
    list << "../../skotina/poi/football/resource/data/england/09.csv";
    list << "../../skotina/poi/football/resource/data/england/10.csv";
    list << "../../skotina/poi/football/resource/data/england/11.csv";
    list << "../../skotina/poi/football/resource/data/england/12.csv";
    list << "../../skotina/poi/football/resource/data/england/13.csv";
    list << "../../skotina/poi/football/resource/data/england/14.csv";
    list << "../../skotina/poi/football/resource/data/england/15.csv";
  }
  else if(champName == "spain")
  {
    list << "../../skotina/poi/football/resource/data/spain/06.csv";
    list << "../../skotina/poi/football/resource/data/spain/07.csv";
    list << "../../skotina/poi/football/resource/data/spain/08.csv";
    list << "../../skotina/poi/football/resource/data/spain/09.csv";
    list << "../../skotina/poi/football/resource/data/spain/10.csv";
    list << "../../skotina/poi/football/resource/data/spain/11.csv";
    list << "../../skotina/poi/football/resource/data/spain/12.csv";
    list << "../../skotina/poi/football/resource/data/spain/13.csv";
    list << "../../skotina/poi/football/resource/data/spain/14.csv";
    list << "../../skotina/poi/football/resource/data/spain/15.csv";
  }
  else if(champName == "italy")
  {
    list << "../../skotina/poi/football/resource/data/italy/06.csv";
    list << "../../skotina/poi/football/resource/data/italy/07.csv";
    list << "../../skotina/poi/football/resource/data/italy/08.csv";
    list << "../../skotina/poi/football/resource/data/italy/09.csv";
    list << "../../skotina/poi/football/resource/data/italy/10.csv";
    list << "../../skotina/poi/football/resource/data/italy/11.csv";
    list << "../../skotina/poi/football/resource/data/italy/12.csv";
    list << "../../skotina/poi/football/resource/data/italy/13.csv";
    list << "../../skotina/poi/football/resource/data/italy/14.csv";
    list << "../../skotina/poi/football/resource/data/italy/15.csv";
  }
  else if(champName == "germany")
  {
    list << "../../skotina/poi/football/resource/data/germany/06.csv";
    list << "../../skotina/poi/football/resource/data/germany/07.csv";
    list << "../../skotina/poi/football/resource/data/germany/08.csv";
    list << "../../skotina/poi/football/resource/data/germany/09.csv";
    list << "../../skotina/poi/football/resource/data/germany/10.csv";
    list << "../../skotina/poi/football/resource/data/germany/11.csv";
    list << "../../skotina/poi/football/resource/data/germany/12.csv";
    list << "../../skotina/poi/football/resource/data/germany/13.csv";
    list << "../../skotina/poi/football/resource/data/germany/14.csv";
    list << "../../skotina/poi/football/resource/data/germany/15.csv";
  }
  else if(champName == "france")
  {
    list << "../../skotina/poi/football/resource/data/france/06.csv";
    list << "../../skotina/poi/football/resource/data/france/07.csv";
    list << "../../skotina/poi/football/resource/data/france/08.csv";
    list << "../../skotina/poi/football/resource/data/france/09.csv";
    list << "../../skotina/poi/football/resource/data/france/10.csv";
    list << "../../skotina/poi/football/resource/data/france/11.csv";
    list << "../../skotina/poi/football/resource/data/france/12.csv";
    list << "../../skotina/poi/football/resource/data/france/13.csv";
    list << "../../skotina/poi/football/resource/data/france/14.csv";
    list << "../../skotina/poi/football/resource/data/france/15.csv";
  }

  if (0 == confidentialSeasons)
    return list;
  else
    return list.mid(list.count() - confidentialSeasons, confidentialSeasons);


//  QStringList list;
//  QString str1 = QApplication::applicationDirPath();
//  if (champName == "russia")
//  {
//    list << "../../skotina/poi/football/resource/data/russia/02.csv";
//    list << "../../skotina/poi/football/resource/data/russia/03.csv";
//    list << "../../skotina/poi/football/resource/data/russia/04.csv";
//    list << "../../skotina/poi/football/resource/data/russia/05.csv";
//    list << "../../skotina/poi/football/resource/data/russia/06.csv";
//    list << "../../skotina/poi/football/resource/data/russia/07.csv";
//    list << "../../skotina/poi/football/resource/data/russia/08.csv";
//    list << "../../skotina/poi/football/resource/data/russia/09.csv";
//    list << "../../skotina/poi/football/resource/data/russia/10.csv";
//    list << "../../skotina/poi/football/resource/data/russia/11.csv";
//    list << "../../skotina/poi/football/resource/data/russia/12.csv";
//    list << "../../skotina/poi/football/resource/data/russia/13.csv";
//    list << "../../skotina/poi/football/resource/data/russia/14.csv";
//    list << "../../skotina/poi/football/resource/data/russia/15.csv";
//    //list << "../../skotina/poi/football/resource/data/russia/14!.csv";
////    list << "../../skotina/poi/football/resource/data/russia/15!.csv";
////    list << "../../skotina/poi/football/resource/data/russia/15!!.csv";
//  }
//  else if(champName == "england")
//  {
//    list << "../../skotina/poi/football/resource/data/england/06.csv";
//    list << "../../skotina/poi/football/resource/data/england/07.csv";
//    list << "../../skotina/poi/football/resource/data/england/08.csv";
//    list << "../../skotina/poi/football/resource/data/england/09.csv";
//    list << "../../skotina/poi/football/resource/data/england/10.csv";
//    list << "../../skotina/poi/football/resource/data/england/11.csv";
//    list << "../../skotina/poi/football/resource/data/england/12.csv";
//    list << "../../skotina/poi/football/resource/data/england/13.csv";
//    list << "../../skotina/poi/football/resource/data/england/14.csv";
//    list << "../../skotina/poi/football/resource/data/england/15.csv";
//  }
//  else if(champName == "spain")
//  {
//    list << "../../skotina/poi/football/resource/data/spain/06.csv";
//    list << "../../skotina/poi/football/resource/data/spain/07.csv";
//    list << "../../skotina/poi/football/resource/data/spain/08.csv";
//    list << "../../skotina/poi/football/resource/data/spain/09.csv";
//    list << "../../skotina/poi/football/resource/data/spain/10.csv";
//    list << "../../skotina/poi/football/resource/data/spain/11.csv";
//    list << "../../skotina/poi/football/resource/data/spain/12.csv";
//    list << "../../skotina/poi/football/resource/data/spain/13.csv";
//    list << "../../skotina/poi/football/resource/data/spain/14.csv";
//    list << "../../skotina/poi/football/resource/data/spain/15.csv";
//  }
//  else if(champName == "italy")
//  {
//    list << "../../skotina/poi/football/resource/data/italy/06.csv";
//    list << "../../skotina/poi/football/resource/data/italy/07.csv";
//    list << "../../skotina/poi/football/resource/data/italy/08.csv";
//    list << "../../skotina/poi/football/resource/data/italy/09.csv";
//    list << "../../skotina/poi/football/resource/data/italy/10.csv";
//    list << "../../skotina/poi/football/resource/data/italy/11.csv";
//    list << "../../skotina/poi/football/resource/data/italy/12.csv";
//    list << "../../skotina/poi/football/resource/data/italy/13.csv";
//    list << "../../skotina/poi/football/resource/data/italy/14.csv";
//    list << "../../skotina/poi/football/resource/data/italy/15.csv";
//  }
//  else if(champName == "germany")
//  {
//    list << "../../skotina/poi/football/resource/data/germany/06.csv";
//    list << "../../skotina/poi/football/resource/data/germany/07.csv";
//    list << "../../skotina/poi/football/resource/data/germany/08.csv";
//    list << "../../skotina/poi/football/resource/data/germany/09.csv";
//    list << "../../skotina/poi/football/resource/data/germany/10.csv";
//    list << "../../skotina/poi/football/resource/data/germany/11.csv";
//    list << "../../skotina/poi/football/resource/data/germany/12.csv";
//    list << "../../skotina/poi/football/resource/data/germany/13.csv";
//    list << "../../skotina/poi/football/resource/data/germany/14.csv";
//    list << "../../skotina/poi/football/resource/data/germany/15.csv";
//  }
//  else if(champName == "france")
//  {
//    list << "../../skotina/poi/football/resource/data/france/06.csv";
//    list << "../../skotina/poi/football/resource/data/france/07.csv";
//    list << "../../skotina/poi/football/resource/data/france/08.csv";
//    list << "../../skotina/poi/football/resource/data/france/09.csv";
//    list << "../../skotina/poi/football/resource/data/france/10.csv";
//    list << "../../skotina/poi/football/resource/data/france/11.csv";
//    list << "../../skotina/poi/football/resource/data/france/12.csv";
//    list << "../../skotina/poi/football/resource/data/france/13.csv";
//    list << "../../skotina/poi/football/resource/data/france/14.csv";
//    list << "../../skotina/poi/football/resource/data/france/15.csv";
//  }

//  if (0 == confidentialSeasons)
//    return list;
//  else
//    return list.mid(list.count() - confidentialSeasons, confidentialSeasons);


//  QStringList list;
//  QString str1 = QApplication::applicationDirPath();
//  if (champName == "russia")
//  {
//    list << "../kpso/football/data/russia/02.csv";
//    list << "../kpso/football/data/russia/03.csv";
//    list << "../kpso/football/data/russia/04.csv";
//    list << "../kpso/football/data/russia/05.csv";
//    list << "../kpso/football/data/russia/06.csv";
//    list << "../kpso/football/data/russia/07.csv";
//    list << "../kpso/football/data/russia/08.csv";
//    list << "../kpso/football/data/russia/09.csv";
//    list << "../kpso/football/data/russia/10.csv";
//    list << "../kpso/football/data/russia/11.csv";
//    list << "../kpso/football/data/russia/12.csv";
//    list << "../kpso/football/data/russia/13.csv";
//    list << "../kpso/football/data/russia/14.csv";
//    list << "../kpso/football/data/russia/15.csv";
//    //list << "../kpso/football/data/russia/14!.csv";
////    list << "../kpso/football/data/russia/15!.csv";
////    list << "../kpso/football/data/russia/15!!.csv";
//  }
//  else if(champName == "england")
//  {
//    list << "../kpso/football/data/england/06.csv";
//    list << "../kpso/football/data/england/07.csv";
//    list << "../kpso/football/data/england/08.csv";
//    list << "../kpso/football/data/england/09.csv";
//    list << "../kpso/football/data/england/10.csv";
//    list << "../kpso/football/data/england/11.csv";
//    list << "../kpso/football/data/england/12.csv";
//    list << "../kpso/football/data/england/13.csv";
//    list << "../kpso/football/data/england/14.csv";
//    list << "../kpso/football/data/england/15.csv";
//  }
//  else if(champName == "spain")
//  {
//    list << "../kpso/football/data/spain/06.csv";
//    list << "../kpso/football/data/spain/07.csv";
//    list << "../kpso/football/data/spain/08.csv";
//    list << "../kpso/football/data/spain/09.csv";
//    list << "../kpso/football/data/spain/10.csv";
//    list << "../kpso/football/data/spain/11.csv";
//    list << "../kpso/football/data/spain/12.csv";
//    list << "../kpso/football/data/spain/13.csv";
//    list << "../kpso/football/data/spain/14.csv";
//    list << "../kpso/football/data/spain/15.csv";
//  }
//  else if(champName == "italy")
//  {
//    list << "../kpso/football/data/italy/06.csv";
//    list << "../kpso/football/data/italy/07.csv";
//    list << "../kpso/football/data/italy/08.csv";
//    list << "../kpso/football/data/italy/09.csv";
//    list << "../kpso/football/data/italy/10.csv";
//    list << "../kpso/football/data/italy/11.csv";
//    list << "../kpso/football/data/italy/12.csv";
//    list << "../kpso/football/data/italy/13.csv";
//    list << "../kpso/football/data/italy/14.csv";
//    list << "../kpso/football/data/italy/15.csv";
//  }
//  else if(champName == "germany")
//  {
//    list << "../kpso/football/data/germany/06.csv";
//    list << "../kpso/football/data/germany/07.csv";
//    list << "../kpso/football/data/germany/08.csv";
//    list << "../kpso/football/data/germany/09.csv";
//    list << "../kpso/football/data/germany/10.csv";
//    list << "../kpso/football/data/germany/11.csv";
//    list << "../kpso/football/data/germany/12.csv";
//    list << "../kpso/football/data/germany/13.csv";
//    list << "../kpso/football/data/germany/14.csv";
//    list << "../kpso/football/data/germany/15.csv";
//  }
//  else if(champName == "france")
//  {
//    list << "../kpso/football/data/france/06.csv";
//    list << "../kpso/football/data/france/07.csv";
//    list << "../kpso/football/data/france/08.csv";
//    list << "../kpso/football/data/france/09.csv";
//    list << "../kpso/football/data/france/10.csv";
//    list << "../kpso/football/data/france/11.csv";
//    list << "../kpso/football/data/france/12.csv";
//    list << "../kpso/football/data/france/13.csv";
//    list << "../kpso/football/data/france/14.csv";
//    list << "../kpso/football/data/france/15.csv";
//  }

//  if (0 == confidentialSeasons)
//    return list;
//  else
//    return list.mid(list.count() - confidentialSeasons, confidentialSeasons);
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
  dir.cd(QString("../"));

  while(!dir.cd(QString("football")))
    dir.mkdir(QString("football"));

  while(!dir.cd(QString("rates")))
    dir.mkdir(QString("rates"));

  while(!dir.cd(champName))
    dir.mkdir(champName);

  QString fileName("../football/rates/%1/draw.csv");
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
  fileName.remove(QString("!"));
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
      match1.difference = line[3].toInt() - line[4].toInt();
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
      match2.difference = line[4].toInt() - line[3].toInt();
      season << match2;
    }
  }
  file.close();
  return season;
}

void CStorage::AddTeam(CTeam team, QString champName, int type)
{
  QString fileName("../football/rates/%1/%2.csv");
  fileName = fileName.arg(champName);
  if (PARITY == type)
    fileName = fileName.arg("draw");

  QFile file(fileName);
  if (!file.open(QIODevice::Append))
    return;

  QTextStream out(&file);
  out << team.GetName() << QString(",") << team.CashParityPosition() << QString(",") << 1000 << "\n";

  file.close();
}

int CStorage::ReadCurrentRate(QString teamName, QString champName, int type)
{
  QString fileName("../football/rates/%1/%2.csv");
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
    if (str.contains(teamName))
    {
      QStringList list = str.split(",");
      numCash = list.value(1).toInt();
    }
  }

  file.close();
  return numCash;
}

void CStorage::ExchangeName(QString teamName, QString champName, int type, QString targetName)
{
  QString fileName("../football/rates/%1/%2.csv");
  fileName = fileName.arg(champName);
  if (PARITY == type)
    fileName = fileName.arg("draw");

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QVector<QString> list;
  QTextStream in(&file);
  while (!in.atEnd())
  {
    list.push_back(in.readLine());
  }
  file.close();

  for(int i = 0; i < list.count(); ++i)
  {
    if(list[i].contains(teamName))
      list[i].replace(teamName, targetName);
  }

  if (!file.open(QIODevice::WriteOnly))
    return;

  QTextStream out(&file);
  foreach(QString str, list)
    out << str << "\n";

  file.close();
}

void CStorage::ExchangeRate(QString teamName, QString champName, int type, int targetValue)
{
  QString fileName("../football/rates/%1/%2.csv");
  fileName = fileName.arg(champName);
  if (PARITY == type)
    fileName = fileName.arg("draw");

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QVector<QString> list;
  QTextStream in(&file);
  while (!in.atEnd())
  {
    list.push_back(in.readLine());
  }
  file.close();

  for(int i = 0; i < list.count(); ++i)
  {
    if(list[i].contains(teamName))
    {
      QVector<QString> str = list[i].split(",").toVector();
      str[1] = QString::number(targetValue);
      list[i] = str[0] + QString(",") + str[1] + QString(",") + str[2];
    }
  }

  if (!file.open(QIODevice::WriteOnly))
    return;

  QTextStream out(&file);
  foreach(QString str, list)
    out << str << "\n";

  file.close();
}

NextTur CStorage::ReadNextTur(QString champName)
{
  NextTur nextTur;
  QFile file(GetFileNames(champName, 1).last());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return nextTur;

  QTextStream in(&file);
  while (!in.atEnd())
  {
    QString str = in.readLine();
    str.remove(" ");
    QStringList line = str.split(",");

    if (line[3] == "-" && line[4] == "-")
    {
      if (!NextContainStr(nextTur, line[1]))
        nextTur << QPair<QString, QString>(line[1], line[2]);
    }
  }
  file.close();
  return nextTur;
}

int CStorage::Report(QString champName, QString teamName)
{
  QString fileName("../football/rates/%1/draw.csv");
  fileName = fileName.arg(champName);

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return -1;

  int isReport = -1;
  QTextStream in(&file);
  while (!in.atEnd())
  {
    QString str = in.readLine();
    if (str.contains(teamName))
    {
      QStringList list = str.split(",");
      isReport = list.last().toInt();
    }
  }

  file.close();
  return isReport;
}

void CStorage::Reported(QString champName, QString teamName, int value)
{
  QString fileName("../football/rates/%1/draw.csv");
  fileName = fileName.arg(champName);

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QVector<QString> list;
  QTextStream in(&file);
  while (!in.atEnd())
  {
    list.push_back(in.readLine());
  }
  file.close();

  int lengthValue = QString::number(value).count();

  for(int i = 0; i < list.count(); ++i)
  {
    if(list[i].contains(teamName))
    {
      list[i].remove(list[i].count() - lengthValue, lengthValue);
      list[i].push_back(QString::number(value));
    }
  }

  if (!file.open(QIODevice::WriteOnly))
    return;

  QTextStream out(&file);
  foreach(QString str, list)
    out << str << "\n";
  file.close();

  bool allCheck = true;
  foreach(QString str, list)
    if(str.contains(QString::number(NOTREPORT)))
      allCheck = false;

  if (allCheck)
    emit AllChecked(true);
}

bool CStorage::NextContainStr(NextTur nextTur, const QString &str)
{
  bool contain = false;
  for(int i = 0; i < nextTur.count(); ++i)
    if(nextTur.value(i).first == str || nextTur.value(i).second == str)
      contain = true;
  return contain;
}

QMap<QString, NextTur> CStorage::ReadNext()
{
  QMap<QString, NextTur> next;
  foreach(QString champName, GetChampNames())
    next.insert(champName, ReadNextTur(champName));
  return next;
}

void CStorage::CreatePlays(QString champName, NextTur tur)
{
  QString fileName("../football/rates/%1/temp.csv");
  fileName = fileName.arg(champName);

  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly))
    return;

  QTextStream out(&file);
  for(int i = 0; i < tur.count(); ++i)
    out << tur[i].first << QString(",") << tur[i].second << "\n";

  file.close();
}

NextTur CStorage::ReadPlays(QString champName)
{
  NextTur tur;
  QString fileName("../football/rates/%1/temp.csv");
  fileName = fileName.arg(champName);

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return tur;

  QTextStream in(&file);
  while (!in.atEnd())
  {
    QVector<QString> str = in.readLine().split(",").toVector();
    tur << QPair<QString, QString>(str.first(), str.last());
  }

  file.close();
  return tur;
}
