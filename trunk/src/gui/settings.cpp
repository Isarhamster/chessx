/***************************************************************************
                          Settings - maintaining configuration
                             -------------------
    begin                : Mon 15 Aug 2005
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

#include "settings.h"
#include <qwidget.h>

Settings::Settings() : QSettings(QSettings::Ini)
{
  setPath(QString::null, "ChessX");
  beginGroup("/ChessX");
}

Settings::~Settings()
{
  endGroup();
}

void Settings::readLayout(QWidget* w, unsigned flags)
{
  QString prefix = QString("/Geometry/%1/").arg(w->name());
  beginGroup(prefix);
  int x = readNumEntry("x", w->x());
  int y = readNumEntry("y", w->y());
  int width = readNumEntry("w", w->width());
  int height = readNumEntry("h", w->height());
  w->resize(QSize(width, height));
  w->move(QPoint(x, y));
  if ((flags && Show) && readBoolEntry("visible"))
    w->show();
  endGroup();
}

void Settings::writeLayout(const QWidget* w)
{
  QString prefix = QString("/Geometry/%1/").arg(w->name());
  beginGroup(prefix);
  writeEntry("x", w->x());
  writeEntry("y", w->y());
  writeEntry("w", w->width());
  writeEntry("h", w->height());
  writeEntry("visible",w->isVisible());
  endGroup();
}


Settings* AppSettings;
