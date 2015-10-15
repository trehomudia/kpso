#include "table_view.h"
#include <QDateTime>
#include <QHeaderView>
#include <QFile>
#include <QTextStream>
#include <math.h>

const static int DELTA_WIDTH_FOR_COLUMN_MINIMUM(30);

CTableViewWd::CTableViewWd(QWidget* parent)
: QTableView(parent)
, m_autoResize(true)
, m_doublePrecision(5)
, m_horizontalHeaderLength(0)
{
  setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  //horizontalHeader()->setResizeMode(QHeaderView::Interactive);
  setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  verticalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  resize(minimumSize());
}

CTableViewWd::~CTableViewWd()
{

}

void CTableViewWd::applyTransformMap(const QMap<QString, QPair<QString, QString> >& map)
{
  m_transformMap = map;
  updateToolTips();
}

void CTableViewWd::setColumnDelegate(const int column, Qt::Alignment alignment)
{
  if ((column < 0) || (column > horizontalHeader()->count() - 1))
    return;

  CTableViewWdItemDelegate* columnDelegate = new CTableViewWdItemDelegate(this, m_doublePrecision, alignment);
  setItemDelegateForColumn(column, columnDelegate);
}

void CTableViewWd::setAllColumnsDelegate(Qt::Alignment alignment)
{
  for (int i = 0; i < horizontalHeader()->count(); i++)
  {
    CTableViewWdItemDelegate* columnDelegate = new CTableViewWdItemDelegate(this, m_doublePrecision, alignment);
    setItemDelegateForColumn(i, columnDelegate);
  }
}

void CTableViewWd::setAutoResize(bool value)
{
  m_autoResize = value;
}

void CTableViewWd::setDoublePrecision(int value)
{
  if ((value >= 0) && (value < 16))
    m_doublePrecision = value;
  else
    m_doublePrecision = 5;
}

void CTableViewWd::updateToolTips()
{
  QAbstractItemModel* model = this->model();

  if (!model)
    return;

  int sectionCount = model->columnCount();

  while (--sectionCount >= 0)
  {
    QString caption = model->headerData(sectionCount, Qt::Horizontal).toString();
    if (m_transformMap.contains(caption))
    {
      model->setHeaderData(sectionCount, Qt::Horizontal, caption, Qt::UserRole);
      QString toolTip(m_transformMap[caption].first);
      caption = m_transformMap[caption].second;
      model->setHeaderData(sectionCount, Qt::Horizontal, caption, Qt::DisplayRole);
      model->setHeaderData(sectionCount, Qt::Horizontal, toolTip, Qt::ToolTipRole);
    }
  }
}

void CTableViewWd::paintEvent(QPaintEvent* event)
{
  if (m_autoResize)
    resizeTableToContents();

  QTableView::paintEvent(event);
}

void CTableViewWd::resizeEvent(QResizeEvent* event)
{
  QTableView::resizeEvent(event);

  if (m_autoResize)
    resizeTableToContents();
}

bool CTableViewWd::resizeTableToContents()
{
  if (this->model() == 0)
    return false;

  QHeaderView* header = horizontalHeader();
  int headerWidth = header->viewport()->width();

  if (headerWidth == m_horizontalHeaderLength)
    return false;

  m_horizontalHeaderLength = headerWidth;

  int SECTION_COUNT = header->count();
  int VISIBLE_SECTION_COUNT = header->count() - header->hiddenSectionCount();

// Вычисление ширины видимой части хедера
  int initialSize = m_horizontalHeaderLength;

// Вычисление размера по полям таблицы
  resizeColumnsToContents();

  int size = 0;
  int maxIdx = -1;
  int maxSize = -1;

  bool emptyTable = (this->model()->rowCount() == 0);
  QFontMetrics fm(fontMetrics());
  int sectionSize;
  QVector<int> sectionsSizes;

  for (int i = 0; i < SECTION_COUNT; i++)
  {
    if (header->isSectionHidden(i))
    {
      sectionsSizes.push_back(0);
      continue;
    }

    if (!emptyTable)
      sectionSize = header->sectionSize(i);
    else
      sectionSize = fm.width(model()->headerData(i, Qt::Horizontal).toString()) + DELTA_WIDTH_FOR_COLUMN_MINIMUM;

    size += sectionSize;

    if (sectionSize > maxSize)
    {
      maxIdx = i;
      maxSize = sectionSize;
    }

    sectionsSizes.push_back(sectionSize);
  }

  if (maxIdx == -1)
    return false;

  if (size < initialSize)
  {
    if (maxSize * VISIBLE_SECTION_COUNT <= initialSize)
    {
      int delta = static_cast<int>(static_cast<double>(initialSize - maxSize * VISIBLE_SECTION_COUNT) / static_cast<double>(VISIBLE_SECTION_COUNT));

      for (int i = 0; i < SECTION_COUNT; i++)
      {
        if (header->isSectionHidden(i))
        {
          header->resizeSection(i, 0);
          continue;
        }

        header->resizeSection(i, maxSize + delta);
      }

      if (delta * (VISIBLE_SECTION_COUNT) < initialSize - maxSize * VISIBLE_SECTION_COUNT)
        header->resizeSection(maxIdx, maxSize + delta + ((initialSize - maxSize * VISIBLE_SECTION_COUNT) - delta * VISIBLE_SECTION_COUNT));
    }
    else
    {
      int delta = static_cast<int>(static_cast<double>(initialSize - size) / static_cast<double>(VISIBLE_SECTION_COUNT - 1));

      for (int i = 0; i < SECTION_COUNT; i++)
      {
        if (header->isSectionHidden(i))
        {
          header->resizeSection(i, 0);
          continue;
        }

        if (i != maxIdx)
        {
          if (!emptyTable)
            header->resizeSection(i, sectionsSizes[i] + delta);
          else
            header->resizeSection(i, fm.width(model()->headerData(i, Qt::Horizontal).toString()) + DELTA_WIDTH_FOR_COLUMN_MINIMUM + delta);
        }
      }

      if (delta * (VISIBLE_SECTION_COUNT - 1) < initialSize - size)
        header->resizeSection(maxIdx, maxSize + ((initialSize - size) - delta * (VISIBLE_SECTION_COUNT - 1)));
    }
  }
  else
  {
    if (emptyTable)
    {
      for (int i = 0; i < SECTION_COUNT; i++)
      {
        if (header->isSectionHidden(i))
          header->resizeSection(i, 0);
        else
          header->resizeSection(i, fm.width(model()->headerData(i, Qt::Horizontal).toString()) + DELTA_WIDTH_FOR_COLUMN_MINIMUM);
      }
    }
  }

  return true;
}

