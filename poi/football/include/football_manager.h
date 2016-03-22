#ifndef STATISTICA_H
#define STATISTICA_H

#include <QMainWindow>
#include "team.h"
#include "declaration.h"
#include "storage.h"

class QSplitter;
class QStandardItemModel;

class CFootbolManager : public QMainWindow
{
  Q_OBJECT

public:
  explicit CFootbolManager(QWidget *parent = 0);
  ~CFootbolManager();
  void Do();

private:
  class PrivateData;
  PrivateData* m_pData;

  void ReplaceData();
  void FormDataTeams(QMap<QString, Championat>& championat);
  void AnalizeCommonPosition(QMap<QString, Championat>& championat);
  void FormDeltaConcurents();
  void FindLowGool(QMap<QString, Championat>& championat);
  void Analize(QMap<QString, Championat>& championat);

  void ShowSource();
  CWidget* AddTable(const QString& tableName);
  QVector<QString> GetSortNextNames(QString champName);
};

QVector<int> FindCashList(int minCash, double koef, int limit);

#endif // STATISTICA_H
