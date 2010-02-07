/***************************************************************************
 *   (C) 2005-2009 by Michal Rudolf mrudolf@kdewebdev.org                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "boardsetup.h"
#include "boardview.h"
#include "copydialog.h"
#include "chessbrowser.h"
#include "databaseinfo.h"
#include "ecothread.h"
#include "filtermodel.h"
#include "game.h"
#include "gamelist.h"
#include "helpwindow.h"
#include "mainwindow.h"
#include "messagedialog.h"
#include "memorydatabase.h"
#include "openingtree.h"
#include "output.h"
#include "pgndatabase.h"
#include "playerdialog.h"
#include "preferences.h"
#include "savedialog.h"
#include "settings.h"
#include "tablebase.h"
#include "tableview.h"
#include "tipoftheday.h"
#include "analysiswidget.h"
#include <time.h>

MainWindow::MainWindow() : QMainWindow(),
		m_playerDialog(0), m_saveDialog(0), m_helpWindow(0), m_tipDialog(0),
		m_showPgnSource(false)
{
	setObjectName("MainWindow");

	/* Create clipboard database */
	m_databases.append(new DatabaseInfo);
	m_currentDatabase = 0;
	
	m_tablebase = new Shredder;
	connect(m_tablebase, SIGNAL(bestMove(Move, int)), this, SLOT(showTablebaseMove(Move, int)));

	/* Actions */
	m_actions = new QActionGroup(this);
	m_actions->setExclusive(false);
	setupActions();

	/* Delete on close */
	setAttribute(Qt::WA_DeleteOnClose);

	/* Recent files */
	m_recentFiles.restore("History", "RecentFiles");
	m_recentFiles.removeMissingFiles();
	updateMenuRecent();

	/* Output */
	m_output = new Output(Output::NotationWidget);

	setDockNestingEnabled(true);

	/* Board */
	m_boardSplitter = new QSplitter(Qt::Vertical);
	m_boardSplitter->setChildrenCollapsible(false);
	setCentralWidget(m_boardSplitter);
	m_boardView = new BoardView(m_boardSplitter);
	m_boardView->setObjectName("BoardView");
	m_boardView->setMinimumSize(200, 200);
	m_boardView->resize(500, 540);
	connect(this, SIGNAL(reconfigure()), m_boardView, SLOT(configure()));
	connect(m_boardView, SIGNAL(moveMade(Square, Square)), SLOT(slotBoardMove(Square, Square)));
	connect(m_boardView, SIGNAL(clicked(Square, int)), SLOT(slotBoardClick(Square, int)));
	connect(m_boardView, SIGNAL(wheelScrolled(int)), SLOT(slotBoardMoveWheel(int)));

	/* Move view */
	m_moveView = new ChessBrowser(m_boardSplitter);
	m_moveView->setMinimumHeight(80);
	m_moveView->slotReconfigure();
	connect(m_moveView, SIGNAL(anchorClicked(const QUrl&)), SLOT(slotGameViewLink(const QUrl&)));

	/* Board layout */
	m_boardSplitter->addWidget(m_boardView);
	m_boardSplitter->addWidget(m_moveView);

	/* Game view */
	QDockWidget* dock = new QDockWidget(tr("Game Text"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dock->setObjectName("GameTextDock");
	m_gameView = new ChessBrowser(dock, true);
	m_gameView->setMinimumSize(150, 100);
	m_gameView->slotReconfigure();
	connect(m_gameView, SIGNAL(anchorClicked(const QUrl&)), SLOT(slotGameViewLink(const QUrl&)));
	connect(m_gameView, SIGNAL(actionRequested(int, int)), SLOT(slotGameModify(int, int)));
	connect(this, SIGNAL(databaseChanged(DatabaseInfo*)), m_gameView, SLOT(slotDatabaseChanged(DatabaseInfo*)));
	dock->setWidget(m_gameView);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	m_menuView->addAction(dock->toggleViewAction());
	dock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_P);

	/* Game List */
	dock = new QDockWidget(tr("Game List"), this);
	dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
	dock->setObjectName("GameList");
	m_gameList = new GameList(databaseInfo()->filter(), dock);
	m_gameList->setMinimumSize(150, 100);
	connect(m_gameList, SIGNAL(selected(int)), SLOT(slotFilterLoad(int)));
	connect(m_gameList, SIGNAL(searchDone()), SLOT(slotFilterChanged()));
	dock->setWidget(m_gameList);
	addDockWidget(Qt::BottomDockWidgetArea, dock);
	m_menuView->addAction(dock->toggleViewAction());
	dock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_L);

	/* Opening Tree */
	dock = new QDockWidget(tr("Opening Tree"), this);
	dock->setObjectName("OpeningTreeDock");
	m_openingTree = new OpeningTree;
	g_openingTree = new TableView(dock);
	g_openingTree->setObjectName("OpeningTree");
	g_openingTree->setMinimumSize(150, 100);
	g_openingTree->setSortingEnabled(true);
	g_openingTree->setModel(m_openingTree);
	g_openingTree->sortByColumn(1, Qt::DescendingOrder);
	g_openingTree->slotReconfigure();
	connect(g_openingTree, SIGNAL(clicked(const QModelIndex&)),
		SLOT(slotSearchTreeMove(const QModelIndex&)));
	connect(m_openingTree, SIGNAL(progress(int)), SLOT(slotOperationProgress(int)));
	dock->setWidget(g_openingTree);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	m_menuView->addAction(dock->toggleViewAction());
	connect(dock->toggleViewAction(), SIGNAL(triggered()), SLOT(slotSearchTree()));
	dock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_T);

	/* Analysis Dock */
	dock = new QDockWidget(tr("Analysis"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dock->setObjectName("AnalysisDock");
	m_analysis = new AnalysisWidget;
	dock->setWidget(m_analysis);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	connect(this, SIGNAL(boardChange(const Board&)), m_analysis, SLOT(setPosition(const Board&)));
	connect(this, SIGNAL(reconfigure()), m_analysis, SLOT(slotReconfigure()));
	// Make sure engine is disabled if dock is hidden
	connect(dock, SIGNAL(visibilityChanged(bool)), m_analysis, SLOT(visibilityChanged()));
	m_menuView->addAction(dock->toggleViewAction());
	dock->toggleViewAction()->setShortcut(Qt::Key_F2);

	/* Randomize */
	srand(time(0));

	/* Restoring layouts */
	if (!AppSettings->layout(this))
		resize(800, 600);
	AppSettings->beginGroup("MainWindow");
	m_boardSplitter->restoreState(AppSettings->value("BoardSplit").toByteArray());
	AppSettings->endGroup();
	slotReconfigure();

	/* Status */
	m_statusFilter = new QLabel(statusBar());
	statusBar()->addPermanentWidget(m_statusFilter);
	m_progressBar = new QProgressBar;

	/* Reset board - not earlier, as all widgets have to be created. */
	slotGameChanged();

	/* Display main window */
	show();

	/* Load files from command line */
	QStringList args = qApp->arguments();
	for (int i = 1; i < args.count(); i++)
		if (QFile::exists(args[i]))
			openDatabase(args[i]);

	/* Activate clipboard */
	updateMenuDatabases();
	slotDatabaseChanged();

	/* Tip of the day */
	AppSettings->beginGroup("/Tips/");
	if (AppSettings->value("showTips", true).toBool())
		slotHelpTip();
	AppSettings->endGroup();

	/* Load ECO file */
	slotStatusMessage(tr("Loading ECO file..."));
	qApp->setOverrideCursor(Qt::WaitCursor);
	m_ecothread = new EcoThread(AppSettings->dataPath() + "/chessx");
	connect(m_ecothread, SIGNAL(loaded()), this, SLOT(ecoLoaded()));
	m_ecothread->start();

	m_timer = new QTimer(this);
	m_timer->setInterval(100);
	m_timer->setSingleShot(true);
   connect(m_timer, SIGNAL(timeout()), this, SLOT(slotGameLoadPending()));

}

MainWindow::~MainWindow()
{
	m_timer->stop();
	/* Stop analysis. */
	m_analysis->stopEngine();
	qDeleteAll(m_databases.begin(), m_databases.end());
	delete m_saveDialog;
	delete m_playerDialog;
	delete m_helpWindow;
	delete m_output;
	delete m_tipDialog;
	delete m_tablebase;
}

void MainWindow::ecoLoaded()
{
	qApp->restoreOverrideCursor();
	slotStatusMessage(tr("ECO Loaded."));
	m_ecothread->wait();
	delete m_ecothread;
	m_ecothread = NULL;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
	if (confirmQuit()) {
		m_recentFiles.save("History", "RecentFiles");
		AppSettings->setLayout(m_playerDialog);
		AppSettings->setLayout(m_helpWindow);
		m_gameList->saveConfig();
		g_openingTree->saveConfig();
		m_gameView->saveConfig();
		m_moveView->saveConfig();
		AppSettings->setLayout(this);
		AppSettings->beginGroup("MainWindow");
		AppSettings->setValue("BoardSplit", m_boardSplitter->saveState());
		AppSettings->endGroup();
		e->accept();
		qApp->quit();
	} else
		e->ignore();
}

DatabaseInfo* MainWindow::databaseInfo()
{
	return m_databases[m_currentDatabase];
}

const DatabaseInfo* MainWindow::databaseInfo() const
{
	return m_databases[m_currentDatabase];
}

Database* MainWindow::database()
{
	return databaseInfo()->database();
}

QString MainWindow::databaseName(int index) const
{
	if (index < 0) index = m_currentDatabase;
	QString name = m_databases[index]->database()->name();
	if (name.isEmpty())
		return tr("[Clipboard]");
	return name;
}

Game& MainWindow::game()
{
	return databaseInfo()->currentGame();
}

int MainWindow::gameIndex() const
{
	return databaseInfo()->currentIndex();
}

void MainWindow::gameLoad(int index, bool force, bool reload)
{
	if (databaseInfo()->loadGame(index, reload))
		m_gameList->selectGame(index);
	else if (!force)
		return;
	else
		databaseInfo()->newGame();
	slotGameChanged();
}

void MainWindow::gameMoveBy(int change)
{
	if (game().moveByPly(change)) {
		slotMoveChanged();
		m_gameView->setFocus();
	}
}

void MainWindow::updateMenuRecent()
{
	for (int i = 0; i < m_recentFiles.count(); i++) {
		m_recentFileActions[i]->setVisible(true);
		m_recentFileActions[i]->setText(QString("&%1: %2").arg(i + 1).arg(m_recentFiles[i]));
		m_recentFileActions[i]->setData(m_recentFiles[i]);
	}
	for (int i = m_recentFiles.count(); i < m_recentFileActions.count(); i++)
		m_recentFileActions[i]->setVisible(false);
}

void MainWindow::updateMenuDatabases()
{
	while (m_databases.count() > m_databaseActions.count()) {
		QAction* action = new QAction(this);
		connect(action, SIGNAL(triggered()), SLOT(slotDatabaseChange()));
		m_databaseActions.append(action);
		m_menuDatabases->addAction(action);
	}
	for (int i = 0; i < m_databases.count(); i++) {
		m_databaseActions[i]->setVisible(true);
		m_databaseActions[i]->setData(i);
		m_databaseActions[i]->setText(QString("&%1: %2").arg(i).arg(databaseName(i)));
		int key = Qt::CTRL + Qt::Key_1 + (i - 1);
		if (i < 10)
			m_databaseActions[i]->setShortcut(key);
	}
	for (int i = m_databases.count(); i < m_databaseActions.count(); i++) {
		m_databaseActions[i]->setVisible(false);
		m_databaseActions[i]->setShortcut(0);
	}
}

bool MainWindow::openDatabase(const QString& fname)
{
	/* Check if the database isn't already open */
	for (int i = 0; i < m_databases.count(); i++)
		if (m_databases[i]->database()->filename() == fname) {
			m_currentDatabase = i;
			slotDatabaseChanged();
			slotStatusMessage(tr("Database %1 is already opened.").arg(fname.section('/', -1)));
			return false;
		}


	// Create database, connect progress bar and open file
	DatabaseInfo* db = new DatabaseInfo(fname);
	QString basefile = QFileInfo(fname).completeBaseName();
	startOperation(tr("Opening %1...").arg(basefile));
	connect(db->database(), SIGNAL(progress(int)), SLOT(slotOperationProgress(int)));

	if (!db->open()) {
		cancelOperation(tr("Cannot open file"));
		delete db;
		return false;
	}

	finishOperation(tr("%1 opened").arg(basefile));
	m_databases.append(db);
	m_currentDatabase = m_databases.count() - 1;
	m_recentFiles.append(fname);
	
	updateMenuRecent();
	updateMenuDatabases();
	slotDatabaseChanged();
	return true;
}

QString MainWindow::exportFileName(int& format)
{
	QFileDialog fd(this);
	fd.setAcceptMode(QFileDialog::AcceptSave);
	fd.setFileMode(QFileDialog::AnyFile);
	fd.setWindowTitle(tr("Export games"));
	fd.setViewMode(QFileDialog::Detail);
	fd.setDirectory(QDir::homePath());
	QStringList filters;
	filters << tr("PGN file (*.pgn)")
	<< tr("HTML page (*.html)")
	<< tr("LaTeX document (*.tex)");
	fd.setFilters(filters);
	if (fd.exec() != QDialog::Accepted)
		return QString();
	if (fd.selectedFilter().contains("*.tex"))
		format = Output::Latex;
	else if (fd.selectedFilter().contains("*.html"))
		format = Output::Html;
	else format = Output::Pgn;
	return fd.selectedFiles().first();
}

bool MainWindow::gameEditComment(Output::CommentType type)
{
	bool ok;
	QString annotation;
	if (type == Output::Precomment)
		annotation = game().annotation(CURRENT_MOVE, Game::BeforeMove);
	else annotation = game().annotation();
	QString cmt = QInputDialog::getText(this, tr("Edit comment"), tr("Comment:"),
					    QLineEdit::Normal, annotation, &ok);
	if (ok) {
		if (type == Output::Precomment)
			game().setAnnotation(cmt, CURRENT_MOVE, Game::BeforeMove);
		else game().setAnnotation(cmt);
	}
	return ok;
}

PlayerDialog* MainWindow::playerDialog()
{
	if (!m_playerDialog) {
		m_playerDialog = new PlayerDialog(this);
		AppSettings->layout(m_playerDialog);
	}
	return m_playerDialog;
}

SaveDialog* MainWindow::saveDialog()
{
	if (!m_saveDialog)
		m_saveDialog = new SaveDialog(this);
	return m_saveDialog;
}

HelpWindow* MainWindow::helpWindow()
{
	if (!m_helpWindow) {
		m_helpWindow = new HelpWindow;
		AppSettings->layout(m_helpWindow);
	}
	return m_helpWindow;
}

TipOfDayDialog* MainWindow::tipDialog()
{
	if (!m_tipDialog)
		m_tipDialog = new TipOfDayDialog(this);
	return m_tipDialog;
}

void MainWindow::showTablebaseMove(Move move, int score)
{
	QString result;
	if (score < 0)
		result = tr("Loses in %n move(s)", "", score * -1);
	else if (score > 0)
		result = tr("Wins in %n move(s)", "", score);
	else
		result = tr("Draw");

	QString san(m_boardView->board().moveToSan(move));
	QString update = m_moveView->toHtml();
	int s = update.lastIndexOf("</p>");
	update.insert(s, tr("<br>Tablebase: <a href=\"egtb:%1\">%2%3 %1</a> -- %4")
		      .arg(san).arg(game().moveNumber())
		      .arg(game().board().toMove() == White ? "." : "...").arg(result));
	m_moveView->setHtml(update);
}

QAction* MainWindow::createAction(const QString& name, const char* slot, const QKeySequence& key,
				  const QString& tip)
{
	QAction* action = new QAction(name, m_actions);
	if (!tip.isEmpty())
		action->setStatusTip(tip);
	if (!key.isEmpty())
		action->setShortcut(key);
	if (slot)
		connect(action, SIGNAL(triggered()), slot);
	return action;
}

void MainWindow::setupActions()
{
	/* File menu */
	QMenu* file = menuBar()->addMenu(tr("&File"));
	file->addAction(createAction(tr("&New database..."), SLOT(slotFileNew())));
	file->addAction(createAction(tr("&Open..."), SLOT(slotFileOpen()), Qt::CTRL + Qt::Key_O));
	QMenu* menuRecent = file->addMenu(tr("Open &recent..."));
	const int MaxRecentFiles = 10;
	for (int i = 0; i < MaxRecentFiles; ++i) {
		QAction* action = new QAction(this);
		action->setVisible(false);
		connect(action, SIGNAL(triggered()), SLOT(slotFileOpenRecent()));
		m_recentFileActions.append(action);
		menuRecent->addAction(action);
	}
	file->addAction(createAction(tr("&Save"), SLOT(slotFileSave()), Qt::CTRL + Qt::SHIFT + Qt::Key_S));
	QMenu* exportMenu = file->addMenu(tr("&Export..."));
	exportMenu->addAction(createAction(tr("&Games in filter"), SLOT(slotFileExportFilter())));
	exportMenu->addAction(createAction(tr("&All games"), SLOT(slotFileExportAll())));
	file->addAction(createAction(tr("&Close"), SLOT(slotFileClose()), Qt::CTRL + Qt::Key_W));
	file->addAction(createAction(tr("&Quit"), SLOT(slotFileQuit()), Qt::CTRL + Qt::Key_Q));

	/* Edit menu */
	QMenu* edit = menuBar()->addMenu(tr("&Edit"));
	edit->addAction(createAction(tr("Position &Setup..."), SLOT(slotEditBoard()),
				     Qt::CTRL + Qt::Key_E));
	QMenu* editremove = edit->addMenu(tr("&Remove"));
	editremove->addAction(createAction(tr("Moves from the beginning"),
					   SLOT(slotEditTruncateStart())));
	editremove->addAction(createAction(tr("Moves to the end"), SLOT(slotEditTruncateEnd()),
					   Qt::SHIFT + Qt::Key_Delete));
	edit->addSeparator();
	edit->addAction(createAction(tr("&Copy FEN"), SLOT(slotEditCopyFEN()),
				     Qt::CTRL + Qt::SHIFT + Qt::Key_C));
	edit->addAction(createAction(tr("&Paste FEN"), SLOT(slotEditPasteFEN()),
				     Qt::CTRL + Qt::SHIFT + Qt::Key_V));
	edit->addSeparator();
	edit->addAction(createAction(tr("&Preferences..."), SLOT(slotConfigure())));

	/* Game menu */
	QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));
	QMenu* loadMenu = gameMenu->addMenu(tr("&Load"));

	/* Game->Load submenu */
	loadMenu->addAction(createAction(tr("&First"), SLOT(slotGameLoadFirst()), Qt::CTRL + Qt::SHIFT + Qt::Key_Up));
	loadMenu->addAction(createAction(tr("&Last"), SLOT(slotGameLoadLast()), Qt::CTRL + Qt::SHIFT + Qt::Key_Down));
	loadMenu->addAction(createAction(tr("&Next"), SLOT(slotGameLoadNext()), Qt::CTRL + Qt::Key_Down));
	loadMenu->addAction(createAction(tr("&Previous"), SLOT(slotGameLoadPrevious()), Qt::CTRL + Qt::Key_Up));
	loadMenu->addAction(createAction(tr("&Go to game..."), SLOT(slotGameLoadChosen()), Qt::CTRL + Qt::Key_G));
	loadMenu->addAction(createAction(tr("&Random"), SLOT(slotGameLoadRandom()), Qt::CTRL + Qt::Key_Question));

	/* Game->Go to submenu */
	QMenu* goMenu = gameMenu->addMenu(tr("&Go to"));
	goMenu->addAction(createAction(tr("&Start"), SLOT(slotGameMoveFirst()), Qt::Key_Home));
	goMenu->addAction(createAction(tr("&End"), SLOT(slotGameMoveLast()), Qt::Key_End));
	goMenu->addAction(createAction(tr("&Next move"), SLOT(slotGameMoveNext()), Qt::Key_Right));
	goMenu->addAction(createAction(tr("&Previous move"), SLOT(slotGameMovePrevious()), Qt::Key_Left));
	goMenu->addAction(createAction(tr("5 moves &forward"), SLOT(slotGameMoveNextN()), Qt::Key_Down));
	goMenu->addAction(createAction(tr("5 moves &backward"), SLOT(slotGameMovePreviousN()), Qt::Key_Up));

	gameMenu->addAction(createAction(tr("&New"), SLOT(slotGameNew()), Qt::CTRL + Qt::Key_N));
	gameMenu->addAction(createAction(tr("&Save...."), SLOT(slotGameSave()), Qt::CTRL + Qt::Key_S));

	/* Search menu */
	QMenu* search = menuBar()->addMenu(tr("Fi&nd"));
	search->addAction(createAction(tr("Find &tag"), SLOT(slotSearchTag()), Qt::CTRL +
				       Qt::SHIFT + Qt::Key_T));
	search->addAction(createAction(tr("Find &position"), SLOT(slotSearchBoard()), Qt::CTRL +
				       Qt::SHIFT + Qt::Key_B));
	search->addSeparator();
	search->addAction(createAction(tr("&Reset filter"), SLOT(slotSearchReset()), Qt::CTRL + Qt::Key_F));
	search->addAction(createAction(tr("&Reverse filter"), SLOT(slotSearchReverse()),
				       Qt::CTRL + Qt::SHIFT + Qt::Key_F));

	/* Database menu */
	QMenu* menuDatabase = menuBar()->addMenu(tr("&Database"));
	m_menuDatabases = menuDatabase->addMenu(tr("&Switch to"));
	menuDatabase->addAction(createAction(tr("&Copy games..."), SLOT(slotDatabaseCopy()),
					     Qt::Key_F5));
	QMenu* menuRemove = menuDatabase->addMenu(tr("Delete"));
	menuRemove->addAction(createAction(tr("&Current game"), SLOT(slotDatabaseDeleteGame())));
	menuRemove->addAction(createAction(tr("&Games in filter"), SLOT(slotDatabaseDeleteFilter())));
	menuDatabase->addAction(createAction(tr("&Compact"), SLOT(slotDatabaseCompact())));

	/* View menu */
	m_menuView = menuBar()->addMenu(tr("&View"));
	QAction* flip = createAction(tr("&Flip board"), SLOT(slotConfigureFlip()), Qt::CTRL + Qt::Key_B);
	flip->setCheckable(true);
	m_menuView->addAction(flip);
	m_menuView->addAction(createAction(tr("&Player Database..."), SLOT(slotPlayerDialog()),
					   Qt::CTRL + Qt::SHIFT + Qt::Key_P));


	/* Help menu */
	menuBar()->addSeparator();
	QMenu *help = menuBar()->addMenu(tr("&Help"));
