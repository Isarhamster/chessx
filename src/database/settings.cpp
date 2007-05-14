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
#include <QWidget>
#include <QMainWindow>

Settings::Settings() : QSettings(IniFormat, UserScope, "chessx", "chessx")
{}

Settings::~Settings()
{}

bool Settings::layout(QWidget* w)
{
	if (!w || w->objectName().isEmpty())
		return false;
	beginGroup("Geometry");
	QList<int> values;
	bool valid = list(w->objectName(), values, 5);
	if (valid)  // Enough values
	{
		w->resize(QSize(values[2], values[3]));
		w->move(QPoint(values[0], values[1]));
		if (qobject_cast<QMainWindow*>(w))
		{
			QByteArray docks = value("Docks", QByteArray()).toByteArray();
			if (docks.count())
				qobject_cast<QMainWindow*>(w)->restoreState(docks, 0);
		} else if (values[4]) // restore non-main windows
			w->show();
	}
	endGroup();
	return valid;
}

void Settings::setLayout(const QWidget* w)
{
	if (!w || w->objectName().isEmpty())
		return;
	beginGroup("Geometry");
	QList<int> values;
	values << w->x() << w->y() << w->width() << w->height() << w->isVisible();
	setList(w->objectName(), values);
	if (qobject_cast<const QMainWindow*>(w))
		setValue("Docks", qobject_cast<const QMainWindow*>(w)->saveState(0));
	endGroup();
}

QString Settings::dataPath()
{
	if (m_dataPath.isNull()) {
		m_dataPath = qApp->applicationDirPath();
		if (m_dataPath.endsWith("/games/bin")) {
			m_dataPath.truncate(m_dataPath.length() - 10);
			m_dataPath.append("/share/games/chessx");
		} else if (m_dataPath.endsWith("/games")) {
			m_dataPath.truncate(m_dataPath.length() - 6);
			m_dataPath.append("/share/games/chessx");
		} else {
			if (m_dataPath.endsWith("/chessdatabase.app/Contents/MacOS"))
				//Changed from 33 to 15 to include the data directory in the application directory for MaxOSX
				m_dataPath.truncate(m_dataPath.length() - 15);
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

void Settings::setList(const QString& key, QList<int> list)
{
	QList<QVariant> varlist;
	int i;
	foreach(i, list)
	varlist.append(QVariant(i));
	setValue(key, varlist);
}

bool Settings::list(const QString &key, QList<int>& list, int items)
{
	QList<QVariant> varlist = value(key).toList();
	if (items >= 0 && varlist.count() + list.count() != items)
		return false;
	QVariant v;
	foreach(v, varlist)
	list.append(v.toInt());
	return true;
}

Settings* AppSettings;

