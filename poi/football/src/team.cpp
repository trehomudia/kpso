#include "team.h"
#include <QSet>
#include <QPair>

CTeam::CTeam(const QString& name)
  : m_name(name)
  , cashParityPosition(0)
  , pointsCommon(0)
  , m_noParity(0)
  , m_noWin(0)
  , m_parity(0)
  , m_noSum15(0)
  , m_noSum25(0)
  , m_noSum35(0)
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
  FormNoParity();
  FormWin();
  FormParity();
  FormSum();
}

void CTeam::FormNoParity()
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
        m_noParityes << noParity;
        noParity = 0;
      }
    }
  }

  for (int i = GetSeasons().count() - 1; i > 0; --i)
  {
    if (m_concurents.contains(GetSeasons()[i].opponent))
    {
      if(GetSeasons()[i].point != 1)
        m_noParity++;
      else
        i = 0;
    }
  }
}

void CTeam::FormWin()
{
  int noWin = 0;
  foreach(CMatch match, GetSeasons())
  {
    if (m_concurents.contains(match.opponent))
      continue;

    if (m_concurentPositions.value(m_name) > m_concurentPositions.value(match.opponent))
      continue;

    if (match.point != 3)
    {
      noWin++;
    }
    else
    {
      m_noWins << noWin;
      noWin = 0;
    }
  }

  for (int i = GetSeasons().count() - 1; i > 0; --i)
  {
    if (m_concurents.contains(GetSeasons()[i].opponent))
      continue;

    if (m_concurentPositions.value(m_name) > m_concurentPositions.value(GetSeasons()[i].opponent))
      continue;

    if (GetSeasons()[i].point != 3)
      m_noWin++;
    else
      i = 0;
  }
}

void CTeam::FormParity()
{
  int parity = 0;
  foreach(CMatch match, GetSeasons())
  {
    if (m_concurents.contains(match.opponent))
      continue;

    if(match.point == 1)
    {
      parity++;
    }
    else
    {
      m_parityes << parity;
      parity = 0;
    }
  }

  for (int i = GetSeasons().count() - 1; i > 0; --i)
  {
    if (m_concurents.contains(GetSeasons()[i].opponent))
      continue;

    if(GetSeasons()[i].point == 1)
      m_parity++;
    else
      i = 0;
  }
}

void CTeam::FormSum()
{
  int noSum15 = 0;
  int noSum25 = 0;
  int noSum35 = 0;
  foreach(CMatch match, GetSeasons())
  {
    if (m_concurents.contains(match.opponent))
      continue;

    if(static_cast<double>(match.sum) < 1.5)
    {
      noSum15++;
    }
    else
    {
      m_noSum15s << noSum15;
      noSum15 = 0;
    }
  }

  foreach(CMatch match, GetSeasons())
  {
    if (m_concurents.contains(match.opponent))
      continue;

    if(static_cast<double>(match.sum) < 2.5)
    {
      noSum25++;
    }
    else
    {
      m_noSum25s << noSum25;
      noSum25 = 0;
    }
  }

  foreach(CMatch match, GetSeasons())
  {
    if (m_concurents.contains(match.opponent))
      continue;

    if(static_cast<double>(match.sum) < 3.5)
    {
      noSum35++;
    }
    else
    {
      m_noSum35s << noSum35;
      noSum35 = 0;
    }
  }

  for (int i = GetSeasons().count() - 1; i > 0; --i)
  {
    if (m_concurents.contains(GetSeasons()[i].opponent))
      continue;

    if(static_cast<double>(GetSeasons()[i].sum) < 1.5)
      m_noSum15++;
    else
      i = 0;
  }

  for (int i = GetSeasons().count() - 1; i > 0; --i)
  {
    if (m_concurents.contains(GetSeasons()[i].opponent))
      continue;

    if(static_cast<double>(GetSeasons()[i].sum) < 2.5)
      m_noSum25++;
    else
      i = 0;
  }

  for (int i = GetSeasons().count() - 1; i > 0; --i)
  {
    if (m_concurents.contains(GetSeasons()[i].opponent))
      continue;

    if(static_cast<double>(GetSeasons()[i].sum) < 3.5)
      m_noSum35++;
    else
      i = 0;
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
