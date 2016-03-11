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
    m_pData->deltaConcurents.insert(name, m_pData->nextTurs.value(name).count() / 2 - 1);
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
  Analize(m_pData->championats);

  FormRates();
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
}

void CFootbolManager::Analize(QMap<QString, Championat>& championat)
{
  QVector<int> parityes;
  QVector<int> noSum15s;
  QVector<int> noSum25s;
  QVector<int> noSum35s;
  QVector<int> noWins;
  double koef = 0.0001;
  double koefParity = 0.0005;
  double koefWin = 0.001;
  foreach(QString campName, championat.keys())
  {
    foreach(CTeam team, championat.value(campName))
    {
      parityes << team.Parityes();
      noSum15s << team.NoSum15s();
      noSum25s << team.NoSum25s();
      noSum35s << team.NoSum35s();
      noWins << team.NoWins();
    }
  }
  qSort(parityes);
  qSort(noSum15s);
  qSort(noSum25s);
  qSort(noSum35s);
  qSort(noWins);

//  int k1 = parityes.count() * koef;
  int k1 = parityes.count() * koefParity;
  int k2 = noWins.count() * koefWin;
  int k3 = noSum15s.count() * koef;
  int k4 = noSum25s.count() * koef;
  int k5 = noSum35s.count() * koef;


//  parityes.remove(parityes.count() - k1, k1);
//  noWins.remove(noWins.count() - k2, k2);
//  noSum15s.remove(noSum15s.count() - k3, k3);
//  noSum25s.remove(noSum25s.count() - k4, k4);
//  noSum35s.remove(noSum35s.count() - k5, k5);


  QVector<int> v1 = FindCashList(20, PARITY_KOEF, 10000);
  QVector<int> v2 = FindCashList(20, NO_PARITY_KOEF, 10000);
  QVector<int> v3 = FindCashList(20, WIN_KOEF, 10000);
  QVector<int> v4 = FindCashList(20, SUM15_KOEF, 10000);
  QVector<int> v5 = FindCashList(20, SUM25_KOEF, 10000);
  QVector<int> v6 = FindCashList(20, SUM35_KOEF, 10000);

  int l1 = parityes.last();
  int l2 = noWins.last();
  int l3 = noSum15s.last();
  int l4 = noSum25s.last();
  int l5 = noSum35s.last();

  int l = 9;
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
      QVector<QString> list = GetSortNextNames(champName);

      for(int i = 0; i < list.count(); ++i)
      {
        if (m_pData->championats[champName][i].GetName() == list[i])
        {
          m_pData->storage.AddTeam(m_pData->championats[champName][i], champName, PARITY);
        }
        else
        {
          m_pData->championats[champName][i].SetName(list[i]);
          m_pData->storage.AddTeam(m_pData->championats[champName][i], champName, PARITY);
        }
      }
    }
    else
    {
      QVector<QString> list = GetSortNextNames(champName);
      AddValues(champName);
      for(int i = 0; i < list.count(); ++i)
      {
        int numCashParity = m_pData->storage.ReadCurrentRate(list[i], champName, PARITY);
        if (-1 != numCashParity && list[i] == m_pData->championats[champName][i].GetName())
        {
          m_pData->championats[champName][i].SetCurrentCashParity(numCashParity);
        }
        else
        {
          for(int j = 0; j < m_pData->championats[champName].count(); ++j)
            m_pData->championats[champName][j].ExchengeConcurents(m_pData->championats[champName][i].GetName(), list[i]);

          m_pData->storage.ExchangeName(m_pData->championats[champName][i].GetName(), champName, PARITY, list[i]);
          m_pData->championats[champName][i].SetName(list[i]);
          m_pData->championats[champName][i].SetCurrentCashParity(m_pData->storage.ReadCurrentRate(m_pData->championats[champName][i].GetName(), champName, PARITY));
        }
      }

    }
  }
}

