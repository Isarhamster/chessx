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

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

#if defined(Q_OS_MAC)
    QDir dir(QApplication::applicationDirPath());
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

    AppSettings = new Settings;

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
        if(translator.load(QString(":/i18n/") + lang) ||
                translator.load(AppSettings->dataPath() + "/lang/" + lang) ||
                translator.load(QString(":/i18n/") + fullSystemLang) ||
                translator.load(AppSettings->dataPath() + "/lang/" + fullSystemLang) ||
                translator.load(QString(":/i18n/") + shortSystemLang) ||
                translator.load(AppSettings->dataPath() + "/lang/" + shortSystemLang))
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
    return result;
}

