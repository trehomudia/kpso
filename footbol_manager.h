#ifndef STATISTICA_H
#define STATISTICA_H

#include <QDialog>
#include "team.h"

class QSplitter;
class QStandardItemModel;
class QTeam;

typedef QVector<CTeam> Championat;

class CFootbolManager : public QDialog
{
  Q_OBJECT

public:
  explicit CFootbolManager(QWidget *parent = 0);
  ~CFootbolManager();
  void Do();

private:
  class PrivateData;
  PrivateData* m_pData;

  void FormTeams(Season season, const QString& champName);
  void FormDataTeams();
  void ReadFiles();
  void ShowSource();
  void CommonResult();
  void SetVisibleColumns(const QString& champName);
  CStandardItemModel* AddTable(const QString& tableName);
  QStringList GetTeamNames(const QString& champName);
};

QStringList GetFileNames(const QString& champName);
QStringList GetChampNames();

#endif // STATISTICA_H
