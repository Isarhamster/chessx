/***************************************************************************
								  main.cpp  -  main application
									  -------------------
	 begin                : sob maj 7 2005
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

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "mainwindow.h"
#include "settings.h"
#include "logstream.h"

// Necessary includes and defines for memory leak detection:
#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // _MSC_VER

#ifdef _MSC_VER
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER


#if defined(_MSC_VER)

// Code to display the memory leak report
// We use a custom report hook to filter out Qt's own memory leaks
// Credit to Andreas Schmidts - http://www.schmidt-web-berlin.de/winfig/blog/?p=154

_CRT_REPORT_HOOK prevHook;

int customReportHook(int /* reportType */, char* message, int* /* returnValue */) {
  // This function is called several times for each memory leak.
  // Each time a part of the error message is supplied.
  // This holds number of subsequent detail messages after
  // a leak was reported
  const int numFollowupDebugMsgParts = 2;
  static bool ignoreMessage = false;
  static int debugMsgPartsCount = 0;

  // check if the memory leak reporting starts
  if ((strncmp(message,"Detected memory leaks!\n", 10) == 0)
    || ignoreMessage)
  {
    // check if the memory leak reporting ends
    if (strncmp(message,"Object dump complete.\n", 10) == 0)
    {
      _CrtSetReportHook(prevHook);
      ignoreMessage = false;
    } else
      ignoreMessage = true;

    // something from our own code?
    if(strstr(message, ".cpp") == NULL)
    {
      if(debugMsgPartsCount++ < numFollowupDebugMsgParts)
        // give it back to _CrtDbgReport() to be printed to the console
        return FALSE;
      else
        return TRUE;  // ignore it
    } else
    {
      debugMsgPartsCount = 0;
      // give it back to _CrtDbgReport() to be printed to the console
      return FALSE;
    }
  } else
    // give it back to _CrtDbgReport() to be printed to the console
    return FALSE;
}

#endif

int main(int argc, char** argv)
{
#if defined(_MSC_VER)
_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
prevHook = _CrtSetReportHook(customReportHook);
// _CrtSetBreakAlloc(157); // Use this line to break at the nth memory allocation
#endif

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontShowIconsInMenus);  // Icons are *no longer shown* in menus

    QDir dir(QApplication::applicationDirPath());
    QStringList l;
    l<<dir.absolutePath();
    QApplication::setLibraryPaths(l);

#ifdef Q_OS_WIN
    dir.cd("platforms");
    QApplication::addLibraryPath(dir.absolutePath());
#endif

#ifdef Q_OS_MAC
    dir.cdUp();
    dir.cd("plugins");
    QApplication::addLibraryPath(dir.absolutePath());
    dir.cdUp();
    dir.cd("Frameworks");
    QApplication::addLibraryPath(dir.absolutePath());
#endif

    app.setWindowIcon(QIcon(":/images/chessx.png"));

    app.setApplicationName("chessx");
    app.setOrganizationName("chessx");
    app.setOrganizationDomain("chessx.sourceforge.net");

    AppSettings = new Settings;

    // app.setStyle("plastique");

#ifdef Q_OS_MAC
    signal(SIGPIPE, SIG_IGN);
#endif

    QString shortSystemLang = QString("chessx_%1.qm").arg(QLocale::system().name().left(2));
    QString fullSystemLang = QString("chessx_%1.qm").arg(QLocale::system().name().left(5));

    // Language may have two forms: "pt_BR" or "pl"
    QString lang = QString("chessx_%1.qm").arg(AppSettings->getValue("/General/language").toString());

    QDir().mkpath(AppSettings->dataPath() + "/lang/");

    QTranslator translator;
    if (lang != "chessx_en.qm")
    {
        QString filePath = AppSettings->dataPath() + QDir::separator() + "lang" + QDir::separator();

        if(translator.load(QString(":/i18n/") + lang) ||
                translator.load(filePath + lang) ||
                translator.load(QString(":/i18n/") + fullSystemLang) ||
                translator.load(filePath + fullSystemLang) ||
                translator.load(QString(":/i18n/") + shortSystemLang) ||
                translator.load(filePath + shortSystemLang))
        {
            app.installTranslator(&translator);
        }
    }

    MainWindow* mainWindow = new MainWindow;

    mainWindow->show();

    // Destroy main window and close application
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

    startFileLog();
    int result = app.exec();
    stopFileLog();

    delete AppSettings;

#if defined(_MSC_VER)
// Once the app has finished running and has been deleted,
// we run this command to view the memory leaks:
_CrtDumpMemoryLeaks();
#endif

return result;
}

