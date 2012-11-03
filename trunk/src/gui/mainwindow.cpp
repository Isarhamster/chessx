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
#include "commentdialog.h"
#include "databaseinfo.h"
#include "ecothread.h"
#include "filtermodel.h"
#include "game.h"
#include "gamelist.h"
#include "mainwindow.h"
#include "messagedialog.h"
#include "memorydatabase.h"
#include "openingtree.h"
#include "output.h"
#include "pgndatabase.h"
#include "playerdialog.h"
#include "playerlist.h"
#include "databaselist.h"
#include "preferences.h"
#include "savedialog.h"
#include "settings.h"
#include "tablebase.h"
#include "tableview.h"
#include "analysiswidget.h"
#include "dockwidgetex.h"
#include "helpbrowser.h"
#include <time.h>

MainWindow::MainWindow() : QMainWindow(),
    m_playerDialog(0), m_saveDialog(0),
    m_showPgnSource(false),
    m_bGameChange(false)
{
	setObjectName("MainWindow");

	/* Create clipboard database */
	m_databases.append(new DatabaseInfo);
	m_currentDatabase = 0;

	/* Actions */
	m_actions = new QActionGroup(this);
	m_actions->setExclusive(false);
	setupActions();

	/* Delete on close */
	setAttribute(Qt::WA_DeleteOnClose);

	/* Output */
	m_output = new Output(Output::NotationWidget);

	setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AllowNestedDocks);

	/* Board */
	m_boardSplitter = new QSplitter(Qt::Vertical);
	m_boardSplitter->setChildrenCollapsible(false);
	setCentralWidget(m_boardSplitter);
	m_boardView = new BoardView(m_boardSplitter);
	m_boardView->setObjectName("BoardView");
	m_boardView->setMinimumSize(200, 200);
	m_boardView->resize(500, 540);
	connect(this, SIGNAL(reconfigure()), m_boardView, SLOT(configure()));
    connect(m_boardView, SIGNAL(moveMade(Square, Square, int)), SLOT(slotBoardMove(Square, Square, int)));
	connect(m_boardView, SIGNAL(clicked(Square, int)), SLOT(slotBoardClick(Square, int)));
	connect(m_boardView, SIGNAL(wheelScrolled(int)), SLOT(slotBoardMoveWheel(int)));

	/* Board layout */
	m_boardSplitter->addWidget(m_boardView);

	/* Game view */
    DockWidgetEx* gameTextDock = new DockWidgetEx(tr("Game Text"), this);
	gameTextDock->setObjectName("GameTextDock");
	gameTextDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
	m_gameView = new ChessBrowser(gameTextDock, true);
    m_gameView->setMinimumSize(200, 200);
	m_gameView->slotReconfigure();
	connect(m_gameView, SIGNAL(anchorClicked(const QUrl&)), SLOT(slotGameViewLink(const QUrl&)));
	connect(m_gameView, SIGNAL(actionRequested(EditAction)), SLOT(slotGameModify(EditAction)));
	connect(this, SIGNAL(databaseChanged(DatabaseInfo*)), m_gameView, SLOT(slotDatabaseChanged(DatabaseInfo*)));
    connect(this, SIGNAL(reconfigure()), m_gameView, SLOT(slotReconfigure()));
    gameTextDock->setWidget(m_gameView);
	addDockWidget(Qt::RightDockWidgetArea, gameTextDock);
    m_gameTitle = new QLabel;
    connect(m_gameTitle, SIGNAL(linkActivated(QString)), this, SLOT(slotGameViewLink(QString)));
    gameTextDock->setTitleBarWidget(m_gameTitle);

	/* Game List */
    DockWidgetEx* gameListDock = new DockWidgetEx(tr("Game List"), this);
	gameListDock->setObjectName("GameList");
	m_gameList = new GameList(databaseInfo()->filter(), gameListDock);
	m_gameList->setMinimumSize(150, 100);
	connect(m_gameList, SIGNAL(selected(int)), SLOT(slotFilterLoad(int)));
	connect(m_gameList, SIGNAL(searchDone()), SLOT(slotFilterChanged()));
    connect(m_gameList, SIGNAL(requestCopyGame()), SLOT(slotDatabaseCopySingle()));
    connect(m_gameList, SIGNAL(requestGameData(Game&)), SLOT(slotGetGameData(Game&)));
    connect(this, SIGNAL(reconfigure()), m_gameList, SLOT(slotReconfigure()));
    gameListDock->setWidget(m_gameList);
    addDockWidget(Qt::RightDockWidgetArea, gameListDock);
	m_menuView->addAction(gameListDock->toggleViewAction());
	gameListDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_L);
    connect(m_gameList, SIGNAL(raiseRequest()), gameListDock, SLOT(raise()));

    // Player List
    DockWidgetEx* playerListDock = new DockWidgetEx(tr("Players"), this);
    playerListDock->setObjectName("PlayerList");
    m_playerList = new PlayerList(this);
    m_playerList->setMinimumSize(150, 100);
    playerListDock->setWidget(m_playerList);
    addDockWidget(Qt::RightDockWidgetArea, playerListDock);
    m_menuView->addAction(playerListDock->toggleViewAction());
    playerListDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_P);
    connect(m_playerList, SIGNAL(raiseRequest()), playerListDock, SLOT(raise()));
    connect(m_playerList, SIGNAL(filterRequest(QString)), m_gameList, SLOT(slotFilterListByPlayer(QString)));
    connect(this, SIGNAL(reconfigure()), m_playerList, SLOT(slotReconfigure()));
    playerListDock->hide();

    // Database List
    DockWidgetEx* dbListDock = new DockWidgetEx(tr("Databases"), this);
    dbListDock->setObjectName("Databases");
    m_databaseList = new DatabaseList(this);
    m_databaseList->setMinimumSize(150, 100);
    dbListDock->setWidget(m_databaseList);
    addDockWidget(Qt::RightDockWidgetArea, dbListDock);
    m_menuView->addAction(dbListDock->toggleViewAction());
    dbListDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_D);
    connect(m_databaseList, SIGNAL(requestOpenDatabase(QString,bool)),
            this, SLOT(openDatabaseEx(QString,bool)));
    connect(m_databaseList, SIGNAL(requestLinkDatabase(QString)),
            this, SLOT(setFavoriteDatabase(QString)));
    connect(m_databaseList, SIGNAL(requestAppendGame(QString,const Game&)),
            this, SLOT(copyGame(QString,const Game&)));
    connect(this, SIGNAL(reconfigure()), m_databaseList, SLOT(slotReconfigure()));
    m_databaseList->addFileOpen(QString(), false);
    m_databaseList->setFileCurrent(QString());
    restoreRecentFiles();

    /* Recent files */
    m_recentFiles.restore("History", "MaxEntries", "RecentFiles");
    m_recentFiles.removeMissingFiles();
    updateMenuRecent();

    /* Opening Tree */
    DockWidgetEx* openingDock = new DockWidgetEx(tr("Opening Tree"), this);
	openingDock->setObjectName("OpeningTreeDock");
	m_openingTree = new OpeningTree;
    m_openingTreeView = new TableView(openingDock);
    m_openingTreeView->setObjectName("OpeningTree");
    m_openingTreeView->setMinimumSize(150, 100);
    m_openingTreeView->setSortingEnabled(true);
    m_openingTreeView->setModel(m_openingTree);
    m_openingTreeView->sortByColumn(1, Qt::DescendingOrder);
    connect(m_openingTreeView, SIGNAL(clicked(const QModelIndex&)), SLOT(slotSearchTreeMove(const QModelIndex&)));
    connect(openingDock, SIGNAL(visibilityChanged(bool)), m_openingTree, SLOT(cancel(bool)));
    connect(m_openingTree, SIGNAL(progress(int)), SLOT(slotOperationProgress(int)));
    connect(m_openingTree, SIGNAL(openingTreeUpdated()), SLOT(slotTreeUpdate()));
    connect(m_openingTree, SIGNAL(openingTreeUpdateStarted()), SLOT(slotTreeUpdateStarted()));
    openingDock->setWidget(m_openingTreeView);
    addDockWidget(Qt::RightDockWidgetArea, openingDock);
	m_menuView->addAction(openingDock->toggleViewAction());
	connect(openingDock->toggleViewAction(), SIGNAL(triggered()), SLOT(slotSearchTree()));
    connect(this, SIGNAL(reconfigure()), m_openingTreeView, SLOT(slotReconfigure()));
	openingDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_T);
    openingDock->hide();
	
	/* Analysis Dock */
    DockWidgetEx* analysisDock = new DockWidgetEx(tr("Analysis 1"), this);
    analysisDock->setObjectName("AnalysisDock1");
    AnalysisWidget* analyis = new AnalysisWidget;
    analyis->setObjectName("Analysis");
    analysisDock->setWidget(analyis);
    addDockWidget(Qt::RightDockWidgetArea, analysisDock);
    connect(analyis, SIGNAL(addVariation(Analysis)),
			  SLOT(slotGameAddVariation(Analysis)));
    connect(analyis, SIGNAL(addVariation(QString)),
              SLOT(slotGameAddVariation(QString)));
    connect(this, SIGNAL(boardChange(const Board&)), analyis, SLOT(setPosition(const Board&)));
    connect(this, SIGNAL(reconfigure()), analyis, SLOT(slotReconfigure()));
	// Make sure engine is disabled if dock is hidden
	connect(analysisDock, SIGNAL(visibilityChanged(bool)),
              analyis, SLOT(slotVisibilityChanged(bool)));
	m_menuView->addAction(analysisDock->toggleViewAction());
    analysisDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_F2);
    analysisDock->hide();

	/* Analysis Dock 2 */
    DockWidgetEx* analysisDock2 = new DockWidgetEx(tr("Analysis 2"), this);
	analysisDock2->setObjectName("AnalysisDock2");
    analyis = new AnalysisWidget;
    analyis->setObjectName("Analysis2");
    analysisDock2->setWidget(analyis);
	addDockWidget(Qt::RightDockWidgetArea, analysisDock2);
    connect(analyis, SIGNAL(addVariation(Analysis)),
			  SLOT(slotGameAddVariation(Analysis)));
    connect(this, SIGNAL(boardChange(const Board&)), analyis, SLOT(setPosition(const Board&)));
    connect(this, SIGNAL(reconfigure()), analyis, SLOT(slotReconfigure()));
	// Make sure engine is disabled if dock is hidden
	connect(analysisDock2, SIGNAL(visibilityChanged(bool)),
              analyis,SLOT(slotVisibilityChanged(bool)));
	m_menuView->addAction(analysisDock2->toggleViewAction());
    analysisDock2->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_F3);
    analysisDock2->hide();

	/* Randomize */
	srand(time(0));

	/* Restoring layouts */
	if (!AppSettings->layout(this))
		resize(800, 600);
	AppSettings->beginGroup("MainWindow");
	m_boardSplitter->restoreState(AppSettings->value("BoardSplit").toByteArray());
    m_gameList->m_FilterActive = AppSettings->value("FilterFollowsGame", QVariant(false)).toBool();
	AppSettings->endGroup();
    m_toggleFilter->setChecked(m_gameList->m_FilterActive);

	/* Status */
	m_statusFilter = new QLabel(statusBar());
	statusBar()->addPermanentWidget(m_statusFilter);
	m_progressBar = new QProgressBar;

	/** Reconfigure. */
	slotReconfigure();

	/* Reset board - not earlier, as all widgets have to be created. */
	slotGameChanged();

	/* Display main window */
	show();

	/* Load files from command line */
	QStringList args = qApp->arguments();
	for (int i = 1; i < args.count(); i++)
		if (QFile::exists(args[i]))
			openDatabase(args[i]);

	qApp->installEventFilter(this);
	/* Activate clipboard */
	updateMenuDatabases();
	slotDatabaseChanged();

	/* Load ECO file */
	slotStatusMessage(tr("Loading ECO file..."));
	qApp->setOverrideCursor(Qt::WaitCursor);
	m_ecothread = new EcoThread(AppSettings->dataPath() + "/chessx");
	connect(m_ecothread, SIGNAL(loaded()), this, SLOT(ecoLoaded()));
	m_ecothread->start();
}

