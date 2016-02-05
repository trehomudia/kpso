#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include <QItemDelegate>

class CTableViewWd: public QTableView
{
  Q_OBJECT

public:
  CTableViewWd(QWidget* parent = 0);
  virtual ~CTableViewWd();

  void applyTransformMap(const QMap<QString, QPair<QString, QString> >& map);
  void setColumnDelegate(const int column, Qt::Alignment alignment = Qt::AlignVCenter | Qt::AlignHCenter);
  void setAllColumnsDelegate(Qt::Alignment alignment = Qt::AlignVCenter | Qt::AlignHCenter);
  void setAutoResize(bool value);
  void setDoublePrecision(int value);
  void resetTable();
  QModelIndexList getSelectedIndexes();
  QString getCsvData();

protected:
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);
  int m_doublePrecision;

private:
  bool m_autoResize;
  QMap<QString, QPair<QString, QString> > m_transformMap;
  int m_horizontalHeaderLength;
  void updateToolTips();
  bool resizeTableToContents();
};

//// Уласс делагата для ячейки таблицы
class CTableViewWdItemDelegate : public QItemDelegate
{
  Q_OBJECT

public:
  CTableViewWdItemDelegate(QObject* parent, int doublePrecision, Qt::Alignment alignment = Qt::AlignVCenter | Qt::AlignHCenter);
  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
  QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex &index) const;

private:
  unsigned int size;
  int m_doublePrecision;
  Qt::Alignment m_alignment;
};

QString GetValueString(QVariant value, int doublePrecision);

#endif // TABLEVIEW_H
