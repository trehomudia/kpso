#include "footbol_manager.h"
#include "team.h"
#include "storage.h"
#include "ui_footbol_manager.h"
#include "table_view.h"
#include <QStandardItemModel>

//QList<int> cashList = QList<int>() << 20 << 20 << 40 << 80 << 160 << 320 << 640 << 1280 << 2560 << 5120 << 10240 << 20480 << 40960;
QList<int> cashList = QList<int>() << 20 << 20 << 34 << 58 << 98 << 167 << 284 << 483 << 820 << 1395 << 2371 << 4031 << 6854 << 11652 << 19807;

//QList<int> cashList3 = QList<int>() << 30 << 200 << 1200 << 7200;

const int deltaConcurent = 2;

class CFootbolManager::PrivateData
{
public:
  PrivateData(){}
  ~PrivateData(){}

  QMap<QString, Championat> championats;
  Ui::statistica ui;
  CStorage storage;
};

CFootbolManager::CFootbolManager(QWidget *parent)
  : QDialog(parent)
{
  m_pData = new PrivateData();
  m_pData->ui.setupUi(this);
}

CFootbolManager::~CFootbolManager()
{
  delete m_pData;
}

void CFootbolManager::CommonResult()
{
  foreach(QString champName, m_pData->championats.keys())
  {
    CStandardItemModel* table = AddTable(tr("Общий результат: ") + champName);
    table->SetColumns(m_pData->storage.GetTeamNames(champName, m_pData->championats));
    table->setVerticalHeaderLabels(QStringList() << tr("Без ничьих") << tr("Ничьи подряд"));
    foreach(CTeam team, m_pData->championats.value(champName))
    {
      table->PrintCommonResult(team, NO_PARITY);
      table->PrintCommonResult(team, PARITY);
    }
  }
}

void CFootbolManager::Do()
{
  m_pData->championats = m_pData->storage.ReadFiles();
  FormDataTeams();
  ShowSource();
}

void CFootbolManager::AnalizeCommonPosition()
{
  foreach(QString champName, m_pData->championats.keys())
  {
    for(int i = 0; i < m_pData->championats[champName].count(); ++i)
    {
      for(int j = i; j < m_pData->championats[champName].count(); j++)
      {
        if (m_pData->championats[champName][j].PointsCommon() > m_pData->championats[champName][i].PointsCommon())
        {
          CTeam team = m_pData->championats[champName][j];
          m_pData->championats[champName][j] = m_pData->championats[champName][i];
          m_pData->championats[champName][i] = team;
        }
      }
    }
  }

  foreach(QString champName, m_pData->championats.keys())
  {
    for(int i = 0; i < m_pData->championats[champName].count(); ++i)
    {
      QList<QString> con_s;
      for(int j = 0; j < m_pData->championats[champName].count(); j++)
      {
        if (abs(j - i) <= deltaConcurent)
        {
          con_s << m_pData->championats[champName][j].GetName();
        }
      }
      con_s.removeAll(m_pData->championats[champName][i].GetName());
      m_pData->championats[champName][i].SetConcurents(con_s.toVector());
    }
  }
}

