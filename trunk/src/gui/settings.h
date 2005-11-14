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

#include <qsettings.h>

class QWidget;


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
private:
  QString m_dataPath;
};

extern Settings* AppSettings;

