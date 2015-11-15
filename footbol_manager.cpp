#include "footbol_manager.h"
#include "storage.h"
#include "ui_footbol_manager.h"
#include "table_view.h"
#include <QStandardItemModel>

const int deltaConcurent = 3;
const int confidentialSeasons = 3;
const int parityColumn = 0;

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
    table->setVerticalHeaderLabels(QStringList() << tr("Ничьи"));
    foreach(CTeam team, m_pData->championats.value(champName))
    {
      //table->PrintCommonResult(team, NO_PARITY);
      table->PrintCommonResult(team, parityColumn, PARITY);
    }
  }
}

void CFootbolManager::Do()
{
  m_pData->championats = m_pData->storage.ReadFiles(confidentialSeasons);
  FormDataTeams();
  FormRates();
  ShowSource();
}

void CFootbolManager::FormRates()
{
  foreach(QString champName, m_pData->championats.keys())
  {
    if (m_pData->storage.RateIsEmpty(champName))
    {
      //в случае отсутствия или пустоты файлов формирование номеров ставок
      for(int i = 0; i < m_pData->championats[champName].count(); ++i)
        m_pData->championats[champName][i].FindCurrentCashParity();

      Championat ch = m_pData->storage.ReadFiles(1).last();
      QVector<QString> list;
      foreach(CTeam team, ch)
        list << team.GetName();

      foreach(CTeam team, m_pData->championats[champName])
      {
        if (list.contains(team.GetName()))
          m_pData->storage.AddTeam(team, champName, PARITY);
      }
    }
    else
    {
      //если файлы заполнены то считываем номера ставок
      for(int i = 0; i < m_pData->championats[champName].count(); ++i)
        m_pData->championats[champName][i].SetCurrentCashParity(m_pData->storage.ReadCurrentRate(m_pData->championats[champName][i], champName, PARITY));
    }
  }
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
  foreach(QString str, m_pData->championats.keys())
  {
    foreach(CTeam team, m_pData->championats.value(str))
    {
      no_paritysCommon << team.NoParityesCommon();
    }
  }

  qSort(no_paritysCommon);

  int max = 14;
  int overC = 0;
  foreach(int npc, no_paritysCommon)
  {
    if(npc > max)
      overC++;
  }

  double pC = static_cast<double>(overC) / static_cast<double>(no_paritysCommon.count());

  QMap<int, int> map1;
  foreach(int v, no_paritysCommon)
  {
    if (!map1.contains(v))
      map1.insert(v, 0);
    map1[v]++;
  }

  int cash = 0;
  int koef = 3;
  for(int i = 1; i < map1.lastKey(); ++i)
  {
    if(!map1.contains(i))
      map1.insert(i, 0);
  }

  foreach(int count, map1.keys())
  {
    int plus = 0;
    int minus = 0;
    int mCount = 0;

    while((count - cashList.count() * mCount) >= cashList.count())
      mCount++;

    plus = map1.value(count) * cashList.value(count - cashList.count() * mCount) * koef;

    while(mCount >= 0)
    {
      for(int i = 0; i <= count - cashList.count() * mCount; ++i)
        minus += map1.value(count) * cashList.value(i);
      --mCount;
    }
    cash += plus - minus;
  }

  int g = cashList.count();
  int k = g;
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
  Season season = m_pData->storage.ReadFile(m_pData->storage.GetFileNames(champName).last());
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
