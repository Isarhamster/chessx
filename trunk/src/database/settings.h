/***************************************************************************
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <QSettings>

class QWidget;

/** @ingroup Core
    The Settings class provides a wrapper to Qt QSettings class. It allows to
    easily save/restore application settings and get paths for various resources.
 */

#ifdef Q_OS_MAC
    #define MIN_WHEEL_COUNT 120
    #define DEFAULT_FONTSIZE 12
    #define DEFAULT_LISTFONTSIZE 12
#else
    #define MIN_WHEEL_COUNT 0
    #define DEFAULT_FONTSIZE 11
    #define DEFAULT_LISTFONTSIZE 11
#endif

class Settings : public QSettings
{
	Q_OBJECT
public:
	enum {Show = 1} LayoutFlags;
	Settings();
	~Settings();
	/** Restore widget's layout based on its name. Optionally show window if it is visible.
	@return @p true if the state was restored. */
    bool layout(QWidget* w);
	/** Write widget's layout with its name. */
    void setLayout(const QWidget* w);
	/** @return directory where data are stored. */
    QString dataPath();
	/** Write integer list to configuration file. Does it by converting it to QString */
    void setList(const QString& key, QList<int> list);
	/** Appends values to the list. @return @p true if the list contains exact number of items.
	If @p items is @p -1 , always return @p true. */
    bool list(const QString& key, QList<int>& list, int items = -1);
    void setByteArray(const QString& key, const QByteArray& arr);
    QByteArray byteArray(const QString& key);
    template<class KEY, class VALUE>
    void setMap(const QString& key, const QMap<KEY,VALUE>& map);
    template<class KEY, class VALUE>
    void getMap(const QString& key, QMap<KEY,VALUE>& map);
private:
	QString m_dataPath;
};

template<class KEY, class VALUE>
void setMap(const QString& key, const QMap<KEY,VALUE>& map)
{

}

template<class KEY, class VALUE>
void getMap(const QString& key, QMap<KEY,VALUE>& map)
{
}

extern Settings* AppSettings;

#endif

