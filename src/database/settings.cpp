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

#include <QApplication>
//#include <QGlobal>
#include <QWidget>
#include <QMainWindow>

Settings::Settings() : QSettings(IniFormat, UserScope, "chessx", "chessx")
{
}

Settings::~Settings()
{
}

void Settings::readLayout(QWidget* w, unsigned flags)
{
  if (!w)
    return;
  QString prefix = QString("/Geometry/%1/").arg(w->objectName());
  beginGroup(prefix);
  int x = value("x", w->x()).toInt();
  int y = value("y", w->y()).toInt();
  int width = value("w", w->width()).toInt();
  int height = value("h", w->height()).toInt();
  w->resize(QSize(width, height));
  w->move(QPoint(x, y));
  if ((flags && Show) && value("visible").toBool())
    w->show();
  QMainWindow* main = qobject_cast<QMainWindow*>(w);
  if (main)
  {
    QByteArray docks = value("docks", QByteArray()).toByteArray();
    if (docks.count())
      main->restoreState(docks, 0);
  }
  endGroup();
}

void Settings::writeLayout(const QWidget* w)
{
  if (!w)
    return;
  QString prefix = QString("/Geometry/%1/").arg(w->objectName());
  beginGroup(prefix);
  setValue("x", w->x());
  setValue("y", w->y());
  setValue("w", w->width());
  setValue("h", w->height());
  setValue("visible",w->isVisible());
  const QMainWindow* main = qobject_cast<const QMainWindow*>(w);
  if (main)
    setValue("docks", main->saveState(0));
  endGroup();
}

QString Settings::dataPath()
{
  if (m_dataPath.isNull())
  {
    m_dataPath = qApp->applicationDirPath();
    if (m_dataPath.endsWith("/games/bin"))
    {
      m_dataPath.truncate(m_dataPath.length() - 10);
      m_dataPath.append("/share/games/chessx");
    }
    else
    {
     if(m_dataPath.endsWith("/chessdatabase.app/Contents/MacOS"))
       m_dataPath.truncate(m_dataPath.length() - 33);
     if (m_dataPath.endsWith("/bin"))
       m_dataPath.truncate(m_dataPath.length() - 4);
     m_dataPath.append("/data");
    }
  }
  return m_dataPath;
}

QString Settings::iconPath(const QString& name)
{
  return QString("%1/images/%2.png").arg(dataPath()).arg(name);
}

Settings* AppSettings;

