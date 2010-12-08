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

int main(int argc, char** argv)
{
	AppSettings = new Settings;
	QApplication a(argc, argv);

	QString shortLang = QString("chessx_%1.qm").arg(QLocale::system().name().left(2));
	QString fullLang = QString("chessx_%1.qm").arg(QLocale::system().name().left(5));

	// Lanuage may have two forms: "pt_BR" or "pl"
	QTranslator translator;
	if (translator.load(AppSettings->dataPath() + "/lang/" + fullLang) ||
		 translator.load(QString(":i18n/") + fullLang) ||
		 translator.load(AppSettings->dataPath() + "/lang/" + shortLang) ||
		 translator.load(QString(":i18n/") + shortLang))
		a.installTranslator(&translator);

	MainWindow* mainWindow = new MainWindow;
	mainWindow->show();

	// Destroy main window and close application
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	int result = a.exec();

	delete AppSettings;
	return result;
}

