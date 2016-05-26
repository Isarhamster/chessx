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
#include <QDialog>
#include <QtGui>
#include <QWidget>
#include <QMainWindow>
#include <QSplitter>
#include <QLayout>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

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
    if (w->objectName() != "MainWindow")
    {
        beginGroup(w->objectName());
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

        QSplitter* s = qobject_cast<QSplitter*>(w);
        if (s)
        {
            QByteArray splits = value("Splitter", QByteArray()).toByteArray();
            if (splits.size())
            {
                s->restoreState(splits);
            }
        }
        else
        {
            QMainWindow* m = qobject_cast<QMainWindow*>(w);
            if(m)
            {
                QByteArray docks = value("Docks", QByteArray()).toByteArray();
                if(docks.count())
                {
                    m->restoreState(docks, 0);
                }
            }
            else
            {
                if(values[4])
                {
                    QDialog* d = qobject_cast<QDialog*>(w);
                    if (d) d->setModal(true);
                    w->show();
                }
            }
        }
    }
    endGroup();
    if (w->objectName() != "MainWindow")
    {
        endGroup();
    }
    return valid;
}

void Settings::setLayout(const QWidget* w)
{
    if(!w || w->objectName().isEmpty())
    {
        return;
    }
    if (w->objectName() != "MainWindow")
    {
        beginGroup(w->objectName());
    }
    beginGroup("Geometry");
    QList<int> values;
    int x = w->x();
    if (w->isFullScreen())     x |= 0x40000000;
    else if (w->isMaximized()) x |= 0x80000000;
    values << x << w->y() << w->width() << w->height() << w->isVisible();
    setList(w->objectName(), values);
    const QMainWindow* m = qobject_cast<const QMainWindow*>(w);
    if (m)
    {
        QByteArray docks = m->saveState(0);
        setValue("Docks", docks);
    }
    const QSplitter* s = qobject_cast<const QSplitter*>(w);
    if (s)
    {
        setValue("Splitter", s->saveState());
    }

    endGroup();
    if (w->objectName() != "MainWindow")
    {
        endGroup();
    }
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

        m_dataPath.append(QDir::separator());
        m_dataPath.append("data");
    }

    return m_dataPath;
}

QString Settings::logPath()
{
    QString logPath = AppSettings->dataPath() + "/log/";
    QDir().mkpath(logPath);
    return logPath;
}

QString Settings::programDataPath()
{
    QString programDataPath = QCoreApplication::applicationDirPath();
    programDataPath.append(QDir::separator());
    programDataPath.append("data");
    return programDataPath;
}

QString Settings::uciPath()
{
#ifdef Q_OS_WIN
    QString path(AppSettings->programDataPath());
    return (path + QDir::separator() + "engines" + QDir::separator() + "uci");
#endif
#ifdef Q_OS_MAC
    QString path(AppSettings->programDataPath());
    return (path + QDir::separator() + "engines-mac" + QDir::separator() + "uci");
#endif
    return "";
}

QString Settings::winboardPath()
{
#ifdef Q_OS_WIN
    QString path(AppSettings->programDataPath());
    return (path + QDir::separator() + "engines" + QDir::separator() + "winboard");
#endif
#ifdef Q_OS_MAC
    QString path(AppSettings->programDataPath());
    return (path + QDir::separator() + "engines-mac" + QDir::separator() + "winboard");
#endif
    return "";
}

QString Settings::timesealFilePath()
{
#ifdef Q_OS_WIN
    QString path(AppSettings->programDataPath());
    return (path + QDir::separator() + "timeseal" + QDir::separator() + "windows" + QDir::separator() + "timeseal.exe");
#endif
#ifdef Q_OS_MAC
    QString path(AppSettings->programDataPath());
    return (path + QDir::separator() + "timeseal" + QDir::separator() + "mac" + QDir::separator() + "timeseal");
#endif
    return "";
}

