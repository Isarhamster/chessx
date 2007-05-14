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

#include <QSettings>

class QWidget;

/** @ingroup Database
    The Settings class provides a wrapper to Qt QSettings class. It allows to
    easily save/restore application settings and get paths for various resources.
 */

class Settings : public QSettings
{
	Q_OBJECT
public:
	enum {Show = 1} LayoutFlags;
	Settings();
	~Settings();
	/** Restore widget's layout based on its name. Optionally show window if it is visible.
	@return @p true if the state was restored. */
	virtual bool layout(QWidget* w);
	/** Write widget's layout with its name. */
	virtual void setLayout(const QWidget* w);
	/** @return directory where data are stored. */
	virtual QString dataPath();
	/** @return directory where given icon is stored. Path and extension will be added automatically. */
	virtual QString iconPath(const QString& name);
	/** Write integer list to configuration file. Does it by converting it to QString */
	virtual void setList(const QString& key, QList<int> list);
	/** Appends values to the list. @return @p true if the list contains exact number of items.
	If @p items is @p -1 , always return @p true. */
	virtual bool list(const QString& key, QList<int>& list, int items = -1);
private:
	QString m_dataPath;
};

extern Settings* AppSettings;

#endif

