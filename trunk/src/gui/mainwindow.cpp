/***************************************************************************
 *   (C) 2005-2009 by Michal Rudolf mrudolf@kdewebdev.org                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "analysiswidget.h"
#include "boardsetup.h"
#include "boardview.h"
#include "copydialog.h"
#include "chessbrowser.h"
#include "commentdialog.h"
#include "databaseinfo.h"
#include "databaselist.h"
#include "dockwidgetex.h"
#include "downloadmanager.h"
#include "ecothread.h"
#include "eventlistwidget.h"
#include "filtermodel.h"
#include "game.h"
#include "gamelist.h"
#include "helpbrowser.h"
#include "mainwindow.h"
#include "messagedialog.h"
#include "memorydatabase.h"
#include "openingtree.h"
#include "output.h"
#include "pgndatabase.h"
#include "playerlistwidget.h"
#include "preferences.h"
#include "quazip.h"
#include "quazipfile.h"
#include "savedialog.h"
#include "settings.h"
#include "tablebase.h"
#include "tableview.h"
#include "toolmainwindow.h"

#include <time.h>

#include <QFileDialog>
#include <QLabel>
#include <QLCDNumber>
#include <QMenu>
#include <QMenuBar>
#include <QProgressBar>
#include <QSizePolicy>
#include <QSplitter>
#include <QStatusBar>
#include <QTimer>
#include <QToolBar>

MainWindow::MainWindow() : QMainWindow(),
    m_saveDialog(0),
    m_gameWindow(0),
    m_gameToolBar(0),
    m_showPgnSource(false),
    m_autoPlayTimer(0),
    m_bGameChange(false),
    m_currentFrom(InvalidSquare),
    m_currentTo(InvalidSquare)
{
	setObjectName("MainWindow");

    m_autoPlayTimer = new QTimer(this);
    m_autoPlayTimer->setInterval(3000);
    m_autoPlayTimer->setSingleShot(true);
    connect(m_autoPlayTimer, SIGNAL(timeout()), this, SLOT(slotAutoPlayTimeout()));

	/* Create clipboard database */
	m_databases.append(new DatabaseInfo);
	m_currentDatabase = 0;
    m_prevDatabase = 0;

	/* Actions */
	m_actions = new QActionGroup(this);
	m_actions->setExclusive(false);
	setupActions();

	/* Delete on close */
	setAttribute(Qt::WA_DeleteOnClose);

	/* Output */
	m_output = new Output(Output::NotationWidget);

    if (AppSettings->getValue("/MainWindow/VerticalTabs").toBool())
    {
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::VerticalTabs);
    }
    else
    {
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AllowNestedDocks);
    }

	/* Board */
    m_boardSplitter = new QSplitter(Qt::Horizontal);
	m_boardSplitter->setChildrenCollapsible(false);
	setCentralWidget(m_boardSplitter);
	m_boardView = new BoardView(m_boardSplitter);
	m_boardView->setObjectName("BoardView");
	m_boardView->setMinimumSize(200, 200);
	m_boardView->resize(500, 540);
	connect(this, SIGNAL(reconfigure()), m_boardView, SLOT(configure()));
    connect(m_boardView, SIGNAL(moveMade(Square, Square, int)), SLOT(slotBoardMove(Square, Square, int)));
    connect(m_boardView, SIGNAL(clicked(Square, int, QPoint)), SLOT(slotBoardClick(Square, int, QPoint)));
	connect(m_boardView, SIGNAL(wheelScrolled(int)), SLOT(slotBoardMoveWheel(int)));

	/* Board layout */
	m_boardSplitter->addWidget(m_boardView);

	/* Game view */
    DockWidgetEx* gameTextDock = new DockWidgetEx(tr("Game Text"), this);
	gameTextDock->setObjectName("GameTextDock");
	gameTextDock->setFeatures(QDockWidget::NoDockWidgetFeatures);

    m_gameWindow = new ToolMainWindow(gameTextDock);
    m_gameWindow->setObjectName("GameWindow");
    connect(this, SIGNAL(reconfigure()), m_gameWindow, SLOT(slotReconfigure()));

    m_gameToolBar = new QToolBar(tr("Game Time"), m_gameWindow);
    m_gameToolBar->setObjectName("GameToolBar");
    m_gameToolBar->setMovable(false);
    m_gameWindow->addToolBar(Qt::BottomToolBarArea, m_gameToolBar);
    for (int i=0; i<2; ++i)
    {
        QLCDNumber* annotatedTime = new QLCDNumber(m_gameToolBar);
        annotatedTime->setObjectName(QString("Clock") + QString::number(i));
        m_gameToolBar->addWidget(annotatedTime);
        annotatedTime->setDigitCount(7);
        annotatedTime->display("1:00:00");
        if (i==0)
        {
            QWidget* spacer = new QWidget();
            spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            m_gameToolBar->addWidget(spacer);
        }
    }

    m_menuView->addAction(m_gameToolBar->toggleViewAction());
    m_gameToolBar->setVisible(AppSettings->getValue("/MainWindow/GameToolBar").toBool());
    m_gameView = new ChessBrowser(m_gameWindow, true);
    m_gameView->toolBar = m_gameToolBar;
    m_gameView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    m_gameView->setMinimumSize(200, 200);
	m_gameView->slotReconfigure();
	connect(m_gameView, SIGNAL(anchorClicked(const QUrl&)), SLOT(slotGameViewLink(const QUrl&)));
	connect(m_gameView, SIGNAL(actionRequested(EditAction)), SLOT(slotGameModify(EditAction)));
	connect(this, SIGNAL(databaseChanged(DatabaseInfo*)), m_gameView, SLOT(slotDatabaseChanged(DatabaseInfo*)));
    connect(this, SIGNAL(displayTime(const QString&, Color)), m_gameView, SLOT(slotDisplayTime(const QString&, Color)));
    gameTextDock->setWidget(m_gameWindow);
    m_gameWindow->setCentralWidget(m_gameView);
    connect(this, SIGNAL(reconfigure()), m_gameView, SLOT(slotReconfigure()));
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
    connect(m_gameList, SIGNAL(requestDeleteGame()), SLOT(slotDatabaseDeleteGame()));
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
    m_playerList = new PlayerListWidget(this);
    m_playerList->setMinimumSize(150, 100);
    playerListDock->setWidget(m_playerList);
    addDockWidget(Qt::RightDockWidgetArea, playerListDock);
    m_menuView->addAction(playerListDock->toggleViewAction());
    playerListDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_P);
    connect(m_playerList, SIGNAL(raiseRequest()), playerListDock, SLOT(raise()));
    connect(m_playerList, SIGNAL(filterRequest(QString)), m_gameList, SLOT(slotFilterListByPlayer(QString)));
    connect(m_playerList, SIGNAL(filterEcoPlayerRequest(QString,QString,QString)), m_gameList, SLOT(slotFilterListByEcoPlayer(QString, QString,QString)));
    connect(this, SIGNAL(reconfigure()), m_playerList, SLOT(slotReconfigure()));
    playerListDock->hide();

    // Event List
    DockWidgetEx* eventListDock = new DockWidgetEx(tr("Events"), this);
    eventListDock->setObjectName("EventList");
    m_eventList = new EventListWidget(this);
    m_eventList->setMinimumSize(150, 100);
    eventListDock->setWidget(m_eventList);
    addDockWidget(Qt::RightDockWidgetArea, eventListDock);
    m_menuView->addAction(eventListDock->toggleViewAction());
    eventListDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_P);
    connect(m_eventList, SIGNAL(raiseRequest()), eventListDock, SLOT(raise()));
    connect(m_eventList, SIGNAL(filterRequest(QString)), m_gameList, SLOT(slotFilterListByEvent(QString)));
    connect(m_eventList, SIGNAL(filterEventPlayerRequest(QString,QString)), m_gameList, SLOT(slotFilterListByEventPlayer(QString,QString)));
    connect(m_eventList, SIGNAL(filterEventPlayerRequest(QString,QString)), m_playerList, SLOT(slotSelectPlayer(QString)));
    connect(this, SIGNAL(reconfigure()), m_eventList, SLOT(slotReconfigure()));
    eventListDock->hide();

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
            this, SLOT(openDatabaseUrl(QString,bool)));
    connect(m_databaseList, SIGNAL(requestCloseDatabase(QString)),
            this, SLOT(slotFileCloseName(QString)));
    connect(m_databaseList, SIGNAL(requestLinkDatabase(QString)),
            this, SLOT(setFavoriteDatabase(QString)));
    connect(m_databaseList, SIGNAL(requestAppendGame(QString,const Game&)),
            this, SLOT(copyGame(QString,const Game&)));
    connect(this, SIGNAL(reconfigure()), m_databaseList, SLOT(slotReconfigure()));
    m_databaseList->addFileOpen(QString(), false);
    m_databaseList->setFileCurrent(QString());
    restoreRecentFiles();

    /* Recent files */
    m_recentFiles.restore();
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
    AnalysisWidget* analysis = new AnalysisWidget;
    analysis->setObjectName("Analysis");
    analysisDock->setWidget(analysis);
    addDockWidget(Qt::RightDockWidgetArea, analysisDock);
    connect(analysis, SIGNAL(addVariation(Analysis)),
			  SLOT(slotGameAddVariation(Analysis)));
    connect(analysis, SIGNAL(addVariation(QString)),
              SLOT(slotGameAddVariation(QString)));
    connect(this, SIGNAL(boardChange(const Board&)), analysis, SLOT(setPosition(const Board&)));
    connect(this, SIGNAL(reconfigure()), analysis, SLOT(slotReconfigure()));
	// Make sure engine is disabled if dock is hidden
	connect(analysisDock, SIGNAL(visibilityChanged(bool)),
              analysis, SLOT(slotVisibilityChanged(bool)));
    m_menuView->addAction(analysisDock->toggleViewAction());
    analysisDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_F2);
    analysisDock->hide();

    m_mainAnalysis = analysis;

	/* Analysis Dock 2 */
    DockWidgetEx* analysisDock2 = new DockWidgetEx(tr("Analysis 2"), this);
	analysisDock2->setObjectName("AnalysisDock2");
    analysis = new AnalysisWidget;
    analysis->setObjectName("Analysis2");
    analysisDock2->setWidget(analysis);
	addDockWidget(Qt::RightDockWidgetArea, analysisDock2);
    connect(analysis, SIGNAL(addVariation(Analysis)),
			  SLOT(slotGameAddVariation(Analysis)));
    connect(analysis, SIGNAL(addVariation(QString)),
              SLOT(slotGameAddVariation(QString)));
    connect(this, SIGNAL(boardChange(const Board&)), analysis, SLOT(setPosition(const Board&)));
    connect(this, SIGNAL(reconfigure()), analysis, SLOT(slotReconfigure()));
	// Make sure engine is disabled if dock is hidden
	connect(analysisDock2, SIGNAL(visibilityChanged(bool)),
              analysis,SLOT(slotVisibilityChanged(bool)));
	m_menuView->addAction(analysisDock2->toggleViewAction());
    analysisDock2->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_F3);
    analysisDock2->hide();

	/* Randomize */
	srand(time(0));

	/* Restoring layouts */
	if (!AppSettings->layout(this))
		resize(800, 600);
    AppSettings->beginGroup("/MainWindow/");
	m_boardSplitter->restoreState(AppSettings->value("BoardSplit").toByteArray());
    m_gameList->m_FilterActive = AppSettings->getValue("FilterFollowsGame").toBool();
	AppSettings->endGroup();
    m_toggleFilter->setChecked(m_gameList->m_FilterActive);

	/* Status */
    m_statusFilter = new QLabel();
    statusBar()->addPermanentWidget(m_statusFilter);
    statusBar()->setFixedHeight(statusBar()->height());
    statusBar()->setSizeGripEnabled(true);
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
            openDatabaseUrl(args[i], false);

	qApp->installEventFilter(this);
	/* Activate clipboard */
	updateMenuDatabases();
	slotDatabaseChanged();

    //QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + "/chessdata";
