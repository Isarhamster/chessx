/***************************************************************************
                          gamelist  -  description
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "gamelist.h"
#include "gamelistitem.h"
#include "database.h"

#include <qcombobox.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qspinbox.h>

GameListDialog::GameListDialog(QWidget* parent) : GameListBase(parent), m_filter(0), m_database(0)
{
  connect(firstPageButton, SIGNAL(clicked()), SLOT(gotoFirstPage()));
  connect(lastPageButton, SIGNAL(clicked()), SLOT(gotoLastPage()));
  connect(pageSpin, SIGNAL(valueChanged(int)), SLOT(setPage(int)));
  //connect(filterButton, SIGNAL(clicked()), SLOT(filter()));
  connect(gameListView, SIGNAL(clicked(QListViewItem*)), SLOT(gameHighlighted(QListViewItem*)));
  connect(gameListView, SIGNAL(doubleClicked(QListViewItem*, const QPoint&, int)),
          SLOT(gameSelected(QListViewItem*)));
  m_pageLength = 50;

  gameListView->setColumnAlignment(GameListItem::Index, Qt::AlignRight);
  gameListView->setColumnAlignment(GameListItem::Length, Qt::AlignRight);
  gameListView->setAllColumnsShowFocus(true);
}

GameListDialog::~GameListDialog()
{
}

int GameListDialog::page() const
{
  return pageSpin->value();
}

void GameListDialog::gotoFirstPage()
{
  setPage(1);
}

void GameListDialog::gotoLastPage()
{
  setPage(pageSpin->maxValue());
}

void GameListDialog::setPage(int page)
{
  pageSpin->setValue(page);
  gameListView->clear();
  int start = (page - 1) * m_pageLength;
  Game g;
  for (int i = 0; i < m_pageLength; i++)
  {
    if (!m_database->load(start + i, g))
      break;
    GameListItem* item = new GameListItem(gameListView);
    item->setText(GameListItem::Index, QString::number(start+i+1));
    item->setText(GameListItem::White, g.tag("White"));
    item->setText(GameListItem::Black, g.tag("Black"));
    item->setText(GameListItem::Event, g.tag("Event"));
    item->setText(GameListItem::Site, g.tag("Site"));
    item->setText(GameListItem::Date, g.tag("Date"));
    item->setText(GameListItem::Result, g.tag("Result").left(3));
    item->setText(GameListItem::ECO, g.tag("ECO"));
    item->setText(GameListItem::Length, QString::number((g.ply() + 1) / 2));
  }
}

void GameListDialog::setFilter(Filter* filter)
{
  /*
  m_filter = filter;
  pageSpin->setValue((m_filter->count() + m_pageLength - 1) / m_pageLength);
  setPage(0);
  */
}

void GameListDialog::filter()
{
}

void GameListDialog::resetFilter()
{
  /*
  for (uint i = 0; i < m_filter->count(); i++)
    m_filter->set(i, 0);
  setFilter(m_filter);
  */
}

void GameListDialog::setDatabase(Database* database)
{
  m_database = database;
  pageSpin->setMaxValue((m_database->count() + m_pageLength - 1) / m_pageLength);
  setPage(1);
}

void GameListDialog::gameHighlighted(QListViewItem * item)
{
  emit highlighted(item->text(0).toInt() - 1);
}

void GameListDialog::gameSelected(QListViewItem* item)
{
  emit selected(item->text(0).toInt() - 1);
}

