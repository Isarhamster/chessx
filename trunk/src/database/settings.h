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

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <qsettings.h>

class QWidget;

/** The Settings class provides a wrapper to Qt QConfig class. It allows to
    easily save/restore application settings and get paths for various resources.
 */

class Settings : public QSettings
{
public:
  enum {Show = 1} LayoutFlags;
  Settings();
  ~Settings();
  /** Restore widget's layout based on its name. Optionally show window if it is visible */
  void readLayout(QWidget* w, unsigned flags = 0);
  /** Write widget's layout with its name. */
  void writeLayout(const QWidget* w);
  /** @return directory where data are stored. */
  QString dataPath();
  /** @return directory where given icon is stored. Path and extension will be added automatically. */
  QString iconPath(const QString& name);
private:
  QString m_dataPath;
};

extern Settings* AppSettings;

#endif

