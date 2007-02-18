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
#include <QInputDialog>

#include "gamelist.h"
#include "filtermodel.h"
#include "search.h"
#include "settings.h"

GameList::GameList(Filter* filter, QWidget* parent) : QTableView(parent)
{
  setObjectName("GameList");
  setWindowTitle(tr("Game list"));

  QFont f = font();
  f.setPointSize(f.pointSize() - 1);
  setFont(f);

  setShowGrid(false);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setTextElideMode(Qt::ElideRight);
  verticalHeader()->setDefaultSectionSize(fontMetrics().lineSpacing());
  verticalHeader()->hide();

  m_model = new FilterModel(filter, this);
  setModel(m_model);
  connect(this, SIGNAL(clicked(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
  configure();

  horizontalHeader()->setClickable(true);
  connect(horizontalHeader(), SIGNAL(sectionClicked(int)), SLOT(simpleSearch(int)));
}

void GameList::itemSelected(const QModelIndex& index)
{
  emit selected(m_model->filter()->indexToGame(index.row()));
}

void GameList::configure()
{
  AppSettings->readLayout(this);
  AppSettings->beginGroup("/GameList");
  QStringList sections  = AppSettings->value("sections").toStringList();
  if (m_model->columnCount() == sections.count())
    for (int i = 0; i < sections.count(); i++)
      setColumnWidth(i, sections[i].toInt());
  AppSettings->endGroup();
}

void GameList::saveConfig()
{
  AppSettings->writeLayout(this);
  AppSettings->beginGroup("/GameList");
  QStringList sections;
  for (int i = 0; i < m_model->columnCount(); i++)
    sections.append(QString::number(columnWidth(i)));
  AppSettings->setValue("sections", sections);
  AppSettings->endGroup();
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
  const QString tagNames[] = {"Nr", "White", "Black", "Event", "Site", "Round", "Date",
     "Result", "ECO", "Length", ""};

  QString tag = tagNames[tagid];
  if (tag.isEmpty())
    return;
  QString value = QInputDialog::getText(0, tr("Find %1").arg(tag), tr("Value:"));
  if (value.isEmpty())
    return;
  TagSearch ts(tag, value);
  m_model->filter()->executeSearch(ts);
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