MainWindow::~MainWindow()
{
    m_openingTree->cancel(false);
    foreach (DatabaseInfo* database, m_databases) {
        database->close();
    }
	qDeleteAll(m_databases.begin(), m_databases.end());
	delete m_saveDialog;
	delete m_playerDialog;
	delete m_output;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FileOpen)
    {
        openDatabase(static_cast<QFileOpenEvent*>(event)->file());
        return true;
    }
    else
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent && (keyEvent->key() == Qt::Key_Escape ||
                    keyEvent->key() == Qt::Key_Return ||
                    keyEvent->key() == Qt::Key_Enter))
            {
                keyPressEvent(keyEvent);
                return true;
            }
        }
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
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
    if (confirmQuit())
    {
		m_recentFiles.save("History", "RecentFiles");
        m_databaseList->save();
		AppSettings->setLayout(m_playerDialog);
		m_gameList->saveConfig();
        m_databaseList->saveConfig();
        m_playerList->saveConfig();
        m_openingTreeView->saveConfig();
		m_gameView->saveConfig();
		AppSettings->setLayout(this);
		AppSettings->beginGroup("MainWindow");
		AppSettings->setValue("BoardSplit", m_boardSplitter->saveState());
        AppSettings->setValue("FilterFollowsGame", m_gameList->m_FilterActive);
		AppSettings->endGroup();
    }
    else
    {
		e->ignore();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape)
		m_nagText.clear();
	if (game().atLineStart() || game().atGameStart() ||
		 e->key() == Qt::Key_Escape || e->text().isEmpty())
		return;

    bool enterPressed = (e->key()==Qt::Key_X || (e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return));

    if (!enterPressed)
		m_nagText.append(e->text());
	int matches = NagSet::prefixCount(m_nagText);
	if (matches == 0)
		m_nagText.clear();
    else if (matches == 1 || enterPressed) {
		game().addNag(NagSet::fromString(m_nagText));
		slotGameChanged();
	}
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
    QuerySaveGame();
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
    for (int i = 0; (i < m_recentFiles.count()) && (i<MaxRecentFiles); ++i) {
		m_recentFileActions[i]->setVisible(true);
		m_recentFileActions[i]->setText(QString("&%1: %2").arg(i + 1).arg(m_recentFiles[i]));
		m_recentFileActions[i]->setData(m_recentFiles[i]);
	}
    for (int i = m_recentFiles.count(); i < MaxRecentFiles; i++)
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
    int n = 1;
	for (int i = 0; i < m_databases.count(); i++) {
        if (m_databases[i]->isValid())
        {
            m_databaseActions[i]->setVisible(true);
            m_databaseActions[i]->setData(i);
            m_databaseActions[i]->setText(QString("&%1: %2").arg(i).arg(databaseName(i)));
            if (n < 10)
            {
                int key = Qt::CTRL + Qt::SHIFT + Qt::Key_1 + (n - 1);
                m_databaseActions[i]->setShortcut(key);
                ++n;
            }
        }
	}
	for (int i = m_databases.count(); i < m_databaseActions.count(); i++) {
		m_databaseActions[i]->setVisible(false);
		m_databaseActions[i]->setShortcut(0);
	}
}

