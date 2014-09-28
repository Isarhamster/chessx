/***************************************************************************
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "boardtheme.h"
#include "settings.h"

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
    if(!w || w->objectName().isEmpty())
    {
        return false;
    }
    beginGroup("Geometry");
    QList<int> values;
    bool valid = list(w->objectName(), values, 5);
    if(valid)    // Enough values
    {
        int x = values[0];
        x &= ~0xC0000000;
        if (values[0] & 0x80000000)      w->setWindowState(Qt::WindowMaximized);
        else if (values[0] & 0x40000000) w->setWindowState(Qt::WindowFullScreen);
        else
        {
            w->resize(values[2], values[3]);
            w->move(x, values[1]);
        }
        if(qobject_cast<QMainWindow*>(w))
        {
            QByteArray docks = value("Docks", QByteArray()).toByteArray();
            if(docks.count())
            {
                qobject_cast<QMainWindow*>(w)->restoreState(docks, 0);
            }
        }
        else
        {
            if(values[4])
            {
                w->show();
            }
        }
    }
    endGroup();
    return valid;
}

void Settings::setLayout(const QWidget* w)
{
    if(!w || w->objectName().isEmpty())
    {
        return;
    }
    beginGroup("Geometry");
    QList<int> values;
    int x = w->x();
    if (w->isFullScreen())     x |= 0x40000000;
    else if (w->isMaximized()) x |= 0x80000000;
    values << x << w->y() << w->width() << w->height() << w->isVisible();
    setList(w->objectName(), values);
    if(qobject_cast<const QMainWindow*>(w))
    {
        setValue("Docks", qobject_cast<const QMainWindow*>(w)->saveState(0));
    }
    endGroup();
}

QString Settings::dataPath()
{
    if(m_dataPath.isNull())
    {
#if QT_VERSION < 0x050000
        m_dataPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#else
        m_dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#endif

        m_dataPath.append("/data");
    }

    return m_dataPath;
}

QString Settings::programDataPath()
{
    QString programDataPath = QCoreApplication::applicationDirPath();
    programDataPath.append("/data");
    return programDataPath;
}

QString Settings::uciPath()
{
#ifdef Q_OS_WIN
    QString path(AppSettings->programDataPath());
    return (path + "/engines/uci");
#endif
#ifdef Q_OS_MAC
    QString path(AppSettings->programDataPath());
    return (path + "/engines-mac/uci");
#endif
    return "";
}

QString Settings::winboardPath()
{
#ifdef Q_OS_WIN
    QString path(AppSettings->programDataPath());
    return (path + "/engines/winboard");
#endif
#ifdef Q_OS_MAC
    QString path(AppSettings->programDataPath());
    return (path + "/engines-mac/winboard");
#endif
    return "";
}

QString Settings::commonDataPath()
{
#if QT_VERSION < 0x050000
    QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + "/chessdata";
#else
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/chessdata";
#endif

    QString dir = value("/General/DefaultDataPath", dataPath).toString();
    return dir;
}

void Settings::setList(const QString& key, QList<int> list)
{
    QList<QVariant> varlist;
    int i;
    foreach(i, list)
    {
        varlist.append(QVariant(i));
    }
    setValue(key, varlist);
}

bool Settings::list(const QString &key, QList<int>& list, int items)
{
    QList<QVariant> varlist = value(key).toList();
    if(items >= 0 && varlist.count() + list.count() != items)
    {
        return false;
    }
    QVariant v;
    foreach(v, varlist)
    {
        list.append(v.toInt());
    }
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

QMap<QString, QVariant> Settings::initDefaultValues() const
{
    QMap<QString, QVariant> map;
    map.insert("ColumnCount", -1);

    map.insert("/General/EditLimit", 10);
    map.insert("/General/automaticECO", true);
    map.insert("/General/useIndexFile", true);
    map.insert("/General/ListFontSize", DEFAULT_LISTFONTSIZE);
    map.insert("/General/onlineTablebases", true);
    map.insert("/General/onlineVersionCheck", true);
    map.insert("/General/autoCommitDB", false);
    map.insert("/General/language", "Default");
    map.insert("/General/BuiltinDbInstalled", false);
    map.insert("/GameText/FontSize", DEFAULT_FONTSIZE);

    map.insert("/GameText/ColumnStyle", false);
    map.insert("/GameText/SymbolicNag", true);
    map.insert("/GameText/TextWidth", 0);
    map.insert("/GameText/VariationIndentLevel", 1);
    map.insert("/GameText/VariationIndentSize", 3);
    map.insert("/GameText/CommentIndent", "OnlyMainline");
    map.insert("/GameText/MainLineMoveColor", "black");
    map.insert("/GameText/VariationColor", "blue");
    map.insert("/GameText/CommentColor", "green");
    map.insert("/GameText/NagColor", "red");
    map.insert("/GameText/HeaderColor", "blue");
    map.insert("/GameText/ShowHeader", false);
    map.insert("/GameText/ShowDiagrams", true);
    map.insert("/GameText/DiagramSize", 200);
    map.insert("/GameText/FontBrowserText", "'Arial Unicode MS',Menlo");
    map.insert("/GameText/FontBrowserMove", "'Arial Unicode MS',Menlo");
    map.insert("/GameText/PieceString", " KQRBN");

    map.insert("/MainWindow/GameToolBar", false);
    map.insert("/MainWindow/VerticalTabs", false);
    map.insert("/MainWindow/StayOnTop", false);
    map.insert("/MainWindow/FilterFollowsGame", false);
    map.insert("/MainWindow/ShowMenuIcons", true);

    map.insert("/History/MaxEntries", 4);

    map.insert("/FICS/userName", "guest");
    map.insert("/FICS/passWord", "");

    map.insert("/Board/showFrame", true);
    map.insert("/Board/showCoordinates", true);
    map.insert("/Board/showCurrentMove", 2);
    map.insert("/Board/showMoveIndicator", 0);
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
    map.insert("/Tools/Path1", "");
    map.insert("/Tools/CommandLine1", "");
    return map;
}

QVariant Settings::getValue(const QString &key) const
{
    static QMap<QString, QVariant> defaultValues = initDefaultValues();
    if(defaultValues.contains(key))
    {
        return value(key, defaultValues.value(key));
    }
    else
    {
        QString groupKey = QString("/") + group() + "/" + key;
        if(defaultValues.contains(groupKey))
        {
            return value(key, defaultValues.value(groupKey));
        }
    }
    return value(key);
}

QString Settings::getThemaPath() const
{
    QString themeDir(AppSettings->dataPath() + "/themes");

    if(!QFile::exists(themeDir))
    {
        themeDir = QString(":/themes");
    }

    return themeDir;
}

QStringList Settings::getThemeList() const
{
    QStringList themes = QDir(getThemaPath()).entryList(QStringList("*.png"));
    return themes;
}

QString Settings::getBoardPath() const
{
    QString boardDir(AppSettings->dataPath() + "/themes/boards");

    if(!QFile::exists(boardDir))
    {
        boardDir = QString(":/themes/boards");
    }

    return boardDir;
}

QStringList Settings::getBoardList() const
{
    QStringList boards = QDir(getBoardPath()).entryList(QStringList("*.png"));
    return boards;
}

QString Settings::getImagePath() const
{
    QString imgDir(AppSettings->dataPath() + "/images");

    if(!QFile::exists(imgDir))
    {
        imgDir = QString(":/data/images");
    }

    return imgDir;
}

QStringList Settings::getTranslationPaths() const
{
    QStringList list;
    list.append(":i18n");

    QString langDir(AppSettings->dataPath() + "/lang");

    if(QFile::exists(langDir))
    {
        list.append(langDir);
    }

    return list;
}

QStringList Settings::getTranslations() const
{
    QStringList total;
    QStringList langDirs = getTranslationPaths();
    foreach(QString dir, langDirs)
    {
        QStringList translations = QDir(dir).entryList(QStringList("*.qm"));
        total.append(translations);
    }
    total.removeDuplicates();
    return total;
}

QString Settings::getUserDataPath() const
{
#if QT_VERSION < 0x050000
    QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + "/chessdata";
#else
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/chessdata";
#endif
    return dataPath;
}

QString Settings::getTempPath() const
{
#if QT_VERSION < 0x050000
    QString path = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
#else
    QString path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
#endif
    return path;
}


//////////////////////////////////////////////////////////////////////////////
// The singleton instance pointer of our AppSettings
//////////////////////////////////////////////////////////////////////////////

Settings* AppSettings;

//////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////

