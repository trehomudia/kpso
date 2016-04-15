#include "football_manager.h"
#include "storage.h"
#include "ui_football_manager.h"
#include "table_view.h"
#include <QStandardItemModel>
#include <QDebug>

const int confidentialSeasons = 2;
const int parityColumn = 0;

class CFootbolManager::PrivateData
{
public:
  PrivateData(){}
  ~PrivateData(){}

  QMap<QString, Championat> bigData;
  QMap<QString, Championat> championats;
  QMap<QString, NextTur> nextTurs;
  QMap<QString, int> deltaConcurents;
  Ui::statistica ui;
  CStorage storage;

  QVector<QString> lowGoalListBigData;
  QVector<QString> lowGoalListChampionats;
  QMap<QString, QVector<int> > parityCash;
  QMap<QString, QVector<int> > winCash;
  QMap<QString, QVector<int> > noParityCash;
  QMap<QString, QVector<int> > sum15Cash;
  QMap<QString, QVector<int> > sum25Cash;
  QMap<QString, QVector<int> > sum35Cash;
};

CFootbolManager::CFootbolManager(QWidget *parent)
  : QMainWindow(parent)
{
  m_pData = new PrivateData();
  m_pData->ui.setupUi(this);
}

CFootbolManager::~CFootbolManager()
{
  delete m_pData;
}

void CFootbolManager::ReplaceData()
{
  //первичная обработка данных
  //заменая имен выбывших команд на вошедшие, перемещение результатов их игр в результаты новых
  //таким образом при малых количествах игр вошедших коменд им добавятся очки выбывших
  //ограничение количества игр количеством игр в одном сезоне
  foreach(QString champName, m_pData->championats.keys())
  {
    QVector<QString> namesNext;
    for(int i = 0; i < m_pData->nextTurs[champName].count(); ++i)
    {
      namesNext << m_pData->nextTurs[champName][i].first;
      namesNext << m_pData->nextTurs[champName][i].second;
    }

    QSet<QString> setPrev;
    foreach(CTeam team, m_pData->championats[champName])
      if (!namesNext.contains(team.GetName()))
        setPrev << team.GetName();

    QMap<QString, Championat> championats;
    championats.insert(champName, Championat());
    m_pData->storage.FormTeams(m_pData->storage.ReadFile(m_pData->storage.GetFileNames(champName, 2).first()), champName, championats);
    Championat championat(championats.value(championats.keys().first()));

    QSet<QString> namesPrev;
    foreach(CTeam team, championat)
      namesPrev << team.GetName();

    QSet<QString> setNext;
    foreach(QString name, namesNext)
      if (!namesPrev.contains(name))
        setNext << name;

    if (setNext.count() != setPrev.count())
      return;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Впилить лог  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1

    QVector< QPair< QString, QString> > pair;
    for(int i = 0; i < setPrev.count(); ++i)
      pair << QPair<QString, QString>(setPrev.toList()[i], setNext.toList()[i]);
///Найдено соответствие

    for(int pairCount = 0; pairCount < pair.count(); ++pairCount)
    {
      for (int i = 0; i < m_pData->championats[champName].count(); ++i)
      {
        if (m_pData->championats[champName][i].GetName() == pair[pairCount].first)
          m_pData->championats[champName][i].Rename(pair[pairCount].second);

        for (int j = 0; j < m_pData->championats[champName].count(); ++j)
        {
          if (m_pData->championats[champName][i].GetName() == m_pData->championats[champName][j].GetName() && m_pData->championats[champName][i].GetSeasons().count() != m_pData->championats[champName][j].GetSeasons().count())
          {
            m_pData->championats[champName][i].GetSeasons() << m_pData->championats[champName][j].GetSeasons();
            m_pData->championats[champName].remove(j);
            --j;
          }
        }
      }
    }

    for(int i = 0; i < m_pData->championats.value(champName).count(); ++i)
      m_pData->championats[champName][i].CropMatches(2 * (championat.count() - 1));
  }
}

void CFootbolManager::FormDeltaConcurents()
{
  m_pData->deltaConcurents.clear();
  foreach(QString name, m_pData->nextTurs.keys())
    m_pData->deltaConcurents.insert(name, m_pData->nextTurs.value(name).count() / 2 - 2);
}