void MainWindow::setFavoriteDatabase(QString fname)
{
   m_databaseList->setFileFavorite(fname, true);
}

void MainWindow::openDatabase(QString fname)
{
    openDatabaseEx(fname, false);
}

void MainWindow::openDatabaseEx(QString fname, bool utf8)
{
    QuerySaveGame();

    QFileInfo fi = QFileInfo(fname);

    if (!fname.isEmpty())
    {
        fname = fi.canonicalFilePath();

        if (!QFile::exists(fname))
        {
            slotStatusMessage(tr("Cannot open file '%1'.").arg(fi.fileName()));
            return;
        }
    }

	/* Check if the database isn't already open */
	for (int i = 0; i < m_databases.count(); i++)
    {
		if (m_databases[i]->database()->filename() == fname) {
            if (m_databases[i]->isValid())
            {
                m_currentDatabase = i;
                m_databaseList->setFileCurrent(fname);
                slotDatabaseChanged();
            }
            else
            {
                slotStatusMessage(tr("Database %1 cannot be accessed at the moment.").arg(fi.fileName()));
            }
            return;
		}
    }

	// Create database, connect progress bar and open file
	DatabaseInfo* db = new DatabaseInfo(fname);
    QString basefile = fi.completeBaseName();

	startOperation(tr("Opening %1...").arg(basefile));
    connect(db->database(), SIGNAL(progress(int)), SLOT(slotOperationProgress(int)));
    connect(db, SIGNAL(LoadFinished(DatabaseInfo*)), this, SLOT(slotDataBaseLoaded(DatabaseInfo*)));
    if (!db->open(utf8))
    {
        slotDataBaseLoaded(db);
    }
    else
    {
        m_databases.append(db);
    }
}

