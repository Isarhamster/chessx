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

TableView::TableView(QWidget *parent)
    : QTableView(parent)
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

void TableView::saveConfig()
{
    AppSettings->setLayout(this);
    AppSettings->beginGroup(objectName());
    int n = model()->columnCount();
    AppSettings->setValue("ColumnCount", n);
    QByteArray visualIndex = horizontalHeader()->saveState();
    AppSettings->setByteArray("VisualIndex", visualIndex);
    AppSettings->endGroup();
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
        for(int i = 0; i < model()->columnCount(); ++i)
        {
            showColumn(i);
        }
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

