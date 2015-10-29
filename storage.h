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
  QStringList GetFileNames(const QString& champName);
  QStringList GetChampNames();
  void FormTeams(Season season, const QString& champName, QMap<QString, Championat>& championats);
  QMap<QString, Championat> ReadFiles();
  QStringList GetTeamNames(const QString& champName, const QMap<QString, Championat>& championats);
};

#endif // CSTORAGE_H
