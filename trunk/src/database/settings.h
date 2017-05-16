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
#include "engineoptiondata.h"

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
#define DEFAULT_FONTSIZE 10
#define DEFAULT_LISTFONTSIZE 10
#endif

class Settings : public QSettings
{
    Q_OBJECT
public:
    enum {Show = 1} LayoutFlags;
    Settings();
    ~Settings();
public slots:
    /** Restore widget's layout based on its name. Optionally show window if it is visible.
    @return @p true if the state was restored. */
    bool layout(QWidget* w);
    /** Write widget's layout with its name. */
    void setLayout(const QWidget* w);
public:
    /** @return directory where data are stored. */
    QString dataPath();
    /** @return directory where executable sub programs are stored. */
    QString programDataPath();
    /** @return directory where preinstalled uci engines are stored. */
    QString uciPath();
    /** @return directory where preinstalled winboard engines are stored. */
    QString winboardPath();
    /** @return directory where common chess data are stored. */
    QString commonDataPath();
    /** @return directory where preinstalled timeseal is stored. */
    QString timesealFilePath();
    /** @return directory where logging is sent to */
    QString logPath();

    /** Write integer list to configuration file. Does it by converting it to QString */
    void setList(const QString& key, QList<int> list);
    void setList(const QString& key, QList<QVariant> list);

    /** Appends values to the list. @return @p true if the list contains exact number of items.
    If @p items is @p -1 , always return @p true. */
    bool list(const QString& key, QList<int>& list, int items = -1);
    bool list(const QString& key, QList<QVariant>& list, int items = -1);

    /// set a QByteArray into the settings
    void setByteArray(const QString& key, const QByteArray& arr);
    /// Read a QByteArray from the Settings
    QByteArray byteArray(const QString& key);

    /// Overloading value from QSettings with a single place where defaults come from
    QVariant getValue(const QString &key) const;

    void setMap(const QString& key, const OptionValueList& map);
    void getMap(const QString& key, OptionValueList& map);

    QString getTempPath() const;

    QString getThemePath(QString effect, QString pieces) const;
    QStringList getThemeList(QString path) const;

    QString getImagePath() const;

    QString getBoardPath(QString) const;
    QStringList getBoardList() const;

    QString getBuiltinDbPath() const;
    QStringList getBuiltinDatabases() const;

    QStringList getTranslationPaths() const;
    QStringList getTranslations() const;
    QString indexPath() const;
    QString shotsPath() const;
private:

    QMap<QString, QVariant> initDefaultValues() const;
    QString m_dataPath;

    QStringList getImageList(QString userPath, QString internalPath) const;
};

extern Settings* AppSettings;

#endif

