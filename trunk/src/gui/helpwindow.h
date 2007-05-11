/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 01/10/2005
    copyright            : (C) 2005 Heinz R. Hopfgartner
                           <heinz.hopfgartner@gmx.at>
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


#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class QTextBrowser;
class QComboBox;
class QUrl;
class QStringList;

/** @ingroup GUI
The HelpWindow class displays help pages in html format. */

class HelpWindow : public QMainWindow
{
	Q_OBJECT
public:
	HelpWindow();
	~HelpWindow();

protected:
	/** Close event is reimplemented to hide the window when the frame is closed */
	void closeEvent(QCloseEvent*);

private slots:

	void about();
	void aboutQt();
	void openFile();
	void print();
	void sourceChanged(const QUrl&);

	/** When using the History ComboBox for navigation this method is called.
	    Sets a new source in the QTextBrowser */
	void historyComboChanged();

private:

	void configure();
	void createActions();
	void createMenus();
	void createToolBar();
	void createStatusBar();

	QTextBrowser *m_browser;
	QComboBox *m_historyCombo;
	QString *m_bookmarkFile;
	QString m_home;

	QMenu *m_fileMenu;
	QMenu *m_navigateMenu;
	QMenu *m_helpMenu;
	QToolBar *m_toolBar;

	QAction *m_openAct;
	QAction *m_printAct;
	QAction *m_exitAct;
	QAction *m_forwardAct;
	QAction *m_backwardAct;
	QAction *m_homeAct;
	QAction *m_aboutAct;
	QAction *m_aboutQtAct;
};

#endif