void CFootbolManager::Do()
{
  m_pData->bigData = m_pData->storage.ReadFiles();
  m_pData->championats = m_pData->storage.ReadFiles(confidentialSeasons);
  m_pData->nextTurs = m_pData->storage.ReadNext();
  FormDeltaConcurents();
  ReplaceData();
  FormDataTeams(m_pData->bigData);
  FormDataTeams(m_pData->championats);
//  FindLowGool(m_pData->bigData);
//  FindLowGool(m_pData->championats);
  Analize(m_pData->bigData);

  ShowSource();
}

void CFootbolManager::FindLowGool(QMap<QString, Championat> &championat)
{
  QVector<QString> lowGoalList;
  QMultiMap<double, QString> moComand;
  foreach(QString name, championat.keys())
  {
    foreach(CTeam team, championat.value(name))
    {
      double mo = 0;
      foreach(CMatch match, team.GetSeasons())
        mo += match.sum;
      mo /= team.GetSeasons().count();
      moComand.insert(mo, team.GetName());
    }
  }

  foreach(double v, moComand.keys())
    if (v < 2)
      lowGoalList << moComand.value(v);
  int k = 0;
}

void CFootbolManager::Analize(QMap<QString, Championat>& championat)
{
  foreach(QString champName, championat.keys())
  {
    QVector<int> noParityes;
    QVector<int> noWins;
    QVector<int> parityes;
    QVector<int> noSum15s;
    QVector<int> noSum25s;
    QVector<int> noSum35s;

    double koefNoParity = 0.004;
    double koefWin = 0.004;
    double koefParity = 0.001;
    double koefSum = 0.001;

    foreach(CTeam team, championat.value(champName))
    {
      noParityes << team.NoParityes();
      noWins << team.NoWins();
      parityes << team.Parityes();
      noSum15s << team.NoSum15s();
      noSum25s << team.NoSum25s();
      noSum35s << team.NoSum35s();
    }

    qSort(noParityes);
    qSort(noWins);
    qSort(parityes);
    qSort(noSum15s);
    qSort(noSum25s);
    qSort(noSum35s);

    int k1 = noParityes.count() * koefNoParity;
    int k2 = noWins.count() * koefWin;
    int k3 = parityes.count() * koefParity;
    int k4 = noSum15s.count() * koefSum;
    int k5 = noSum25s.count() * koefSum;
    int k6 = noSum35s.count() * koefSum;

    noParityes.remove(noParityes.count() - k1, k1);
    noWins.remove(noWins.count() - k2, k2);
    parityes.remove(parityes.count() - k3, k3);
    noSum15s.remove(noSum15s.count() - k4, k4);
    noSum25s.remove(noSum25s.count() - k5, k5);
    noSum35s.remove(noSum35s.count() - k6, k6);

    int l1 = noParityes.last();
    int l2 = noWins.last();
    int l3 = parityes.last();
    int l4 = noSum15s.last();
    int l5 = noSum25s.last();
    int l6 = noSum35s.last();

    for(int i = 0; i < championat.value(champName).count(); ++i)
    {
      if(!championat[champName][i].NoParityes().isEmpty())
      {
        if (noParityes.last() >= championat[champName][i].NoParityes().last())
          championat[champName][i].NoParityesCashList() = FindCashList(20, PARITY_KOEF, 10000, noParityes.last());
        else
          championat[champName][i].NoParityesCashList() = FindCashList(20, PARITY_KOEF, 10000, championat[champName][i].NoParityes().last());
      }

      if(!championat[champName][i].NoWins().isEmpty())
      {
        if (noWins.last() >= championat[champName][i].NoWins().last())
          championat[champName][i].NoWinCashList() = FindCashList(20, WIN_KOEF, 10000, noWins.last());
        else
          championat[champName][i].NoWinCashList() = FindCashList(20, WIN_KOEF, 10000, championat[champName][i].NoWins().last());
      }

      if(!championat[champName][i].Parityes().isEmpty())
      {
        if (parityes.last() >= championat[champName][i].Parityes().last())
          championat[champName][i].ParityesCashList() = FindCashList(20, NO_PARITY_KOEF, 10000, parityes.last());
        else
          championat[champName][i].ParityesCashList() = FindCashList(20, NO_PARITY_KOEF, 10000, championat[champName][i].Parityes().last());
      }

      if(!championat[champName][i].NoSum15s().isEmpty())
      {
        if (noSum15s.last() >= championat[champName][i].NoSum15s().last())
          championat[champName][i].NoSum15CashList() = FindCashList(20, SUM15_KOEF, 10000, noSum15s.last());
        else
          championat[champName][i].NoSum15CashList() = FindCashList(20, SUM15_KOEF, 10000, championat[champName][i].NoSum15s().last());
      }

      if(!championat[champName][i].NoSum25s().isEmpty())
      {
        if (noSum25s.last() >= championat[champName][i].NoSum25s().last())
          championat[champName][i].NoSum25CashList() = FindCashList(20, SUM25_KOEF, 10000, noSum25s.last());
        else
          championat[champName][i].NoSum25CashList() = FindCashList(20, SUM25_KOEF, 10000, championat[champName][i].NoSum25s().last());
      }

      if(!championat[champName][i].NoSum35s().isEmpty())
      {
        if (noSum35s.last() >= championat[champName][i].NoSum35s().last())
          championat[champName][i].NoSum35CashList() = FindCashList(20, SUM35_KOEF, 10000, noSum35s.last());
        else
          championat[champName][i].NoSum35CashList() = FindCashList(20, SUM35_KOEF, 10000, championat[champName][i].NoSum35s().last());
      }
    }

    m_pData->parityCash.insert(champName, FindCashList(20, PARITY_KOEF, 10000, noParityes.last()));
    m_pData->winCash.insert(champName, FindCashList(20, WIN_KOEF, 10000, noWins.last()));
    m_pData->noParityCash.insert(champName, FindCashList(20, NO_PARITY_KOEF, 10000, parityes.last()));
    m_pData->sum15Cash.insert(champName, FindCashList(20, SUM15_KOEF, 10000, noSum15s.last()));
    m_pData->sum25Cash.insert(champName, FindCashList(20, SUM25_KOEF, 10000, noSum25s.last()));
    m_pData->sum35Cash.insert(champName, FindCashList(20, SUM35_KOEF, 10000, noSum35s.last()));




  }









}

