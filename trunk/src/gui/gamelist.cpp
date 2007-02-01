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

#include "gamelist.h"
#include "filtermodel.h"
#include "settings.h"

GameList::GameList(Filter* filter, QWidget* parent) : QTreeView(parent)
{
  setName("GameList");
  setCaption(tr("Game list"));

  setRootIsDecorated(false);
  setAllColumnsShowFocus(true);

  m_model = new FilterModel(filter);
  setModel(m_model);
  connect(this, SIGNAL(clicked(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
  configure();
}

void GameList::itemSelected(const QModelIndex& index)
{
  emit selected(index.row());
}

void GameList::configure()
{
  AppSettings->readLayout(this);
  AppSettings->beginGroup("/GameList");
  QStringList sections  = AppSettings->readListEntry("sections");
  if (header()->count() == sections.count())
    for (int i = 0; i < header()->count(); i++)
      header()->resizeSection(i, sections[i].toInt());
  AppSettings->endGroup();
}

void GameList::saveConfig()
{
  AppSettings->writeLayout(this);
  AppSettings->beginGroup("/GameList");
  QStringList sections;
  for (int i = 0; i < header()->count(); i++)
    sections.append(QString::number(header()->sectionSize(i)));
  AppSettings->writeEntry("sections", sections);
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

