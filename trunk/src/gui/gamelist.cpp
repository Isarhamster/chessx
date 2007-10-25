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

#include "filtermodel.h"
#include "gamelist.h"
#include "quicksearch.h"
#include "search.h"
#include "settings.h"

GameList::GameList(Filter* filter, QWidget* parent) : TableView(parent)
{
	setObjectName("GameList");
	setWindowTitle(tr("Game list"));
	m_model = new FilterModel(filter, this);
	setModel(m_model);
	connect(this, SIGNAL(clicked(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
	connect(this, SIGNAL(activated(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));

	horizontalHeader()->setClickable(true);
	connect(horizontalHeader(), SIGNAL(sectionClicked(int)), SLOT(simpleSearch(int)));

	slotReconfigure();
}

void GameList::itemSelected(const QModelIndex& index)
{
	emit selected(m_model->filter()->indexToGame(index.row()));
}

void GameList::setFilter(Filter* filter)
{
	m_model->setFilter(filter);
}

GameList::~GameList()
{
	delete m_model;
}

void GameList::simpleSearch(int tagid)
{
#warning Unify with <filtermodel.cpp>
	const QString tagNames[] = {"Nr", "White", "WhiteElo", "BlackElo", "Black", "Event", "Site", "Round", "Date",
				    "Result", "ECO", "PlyCount", ""
				   };

	QuickSearchDialog dialog(this);

	dialog.setTag(tagid);
	if (m_model->filter()->count() <= 1)
		dialog.setMode(1);
	if (dialog.exec() != QDialog::Accepted)
		return;

	QString tag = tagNames[dialog.tag()];
	QString value = dialog.value();
	if (value.isEmpty())
		m_model->filter()->setAll(1);
	else {
#warning Fix after Search::Operator cleanup
		TagSearch ts(m_model->filter()->database(), tag, value);
		if (dialog.mode())
			m_model->filter()->executeSearch(ts, Search::Operator(dialog.mode()));
		else m_model->filter()->executeSearch(ts);
	}
	updateFilter();
	emit searchDone();
}

void GameList::selectGame(int index)
{
	int i = m_model->filter()->gameToIndex(index);
	if (i != -1)
		setCurrentIndex(m_model->index(i, 0));
}

void GameList::updateFilter()
{
	m_model->setFilter(m_model->filter());
}

