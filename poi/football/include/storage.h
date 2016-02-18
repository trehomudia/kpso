#ifndef CSTORAGE_H
#define CSTORAGE_H

#include "team.h"
#include <QObject>

const int REPORT = 1001;
const int NOTREPORT = 1000;

typedef QVector<CTeam> Championat;
typedef QVector<QPair<QString, QString> > NextTur;

class CStorage : public QObject
{
  Q_OBJECT

public:
  CStorage(QObject *parent = 0);
  ~CStorage();
  QStringList GetFileNames(const QString& champName, int confidentialSeasons = 0);
  QStringList GetChampNames();
  void FormTeams(QVector<CMatch> season, const QString& champName, QMap<QString, Championat>& championats);
  QMap<QString, Championat> ReadFiles(int confidentialSeasons = 0);
  QMap<QString, NextTur> ReadNext();
  NextTur ReadNextTur(QString champName);
  QVector<QString> GetTeamNames(const QString& champName, const QMap<QString, Championat>& championats);
  Season ReadFile(QString fileName);








  bool RateIsEmpty(QString champName);
  void AddTeam(CTeam team, QString champName, int type);
  int ReadCurrentRate(QString teamName, QString champName, int type);
  void ExchangeName(QString teamName, QString champName, int type, QString targetName);
  void ExchangeRate(QString teamName, QString champName, int type, int targetValue);
  int Report(QString champName, QString teamName);
  void Reported(QString champName, QString teamName, int value);

  void CreatePlays(QString champName, NextTur tur);
  NextTur ReadPlays(QString champName);

signals:
  void AllChecked(bool);

private:

  bool NextContainStr(NextTur nextTur, const QString& str);
};

#endif // CSTORAGE_H
