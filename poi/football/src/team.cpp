#include "team.h"
#include <QSet>
#include <QPair>

CTeam::CTeam(const QString& name)
  : m_name(name)
  , cashParityPosition(0)
  , pointsCommon(0)
{}

CTeam::CTeam()
{}

CTeam::~CTeam()
{}

void CTeam::SetName(const QString &name)
{
  m_name = name;
}

QString CTeam::GetName()
{
  return m_name;
}

void CTeam::ExchengeConcurents(QString concurent, QString distConcurent)
{
  if (-1 == m_concurents.indexOf(concurent))
  {
    return;
  }
  else
  {
    m_concurents.remove(m_concurents.indexOf(concurent));
    m_concurents << distConcurent;
  }
}

void CTeam::FormData()
{
  pointsCommon = 0;
  differince = 0;
  foreach(CMatch match, GetSeasons())
  {
    pointsCommon += match.point;
    differince += match.difference;
  }
}

void CTeam::FindCurrentCashParity()
{
  QVector<CMatch> matchs;
  foreach(CMatch match, m_seasons)
    matchs << match;

  for(int i = matchs.count() - 1; i >= 0; --i)
  {
    if (m_concurents.contains(matchs[i].opponent))
    {
      if(matchs.value(i).point != PARITY)
        cashParityPosition++;
      else
        i = 0;
    }
  }

//  QVector<CMatch> matchs;
//  foreach(int season, m_seasons.keys())
//    foreach(CMatch match, m_seasons.value(season))
//      matchs << match;

//  for(int i = matchs.count() - 1; i >= 0; --i)
//  {
//    if (m_concurents.contains(matchs[i].opponent))
//    {
//      if(matchs.value(i).point != PARITY)
//        cashParityPosition++;
//      else
//        i = 0;
//    }
//  }
}

void CTeam::SetCurrentCashParity(int numCash)
{
  cashParityPosition = numCash;
}

void CTeam::CropMatches(int num)
{
  if (m_seasons.count() >= num)
    m_seasons.remove(0, m_seasons.count() - num);
}

void CTeam::Rename(const QString &newName)
{
  for(int i = 0; i < m_seasons.count(); ++i)
  {
    if (m_seasons[i].name == newName)
      m_seasons[i].name = newName;
    if (m_seasons[i].opponent == newName)
      m_seasons[i].opponent = newName;
  }
  m_name = newName;
}

void CTeam::FormDataCommon()
{
  int noParity = 0;
  foreach(CMatch match, GetSeasons())
  {
    if (m_concurents.contains(match.opponent))
    {
      if(match.point != 1)
      {
        noParity++;
      }
      else
      {
        noParityesCommon << noParity;
        noParity = 0;
      }
    }
  }
}

Season SelectTeamData(const QMap<int, Season>& data)
{
  QVector<CMatch> plays;
  foreach(int num, data.keys())
    foreach(CMatch match, data.value(num))
      plays << match;
  return plays;
}

CStandardItemModel::CStandardItemModel(QObject * parent)
  : Base(parent) {}

void CStandardItemModel::SetView(CTableViewWd* v)
{
  view = v;
}

CTableViewWd* CStandardItemModel::GetView()
{
  return view;
}

void CStandardItemModel::SetColumns(const QVector<QString> &names)
{
  setColumnCount(names.count());
  setHorizontalHeaderLabels(names.toList());
}

//void CStandardItemModel::PrintCommonResult(CTeam team, int numPrint, int typeOutcome)
//{
//  for(int i = 0; i < columnCount(); ++i)
//    if (team.GetName() == horizontalHeaderItem(i)->data(Qt::DisplayRole).toString())
//      setItem(numPrint, i, new QStandardItem(QString::number(team.CurrentCash(typeOutcome))));
//}

void CStandardItemModel::PrintCommonResult(QString team, int numPrint, int value)
{
  for(int i = 0; i < columnCount(); ++i)
    if (team == horizontalHeaderItem(i)->data(Qt::DisplayRole).toString())
      setItem(numPrint, i, new QStandardItem(QString::number(value)));
}

int GetCash(int num, int type)
{
  int value = 0;
  if (type == PARITY)
  {
    if (num >= PARITY_LIMIT)
      num -= PARITY_LIMIT;

    value = cashListParity.value(num);
  }
  else if (type == NO_PARITY)
  {
    if (num >= NO_PARITY_LIMIT)
      num -= NO_PARITY_LIMIT;

    value = cashListNoParity.value(num);
  }
  return value;
}

CWidget::CWidget(QWidget *parent, Qt::WindowFlags f)
  : Base(parent, f)
{
  ui.setupUi(this);
  ui.commonView->setAllColumnsDelegate();
  ui.resultView->setAllColumnsDelegate();
  CStandardItemModel* tableCommon = new CStandardItemModel(this);
  ui.commonView->setModel(tableCommon);
  tableCommon->SetView(ui.commonView);
  CStandardItemModel* tableResult = new CStandardItemModel(this);
  ui.resultView->setModel(tableResult);
  tableResult->SetView(ui.resultView);
  connect(ui.resultView, SIGNAL(doubleClicked(const QModelIndex&)), this, SIGNAL(doubleClicked(const QModelIndex&)));
  connect(ui.pushButton, SIGNAL(clicked()), this, SIGNAL(clicked()));
  connect(this, SIGNAL(AllChecked(bool)), ui.pushButton, SLOT(setEnabled(bool)));
}

CStandardItemModel* CWidget::TableCommon()
{
  CStandardItemModel* model = qobject_cast<CStandardItemModel*>(ui.commonView->model());
  return model;
}

CStandardItemModel* CWidget::TableResult()
{
  CStandardItemModel* model = qobject_cast<CStandardItemModel*>(ui.resultView->model());
  return model;
}