void MainWindow::slotDataBaseLoaded(DatabaseInfo* db)
{
    if (!db->IsLoaded()) {
        cancelOperation(tr("Cannot open file"));
        m_databases.removeOne(db);
        delete db;
        return;
	}
    QString fname = db->filePath();
    QFileInfo fi = QFileInfo(fname);
    QString basefile = fi.completeBaseName();

    m_databaseList->addFileOpen(fname,db->IsUtf8());

	finishOperation(tr("%1 opened").arg(basefile));

    for (int i = 0; i < m_databases.count(); i++)
    {
        if (m_databases[i]->database()->filename() == fname)
        {
            m_currentDatabase = i;
        }
    }
    m_databaseList->setFileCurrent(fname);
	m_recentFiles.append(fname);

	updateMenuRecent();
	updateMenuDatabases();
	slotDatabaseChanged();
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
	QString annotation;
	if (type == Output::Precomment)
    {
        int moves;
        int comments;
        int nags;
        game().moveCount(&moves,&comments,&nags);
        if( moves > 0 )
        {
            annotation = game().annotation(CURRENT_MOVE, Game::BeforeMove);
        } else
        {
            annotation = game().gameComment();
        }
    }
	else annotation = game().annotation();
	CommentDialog dlg(this);
	dlg.setText(annotation);
	if (!dlg.exec())
		return false;

	if (type == Output::Precomment)
    {
        int moves;
        int comments;
        int nags;
        game().moveCount(&moves,&comments,&nags);
        if( moves > 0 )
        {
            game().setAnnotation(dlg.text(), CURRENT_MOVE, Game::BeforeMove);
        } else
        {
            game().setGameComment(dlg.text());
        }
    }
	else game().setAnnotation(dlg.text());
	return true;
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

QAction* MainWindow::createAction(const QString& name, const char* slot, const QKeySequence& key,
											 const QString& tip, QAction::MenuRole menuRole)
{
	QAction* action = new QAction(name, m_actions);
	if (!tip.isEmpty())
		action->setStatusTip(tip);
	if (!key.isEmpty())
		action->setShortcut(key);
	if (slot)
		connect(action, SIGNAL(triggered()), slot);
		  action->setMenuRole(menuRole);
	return action;
}

void MainWindow::setupActions()
{
	/* File menu */
	QMenu* file = menuBar()->addMenu(tr("&File"));
    file->addAction(createAction(tr("&New database..."), SLOT(slotFileNew())));
    file->addAction(createAction(tr("&Open..."), SLOT(slotFileOpen()), QKeySequence::Open));
    file->addAction(createAction(tr("&Open in UTF8..."), SLOT(slotFileOpenUtf8())));
	QMenu* menuRecent = file->addMenu(tr("Open &recent..."));

	for (int i = 0; i < MaxRecentFiles; ++i) {
		QAction* action = new QAction(this);
		action->setVisible(false);
		connect(action, SIGNAL(triggered()), SLOT(slotFileOpenRecent()));
		m_recentFileActions.append(action);
		menuRecent->addAction(action);
	}
    file->addSeparator();
	file->addAction(createAction(tr("&Save"), SLOT(slotFileSave()), Qt::CTRL + Qt::SHIFT + Qt::Key_S));
	QMenu* exportMenu = file->addMenu(tr("&Export..."));
	exportMenu->addAction(createAction(tr("&Games in filter"), SLOT(slotFileExportFilter())));
	exportMenu->addAction(createAction(tr("&All games"), SLOT(slotFileExportAll())));
    file->addSeparator();
	file->addAction(createAction(tr("&Close"), SLOT(slotFileClose()), QKeySequence::Close));
	file->addAction(createAction(tr("&Quit"), SLOT(slotFileQuit()), QKeySequence(), QString(), QAction::QuitRole));

	/* Edit menu */
	QMenu* edit = menuBar()->addMenu(tr("&Edit"));
	edit->addAction(createAction(tr("Comment"), SLOT(slotEditComment()),
										  Qt::CTRL + Qt::Key_A));
    edit->addAction(createAction(tr("Comment Before"), SLOT(slotEditCommentBefore()),
                                          Qt::CTRL + Qt::ALT + Qt::Key_A));
    QMenu* editVariation = edit->addMenu(tr("Variation"));
	editVariation->addAction(createAction(tr("Promote"), SLOT(slotEditVarPromote()),
													  Qt::CTRL + Qt::Key_J));
	editVariation->addAction(createAction(tr("Remove"), SLOT(slotEditVarRemove()),
													  Qt::CTRL + Qt::Key_Delete));
	QMenu* editremove = edit->addMenu(tr("&Remove"));
	editremove->addAction(createAction(tr("Moves from the beginning"),
						SLOT(slotEditTruncateStart())));
	editremove->addAction(createAction(tr("Moves to the end"), SLOT(slotEditTruncateEnd()),
						Qt::SHIFT + Qt::Key_Delete));
	edit->addSeparator();
	edit->addAction(createAction(tr("Setup &position..."), SLOT(slotEditBoard()),
										  Qt::SHIFT + Qt::CTRL + Qt::Key_E));
	edit->addAction(createAction(tr("&Copy FEN"), SLOT(slotEditCopyFEN()),
					  Qt::CTRL + Qt::SHIFT + Qt::Key_C));
	edit->addAction(createAction(tr("&Paste FEN"), SLOT(slotEditPasteFEN()),
					  Qt::CTRL + Qt::SHIFT + Qt::Key_V));
	edit->addSeparator();
    edit->addAction(createAction(tr("&Copy PGN"), SLOT(slotEditCopyPGN()),
                      Qt::CTRL + Qt::Key_C));
    edit->addAction(createAction(tr("&Paste PGN"), SLOT(slotEditPastePGN()),
                      Qt::CTRL + Qt::Key_V));
    edit->addSeparator();
    edit->addAction(createAction(tr("&Copy Image"), SLOT(slotEditCopyImage()),
                      Qt::CTRL + Qt::ALT + Qt::Key_C));
    edit->addSeparator();
    edit->addAction(createAction(tr("&Preferences..."), SLOT(slotConfigure()), QKeySequence(),
                      QString(), QAction::PreferencesRole));


	/* View menu */
	m_menuView = menuBar()->addMenu(tr("&View"));
	m_menuView->addAction(createAction(tr("&Player information..."), SLOT(slotPlayerDialog()),
						Qt::CTRL + Qt::SHIFT + Qt::Key_P));

	/* Game menu */
	QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));

    QAction* flip = createAction(tr("&Flip board"), SLOT(slotConfigureFlip()), Qt::CTRL + Qt::Key_B);
    flip->setCheckable(true);
    gameMenu->addAction(flip);

    gameMenu->addSeparator();

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
	goMenu->addAction(createAction(tr("Variation"), SLOT(slotGameVarEnter()), Qt::CTRL + Qt::Key_Right));
	goMenu->addAction(createAction(tr("Back to main line"), SLOT(slotGameVarExit()), Qt::CTRL + Qt::Key_Left));

    gameMenu->addAction(createAction(tr("&New"), SLOT(slotGameNew()), QKeySequence::New));
    gameMenu->addAction(createAction(tr("&Save...."), SLOT(slotGameSave()), QKeySequence::Save));

	/* Search menu */
	QMenu* search = menuBar()->addMenu(tr("Fi&nd"));
	search->addAction(createAction(tr("Find &tag"), SLOT(slotSearchTag()), Qt::CTRL +
						 Qt::SHIFT + Qt::Key_T));
	search->addAction(createAction(tr("Find &position"), SLOT(slotSearchBoard()), Qt::CTRL +
						 Qt::SHIFT + Qt::Key_B));
	search->addSeparator();
    m_toggleFilter = createAction(tr("&Enable filter"),SLOT(slotToggleFilter()), Qt::CTRL + Qt::ALT + Qt::Key_F);
    m_toggleFilter->setCheckable(true);
    search->addAction(m_toggleFilter);
    search->addAction(createAction(tr("&Reset filter"), SLOT(slotSearchReset()),     Qt::CTRL + Qt::Key_F));
    search->addAction(createAction(tr("&Reverse filter"), SLOT(slotSearchReverse()), Qt::CTRL + Qt::SHIFT + Qt::Key_F));

	/* Database menu */
	QMenu* menuDatabase = menuBar()->addMenu(tr("&Database"));
	m_menuDatabases = menuDatabase->addMenu(tr("&Switch to"));
	menuDatabase->addAction(createAction(tr("&Copy games..."), SLOT(slotDatabaseCopy()),
						  Qt::Key_F5));

