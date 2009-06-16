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
	m_smallfont = new QAction(tr("Small font"), this);
	m_smallfont->setCheckable(true);
	m_smallfont->setChecked(false);
	connect(m_smallfont, SIGNAL(toggled(bool)), SLOT(slotToggleFont(bool)));
	addAction(m_smallfont);
	setContextMenuPolicy(Qt::ActionsContextMenu);
}


TableView::~TableView()
{
}

void TableView::saveConfig()
{
	AppSettings->setLayout(this);
	AppSettings->beginGroup(objectName());
	AppSettings->setValue("SmallFont", m_smallfont->isChecked());
	QList<int> sections;
	for (int i = 0; i < model()->columnCount(); i++)
		sections.append(columnWidth(i));
	AppSettings->setList("Sections", sections);
	AppSettings->endGroup();
}

void TableView::slotReconfigure()
{
	AppSettings->layout(this);
	AppSettings->beginGroup(objectName());
	if (AppSettings->value("SmallFont", false).toBool())
		m_smallfont->setChecked(true);
	QList<int> sections;
	if (AppSettings->list("Sections", sections, model()->columnCount()))
		for (int i = 0; i < sections.count(); i++)
			setColumnWidth(i, sections[i]);
	AppSettings->endGroup();
}

void TableView::slotToggleFont(bool toggled)
{
	if (toggled) {
		QFont f = qApp->font();
		f.setPointSize(f.pointSize() - 1);
		setFont(f);
		horizontalHeader()->setFont(f);
	} else {
		setFont(qApp->font());
		horizontalHeader()->setFont(qApp->font());
	}
}

