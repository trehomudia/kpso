#ifndef STATISTICA_H
#define STATISTICA_H

#include <QDialog>
#include "team.h"

class QSplitter;
class QStandardItemModel;

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

  void FormDataTeams();
  void FormRates();
  void AnalizeCommonPosition();
  void ShowSource();
  void CommonResult();
  void SetVisibleColumns(const QString& champName);
  CStandardItemModel* AddTable(const QString& tableName);
};

#endif // STATISTICA_H