CTableViewWdItemDelegate::CTableViewWdItemDelegate(QObject* parent, int doublePrecision, Qt::Alignment alignment)
: QItemDelegate(parent)
, m_doublePrecision(doublePrecision)
, m_alignment(alignment)
{

}

void CTableViewWdItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QStyleOptionViewItem opt = option;
  opt.displayAlignment = m_alignment;

  if (index.data().isNull())
  {
    QItemDelegate::paint(painter, opt, index);
    return;
  }

  QString s = GetValueString(index.data(), m_doublePrecision);

  if (!s.isEmpty())
  {
    drawDisplay(painter, opt, opt.rect, s);
    drawFocus(painter, opt, opt.rect);
  }
  else
    QItemDelegate::paint(painter, opt, index);
}

QSize CTableViewWdItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex &index) const
{
  QStyleOptionViewItem opt = option;
  opt.displayAlignment = m_alignment;

  QStyleOptionViewItem optBold = option;
  optBold.font.setBold(true);

  QSize sz(QItemDelegate::sizeHint(opt, index));
  QString header = const_cast<QAbstractItemModel*>(index.model())->headerData(index.column(), Qt::Horizontal).toString();

  QString s = GetValueString(index.data(), m_doublePrecision);

  if (s.isEmpty())
    s = index.data().toString();

  if (opt.fontMetrics.width(s) > optBold.fontMetrics.width(header))
    sz.setWidth(opt.fontMetrics.width(s) + DELTA_WIDTH_FOR_COLUMN_MINIMUM);
  else
    sz.setWidth(optBold.fontMetrics.width(header) + DELTA_WIDTH_FOR_COLUMN_MINIMUM);

  return sz;
}

QString GetValueString(QVariant value, int doublePrecision)
{
  QString s;

  if (value.type() == QVariant::Double)
  {
    double v = value.toDouble();

    if (fabs(v) < 1.0)
    {
      QString g = QString::number(v, 'g', 15);

      if (v < 0)
      {
        if (g.size() - 3 <= doublePrecision)
        {
          s = g;
        }
        else
        {
          QString e = QString::number(v, 'e', doublePrecision);
          QStringList list = e.split("e");
          s = QString::number(list[0].toDouble(), 'g', 15) + "e" + list[1];
        }
      }
      else
      {
        if (g.size() - 2 <= doublePrecision)
        {
          s = g;
        }
        else
        {
          QString e = QString::number(v, 'e', doublePrecision);
          QStringList list = e.split("e");
          s = QString::number(list[0].toDouble(), 'g', 15) + "e" + list[1];
        }
      }
    }
    else
    {
      int temp;

      if (v < 0)
        temp = QString::number(ceil(v), 'f', 0).size() - 1;
      else
        temp = QString::number(floor(v), 'f', 0).size();

      int precision;

      if (v < 0)
        precision = (QString::number(v, 'g', 15).size() - 1) - (temp + 1);
      else
        precision = QString::number(v, 'g', 15).size() - (temp + 1);

      if (precision <= doublePrecision)
        s = QString::number(v, 'g', 15);
      else
        s = QString::number(v, 'g', temp + doublePrecision);
    }
  }
  else if (value.type() == QVariant::DateTime)
  {
    s = value.toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
  }

  return s;
}
