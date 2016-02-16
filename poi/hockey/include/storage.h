#ifndef CSTORAGE_H
#define CSTORAGE_H

#include "team.h"
#include <QObject>

typedef QVector<CTeam> Championat;
typedef QVector<QPair<QString, QString> > NextTur;

const int REPORT = 1001;
const int NOTREPORT = 1000;

class CStorage : public QObject
{
  Q_OBJECT

public:
  CStorage(QObject *parent = 0);
  ~CStorage();
  QStringList GetFileNames(const QString& champName, int confidentialSeasons = 0);
  QStringList GetChampNames();
  void FormTeams(QVector<CMatch> matches, const QString& champName, QMap<QString, Championat>& championats);
  QMap<QString, Championat> ReadFiles(int confidentialSeasons);






  bool RateIsEmpty(QString champName);
  QVector<CMatch> ReadFile(QString fileName);
  void AddTeam(CTeam team, QString champName, int type);
  int ReadCurrentRate(QString teamName, QString champName, int type);
  void ExchangeName(QString teamName, QString champName, int type, QString targetName);
  void ExchangeRate(QString teamName, QString champName, int type, int targetValue);
  QMap<QString, NextTur> ReadNext();
  int Report(QString champName, QString teamName);
  void Reported(QString champName, QString teamName, int value);

  void CreatePlays(QString champName, NextTur tur);
  NextTur ReadPlays(QString champName);

signals:
  void AllChecked(bool);

private:
  NextTur ReadNextTur(QString champName);
  bool NextContainStr(NextTur nextTur, const QString& str);
};

#endif // CSTORAGE_H