void CFootbolManager::AnalizeCommonPosition(QMap<QString, Championat>& championat)
{
  //сортировка команд по очкам
  foreach(QString champName, championat.keys())
  {
    for(int i = 0; i < championat[champName].count(); ++i)
    {
      for(int j = i; j < championat[champName].count(); j++)
      {
        if (championat[champName][j].PointsCommon() > championat[champName][i].PointsCommon())
        {
          CTeam team = championat[champName][j];
          championat[champName][j] = championat[champName][i];
          championat[champName][i] = team;
        }
        else if(championat[champName][j].PointsCommon() == championat[champName][i].PointsCommon() &&
                championat[champName][j].Differince() > championat[champName][i].Differince())
        {
          CTeam team = championat[champName][j];
          championat[champName][j] = championat[champName][i];
          championat[champName][i] = team;
        }
      }
    }
  }

  //поиск конкурентов
  foreach(QString champName, championat.keys())
  {
    QMap<QString, int> positions;
    for(int i = 0; i < championat[champName].count(); ++i)
      positions.insert(championat[champName][i].GetName(), i);

    for(int i = 0; i < championat[champName].count(); ++i)
    {
      QList<QString> con_s;
      int counter = 1;
      while(con_s.count() < m_pData->deltaConcurents.value(champName) * 2)
      {
        if (championat[champName].count() > i + counter)
          con_s << championat[champName][i + counter].GetName();

        if (i - counter >= 0)
          con_s << championat[champName][i - counter].GetName();

        counter++;
      }

      championat[champName][i].SetConcurents(con_s.toVector(), positions);


//      QList<QString> con_s;
//      for(int j = 0; j < championat[champName].count(); j++)
//      {
//        if (abs(j - i) <= m_pData->deltaConcurents.value(champName))
//        {
//          con_s << championat[champName][j].GetName();
//        }
//      }
//      con_s.removeAll(championat[champName][i].GetName());
//      championat[champName][i].SetConcurents(con_s.toVector(), positions);
    }
  }

}

void CFootbolManager::FormDataTeams(QMap<QString, Championat>& championat)
{
  foreach(QString champName, championat.keys())
    for(int i = 0; i < championat[champName].count(); ++i)
      championat[champName][i].FormData();

  AnalizeCommonPosition(championat);

  foreach(QString champName, championat.keys())
    for(int i = 0; i < championat[champName].count(); ++i)
      championat[champName][i].FormDataCommon();
}