void CFootbolManager::AddValues(const QString& tableName)
{
//  NextTur tur = m_pData->storage.ReadPlays(tableName);
//  int count = 0;
//  for (int i = 0; i < tur.count(); ++i)
//  {
//    foreach(CTeam team, m_pData->championats[tableName])
//    {
//      Season s = team.GetSeasons().value(team.GetSeasons().keys().last());
//      if (tur[i].first == team.GetName() && tur[i].second == s.last().opponent)
//      {
//        count++;
//      }
//    }
//  }

//  if (count != tur.count())
//    return;

//  for (int i = 0; i < tur.count(); ++i)
//  {
//    foreach(CTeam team, m_pData->championats[tableName])
//    {
////      Season s = team.GetSeasons().last();
//      Season s = team.GetSeasons().value(team.GetSeasons().keys().last());
//      if (tur[i].first == team.GetName() && tur[i].second == s.last().opponent && team.Concurents().contains(tur[i].second))
//      {
//        if(s.last().point == 1)
//        {
//          m_pData->storage.ExchangeRate(tur[i].first, tableName, PARITY, 0);
//          m_pData->storage.ExchangeRate(tur[i].second, tableName, PARITY, 0);
//        }
//        else
//        {
//          m_pData->storage.ExchangeRate(tur[i].first, tableName, PARITY, m_pData->storage.ReadCurrentRate(tur[i].first, tableName, PARITY) + 1);
//          m_pData->storage.ExchangeRate(tur[i].second, tableName, PARITY, m_pData->storage.ReadCurrentRate(tur[i].second, tableName, PARITY) + 1);
//        }
//      }
//    }
//  }
//  int g = 9;
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
//      QList<QString> con_s;
//      int count = m_pData->deltaConcurents.value(champName) * 2 + 1;
//      int num = -1;
//      for(int j = 0; j < championat[champName].count(); j++)
//      {
//        if (abs(j - i) <= m_pData->deltaConcurents.value(champName) * 2)
//        {
//          num = j;
//          j = championat[champName].count();
//        }
//      }

//      while(count > 0)
//      {
//        con_s << championat[champName][num].GetName();
//        num++;
//        count--;
//      }

//      con_s.removeAll(championat[champName][i].GetName());
//      championat[champName][i].SetConcurents(con_s.toVector(), positions);


      QList<QString> con_s;
      for(int j = 0; j < championat[champName].count(); j++)
      {
        if (abs(j - i) <= m_pData->deltaConcurents.value(champName))
        {
          con_s << championat[champName][j].GetName();
        }
      }
      con_s.removeAll(championat[champName][i].GetName());
      championat[champName][i].SetConcurents(con_s.toVector(), positions);
    }
  }

  int k = 9;
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
    modelCommon->SetColumns(GetSortNextNames(champName));
    modelCommon->setVerticalHeaderLabels(QStringList() << QObject::trUtf8("Ничьи"));
    foreach(CTeam team, m_pData->championats.value(champName))
    {
      //table->PrintCommonResult(team, NO_PARITY);
//      model->PrintCommonResult(team, parityColumn, PARITY);
      modelCommon->PrintCommonResult(team.GetName(), parityColumn, team.CashParityPosition());
    }

    CStandardItemModel* modelResult = table->TableResult();
    modelResult->SetColumns(QVector<QString>() << QObject::trUtf8("Команда1") << QObject::trUtf8("Команда2") << QObject::trUtf8("Ничья")/* << QString("ХНичья")*/);
    for(int i = 0; i < m_pData->nextTurs.value(champName).count(); ++i)
    {
      QColor color;
      if(NOTREPORT == m_pData->storage.Report(champName, m_pData->nextTurs.value(champName)[i].first) && NOTREPORT == m_pData->storage.Report(champName, m_pData->nextTurs.value(champName)[i].second))
        color = Qt::green;
      else
        color = Qt::red;

      QStandardItem* item1 = new QStandardItem(m_pData->nextTurs[champName][i].first);
      item1->setBackground(color);
      modelResult->setItem(i, 0, item1);


      QStandardItem* item2 = new QStandardItem(m_pData->nextTurs.value(champName)[i].second);
      item2->setBackground(color);
      modelResult->setItem(i, 1, item2);

      CTeam team1;
      foreach(CTeam t, m_pData->championats.value(champName))
      {
        if (m_pData->nextTurs.value(champName)[i].first == t.GetName())
          team1 = t;
      }

      QStandardItem* item3 = new QStandardItem();
      item3->setBackground(color);
      if(team1.Concurents().contains(m_pData->nextTurs.value(champName)[i].second))
      {
        CTeam team2;
        foreach(CTeam t, m_pData->championats.value(champName))
        {
          if (m_pData->nextTurs.value(champName)[i].second == t.GetName())
            team2 = t;
        }
        item3->setData(QString::number(GetCash(team1.CashParityPosition(), PARITY) + GetCash(team2.CashParityPosition(), PARITY)), Qt::DisplayRole);
      }
      else
      {
        item3->setData(QString("--"), Qt::DisplayRole);
      }
      modelResult->setItem(i, 2, item3);


//      QStandardItem* item4 = new QStandardItem(QString());
//      item4->setBackground(color);
//      modelResult->setItem(i, 3, item4);

    }

  }
}

void CFootbolManager::onChecked(const QModelIndex &index)
{
  CWidget* widget = qobject_cast<CWidget*>(sender());
  CStandardItemModel* modelResult = widget->TableResult();
  for(int i = 0; i < m_pData->nextTurs.value(widget->objectName()).count(); ++i)
  {
    if (m_pData->nextTurs.value(widget->objectName())[i].first == modelResult->item(index.row(), 0)->data(Qt::DisplayRole).toString()
        && m_pData->nextTurs.value(widget->objectName())[i].second == modelResult->item(index.row(), 1)->data(Qt::DisplayRole).toString())
    {
      m_pData->storage.Reported(widget->objectName(), modelResult->item(index.row(), 0)->data(Qt::DisplayRole).toString(), REPORT);
      m_pData->storage.Reported(widget->objectName(), modelResult->item(index.row(), 1)->data(Qt::DisplayRole).toString(), REPORT);
      for(int j = 0; j < modelResult->columnCount(); ++j)
        modelResult->item(i, j)->setBackground(Qt::red);
    }
  }
}

void CFootbolManager::onClicked()
{
  m_pData->storage.CreatePlays(sender()->objectName(), m_pData->nextTurs.value(sender()->objectName()));
}

CWidget* CFootbolManager::AddTable(const QString& tableName)
{
  CWidget* widget = new CWidget(this);
  widget->setObjectName(tableName);
  connect(widget, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onChecked(const QModelIndex&)));
  connect(widget, SIGNAL(clicked()), this, SLOT(onClicked()));
  connect(&m_pData->storage, SIGNAL(AllChecked(bool)), widget, SIGNAL(AllChecked(bool)));
  m_pData->ui.tabWidget->addTab(widget, tableName);
  return widget;
}

QVector<int> FindCashList(int minCash, double koef, int limit)
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
  return vector;
}
