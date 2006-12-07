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

#include <qapplication.h>
#include <q3hbox.h>
#include <qlayout.h>
#include <q3header.h>
#include <q3listview.h>
#include <QScrollBar>
#include <QStatusBar>
#include <q3vbox.h>
#include <QResizeEvent>
#include <QEvent>
#include <Q3VBoxLayout>

#include "databaseinfo.h"
#include "database.h"
#include "gamelist.h"
#include "settings.h"

GameList::GameList(QWidget* parent, const char* name) : QWidget(parent, name), m_count(0),
  m_pageSize(1), m_database(0)
{
  setName("GameList");
  setCaption(tr("Game list"));
  setMinimumSize(600, 400);

  Q3VBoxLayout* vbox = new Q3VBoxLayout(this);

  Q3HBox* hbox = new Q3HBox(this);
  m_list = new Q3ListView(hbox);
  m_list->addColumn(tr("Index"));
  m_list->addColumn(tr("White"));
  m_list->addColumn(tr("Black"));
  m_list->addColumn(tr("Event"));
  m_list->addColumn(tr("Site"));
  m_list->addColumn(tr("Round"));
  m_list->addColumn(tr("Date"));
  m_list->addColumn(tr("Result"));
  m_list->addColumn(tr("ECO"));
  m_list->addColumn(tr("Length"));
  m_list->setColumnAlignment(Index, Qt::AlignRight);
  m_list->setColumnAlignment(Round, Qt::AlignRight);
  m_list->setColumnAlignment(Length, Qt::AlignRight);
  m_list->setAllColumnsShowFocus(true);
  m_list->setHScrollBarMode(Q3ScrollView::AlwaysOff);
  m_list->setVScrollBarMode(Q3ScrollView::AlwaysOff);
  new Q3ListViewItem(m_list, QString());
  m_itemHeight = m_list->firstChild()->height();
  m_list->clear();
  m_list->setSorting(-1);

  m_scroll = new QScrollBar(0, 0, 1, 10, 0, Qt::Vertical, hbox);
  m_scroll->setTracking(true);

  vbox->addWidget(hbox);

  m_status = new QStatusBar(this);
  vbox->addWidget(m_status);

  /* Keyboard filter */
  m_list->installEventFilter(this);

  connect(m_scroll, SIGNAL(valueChanged(int)), SLOT(scrollList(int)));

  connect(m_list, SIGNAL(doubleClicked(Q3ListViewItem*, const QPoint&, int)), 
          SLOT(itemSelected(Q3ListViewItem*)));
  connect(m_list, SIGNAL(returnPressed(Q3ListViewItem*)), SLOT(itemSelected(Q3ListViewItem*)));

  configure();
  updateScrollbar();
}

void GameList::createItem(int index)
{
  Game g;
  if (!m_database || !m_database->database()->loadHeaders(index, g))
    return;
  Q3ListViewItem* item = new Q3ListViewItem(m_list, m_list->lastItem());
  item->setText(Index, QString::number(index+1));
  item->setText(White, g.tag("White"));
  item->setText(Black, g.tag("Black"));
  item->setText(Event, g.tag("Event"));
  item->setText(Site, g.tag("Site"));
  item->setText(Round, g.tag("Round"));
  item->setText(Date, g.tag("Date"));
  item->setText(Result, g.tag("Result").left(3));
  item->setText(Length, QString::number((g.ply() + 1) / 2));
  item->setText(ECO, g.tag("ECO"));
}

void GameList::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
  updateScrollbar();
}

void GameList::updateScrollbar()
{
  m_pageSize = (m_list->size().height() - m_list->header()->height() - 4) / m_itemHeight;
  m_scroll->setPageStep(m_pageSize - 1);
  scrollList(m_scroll->value());
}

void GameList::updateList()
{
  m_count = m_database ? m_database->database()->count() : 0;
  m_scroll->setMaxValue(m_count ? m_count - 1 : 0);
  if (!m_count)
    m_list->clear();
  else
    scrollList(0);
  m_status->message(QString("%1 games").arg(m_count));
}

void GameList::scrollList(int page)
{
  if (!m_database)
    return;
  int start = page;
  if (start > m_count - m_pageSize)
    start = m_count - m_pageSize;
  if (start < 0)
    start = 0;
  m_list->clear();
  for (int i = 0; i < m_pageSize; i++)
    createItem(start + i);
  Q3ListViewItem* current = m_list->firstChild();
  if (current && page > start)
    for (int i = 0; i < page - start && current->itemBelow(); i++)
      current = current->itemBelow();
  m_list->setSelected(current, true);
}

bool GameList::eventFilter(QObject* o, QEvent* e)
{
  if (o == m_list && (e->type() == QEvent::KeyPress || e->type() == QEvent::Wheel))
  {
    qApp->sendEvent(m_scroll, e);
    return true;
  }
  return false;
}

void GameList::itemSelected(Q3ListViewItem* item)
{
  emit selected(item->text(Index).toInt() - 1);
}

void GameList::setDatabase(DatabaseInfo* database)
{
  m_database = database;
  updateList();
}

void GameList::configure()
{
  AppSettings->readLayout(this);
  AppSettings->beginGroup("/GameList");
  QStringList sections  = AppSettings->readListEntry("sections");
  if (m_list->header()->count() == sections.count())
    for (int i = 0; i < m_list->header()->count(); i++)
      m_list->header()->resizeSection(i, sections[i].toInt());
  AppSettings->endGroup();
}

void GameList::saveConfig()
{
  AppSettings->writeLayout(this);
  AppSettings->beginGroup("/GameList");
  QStringList sections;
  for (int i = 0; i < m_list->header()->count(); i++)
    sections.append(QString::number(m_list->header()->sectionSize(i)));
  AppSettings->writeEntry("sections", sections);
  AppSettings->endGroup();
}

