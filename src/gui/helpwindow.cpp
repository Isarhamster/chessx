/***************************************************************************
                          helpwindow.cpp -  Help Window
                             -------------------
    begin                : 01/10/2005
    copyright            : (C) 2005 Heinz R. Hopfgartner <heinz.hopfgartner@gmx.at>
                           (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   The code is based on Trolltech Qt 3 - Help browser example            *
 ***************************************************************************/

#include <QtGui>
#include "helpwindow.h"
#include "settings.h"

HelpWindow::HelpWindow()
{
	Q_INIT_RESOURCE(helpwindow);
	setObjectName("HelpWindow");
	configure();

	m_browser = new QTextBrowser(this);
	m_browser->setOpenExternalLinks(true);

	createActions();
	createToolBar();
	createMenus();
	createStatusBar();

	m_browser->setSource(m_home);
	setCentralWidget(m_browser);
	setWindowTitle(tr("ChessX - help viewer"));

	connect(m_browser, SIGNAL(sourceChanged(const QUrl&)),
		this, SLOT(sourceChanged(const QUrl&)));

	m_browser->setFocus();
}

HelpWindow::~HelpWindow()
{}

void HelpWindow::about()
{
	QMessageBox::about(this, "About ChessX",
			   "ChessX\n\n"
			   "Copyright 2005-2006\n"
			   "GPL\n\n"
			   "http://chessx.sourceforge.net/\n");
}

void HelpWindow::aboutQt()
{
	QMessageBox::aboutQt(this, "ChessX - Help");
}

void HelpWindow::createActions()
{
	m_openAct = new QAction(QIcon(":fileopen.png"), tr("&Open ..."), this);
	m_openAct->setShortcut(tr("Ctrl+O"));
	m_openAct->setStatusTip(tr("Opens a help page"));
	connect(m_openAct, SIGNAL(triggered()), this, SLOT(openFile()));

	m_printAct = new QAction(QIcon(":print.png"), tr("&Print"), this);
	m_printAct->setShortcut(tr("Ctrl+P"));
	m_printAct->setStatusTip(tr("Prints the current page"));
	connect(m_printAct, SIGNAL(triggered()), this, SLOT(print()));

	m_exitAct = new QAction(tr("E&xit"), this);
	m_exitAct->setShortcut(tr("Ctrl+X"));
	m_exitAct->setStatusTip(tr("Exits the help browser"));
	connect(m_exitAct, SIGNAL(triggered()), this, SLOT(close()));

	m_forwardAct = new QAction(QIcon(":forward.png"), tr("&Forward"), this);
	m_forwardAct->setShortcut(tr("Ctrl+F"));
	m_forwardAct->setStatusTip(tr("Next document in history"));
	connect(m_forwardAct, SIGNAL(triggered()), m_browser, SLOT(forward()));

	m_backwardAct = new QAction(QIcon(":back.png"), tr("&Back"), this);
	m_backwardAct->setShortcut(tr("Ctrl+B"));
	m_backwardAct->setStatusTip(tr("Go back in history"));
	connect(m_backwardAct, SIGNAL(triggered()), m_browser, SLOT(backward()));

	m_homeAct = new QAction(QIcon(":home.png"), tr("&Home"), this);
	m_homeAct->setShortcut(tr("Ctrl+H"));
	m_homeAct->setStatusTip(tr("Open the start page of the helpsystem"));
	connect(m_homeAct, SIGNAL(triggered()), m_browser, SLOT(home()));

	m_aboutAct = new QAction(tr("&About"), this);
	m_aboutAct->setShortcut(tr("Ctrl+A"));
	m_aboutAct->setStatusTip(tr("About ChessX"));
	connect(m_aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	m_aboutQtAct = new QAction(tr("About &Qt"), this);
	m_aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(m_aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void HelpWindow::createToolBar()
{
	m_toolBar = addToolBar(tr("History"));
	m_toolBar->addAction(m_backwardAct);
	m_toolBar->addAction(m_homeAct);
	m_toolBar->addAction(m_forwardAct);
	m_historyCombo = new QComboBox(m_toolBar);
	m_historyCombo->setEditable(true);
	m_toolBar->addWidget(m_historyCombo);
	m_historyCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_historyCombo->addItem(m_home);

	connect(m_historyCombo, SIGNAL(activated(int)),
		this, SLOT(historyComboChanged()));
}

void HelpWindow::createMenus()
{
	m_fileMenu = menuBar()->addMenu(tr("&File"));
	m_fileMenu->addAction(m_openAct);
	m_fileMenu->addAction(m_printAct);
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_exitAct);

	m_navigateMenu = menuBar()->addMenu(tr("Na&vigate"));
	m_navigateMenu->addAction(m_forwardAct);
	m_navigateMenu->addAction(m_backwardAct);
	m_navigateMenu->addAction(m_homeAct);

	menuBar()->addSeparator();

	m_helpMenu = menuBar()->addMenu(tr("&Help"));
	m_helpMenu->addAction(m_aboutAct);
	m_helpMenu->addAction(m_aboutQtAct);
}

void HelpWindow::historyComboChanged()
{
	m_browser->setSource(m_historyCombo->currentText());
}

void HelpWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

//Source taken from Trolltech Qt3 Help browser example modified for Qt4
void HelpWindow::sourceChanged(const QUrl& url)
{
	if (m_browser->documentTitle().isNull())
		setWindowTitle("ChessX - Helpviewer - " + url.path());
	else
		setWindowTitle("ChessX - Helpviewer - " + m_browser->documentTitle()) ;
	if (!url.path().isEmpty() && m_historyCombo) {
		bool exists = FALSE;
		int i;
		for (i = 0; i < m_historyCombo->count(); ++i) {
			if (m_historyCombo->itemText(i) == url.path()) {
				exists = TRUE;
				break;
			}
		}
		if (!exists) {
			m_historyCombo->insertItem(0, url.path());
			m_historyCombo->setCurrentIndex(0);
		} else
			m_historyCombo->setCurrentIndex(i);
	}
}

void HelpWindow::print()
{
	QTextDocument *helpPage = m_browser->document();
	QPrinter printer;

	QPrintDialog *dlg = new QPrintDialog(&printer, this);
	if (dlg->exec() != QDialog::Accepted)
		return;

	helpPage->print(&printer);
}

void HelpWindow::openFile()
{
	QString s = QFileDialog::getOpenFileName(
			    this,
			    tr("Select a help file ..."),
			    "./doc" ,
			    tr("Help pages (*.html *.htm )"));

	m_browser->setSource(s);
}

void HelpWindow::configure()
{
	/** store default - read initial help page to/from the ChessX settings file */
	AppSettings->beginGroup("/helpbrowser/");
	m_home = QDir(AppSettings->value("home", AppSettings->dataPath()
					 + "/doc/index.html").toString()).absolutePath();
	AppSettings->endGroup();
}

void HelpWindow::closeEvent(QCloseEvent* e)
{
	hide();
	e->ignore();
}
