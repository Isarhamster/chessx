/****************************************************************************
* QTableView with some adjustments like storing header layout               *
* (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>                            *
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "settings.h"
#include "tableview.h"

#include <QAction>
#include <QApplication>
#include <QHeaderView>
#include <QMenu>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

TableView::TableView(QWidget *parent)
    : QTableView(parent),
    m_alignDecoration(Qt::AlignCenter),
    m_posDecoration(QStyleOptionViewItem::Left)
{
    setShowGrid(false);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setTextElideMode(Qt::ElideRight);
    verticalHeader()->setDefaultSectionSize(fontMetrics().lineSpacing());
    verticalHeader()->hide();
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->setStretchLastSection(true);
#if QT_VERSION < 0x050000
    horizontalHeader()->setMovable(true);
#else
    horizontalHeader()->setSectionsMovable(true);
#endif
    setTabKeyNavigation(false);
    setContextMenuPolicy(Qt::CustomContextMenu);
    horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(ShowContextMenu(const QPoint&)));
}

TableView::~TableView()
{
}

QStyleOptionViewItem TableView::viewOptions() const
{
    QStyleOptionViewItem option = QTableView::viewOptions();
    option.decorationAlignment = m_alignDecoration;
    option.decorationPosition = m_posDecoration;
    return option;
}

QStyleOptionViewItem::Position TableView::posDecoration() const
{
    return m_posDecoration;
}

void TableView::setPosDecoration(const QStyleOptionViewItem::Position &posDecoration)
{
    m_posDecoration = posDecoration;
}

Qt::Alignment TableView::alignDecoration() const
{
    return m_alignDecoration;
}

void TableView::setAlignDecoration(const Qt::Alignment &alignDecoration)
{
    m_alignDecoration = alignDecoration;
}

void TableView::saveConfig()
{
    if (model())
    {
        AppSettings->setLayout(this);
        AppSettings->beginGroup(objectName());
        int n = model()->columnCount();
        AppSettings->setValue("ColumnCount", n);
        QByteArray visualIndex = horizontalHeader()->saveState();
        AppSettings->setByteArray("VisualIndex", visualIndex);
        AppSettings->endGroup();
    }
}

void TableView::slotReconfigure()
{
    bool sortIndicator = horizontalHeader()->isSortIndicatorShown();
    AppSettings->layout(this);
    QString objName = objectName();

    AppSettings->beginGroup(objName);

    int n = model()->columnCount();
    int nRestore = AppSettings->getValue("ColumnCount").toInt();
    if (n==nRestore)
    {
        QByteArray visualIndex = AppSettings->byteArray("VisualIndex");
        horizontalHeader()->restoreState(visualIndex);
    }
    else
    {
        for(int i = 0; i < n; ++i)
        {
            showColumn(i);
        }
        AppSettings->setValue("ColumnCount", n);
        resizeColumnsToContents();
    }
    AppSettings->endGroup();

    int fontSize = AppSettings->getValue("/General/ListFontSize").toInt();
    QFont f = font();
    f.setPointSize(fontSize);
    setFont(f);

    horizontalHeader()->setSortIndicatorShown(sortIndicator);
    update();
}

void TableView::ShowContextMenu(const QPoint& pos)
{
    QMenu headerMenu;
    QAction* hide = headerMenu.addAction(tr("Hide Column"));
    headerMenu.addSeparator();
    QAction* resizeAll = headerMenu.addAction(tr("Resize visible Columns"));
    QAction* showAll = headerMenu.addAction(tr("Show all Columns"));

    QAction* selectedItem = headerMenu.exec(mapToGlobal(pos));
    if(selectedItem == hide)
    {
        int column = columnAt(pos.x());
        if(column > 0)
        {
            hideColumn(column);
        }
    }
    else if(selectedItem == showAll)
    {
        for(int i = 0; i < model()->columnCount(); ++i)
        {
            showColumn(i);
        }
    }
    else if (selectedItem == resizeAll)
    {
        resizeColumnsToContents();
    }
}

