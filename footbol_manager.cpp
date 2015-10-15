#include "footbol_manager.h"
#include "team.h"
#include "ui_footbol_manager.h"
#include "table_view.h"
#include <QSplitter>
#include <QStandardItemModel>

QList<int> cashList = QList<int>() << 20 << 20 << 40 << 80 << 160 << 320 << 640 << 1280 << 2560 << 5120;
QList<int> cashList2 = QList<int>() << 30 << 200 << 1200 << 7200;

class CFootbolManager::PrivateData
{
public:
  PrivateData(){}
  ~PrivateData(){}

  QVector<CTeam> teams;
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
  CStandardItemModel* table = AddTable(tr("Общий результат"));
  table->SetColumns(GetTeamNames());
  table->setVerticalHeaderLabels(QStringList() << tr("Без ничьих") << tr("Ничьи подряд"));
  foreach(CTeam team, m_pData->teams)
  {
    table->PrintCommonResult(team, NO_PARITY);
    table->PrintCommonResult(team, PARITY);
  }
}

void CFootbolManager::Do()
{
  ReadFiles();
  FormDataTeams();
  ShowSource();
}

QStringList CFootbolManager::GetTeamNames()
{
  QSet<QString> teams;
  foreach(CTeam team, m_pData->teams)
    teams << team.GetName();

  return teams.toList();
}

void CFootbolManager::FormTeams(Season season)
{
  foreach(CMatch match, season)
  {
    if (!GetTeamNames().contains(match.name))
    {
      CTeam team(match.name);
      m_pData->teams << team;
    }
  }

  foreach(CMatch match, season)
  {
    for(int i = 0; i < m_pData->teams.count(); ++i)
    {
      if (m_pData->teams[i].GetName() == match.name)
      {
        if (!m_pData->teams[i].seasons.contains(match.season))
          m_pData->teams[i].seasons.insert(match.season, Season());

        m_pData->teams[i].seasons[match.season] << match;
      }
    }
  }
}

void CFootbolManager::ReadFiles()
{
  foreach(QString fileName, GetFileNames())
    FormTeams(ReadFile(fileName));
}

void CFootbolManager::FormDataTeams()
{
  for(int i = 0; i < m_pData->teams.count(); ++i)
    m_pData->teams[i].FormData();
}

void CFootbolManager::ShowSource()
{
  CommonResult();
  SetVisibleColumns(GetLastNames());
}

void CFootbolManager::SetVisibleColumns(QStringList names)
{
  CTableViewWd* view;
  for(int i = 0; i < m_pData->ui.tabWidget->count(); ++i)
    if (m_pData->ui.tabWidget->tabText(i) == tr("Общий результат"))
      view = qobject_cast<CTableViewWd*>(m_pData->ui.tabWidget->widget(i));

  CStandardItemModel* model = qobject_cast<CStandardItemModel*>(view->model());
  for(int i = 0; i < model->columnCount(); ++i)
  {
    if(!GetLastNames().contains(model->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString()))
    {
      view->horizontalHeader()->setSectionHidden(i, true);
    }
  }
}

QStringList GetFileNames()
{
  QStringList list;
  list << "../kpso/seasons/data2.csv";
  list << "../kpso/seasons/data3.csv";
  list << "../kpso/seasons/data4.csv";
  list << "../kpso/seasons/data5.csv";
  list << "../kpso/seasons/data6.csv";
  list << "../kpso/seasons/data7.csv";
  list << "../kpso/seasons/data8.csv";
  list << "../kpso/seasons/data9.csv";
  list << "../kpso/seasons/data10.csv";
  list << "../kpso/seasons/data11.csv";
  list << "../kpso/seasons/data12.csv";
  list << "../kpso/seasons/data13.csv";
  list << "../kpso/seasons/data14.csv";
  return list;
}

QString GetCurrentSeasonFileName()
{
  return QString("../kpso/seasons/data15.csv");
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

QStringList GetLastNames()
{
  Season last = ReadFile(GetCurrentSeasonFileName());
  QSet<QString> teams;
  foreach(CMatch match, last)
    teams << match.name;

  return teams.toList();
}