#if QT_VERSION < 0x050000
    QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + "/chessdata";
#else
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/chessdata";
#endif

    QString dir = AppSettings->value("/General/DefaultDataPath", dataPath).toString();
    QDir().mkpath(dir+"/index");

	/* Load ECO file */
	slotStatusMessage(tr("Loading ECO file..."));
    EcoThread* ecothread = new EcoThread();
    connect(ecothread, SIGNAL(loaded(QObject*,bool)), this, SLOT(ecoLoaded(QObject*,bool)));
    ecothread->start();
}

MainWindow::~MainWindow()
{
    m_autoPlayTimer->stop();
    m_openingTree->cancel(false);
    foreach (DatabaseInfo* database, m_databases) {
        database->close();
    }
	qDeleteAll(m_databases.begin(), m_databases.end());
	delete m_saveDialog;
	delete m_output;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FileOpen)
    {
        openDatabaseUrl(static_cast<QFileOpenEvent*>(event)->file(),false);
        return true;
    }
    else
    {
        if((obj == this) && (event->type() == QEvent::KeyPress))
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

void MainWindow::ecoLoaded(QObject* ecoThread, bool ok)
{
    slotStatusMessage(ok ? tr("ECO Loaded.") : tr("ECO Load Error."));
    qobject_cast<QThread*>(ecoThread)->wait();
    delete ecoThread;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    if (confirmQuit())
    {
        m_recentFiles.save();
        m_databaseList->save();

        m_gameList->saveConfig();
        m_databaseList->saveConfig();
        m_openingTreeView->saveConfig();
        m_gameWindow->saveConfig();
        m_gameView->saveConfig();

		AppSettings->setLayout(this);
        AppSettings->beginGroup("/MainWindow/");
		AppSettings->setValue("BoardSplit", m_boardSplitter->saveState());
        AppSettings->setValue("FilterFollowsGame", m_gameList->m_FilterActive);
        AppSettings->setValue("GameToolBar", m_gameToolBar->isVisible());
        AppSettings->endGroup();
    }
    else
    {
		e->ignore();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_Escape) || (e->key() == Qt::Key_Backspace))
    {
		m_nagText.clear();
        return;
    }

    if (game().atGameStart())
        return;

    if (e->key() == Qt::Key_Delete)
    {
        game().clearNags();
        slotGameChanged();
        return;
    }

    if (e->text().isEmpty())
    {
        return;
    }

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

const Game& MainWindow::game() const
{
    return databaseInfo()->currentGame();
}

int MainWindow::gameIndex() const
{
	return databaseInfo()->currentIndex();
}

void MainWindow::gameLoad(int index, bool force, bool reload)
{
    if (QuerySaveGame())
    {
        if (databaseInfo()->loadGame(index, reload))
            m_gameList->selectGame(index);
        else if (!force)
            return;
        else
            databaseInfo()->newGame();
        slotGameChanged();
    }
}

bool MainWindow::gameMoveBy(int change)
{
    if (game().moveByPly(change))
    {
        if (m_training->isChecked())
        {
            slotGameChanged();
        }
        else
        {
            slotMoveChanged();
        }
		m_gameView->setFocus();
        return true;
	}
    return false;
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
    QUrl url = QUrl::fromUserInput(fname);
    if ((url.scheme()=="http") || (url.scheme()=="ftp"))
    {
        m_databaseList->setFileFavorite(fname, true);
    }
    else
    {
        m_databaseList->setFileFavorite(url.toLocalFile(), true);
    }
}

void MainWindow::openDatabase(QString fname)
{
    openDatabaseUrl(fname, false);
}

void MainWindow::openDatabaseUrl(QString fname, bool utf8)
{
    if (QuerySaveGame())
    {
        QUrl url = QUrl::fromUserInput(fname);
        if ((url.scheme()=="http") || (url.scheme()=="ftp"))
        {
            DownloadManager* downloadManager = new DownloadManager(this);
            connect(downloadManager, SIGNAL(downloadError(QUrl)), this, SLOT(loadError(QUrl)));
            connect(downloadManager, SIGNAL(downloadFinished(QUrl,QString)), this, SLOT(loadReady(QUrl,QString)));
            downloadManager->doDownload(url);
            return;
        }

        openDatabaseArchive(url.toLocalFile(), utf8);
    }
}

void MainWindow::openDatabaseArchive(QString fname, bool utf8)
{
    QFileInfo fi = QFileInfo(fname);
    if (fname.isEmpty() || fi.suffix().toLower() == "pgn")
    {
        openDatabaseFile(fname, utf8);
    }
    else
    {
#if QT_VERSION < 0x050000
        QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + "/chessdata";
#else
        QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/chessdata";
#endif

        QString dir = AppSettings->value("/General/DefaultDataPath", dataPath).toString();

        fname = fi.canonicalFilePath();

        if (!fname.isEmpty())
        {
            QuaZip zip(fname);
            if (zip.open(QuaZip::mdUnzip))
            {
                // first, we need some information about archive itself
                QString comment=zip.getComment();
                // and now we are going to access files inside it
                QuaZipFile file(&zip);
                for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile())
                {
                    file.open(QIODevice::ReadOnly);
                    QString outName = dir + "/" + file.getActualFileName();
                    QDir pathOut;
                    outName = pathOut.absoluteFilePath(outName);
                    if (!QFile::exists(outName))
                    {
                        QDir().mkpath(dir);

                        QFile out(outName);
                        if (out.open(QIODevice::WriteOnly))
                        {
                            out.write(file.readAll());
                            out.close();
                            openDatabaseFile(outName, utf8);
                        }
                        else
                        {
                            qDebug() << "File Error: " << out.error();
                        }
                    }
                    else
                    {
                        openDatabaseFile(outName, utf8);
                    }

                    file.close();
                }
                zip.close();
            }
        }
    }
}

