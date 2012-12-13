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

#include <QHeaderView>
#include <QMenu>
#include <QDrag>

#include "filtermodel.h"
#include "gamelist.h"
#include "quicksearch.h"
#include "search.h"
#include "settings.h"
#include "GameMimeData.h"
#include "game.h"

GameList::GameList(Filter* filter, QWidget* parent) : TableView(parent)
{
	setObjectName("GameList");
	setWindowTitle(tr("Game list"));
	m_model = new FilterModel(filter, this);
	setModel(m_model);
	connect(this, SIGNAL(clicked(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
	connect(this, SIGNAL(activated(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(slotContextMenu(const QPoint&)));

	horizontalHeader()->setClickable(true);
	connect(horizontalHeader(), SIGNAL(sectionClicked(int)), SLOT(simpleSearch(int)));

    slotReconfigure();

    setSortingEnabled(false);
    setDragEnabled(true);
}


GameList::~GameList()
{
    delete m_model;
}

void GameList::itemSelected(const QModelIndex& index)
{
	emit selected(m_model->filter()->indexToGame(index.row()));
    startToDrag(index);
}

void GameList::setFilter(Filter* filter)
{
    m_model->setFilter(filter);
    emit raiseRequest();
}

void GameList::slotContextMenu(const QPoint& pos)
{
    QModelIndex cell = indexAt(pos);
    QModelIndexList selection = selectedIndexes();
    // Make sure the right click occured on a cell!
    if (cell.isValid() && selection.contains(cell))
    {
        QMenu menu(this);
        menu.addAction(tr("Copy games..."), this, SLOT(slotCopyGame()));
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

/** Select and show current game in the list */
void  GameList::slotFilterListByPlayer(QString s)
{
    TagSearch ts(m_model->filter()->database(), "White", s);
    TagSearch ts2(m_model->filter()->database(), "Black", s);
    m_model->filter()->executeSearch(ts);
    m_model->filter()->executeSearch(ts2,Search::Or);
    updateFilter();
    emit raiseRequest();	
    emit searchDone();
}

/** Select and show current game in the list */
void  GameList::slotFilterListByEvent(QString s)
{
    TagSearch ts(m_model->filter()->database(), "Event", s);
    m_model->filter()->executeSearch(ts);
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
    emit requestCopyGame();
}

void GameList::startToDrag(const QModelIndex&)
{
    GameMimeData *mimeData = new GameMimeData;
    emit requestGameData(mimeData->m_game);
    QPixmap pixmap = style()->standardPixmap(QStyle::SP_FileIcon);

    QDrag* pDrag = new QDrag(this);
    pDrag->setMimeData(mimeData);
    pDrag->setPixmap(pixmap);
    // pDrag->setHotSpot(hotSpot);

    pDrag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
}

