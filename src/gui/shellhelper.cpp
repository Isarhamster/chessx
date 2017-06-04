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

void ShellHelper::sendFileWithMail(QString path, QString recipient)
{
    QFileInfo fi(path);
#ifdef Q_OS_MACX
    // Mac OS can actually suck: the mailto URL does not support attachments as it once did in Leopard.
    // Workaround uses osascript, but this is not expected if user is not using Mail as mail client
    QString osaTemplate =
    "tell application \"Mail\"\n"
        "set newMessage to (a reference to (make new outgoing message))\n"
        "tell newMessage\n"
            "make new recipient at beginning of to recipients with properties {address:\"%1\"}\n"
            "set the subject to \"Database %2\"\n"
            "tell newMessage\n"
                "make new attachment with properties {file name:\"%3\"}\n"
            "end tell\n"
            "set visible to true\n"
        "end tell\n"
        "activate\n"
    "end tell\n";
    QString mail = osaTemplate.arg(recipient).arg(fi.fileName()).arg(path);
    QString osascript = "/usr/bin/osascript";
    QStringList processArguments;
    processArguments << QLatin1String("-e") << mail;
    QProcess osa;
    osa.startDetached(osascript, processArguments);
#else
    QString mailTo("mailto:%1?subject=Database %2&body=See attached file&attachment=%3");
    QUrl url(mailTo.arg(recipient).arg(fi.fileName()).arg(path), QUrl::TolerantMode);
    QDesktopServices::openUrl(url);
#endif
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
