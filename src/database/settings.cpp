/***************************************************************************
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "settings.h"
#include "boardtheme.h"

#include <QtCore>
#include <QtGui>
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
	if (valid) { // Enough values
		w->resize(QSize(values[2], values[3]));
		w->move(QPoint(values[0], values[1]));
		if (qobject_cast<QMainWindow*>(w)) {
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
    // TODO - alles Quark!
	if (m_dataPath.isNull()) {
		m_dataPath = qApp->applicationDirPath();
		if (m_dataPath.endsWith("/games/bin")) {
			m_dataPath.truncate(m_dataPath.length() - 10);
			m_dataPath.append("/share/games/chessx");
		} else if (m_dataPath.endsWith("/games")) {
			m_dataPath.truncate(m_dataPath.length() - 6);
			m_dataPath.append("/share/games/chessx");
		} else {
            if (m_dataPath.endsWith("/Contents/MacOS"))
				//Changed from 33 to 15 to include the data directory in the application directory for MaxOSX
				m_dataPath.truncate(m_dataPath.length() - 15);
            else if (m_dataPath.endsWith("/bin"))
				m_dataPath.truncate(m_dataPath.length() - 4);
			m_dataPath.append("/data");
		}
	}
	return m_dataPath;
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

void Settings::setByteArray(const QString& key, const QByteArray& arr)
{
    setValue(key, arr);
}

QByteArray Settings::byteArray(const QString& key)
{
    return value(key, QByteArray()).toByteArray();
}

void Settings::setMap(const QString& key, const OptionValueList& map)
{
    QByteArray data;
    QDataStream * stream = new QDataStream(&data, QIODevice::WriteOnly);

    (*stream) << map;
    delete stream;

    setByteArray(key, data);
}

void Settings::getMap(const QString& key, OptionValueList& map)
{
    QByteArray data = byteArray(key);
    QDataStream * stream = new QDataStream(&data, QIODevice::ReadOnly);
    (*stream) >> map;
    delete stream;
}

QMap<QString, QVariant> Settings::initDefaultValues() const {
    QMap<QString, QVariant> map;
    map.insert("/General/EditLimit", 10);
    map.insert("/General/automaticECO", true);
    map.insert("/General/useIndexFile", true);
    map.insert("/General/ListFontSize", DEFAULT_LISTFONTSIZE);
    map.insert("/General/onlineTablebases", true);
    map.insert("/General/autoCommitDB", false);
    map.insert("/GameText/FontSize",DEFAULT_FONTSIZE);
    map.insert("/MainWindow/GameToolBar", false);
    map.insert("/MainWindow/VerticalTabs", false);
    map.insert("/MainWindow/FilterFollowsGame", false);
    map.insert("/MainWindow/BlunderCheck", true);
    map.insert("/History/MaxEntries", 4);
    map.insert("/Board/showFrame", true);
    map.insert("/Board/showCurrentMove", true);
    map.insert("/Board/guessMove", true);
    map.insert("/Board/nextGuess", false);
    map.insert("/Board/minWheelCount", MIN_WHEEL_COUNT);
    map.insert("/Board/pieceTheme", "merida");
    map.insert("/Board/pieceEffect", BoardTheme::Shadow);
    map.insert("/Board/boardTheme", "brazilwood");
    map.insert("/Board/lightColor", QColor(Qt::lightGray));
    map.insert("/Board/darkColor", QColor(Qt::darkGray));
    map.insert("/Board/highlightColor", QColor(Qt::yellow));
    map.insert("/Board/frameColor", QColor(Qt::black));
    map.insert("/Board/currentMoveColor", QColor(Qt::blue));
    map.insert("/Board/AutoPlayerInterval", 3000);
    map.insert("/Board/AutoSaveAndContinue", false);
    return map;
}

QVariant Settings::getValue(const QString &key) const
{
    static QMap<QString, QVariant> defaultValues = initDefaultValues();
    if (defaultValues.contains(key))
    {
        return value(key, defaultValues.value(key));
    }
    else
    {
        QString groupKey = QString("/") + group() + "/" + key;
        if (defaultValues.contains(groupKey))
        {
            return value(key, defaultValues.value(groupKey));
        }
    }
    Q_ASSERT(false);
    return QVariant();
}

//////////////////////////////////////////////////////////////////////////////
// The singleton instance pointer of our AppSettings
//////////////////////////////////////////////////////////////////////////////

Settings* AppSettings;

//////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////

