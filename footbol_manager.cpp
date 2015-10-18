#include "footbol_manager.h"
#include "team.h"
#include "ui_footbol_manager.h"
#include "table_view.h"
#include <QStandardItemModel>

QList<int> cashList = QList<int>() << 20 << 20 << 40 << 80 << 160 << 320 << 640 << 1280 << 2560 << 5120;
QList<int> cashList2 = QList<int>() << 30 << 200 << 1200 << 7200;

class CFootbolManager::PrivateData
{
public:
  PrivateData(){}
  ~PrivateData(){}

  QMap<QString, Championat> championats;
  Ui::statistica ui;
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
    table->SetColumns(GetTeamNames(champName));
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
  ReadFiles();
  FormDataTeams();
  ShowSource();
}

QStringList CFootbolManager::GetTeamNames(const QString& champName)
{
  QSet<QString> teams;
  foreach(CTeam team, m_pData->championats[champName])
    teams << team.GetName();

  return teams.toList();
}

void CFootbolManager::FormTeams(Season season, const QString& champName)
{
  foreach(CMatch match, season)
  {
    if (!GetTeamNames(champName).contains(match.name))
    {
      CTeam team(match.name);
      m_pData->championats[champName] << team;
    }
  }

  foreach(CMatch match, season)
  {
    for(int i = 0; i < m_pData->championats[champName].count(); ++i)
    {
      if (m_pData->championats[champName][i].GetName() == match.name)
      {
        if (!m_pData->championats[champName][i].seasons.contains(match.season))
          m_pData->championats[champName][i].seasons.insert(match.season, Season());

        m_pData->championats[champName][i].seasons[match.season] << match;
      }
    }
  }
}

void CFootbolManager::ReadFiles()
{
  foreach(QString champ, GetChampNames())
    m_pData->championats.insert(champ, Championat());

  foreach(QString champName, m_pData->championats.keys())
    foreach(QString fileName, GetFileNames(champName))
      FormTeams(ReadFile(fileName), champName);
}

void CFootbolManager::FormDataTeams()
{
  foreach(QString champName, m_pData->championats.keys())
    for(int i = 0; i < m_pData->championats[champName].count(); ++i)
      m_pData->championats[champName][i].FormData();
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
  Season season = ReadFile(GetFileNames(champName).last());
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

QStringList GetFileNames(const QString& champName)
{
  QStringList list;
  if (champName == "russia")
  {
    list << "../kpso/data/russia/02.csv";
    list << "../kpso/data/russia/03.csv";
    list << "../kpso/data/russia/04.csv";
    list << "../kpso/data/russia/05.csv";
    list << "../kpso/data/russia/06.csv";
    list << "../kpso/data/russia/07.csv";
    list << "../kpso/data/russia/08.csv";
    list << "../kpso/data/russia/09.csv";
    list << "../kpso/data/russia/10.csv";
    list << "../kpso/data/russia/11.csv";
    list << "../kpso/data/russia/12.csv";
    list << "../kpso/data/russia/13.csv";
    list << "../kpso/data/russia/14.csv";
    list << "../kpso/data/russia/15.csv";
  }
  else if(champName == "england")
  {
    list << "../kpso/data/england/06.csv";
    list << "../kpso/data/england/07.csv";
    list << "../kpso/data/england/08.csv";
    list << "../kpso/data/england/09.csv";
    list << "../kpso/data/england/10.csv";
    list << "../kpso/data/england/11.csv";
    list << "../kpso/data/england/12.csv";
    list << "../kpso/data/england/13.csv";
    list << "../kpso/data/england/14.csv";
    list << "../kpso/data/england/15.csv";
  }
  return list;
}

QStringList GetChampNames()
{
  QStringList list;
  list << "russia";
  list << "england";
  return list;
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
