#include "statistica.h"
#include "team.h"
#include "ui_statistica.h"
#include "table_view.h"
#include <QSplitter>
#include <QStandardItemModel>

QList<int> cashList = QList<int>() << 20 << 20 << 40 << 80 << 160 << 320 << 640 << 1280 << 2560 << 5120;
QList<int> cashList2 = QList<int>() << 30 << 200 << 1200 << 7200;

class statistica::PrivateData
{
public:
  PrivateData(){}
  ~PrivateData(){}

  QVector<CTeam> teams;
  Ui::statistica ui;
};

statistica::statistica(QWidget *parent)
  : QDialog(parent)
{
  m_pData = new PrivateData();
  m_pData->ui.setupUi(this);
}

statistica::~statistica()
{
  delete m_pData;
}

void statistica::CommonResult()
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

void statistica::Do()
{
  ReadFiles();
  FormDataTeams();
  ShowSource();
//  CommonCalculation();
//  PrintCommonCalculation();

}

QStringList statistica::GetTeamNames()
{
  QSet<QString> teams;
  foreach(CTeam team, m_pData->teams)
    teams << team.GetName();

  return teams.toList();
}

void statistica::FormTeams(Season season)
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

void statistica::ReadFiles()
{
  foreach(QString fileName, GetFileNames())
    FormTeams(ReadFile(fileName));
}

void statistica::FormDataTeams()
{
  for(int i = 0; i < m_pData->teams.count(); ++i)
    m_pData->teams[i].FormData();
}

void statistica::ShowSource()
{
  CommonResult();
  SetVisibleColumns(GetLastNames());
}

void statistica::SetVisibleColumns(QStringList names)
{
  CTableViewWd* view;
  for(int i = 0; i < m_pData->ui.tabWidget->count(); ++i)
    if (m_pData->ui.tabWidget->tabText(i) == tr("Общий результат"))
      view = qobject_cast<CTableViewWd*>(m_pData->ui.tabWidget->widget(i));

  CStandardItemModel* model = qobject_cast<CStandardItemModel*>(view->model());
  for(int i = 0; i < model->columnCount(); ++i)
  {
    QString str = model->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString();
    if(!GetLastNames().contains(model->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString()))
    {
      view->horizontalHeader()->setSectionHidden(i, true);
    }
  }
}

QStringList GetFileNames()
{
  QStringList list;
  list << "../statistica/seasons/data2.csv";
  list << "../statistica/seasons/data3.csv";
  list << "../statistica/seasons/data4.csv";
  list << "../statistica/seasons/data5.csv";
  list << "../statistica/seasons/data6.csv";
  list << "../statistica/seasons/data7.csv";
  list << "../statistica/seasons/data8.csv";
  list << "../statistica/seasons/data9.csv";
  list << "../statistica/seasons/data10.csv";
  list << "../statistica/seasons/data11.csv";
  list << "../statistica/seasons/data12.csv";
  list << "../statistica/seasons/data13.csv";
  list << "../statistica/seasons/data14.csv";
  return list;
}

QString GetCurrentSeasonFileName()
{
  return QString("../statistica/seasons/data15.csv");
}

CStandardItemModel* statistica::AddTable(const QString& tableName)
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
