#ifndef CSTORAGE_H
#define CSTORAGE_H

#include "team.h"
#include <QObject>

typedef QVector<CTeam> Championat;

class CStorage : public QObject
{
  Q_OBJECT

public:
  CStorage(QObject *parent = 0);
  ~CStorage();
  QStringList GetFileNames(const QString& champName, int confidentialSeasons = 0);
  QStringList GetChampNames();
  void FormTeams(Season season, const QString& champName, QMap<QString, Championat>& championats);
  QMap<QString, Championat> ReadFiles(int confidentialSeasons);
  QStringList GetTeamNames(const QString& champName, const QMap<QString, Championat>& championats);
  bool RateIsEmpty(QString champName);
  Season ReadFile(QString fileName);

  void AddTeam(CTeam team, QString champName, int type);
  int ReadCurrentRate(CTeam team, QString champName, int type);
};

#endif // CSTORAGE_H