//	Hidden until functional
//	QMenu* menuRemove = menuDatabase->addMenu(tr("Delete"));
//	menuRemove->addAction(createAction(tr("&Current game"), SLOT(slotDatabaseDeleteGame())));
//	menuRemove->addAction(createAction(tr("&Games in filter"), SLOT(slotDatabaseDeleteFilter())));
//	menuDatabase->addAction(createAction(tr("&Compact"), SLOT(slotDatabaseCompact())));

	/* Help menu */
	menuBar()->addSeparator();
	QMenu *help = menuBar()->addMenu(tr("&Help"));

    /* Help Window */
    DockWidgetEx* pHelpDock = new DockWidgetEx(tr("Help"), this);
    pHelpDock->setObjectName("Help");
    HelpBrowser* pHelpBrowser = new HelpBrowser(this);
    pHelpBrowser->setMinimumSize(150, 100);
    pHelpDock->setWidget(pHelpBrowser);
    addDockWidget(Qt::RightDockWidgetArea, pHelpDock);
    help->addAction(pHelpDock->toggleViewAction());
    pHelpDock->toggleViewAction()->setShortcut(Qt::Key_F1);
    pHelpDock->hide();

    help->addAction(createAction(tr("&Report a bug..."), SLOT(slotHelpBug())));
	help->addSeparator();
	help->addAction(createAction(tr("&About ChessX"), SLOT(slotHelpAbout()), QString(), QString(), QAction::AboutRole));

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
	QString modified;
	for (int i = 1; i < m_databases.size(); i++)
		if (m_databases[i]->database()->isModified())
			modified += m_databases[i]->database()->name() + '\n';
	if (!modified.isEmpty()) {
		int response = MessageDialog::yesNoCancel(tr("Following databases are modified:")
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

void MainWindow::QuerySaveGame()
{
    if (game().isModified() && !database()->isReadOnly())
    {
        slotGameSave();
        game().setModified(false);
    }
}

void MainWindow::restoreRecentFiles()
{
    AppSettings->beginGroup("Favorites");
    QStringList list = AppSettings->value("Files").toStringList();
    QStringList attributes = AppSettings->value("Attributes").toStringList();
    AppSettings->endGroup();
    QStringList::iterator it = attributes.begin();
    foreach (QString s, list)
    {
        QString attribute = it != attributes.end() ? *it++ : "";
        m_databaseList->setFileFavorite(s,true);
        bool bUtf8 = (attribute.contains("utf8"));
        m_databaseList->setFileUtf8(s, bUtf8);
    }
}