void MainWindow::openDatabaseFile(QString fname, bool utf8)
{
    QFileInfo fi = QFileInfo(fname);
    fname = fi.canonicalFilePath();

	/* Check if the database isn't already open */
	for (int i = 0; i < m_databases.count(); i++)
    {
        if (m_databases[i]->database()->filename() == fname)
        {
            if (m_databases[i]->isValid())
            {
                m_prevDatabase = m_currentDatabase;
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

void MainWindow::loadError(QUrl url)
{
    QFileInfo fi = QFileInfo(url.toString());
    slotStatusMessage(tr("Database %1 cannot be accessed at the moment.").arg(fi.fileName()));
}

void MainWindow::loadReady(QUrl /*url*/, QString fileName)
{
    openDatabaseArchive(fileName, false);
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
            m_prevDatabase = m_currentDatabase;
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
#if QT_VERSION < 0x050000
    fd.setFilters(filters);
#else
    fd.setNameFilters(filters);
#endif
	if (fd.exec() != QDialog::Accepted)
		return QString();
#if QT_VERSION < 0x050000
    if (fd.selectedFilter().contains("*.tex"))
		format = Output::Latex;
	else if (fd.selectedFilter().contains("*.html"))
		format = Output::Html;
#else
    if (fd.selectedNameFilter().contains("*.tex"))
        format = Output::Latex;
    else if (fd.selectedNameFilter().contains("*.html"))
        format = Output::Html;
#endif
	else format = Output::Pgn;
	return fd.selectedFiles().first();
}

bool MainWindow::gameEditComment(Output::CommentType type)
{
	QString annotation;
    int moves;
    int comments;
    int nags;
    game().moveCount(&moves,&comments,&nags);

    if ((type == Output::Precomment) || (moves <= 0))
    {
        if( moves > 0 )
        {
            annotation = game().annotation(CURRENT_MOVE, Game::BeforeMove);
        }
        else
        {
            annotation = game().gameComment();
        }
    }
    else
    {
        annotation = game().annotation();
    }
	CommentDialog dlg(this);
	dlg.setText(annotation);
	if (!dlg.exec())
		return false;

    if ((type == Output::Precomment) || (moves <= 0))
    {
        if( moves > 0 )
        {
            game().setAnnotation(dlg.text(), CURRENT_MOVE, Game::BeforeMove);
        }
        else
        {
            game().setGameComment(dlg.text());
        }
    }
    else
    {
        game().setAnnotation(dlg.text());
    }
	return true;
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
    edit->addAction(createAction(tr("Copy &human FEN"), SLOT(slotEditCopyHumanFEN())));
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

	/* Game menu */
	QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));

    gameMenu->addAction(createAction(tr("&New"), SLOT(slotGameNew()), QKeySequence::New));
    QMenu* loadMenu = gameMenu->addMenu(tr("&Load"));

    /* Game->Load submenu */
    loadMenu->addAction(createAction(tr("&First"), SLOT(slotGameLoadFirst()), Qt::CTRL + Qt::SHIFT + Qt::Key_Up));
    loadMenu->addAction(createAction(tr("&Last"), SLOT(slotGameLoadLast()), Qt::CTRL + Qt::SHIFT + Qt::Key_Down));
    loadMenu->addAction(createAction(tr("&Next"), SLOT(slotGameLoadNext()), Qt::CTRL + Qt::Key_Down));
    loadMenu->addAction(createAction(tr("&Previous"), SLOT(slotGameLoadPrevious()), Qt::CTRL + Qt::Key_Up));
    loadMenu->addAction(createAction(tr("&Go to game..."), SLOT(slotGameLoadChosen()), Qt::CTRL + Qt::Key_G));
    loadMenu->addAction(createAction(tr("&Random"), SLOT(slotGameLoadRandom()), Qt::CTRL + Qt::Key_Question));
    gameMenu->addAction(createAction(tr("&Save...."), SLOT(slotGameSave()), QKeySequence::Save));

    gameMenu->addSeparator();

    m_training = createAction(tr("Training"), SLOT(slotToggleTraining()), Qt::CTRL + Qt::Key_R);
    m_training->setCheckable(true);
    gameMenu->addAction(m_training);

    m_autoPlay = createAction(tr("Auto Player"), SLOT(slotToggleAutoPlayer()), Qt::CTRL + Qt::SHIFT + Qt::Key_R);
    m_autoPlay->setCheckable(true);
    gameMenu->addAction(m_autoPlay);
    m_autoAnalysis = createAction(tr("Auto Analysis"), SLOT(slotToggleAutoAnalysis()), Qt::CTRL + Qt::ALT + Qt::Key_R);
    m_autoAnalysis->setCheckable(true);
    gameMenu->addAction(m_autoAnalysis);

    gameMenu->addSeparator();

    QAction* flip = createAction(tr("&Flip board"), SLOT(slotConfigureFlip()), Qt::CTRL + Qt::Key_B);
    flip->setCheckable(true);
    gameMenu->addAction(flip);

	/* Game->Go to submenu */
	QMenu* goMenu = gameMenu->addMenu(tr("&Go to"));
	goMenu->addAction(createAction(tr("&Start"), SLOT(slotGameMoveFirst()), Qt::Key_Home));
	goMenu->addAction(createAction(tr("&End"), SLOT(slotGameMoveLast()), Qt::Key_End));
	goMenu->addAction(createAction(tr("&Next move"), SLOT(slotGameMoveNext()), Qt::Key_Right));
	goMenu->addAction(createAction(tr("&Previous move"), SLOT(slotGameMovePrevious()), Qt::Key_Left));
	goMenu->addAction(createAction(tr("5 moves &forward"), SLOT(slotGameMoveNextN()), Qt::Key_Down));
	goMenu->addAction(createAction(tr("5 moves &backward"), SLOT(slotGameMovePreviousN()), Qt::Key_Up));
    goMenu->addAction(createAction(tr("Enter Variation"), SLOT(slotGameVarEnter()), Qt::CTRL + Qt::Key_Right));
	goMenu->addAction(createAction(tr("Back to main line"), SLOT(slotGameVarExit()), Qt::CTRL + Qt::Key_Left));

    gameMenu->addSeparator();

    QMenu* refactorMenu = gameMenu->addMenu(tr("Refactor"));
    refactorMenu->addAction(createAction(tr("Uncomment"), SLOT(slotGameUncomment())));
    refactorMenu->addAction(createAction(tr("Remove Variations"), SLOT(slotGameRemoveVariations())));

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
    debug->addAction(createAction("Dump Movenodes", SLOT(slotGameDumpMoveNodes())));
#endif


}

bool MainWindow::confirmQuit()
{
	QString modified;
    if (m_currentDatabase)
    {
        if (!QuerySaveGame())
        {
            return false;
        }
    }
	for (int i = 1; i < m_databases.size(); i++)
		if (m_databases[i]->database()->isModified())
			modified += m_databases[i]->database()->name() + '\n';
	if (!modified.isEmpty()) {
		int response = MessageDialog::yesNoCancel(tr("Following databases are modified:")
					+ '\n' + modified + tr("Save them?"));
		if (response == MessageDialog::Cancel)
        {
			return false;
        }
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
    m_progressBar->setMaximumHeight(m_statusFilter->height()-3);
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

bool MainWindow::QuerySaveGame()
{
    if (game().isModified() && !database()->isReadOnly())
    {
        return slotGameSave();
    }
    return true;
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

