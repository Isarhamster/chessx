/***************************************************************************
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <QSettings>
#include <QString>
#include "engineoptiondata.h"

/** @ingroup Core
    The Settings class provides a wrapper to Qt QSettings class. It allows to
    easily save/restore application settings and get paths for various resources.
 */

#ifdef Q_OS_MAC
#define MIN_WHEEL_COUNT 120
#define DEFAULT_FONTSIZE 12
#define DEFAULT_ICONSIZE 2
#define DEFAULT_LISTFONTSIZE 12
#else
#define MIN_WHEEL_COUNT 0
#define DEFAULT_ICONSIZE 2
#define DEFAULT_FONTSIZE 10
#define DEFAULT_LISTFONTSIZE 10
#endif

class QWidget;

class Settings : public QSettings
{
    Q_OBJECT
public:
    enum {Show = 1} LayoutFlags;
    Settings();
    Settings(const QString &fileName);
    void initialize();
    virtual ~Settings();

public: // Extension Interface for GUI applications
    /** Restore widget's layout based on its name. Optionally show window if it is visible.
    @return @p true if the state was restored. */
    virtual bool layout(QWidget*) { return false; }
    /** Write widget's layout with its name. */
    virtual void setLayout(const QWidget*) {}
public:
    /** @return directory where data are stored. */
    QString dataPath();
    /** @return directory where executable sub programs are stored. */
    QString programDataPath();
    /** @return path where eco data are stored. */
    QString ecoPath() const;
    /** @return path where gtm data are stored. */
    QString gtmPath() const;
    /** @return directory where preinstalled uci engines are stored. */
    QString uciPath();
    /** @return directory where preinstalled winboard engines are stored. */
    QString winboardPath();
    /** @return directory where common chess data are stored. */
    QString commonDataPath();
    /** @return path of a file in the commonData directory. */
    QString commonDataFilePath(QString filename);
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
    /// Shadow `QSettings::setValue()` to have a hook for updating global state
    void setValue(const QString& key, const QVariant& val);

    void setMap(const QString& key, const OptionValueMap& map);
    void getMap(const QString& key, OptionValueMap& map);

    QString getTempPath() const;

    QString getThemePath(QString effect = "", QString pieces = "") const;
    QStringList getThemeList(QString path) const;

    QString getImagePath() const;
    QString getImagePath(QString name) const;
    QPixmap getPixmap(QString name) const;

    QString getBoardPath(QString theme = "") const;
    QStringList getBoardList() const;

    QString getSoundPath(QString sound) const;

    QString getBuiltinDbPath() const;
    QStringList getBuiltinDatabases() const;

    QStringList getTranslationPaths() const;
    QStringList getTranslations() const;
    QString indexPath() const;
    QString shotsPath() const;

    static QString portableIniPath();

    QString getDefaultPieceSet() const;
    QString getDefaultBoard() const;
protected:
    virtual void initWidgetValues(QMap<QString, QVariant>&) const {};

private:

    QString m_dataPath;

    QMap<QString, QVariant> defaultValues;
    QMap<QString, QVariant> initDefaultValues() const;

    QStringList getImageList(QString userPath, QString internalPath) const;
};

extern Settings* AppSettings;

#endif

