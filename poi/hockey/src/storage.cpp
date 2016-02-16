#include "storage.h"
#include <QSet>
#include <QFile>
#include <QTextStream>
#include <QDir>

CStorage::CStorage(QObject *parent) : QObject(parent){}

CStorage::~CStorage(){}

void CStorage::FormTeams(QVector<CMatch> matches, const QString& champName, QMap<QString, Championat>& championats)
{
  foreach(CMatch match, matches)
  {
    QSet<QString> t;
    foreach(CTeam team, championats[champName])
      t << team.GetName();

    QVector<QString> teams = t.toList().toVector();

    if (!teams.contains(match.name))
    {
      CTeam team(match.name);
      championats[champName] << team;
    }
  }

  foreach(CMatch match, matches)
    for(int i = 0; i < championats[champName].count(); ++i)
      if (championats[champName][i].GetName() == match.name)
        championats[champName][i].GetMatches() << match;
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
  if (champName == "khl")
  {
    list << "../../skotina/poi/hockey/resource/data/khl/14.csv";
    list << "../../skotina/poi/hockey/resource/data/khl/15.csv";
  }
  else if(champName == "mhl")
  {
    list << "../../skotina/poi/hockey/resource/data/mhl/14.csv";
    list << "../../skotina/poi/hockey/resource/data/mhl/15.csv";
  }
  else if(champName == "nhl")
  {
    list << "../../skotina/poi/hockey/resource/data/nhl/14.csv";
    list << "../../skotina/poi/hockey/resource/data/nhl/15.csv";
  }
  else if(champName == "vhl")
  {
    list << "../../skotina/poi/hockey/resource/data/vhl/14.csv";
    list << "../../skotina/poi/hockey/resource/data/vhl/15.csv";
  }

  if (0 == confidentialSeasons)
    return list;
  else
    return list.mid(list.count() - confidentialSeasons, confidentialSeasons);
}

QStringList CStorage::GetChampNames()
{
  QStringList list;
  list << "khl";
  list << "mhl";
  list << "nhl";
  list << "vhl";
  return list;
}

bool CStorage::RateIsEmpty(QString champName)
{
  QDir dir(QDir::current());
  dir.cd(QString("../kpso/hockey"));

  while(!dir.cd(QString("rates")))
    dir.mkdir(QString("rates"));

  while(!dir.cd(champName))
    dir.mkdir(champName);

  QString fileName("../kpso/hockey/rates/%1/draw.csv");
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

QVector<CMatch> CStorage::ReadFile(QString fileName)
{
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return QVector<CMatch>();

  fileName.remove(".csv");
  fileName.remove(QString("!"));
  int seasonNum = fileName.remove(0, fileName.size() - 2).toInt();

  QTextStream in(&file);
  QVector<CMatch> season;
  while (!in.atEnd())
  {
    QString str = in.readLine();
    str.remove(" ");
    QVector<QString> line = str.split(",").toVector();
    line.remove(1);
    while(line.count() > 5)
      line.remove(5);
    line[0].remove("\"");
    QVector<QString> date = line[0].split(".").toVector();


    if (line[3] != "-" && line[4] != "-")
    {
      CMatch match1;
      match1.date = QDate(date[2].toInt(), date[1].toInt(), date[0].toInt());
      match1.season = seasonNum;
//      match1.tur = line[0].toInt();
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
      match2.date = QDate(date[2].toInt(), date[1].toInt(), date[0].toInt());
      match2.season = seasonNum;
//      match2.tur = line[0].toInt();
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
  QString fileName("../kpso/hockey/rates/%1/%2.csv");
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
  QString fileName("../kpso/hockey/rates/%1/%2.csv");
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
  QString fileName("../kpso/hockey/rates/%1/%2.csv");
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
  QString fileName("../kpso/hockey/rates/%1/%2.csv");
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
  QString fileName("../kpso/hockey/rates/%1/draw.csv");
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
  QString fileName("../kpso/hockey/rates/%1/draw.csv");
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
  QString fileName("../kpso/hockey/rates/%1/temp.csv");
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
  QString fileName("../kpso/hockey/rates/%1/temp.csv");
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