void CFootbolManager::FormDataTeams()
{
  foreach(QString champName, m_pData->championats.keys())
    for(int i = 0; i < m_pData->championats[champName].count(); ++i)
      m_pData->championats[champName][i].FormData();

  AnalizeCommonPosition();

  foreach(QString champName, m_pData->championats.keys())
    for(int i = 0; i < m_pData->championats[champName].count(); ++i)
      m_pData->championats[champName][i].FormDataCommon();

  QVector<int> no_paritysCommon;
  QVector<int> no_paritys;
  foreach(QString str, m_pData->championats.keys())
  {
    foreach(CTeam team, m_pData->championats.value(str))
    {
      no_paritysCommon << team.NoParityesCommon();
      no_paritys << team.NoParityes();

      //if (str == "russia")
      //{
        QVector<int> no_parC = team.NoParityesCommon();
        QVector<int> no_par = team.NoParityes();
        qSort(no_parC);
        qSort(no_par);

        QMap<int, int> map1;
        foreach(int v, no_parC)
        {
          if (v > 14)
            int g = 8;
          if (!map1.contains(v))
            map1.insert(v, 0);
          map1[v]++;
        }

        QMap<int, int> map2;
        foreach(int v, no_par)
        {
          if (!map2.contains(v))
            map2.insert(v, 0);
          map2[v]++;
        }

        int h = 8;
      //}
    }
  }

  qSort(no_paritysCommon);
  qSort(no_paritys);

//  int p1 = no_paritysCommon.count() * 0.01;
//  p1 = no_paritysCommon.value(no_paritysCommon.count() - p1);
//  int p2 = no_paritys.count() * 0.01;
//  p2 = no_paritys.value(no_paritys.count() - p2);

  int max = 14;
  int overC = 0;
  int over = 0;
  foreach(int npc, no_paritysCommon)
  {
    if(npc > max)
      overC++;
  }
  foreach(int np, no_paritys)
  {
    if(np > max)
      over++;
  }

  double pC = static_cast<double>(overC) / static_cast<double>(no_paritysCommon.count());
  double p = static_cast<double>(over) / static_cast<double>(no_paritys.count());

  QMap<int, int> map1;
  foreach(int v, no_paritysCommon)
  {
    if (!map1.contains(v))
      map1.insert(v, 0);
    map1[v]++;
  }

  QMap<int, int> map2;
  foreach(int v, no_paritys)
  {
    if (!map2.contains(v))
      map2.insert(v, 0);
    map2[v]++;
  }


  int cash = 0;
  int result = 0;
  int koef = 3;
  for(int i = 1; i < map1.size(); ++i)
  {
    if(!map1.contains(i))
      map1.insert(i, 0);
  }

  foreach(int count, map1.keys())
  {
    int l1 = cashList.value(count - 1);
    int l2 = map1.value(count);
    if (count <= cashList.count())
    {
      int k1 = cashList.value(count - 1) * koef;
      int k2 = (cashList.value(count - 1) * koef) - cash;
      result += ((cashList.value(count - 1) * koef) - cash) * map1.value(count);
    }
    else
    {
      result -= map1.value(count) * cashList.last();
    }
    cash += cashList.value(count - 1);
    int f = 8;
  }

  int k = 9;
}

void CFootbolManager::ShowSource()
{
  CommonResult();
  foreach(QString champName, m_pData->championats.keys())
    SetVisibleColumns(champName);
}

void CFootbolManager::SetVisibleColumns(const QString& champName)
{
  CTableViewWd* view;
  for(int i = 0; i < m_pData->ui.tabWidget->count(); ++i)
    if (m_pData->ui.tabWidget->tabText(i) == (tr("Общий результат: ") + champName))
      view = qobject_cast<CTableViewWd*>(m_pData->ui.tabWidget->widget(i));

  QSet<QString> set;
  Season season = ReadFile(m_pData->storage.GetFileNames(champName).last());
  foreach(CMatch match, season)
    set << match.name;

  QStringList names = set.toList();

  CStandardItemModel* model = qobject_cast<CStandardItemModel*>(view->model());
  for(int i = 0; i < model->columnCount(); ++i)
  {
    if(!names.contains(model->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString()))
    {
      view->horizontalHeader()->setSectionHidden(i, true);
    }
  }
}

CStandardItemModel* CFootbolManager::AddTable(const QString& tableName)
{
  CTableViewWd* view = new CTableViewWd(this);
  view->setAllColumnsDelegate();
  CStandardItemModel* table = new CStandardItemModel(this);
  view->setModel(table);
  table->SetView(view);
  m_pData->ui.tabWidget->addTab(view, tableName);
  return table;
}