QVector<QString> CFootbolManager::GetSortNextNames(QString champName)
{
  QVector<QString> names;
  for(int i = 0; i < m_pData->nextTurs.value(champName).count(); ++i)
  {
    names << m_pData->nextTurs.value(champName)[i].first;
    names << m_pData->nextTurs.value(champName)[i].second;
  }

  QVector<QString> sortNames;
  sortNames.resize(names.size());
  for(int i = 0; i < names.count(); ++i)
  {
    if (names.contains(m_pData->championats[champName][i].GetName()))
      sortNames[i] = m_pData->championats[champName][i].GetName();
  }

  foreach(QString name, sortNames)
  {
    if(names.contains(name))
      names.remove(names.indexOf(name));
  }

  for(int i = 0; i < sortNames.count(); ++i)
  {
    if (sortNames[i].isEmpty())
    {
      sortNames[i] = names.first();
      names.remove(0);
    }
  }

  return sortNames;
}

void CFootbolManager::ShowSource()
{
  foreach(QString champName, m_pData->championats.keys())
  {
    CWidget* table = AddTable(champName);
    CStandardItemModel* modelCommon = table->TableCommon();
    modelCommon->setHorizontalHeaderLabels(QStringList() << "1" << "2" << QObject::trUtf8("На ничью") << QObject::trUtf8("На победу") << QObject::trUtf8("На отс-е ничьи") << QObject::trUtf8("На сумму > 1.5") << QObject::trUtf8("На сумму > 2.5") << QObject::trUtf8("На сумму < 3.5"));
    for(int i = 0; i < m_pData->nextTurs.value(champName).count(); ++i)
    {
      CTeam t1, t2;
      foreach(CTeam team, m_pData->championats.value(champName))
      {
        if(team.GetName() == m_pData->nextTurs.value(champName)[i].first)
          t1 = team;

        if(team.GetName() == m_pData->nextTurs.value(champName)[i].second)
          t2 = team;
      }

      modelCommon->setItem(i, 0, new QStandardItem(t1.GetName()));
      modelCommon->setItem(i, 1, new QStandardItem(t2.GetName()));
      modelCommon->setItem(i, 2, new QStandardItem(QString::number(t1.ParityCash(t2, m_pData->parityCash.value(champName)) + t2.ParityCash(t1, m_pData->parityCash.value(champName)))));
      QString str;
      if(t1.ConcurentPositions().value(t1.GetName()) < t2.ConcurentPositions().value(t2.GetName()))
        str = tr("(") + t1.GetName() + tr(")");
      else
        str = tr("(") + t2.GetName() + tr(")");
      modelCommon->setItem(i, 3, new QStandardItem(QString::number(t1.WinCash(t2, m_pData->winCash.value(champName)) + t2.WinCash(t1, m_pData->winCash.value(champName))) + str));
      modelCommon->setItem(i, 4, new QStandardItem(QString::number(t1.NoParityCash(t2, m_pData->noParityCash.value(champName)) + t2.NoParityCash(t1, m_pData->noParityCash.value(champName)))));
      modelCommon->setItem(i, 5, new QStandardItem(QString::number(t1.Sum15Cash(t2, m_pData->sum15Cash.value(champName)) + t2.Sum15Cash(t1, m_pData->sum15Cash.value(champName)))));
      modelCommon->setItem(i, 6, new QStandardItem(QString::number(t1.Sum25Cash(t2, m_pData->sum25Cash.value(champName)) + t2.Sum25Cash(t1, m_pData->sum25Cash.value(champName)))));
      modelCommon->setItem(i, 7, new QStandardItem(QString::number(t1.Sum35Cash(t2, m_pData->sum35Cash.value(champName)) + t2.Sum35Cash(t1, m_pData->sum35Cash.value(champName)))));

    }
    table->View()->setAllColumnsDelegate();
  }
}

CWidget* CFootbolManager::AddTable(const QString& tableName)
{
  CWidget* widget = new CWidget(this);
  widget->setObjectName(tableName);
  m_pData->ui.tabWidget->addTab(widget, tableName);
  return widget;
}

QVector<int> FindCashList(int minCash, double koef, int limit, int count)
{
  QVector<int> vector;
  int current = minCash;
  while(current <= limit)
  {
    vector << current;
    int sum = 0;
    foreach(int value, vector)
      sum += value;
    current = sum / (koef - 1) + minCash;
  }

  if (vector.count() >= count)
  {
    if(count / 10 >= 1)
    {
      vector.push_front(0);
      vector.push_front(0);
    }
    else
    {
      vector.push_front(0);
    }
  }
  else
  {
    count++;
  }

  while (vector.count() < count)
    vector.push_front(0);

  return vector;
}
