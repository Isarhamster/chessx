/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "shellhelper.h"
#include <QtCore>
#include <QDesktopServices>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

ShellHelper::ShellHelper()
{
}

void ShellHelper::showInFinder(QString path)
{
    // Mac, Windows support folder or file.
#if defined(Q_OS_WIN)
    QString param;
    if(!QFileInfo(path).isDir())
    {
        param = QLatin1String("/select,");
    }
    param += QDir::toNativeSeparators(path);
    QProcess::startDetached("explorer.exe", QStringList(param));
#elif defined(Q_OS_MAC)
    QStringList scriptArgs;
    scriptArgs << QLatin1String("-e")
               << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
               .arg(path);
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    scriptArgs.clear();
    scriptArgs << QLatin1String("-e")
               << QLatin1String("tell application \"Finder\" to activate");
    QProcess::execute("/usr/bin/osascript", scriptArgs);
#else
    // At least try to open the folder.
    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(path).absolutePath()));
#endif
}