QString Settings::commonDataPath()
{
#if QT_VERSION < 0x050000
    QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + QDir::separator() + "chessdata";
#else
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QDir::separator() + "chessdata";
#endif
    QString dir = value("/General/DefaultDataPath", dataPath).toString();
    return dir;
}

QString Settings::indexPath() const
{
    QString dir = AppSettings->commonDataPath();
    QString path = dir + QDir::separator() + "index";
    return path;
}

QString Settings::shotsPath() const
{
    QString dir = AppSettings->commonDataPath();
    QString path = dir + QDir::separator() + "shots";
    return path;
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

    map.insert("/GameList/AdditionalTags", "");

    map.insert("/MainWindow/GameToolBar", false);
    map.insert("/MainWindow/VerticalTabs", false);
    map.insert("/MainWindow/StayOnTop", false);
    map.insert("/MainWindow/FilterFollowsGame", false);
    map.insert("/MainWindow/ShowMenuIcons", true);

    map.insert("/History/MaxEntries", 4);

    map.insert("/FICS/useTimeseal", true);
    map.insert("/FICS/guestLogin", false);
    map.insert("/FICS/userName", "guest");
    map.insert("/FICS/minutes", 1);
    map.insert("/FICS/increment", 0);
    map.insert("/FICS/eloLow", 0);
    map.insert("/FICS/eloHigh", 9000);

    map.insert("/Board/showFrame", true);
    map.insert("/Board/showCoordinates", true);
    map.insert("/Board/colorCopy", false);
    map.insert("/Board/fixedImageSize", false);
    map.insert("/Board/copyImageSize", 400);
    map.insert("/Board/showCurrentMove", 2);
    map.insert("/Board/showMoveIndicator", 0);
    map.insert("/Board/guessMove", true);
    map.insert("/Board/showThreat", true);
    map.insert("/Board/showTargets", false);
    map.insert("/Board/noHints", false);
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
    map.insert("/Board/storedMoveColor", QColor(Qt::magenta));
    map.insert("/Board/threatColor", QColor(0xe0,0xe0,0xff,0x80));
    map.insert("/Board/targetColor", QColor(Qt::darkGreen));
    map.insert("/Board/AutoPlayerInterval", 3000);
    map.insert("/Board/AutoSaveAndContinue", false);
    map.insert("/Board/BackwardAnalysis", false);
    map.insert("/Board/AutoPromoteToQueen", false);

    map.insert("/Match/Mode", 0);
    map.insert("/Match/TotalTime", 3000);
    map.insert("/Match/UserBonus", 1000);
    map.insert("/Match/Increment", 0);
    map.insert("/Match/MoveCount", 0);
    map.insert("/Match/AnnotateEgt", true);
    map.insert("/Match/AllowBook", false);
    map.insert("/Match/BookMove", 0);
    map.insert("/Match/EngineStarts", false);

    map.insert("/PlayerListWidget/FilterEditCompleter", QStringList());

    map.insert("/Sound/Move", true);

    map.insert("/Tools/Path1", "");
    map.insert("/Tools/CommandLine1", "");

    map.insert("/Web/Favorite1", "");
    map.insert("/Web/AutoNumber1", 1000);

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

QString Settings::getThemePath(QString path) const
{
    QString themeDir(AppSettings->dataPath() + QDir::separator() + "themes");
    if (!path.isEmpty())
    {
        themeDir.append(QDir::separator() + path);
    }

    if(!QFile::exists(themeDir))
    {
        themeDir = QString(":/themes");
        if (!path.isEmpty())
        {
            themeDir.append(QDir::separator() + path);
        }
    }

    return themeDir;
}

QStringList Settings::getThemeList(QString path) const
{
    QStringList themes = QDir(getThemePath(path)).entryList(QStringList("*.png"));
    return themes;
}

QString Settings::getBoardPath() const
{
    QString boardDir(AppSettings->dataPath() + QDir::separator() + "themes" + QDir::separator() + "boards");

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
    QString imgDir(AppSettings->dataPath() + QDir::separator() + "images");

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

    QString langDir(AppSettings->dataPath() + QDir::separator() + "lang");

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