//  help->addAction(createAction(tr("ChessX &help..."), SLOT(slotHelp()), Qt::CTRL + Qt::Key_F1));
	help->addAction(createAction(tr("&Tip of the day"), SLOT(slotHelpTip())));
	help->addAction(createAction(tr("&Report a bug..."), SLOT(slotHelpBug())));
	help->addSeparator();
	help->addAction(createAction(tr("&About ChessX"), SLOT(slotHelpAbout())));

#ifdef QT_DEBUG
	QMenu* debug = help->addMenu(tr("&Debug"));
	QAction* source;
	debug->addAction(source = createAction("Toggle game view format", 0, Qt::Key_F12));
	source->setCheckable(true);
	connect(source, SIGNAL(toggled(bool)), SLOT(slotGameViewToggle(bool)));
#endif
}

bool MainWindow::confirmQuit() 
{
	if (AppSettings->value("/General/confirmQuit", true).toBool() &&
			!MessageDialog::okCancel(this, tr("Do you want to quit?"), tr("Quit"), tr("Quit")))
		return false;
	QString modified;
	for (int i = 1; i < m_databases.size(); i++)
		if (m_databases[i]->database()->isModified())
			modified += m_databases[i]->database()->name() + '\n';
	if (!modified.isEmpty()) {
		int response = MessageDialog::yesNoCancel(this, tr("Following databases are modified:")
					+ '\n' + modified + tr("Save them?"));
		if (response == MessageDialog::Cancel)
			return false;
		if (response == MessageDialog::Yes) {
			Output output(Output::Pgn);
			for (int i = 1; i < m_databases.size(); i++)
				if (m_databases[i]->database()->isModified())
					output.output(m_databases[i]->database()->filename(), 
							*(m_databases[i]->database()));
		}
	}
	return true;
}

void MainWindow::startOperation(const QString& msg)
{
	m_operationTime.start();
	statusBar()->showMessage(msg);
	m_progressBar->setMaximumHeight(m_statusFilter->height());
	statusBar()->insertPermanentWidget(0, m_progressBar);
	m_progressBar->setValue(0);
	m_progressBar->show();
}

void MainWindow::finishOperation(const QString& msg)
{
	statusBar()->showMessage(msg + tr(" (%1 s.)").arg(m_operationTime.elapsed() / 100 / 10.0));
	statusBar()->removeWidget(m_progressBar);
}

void MainWindow::cancelOperation(const QString& msg)
{
	statusBar()->showMessage(msg);
	statusBar()->removeWidget(m_progressBar);
}
