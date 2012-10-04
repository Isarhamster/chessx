/* QTableView with some adjustments like storing header layout */
/* (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org> */

#include "settings.h"
#include "tableview.h"

#include <QAction>
#include <QApplication>
#include <QHeaderView>

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
	setTabKeyNavigation(false);
    setContextMenuPolicy(Qt::CustomContextMenu);
}


TableView::~TableView()
{
}

void TableView::saveConfig()
{
	AppSettings->setLayout(this);
	AppSettings->beginGroup(objectName());
	QList<int> sections;
    for (int i = 0; i < model()->columnCount(); ++i)
		sections.append(columnWidth(i));
	AppSettings->setList("Sections", sections);
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
	AppSettings->endGroup();
    int fontSize = AppSettings->value("/General/ListFontSize", DEFAULT_LISTFONTSIZE).toInt();
    QFont f = font();
    f.setPointSize(fontSize);
    setFont(f);
    update();
}

