/***************************************************************************
								  gamelist.cpp  -  Game List window
									  -------------------
	 begin                : Sun 23 Jul 2006
	 copyright            : (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "database.h"
#include "filtermodel.h"
#include "game.h"
#include "gamelist.h"
#include "GameMimeData.h"
#include "quicksearch.h"
#include "search.h"
#include "settings.h"

#include <QDrag>
#include <QHeaderView>
#include <QMenu>
#include <QPixmap>

GameList::GameList(Filter* filter, QWidget* parent) : TableView(parent)
{
	setObjectName("GameList");
	setWindowTitle(tr("Game list"));
	m_model = new FilterModel(filter, this);
	setModel(m_model);
	connect(this, SIGNAL(clicked(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
	connect(this, SIGNAL(activated(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(slotContextMenu(const QPoint&)));
    connect(selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotItemSelected(QModelIndex)));
#if QT_VERSION < 0x050000
    horizontalHeader()->setClickable(true);
#else
    horizontalHeader()->setSectionsClickable(true);
#endif
	connect(horizontalHeader(), SIGNAL(sectionClicked(int)), SLOT(simpleSearch(int)));

    slotReconfigure();

    setSortingEnabled(false);
    setDragEnabled(true);
}


GameList::~GameList()
{
    delete m_model;
}

void GameList::slotItemSelected(const QModelIndex& index)
{
    m_index = index;
    scrollTo(index, EnsureVisible);
}

void GameList::itemSelected(const QModelIndex& index)
{
	emit selected(m_model->filter()->indexToGame(index.row()));
}

void GameList::setFilter(Filter* filter)
{
    m_model->setFilter(filter);
    emit raiseRequest();
}

void GameList::slotContextMenu(const QPoint& pos)
{
    QModelIndex cell = indexAt(pos);
    m_index = cell;
    QModelIndexList selection = selectedIndexes();
    // Make sure the right click occured on a cell!
    if (cell.isValid() && selection.contains(cell))
    {
        QMenu menu(this);
        menu.addAction(tr("Copy games..."), this, SLOT(slotCopyGame()));
        menu.addSeparator();
        QAction* deleteAction = menu.addAction(tr("Delete game"), this, SLOT(slotDeleteGame()));
        deleteAction->setCheckable(true);
        deleteAction->setEnabled(!m_model->filter()->database()->isReadOnly());
        int n = m_model->filter()->indexToGame(cell.row());
        deleteAction->setChecked(m_model->filter()->database()->deleted(n));
        menu.exec(mapToGlobal(pos));
    }
}

void GameList::simpleSearch(int tagid)
{
	QuickSearchDialog dialog(this);

	dialog.setTag(tagid);
	if (m_model->filter()->count() <= 1)
		dialog.setMode(1);
	if (dialog.exec() != QDialog::Accepted)
		return;

    QString tag = m_model->GetColumnTags().at(dialog.tag());
	QString value = dialog.value();
	if (value.isEmpty())
    {
		m_model->filter()->setAll(1);
    }
    else if ((dialog.tag() == 0) || (dialog.tag() == 7) || (dialog.tag() == 11))
    {	// filter by number
		NumberSearch ns(m_model->filter()->database(), value);
		if (dialog.mode())
        {
			m_model->filter()->executeSearch(ns, Search::Operator(dialog.mode()));
        }
        else
        {
            m_model->filter()->executeSearch(ns);
        }
	}
    else
    {
        QStringList list = value.split("-", QString::SkipEmptyParts);
        if (list.size()>1)
        {
            // Filter a range
            TagSearch ts(m_model->filter()->database(), tag, list.at(0),list.at(1));
            if (dialog.mode())
            {
                m_model->filter()->executeSearch(ts, Search::Operator(dialog.mode()));
            }
            else
            {
                m_model->filter()->executeSearch(ts);
            }
        }
        else
        {
            // Filter tag using partial values
            TagSearch ts(m_model->filter()->database(), tag, value);
            if (dialog.mode())
            {
                m_model->filter()->executeSearch(ts, Search::Operator(dialog.mode()));
            }
            else
            {
                m_model->filter()->executeSearch(ts);
            }
        }
	}
    updateFilter();
	emit searchDone();
}

void GameList::slotFilterListByPlayer(QString s)
{
    TagSearch ts(m_model->filter()->database(),  TagNameWhite, s);
    TagSearch ts2(m_model->filter()->database(), TagNameBlack, s);
    m_model->filter()->executeSearch(ts);
    m_model->filter()->executeSearch(ts2,Search::Or);
    updateFilter();
    emit raiseRequest();
    emit searchDone();
}

void GameList::slotFilterListByEcoPlayer(QString tag, QString eco, QString player)
{
    TagSearch ts(m_model->filter()->database(),  tag, player);
    TagSearch ts3(m_model->filter()->database(), TagNameECO,   eco);
    m_model->filter()->executeSearch(ts);
    m_model->filter()->executeSearch(ts3,Search::And);
    updateFilter();
    emit raiseRequest();
    emit searchDone();
}

void GameList::slotFilterListByEvent(QString s)
{
    TagSearch ts(m_model->filter()->database(), "Event", s);
    m_model->filter()->executeSearch(ts);
    updateFilter();
    emit raiseRequest();
    emit searchDone();
}

void GameList::slotFilterListByEventPlayer(QString player, QString event)
{
    TagSearch ts(m_model->filter()->database(),  TagNameWhite, player);
    TagSearch ts2(m_model->filter()->database(), TagNameBlack, player);
    TagSearch ts3(m_model->filter()->database(), TagNameEvent, event);
    m_model->filter()->executeSearch(ts);
    m_model->filter()->executeSearch(ts2,Search::Or);
    m_model->filter()->executeSearch(ts3,Search::And);
    updateFilter();
    emit raiseRequest();
    emit searchDone();
}

void GameList::selectGame(int index)
{
	int i = m_model->filter()->gameToIndex(index);
	if (i != -1) {
		setCurrentIndex(m_model->index(i, 0));
		selectRow(i);
	}
}

void GameList::updateFilter()
{
    m_model->setFilter(m_model->filter());
}

void GameList::slotCopyGame()
{
    emit requestCopyGame(m_model->filter()->indexToGame(m_index.row()));
}

void GameList::slotDeleteGame()
{
    emit requestDeleteGame(m_model->filter()->indexToGame(m_index.row()));
}

void GameList::startToDrag(const QModelIndex& index)
{
    GameMimeData *mimeData = new GameMimeData;
    mimeData->m_index = m_model->filter()->indexToGame(index.row());
    QPixmap pixmap = style()->standardPixmap(QStyle::SP_FileIcon);

    QDrag* pDrag = new QDrag(this);
    pDrag->setMimeData(mimeData);
    pDrag->setPixmap(pixmap);
    // pDrag->setHotSpot(hotSpot);

    pDrag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
}

void GameList::startDrag(Qt::DropActions /*supportedActions*/)
{
    startToDrag(m_index);
}
