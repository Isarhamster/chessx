/* QTableView with some adjustments like storing header layout */
/* (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org> */

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
    horizontalHeader()->setMovable(true);
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
    QByteArray visualIndex = horizontalHeader()->saveState();
    AppSettings->setByteArray("VisualIndex", visualIndex);
	AppSettings->endGroup();
}

void TableView::slotReconfigure()
{
	AppSettings->layout(this);
    QString objName = objectName();
    AppSettings->beginGroup(objName);
	QList<int> sections;
	if (AppSettings->list("Sections", sections, model()->columnCount()))
        for (int i = 0; i < sections.count(); ++i)
			setColumnWidth(i, sections[i]);

    QByteArray visualIndex = AppSettings->byteArray("VisualIndex");
    horizontalHeader()->restoreState(visualIndex);

    AppSettings->endGroup();
    int fontSize = AppSettings->value("/General/ListFontSize", DEFAULT_LISTFONTSIZE).toInt();
    QFont f = font();
    f.setPointSize(fontSize);
    setFont(f);
    update();
}

void TableView::ShowContextMenu(const QPoint& pos)
{
    QMenu headerMenu;
    QAction* hide = headerMenu.addAction(tr("Hide Column"));
    headerMenu.addSeparator();
    QAction* showAll = headerMenu.addAction(tr("Show all Columns"));

    QAction* selectedItem = headerMenu.exec(mapToGlobal(pos));
    if (selectedItem == hide)
    {
        int column = columnAt(pos.x());
        if (column > 0)
        {
            hideColumn(column);
        }
    }
    else if (selectedItem == showAll)
    {
        for (int i = 0; i < model()->columnCount(); ++i)
        {
            showColumn(i);
            if (columnWidth(i) < 50)
            {
                setColumnWidth(i, 50); // Fix a bugfeature in Qt
            }
        }
    }
}

