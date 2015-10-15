#ifndef STATISTICA_H
#define STATISTICA_H

#include <QDialog>
#include "team.h"

class QSplitter;
class QStandardItemModel;
class QTeam;

class statistica : public QDialog
{
  Q_OBJECT

public:
  explicit statistica(QWidget *parent = 0);
  ~statistica();
  void Do();


private:
  class PrivateData;
  PrivateData* m_pData;

  void FormTeams(Season season);
  void FormDataTeams();
  void ReadFiles();
  void ShowSource();
  void CommonResult();
  void SetVisibleColumns(QStringList names);
  CStandardItemModel* AddTable(const QString& tableName);
  QStringList GetTeamNames();
};

QStringList GetFileNames();
QString GetCurrentSeasonFileName();
QStringList GetLastNames();

#endif // STATISTICA_H
