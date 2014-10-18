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
#include "chessbrowser.h"
#include "commentdialog.h"
#include "copydialog.h"
#include "databaseinfo.h"
#include "databaselist.h"
#include "dockwidgetex.h"
#include "downloadmanager.h"
#include "ecolistwidget.h"
#include "ecothread.h"
#include "eventlistwidget.h"
#include "exclusiveactiongroup.h"
#include "ficsclient.h"
#include "game.h"
#include "gamelist.h"
#include "GameMimeData.h"
#include "helpbrowser.h"
#include "kbaction.h"
#include "loadquery.h"
#include "mainwindow.h"
#include "messagedialog.h"
#include "memorydatabase.h"
#include "openingtreewidget.h"
#include "output.h"
#include "pgndatabase.h"
#include "playerlistwidget.h"
#include "quazip.h"
#include "quazipfile.h"
#include "savedialog.h"
#include "settings.h"
#include "style.h"
#include "tablebase.h"
#include "tableview.h"
#include "tagdialog.h"
#include "toolmainwindow.h"
#include "translatingslider.h"
#include "version.h"

#include <time.h>

#include <QApplication>
#include <QFileDialog>
#include <QLabel>
#include <QLCDNumber>
#include <QMenu>
#include <QMenuBar>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QProgressBar>
#include <QSizePolicy>
#include <QSplitter>
#include <QStatusBar>
#include <QTabBar>
#include <QTimer>
#include <QToolBar>

#undef FICS_SUPPORT

MainWindow::MainWindow() : QMainWindow(),
    m_tabDragIndex(-1),
    m_pDragTabBar(0),
    m_gameWindow(0),
    m_gameToolBar(0),
    m_output(0),
    m_currentFrom(InvalidSquare),
    m_currentTo(InvalidSquare),
    m_machineHasToMove(false),
    m_bInDrag(false),
    m_gameMode(false)
{
    setObjectName("MainWindow");

    // Style::setStyle(this);

#ifdef FICS_SUPPORT
    m_ficsClient = new FicsClient(this);
#endif

    m_messageTimer = new QTimer(this);
    m_messageTimer->setInterval(5000);
    m_messageTimer->setSingleShot(true);
    connect(m_messageTimer, SIGNAL(timeout()), this, SLOT(slotStatusMessage()));

    m_autoPlayTimer = new QTimer(this);
    m_autoPlayTimer->setInterval(AppSettings->getValue("/Board/AutoPlayerInterval").toInt());
    m_autoPlayTimer->setSingleShot(true);
    connect(m_autoPlayTimer, SIGNAL(timeout()), this, SLOT(slotAutoPlayTimeout()));

    m_dragTimer = new QTimer(this);
    m_dragTimer->setSingleShot(true);
    connect(m_dragTimer, SIGNAL(timeout()), this, SLOT(slotAutoSwitchTab()));

    /* Create clipboard database */
    DatabaseInfo* pClipDB = new DatabaseInfo(&m_undoGroup);
    connect(pClipDB,SIGNAL(signalRestoreState(Game)), SLOT(slotDbRestoreState(Game)));
    connect(pClipDB,SIGNAL(signalGameModified(bool)), SLOT(slotGameChanged()));
    connect(pClipDB,SIGNAL(signalMoveChanged()), SLOT(slotMoveChanged()));
    connect(pClipDB,SIGNAL(searchProgress(int)), SLOT(slotBoardSearchUpdate(int)), Qt::QueuedConnection);
    connect(pClipDB,SIGNAL(searchFinished()), SLOT(slotBoardSearchFinished()), Qt::QueuedConnection);
    connect(pClipDB,SIGNAL(signalGameModified(bool)), SIGNAL(signalGameModified(bool)));
    m_databases.append(pClipDB);
    m_currentDatabase = 0;

    /* Game List */
    DockWidgetEx* gameListDock = new DockWidgetEx(tr("Game List"), this);
    gameListDock->setObjectName("GameList");
    m_gameList = new GameList(databaseInfo()->filter(), gameListDock);

    /* Actions */
    setupActions();

    /* Delete on close */
    setAttribute(Qt::WA_DeleteOnClose);

    /* Board */
    m_boardSplitter = new QSplitter(Qt::Horizontal);
    m_boardSplitter->setChildrenCollapsible(false);
    setCentralWidget(m_boardSplitter);
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setObjectName("BoardView");
    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setElideMode(Qt::ElideMiddle);
    connect(m_tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(slotCloseBoardView(int)));
    connect(m_tabWidget, SIGNAL(tabBarClicked(int)), SLOT(slotActivateBoardView(int)));
    /* Board layout */
    m_boardSplitter->addWidget(m_tabWidget);

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
    for(int i = 0; i < 2; ++i)
    {
        QLCDNumber* annotatedTime = new QLCDNumber(m_gameToolBar);
        annotatedTime->setObjectName(QString("Clock") + QString::number(i));
        m_gameToolBar->addWidget(annotatedTime);
        annotatedTime->setDigitCount(7);
        annotatedTime->setSegmentStyle(QLCDNumber::Flat);
        annotatedTime->display("1:00:00");
        if(i == 0)
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
    m_gameView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_gameView->setMinimumSize(200, 200);
    m_gameView->slotReconfigure();
    connect(m_gameView, SIGNAL(anchorClicked(const QUrl&)), SLOT(slotGameViewLink(const QUrl&)));
    connect(m_gameView, SIGNAL(actionRequested(EditAction)), SLOT(slotGameModify(EditAction)));
    connect(m_gameView, SIGNAL(queryActiveGame(const Game**)), this, SLOT(slotGetActiveGame(const Game**)));
    connect(m_gameView, SIGNAL(signalMergeGame(int)), this, SLOT(slotMergeActiveGame(int)));
    connect(this, SIGNAL(displayTime(const QString&, Color)), m_gameView, SLOT(slotDisplayTime(const QString&, Color)));
    gameTextDock->setWidget(m_gameWindow);
    m_gameWindow->setCentralWidget(m_gameView);
    connect(this, SIGNAL(reconfigure()), m_gameView, SLOT(slotReconfigure()));
    addDockWidget(Qt::RightDockWidgetArea, gameTextDock);
    m_gameTitle = new QLabel;
    connect(m_gameTitle, SIGNAL(linkActivated(QString)), this, SLOT(slotGameViewLink(QString)));
    gameTextDock->setTitleBarWidget(m_gameTitle);

    /* Game List */
    m_gameList->setMinimumSize(150, 100);
    connect(m_gameList, SIGNAL(selected(int)), SLOT(slotFilterLoad(int)));
    connect(m_gameList, SIGNAL(requestCopyGame(QList<int>)), SLOT(slotDatabaseCopySingle(QList<int>)));
    connect(m_gameList, SIGNAL(requestMergeGame(QList<int>)), SLOT(slotMergeActiveGame(QList<int>)));
    connect(m_gameList, SIGNAL(requestMergeAllGames()), SLOT(slotMergeAllGames()));
    connect(m_gameList, SIGNAL(requestMergeFilter()), SLOT(slotMergeFilter()));
    connect(m_gameList, SIGNAL(requestDeleteGame(QList<int>)), SLOT(slotDatabaseDeleteGame(QList<int>)));
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
    connect(m_playerList, SIGNAL(renameRequest(QString)), SLOT(slotRenamePlayer(QString)));
    connect(m_playerList, SIGNAL(filterEcoPlayerRequest(QString, QString, QString)), m_gameList, SLOT(slotFilterListByEcoPlayer(QString, QString, QString)));
    connect(this, SIGNAL(databaseChanged(DatabaseInfo*)), m_playerList, SLOT(setDatabase(DatabaseInfo*)));
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
    connect(m_eventList, SIGNAL(renameRequest(QString)), SLOT(slotRenameEvent(QString)));
    connect(m_eventList, SIGNAL(filterEventPlayerRequest(QString, QString)), m_gameList, SLOT(slotFilterListByEventPlayer(QString, QString)));
    connect(m_eventList, SIGNAL(filterEventPlayerRequest(QString, QString)), m_playerList, SLOT(slotSelectPlayer(QString)));
    connect(this, SIGNAL(databaseChanged(DatabaseInfo*)), m_eventList, SLOT(setDatabase(DatabaseInfo*)));
    connect(this, SIGNAL(reconfigure()), m_eventList, SLOT(slotReconfigure()));
    eventListDock->hide();

    // ECO List
    DockWidgetEx* ecoListDock = new DockWidgetEx(tr("ECO"), this);
    ecoListDock->setObjectName("EcoList");
    m_ecoList = new ECOListWidget(this);
    m_ecoList->setMinimumSize(150, 100);
    ecoListDock->setWidget(m_ecoList);
    addDockWidget(Qt::RightDockWidgetArea, ecoListDock);
    m_menuView->addAction(ecoListDock->toggleViewAction());
    ecoListDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_E);
    connect(m_ecoList, SIGNAL(raiseRequest()), ecoListDock, SLOT(raise()));
    connect(m_ecoList, SIGNAL(filterRequest(QString)), m_gameList, SLOT(slotFilterListByEco(QString)));
    connect(m_ecoList, SIGNAL(filterEcoPlayerRequest(QString, QString, QString)), m_gameList, SLOT(slotFilterListByEcoPlayer(QString, QString, QString)));
    connect(m_ecoList, SIGNAL(filterEcoPlayerRequest(QString, QString)), m_playerList, SLOT(slotSelectPlayer(QString)));
    connect(this, SIGNAL(databaseChanged(DatabaseInfo*)), m_ecoList, SLOT(setDatabase(DatabaseInfo*)));
    connect(this, SIGNAL(reconfigure()), m_ecoList, SLOT(slotReconfigure()));
    ecoListDock->hide();

    // Database List
    DockWidgetEx* dbListDock = new DockWidgetEx(tr("Databases"), this);
    dbListDock->setObjectName("Databases");
    m_databaseList = new DatabaseList(this);
    m_databaseList->setMinimumSize(150, 100);
    dbListDock->setWidget(m_databaseList);
    addDockWidget(Qt::RightDockWidgetArea, dbListDock);
    m_menuView->addAction(dbListDock->toggleViewAction());
    dbListDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_D);
    connect(m_databaseList, SIGNAL(requestOpenDatabase(QString, bool)),
            this, SLOT(openDatabaseUrl(QString, bool)));
    connect(m_databaseList, SIGNAL(requestCloseDatabase(QString)),
            this, SLOT(slotFileCloseName(QString)));
    connect(m_databaseList, SIGNAL(requestLinkDatabase(QString)),
            this, SLOT(setFavoriteDatabase(QString)));
    connect(m_databaseList, SIGNAL(requestAppendGame(QString, int)),
            this, SLOT(copyGame(QString, int)));
    connect(m_databaseList, SIGNAL(requestAppendDatabase(QString, QString)),
            this, SLOT(copyDatabase(QString, QString)));
    connect(this, SIGNAL(reconfigure()), m_databaseList, SLOT(slotReconfigure()));
    connect(m_databaseList, SIGNAL(requestMakeBook(QString)),
            this, SLOT(slotMakeBook(QString)));
    m_databaseList->addFileOpen(QString(), false);
    m_databaseList->setFileCurrent(QString());
    restoreRecentFiles();
    connect(m_databaseList, SIGNAL(raiseRequest()), dbListDock, SLOT(raise()));

    /* Recent files */
    m_recentFiles.restore();
    m_recentFiles.removeMissingFiles();
    updateMenuRecent();

    /* Opening Tree */
    DockWidgetEx* openingDock = new DockWidgetEx(tr("Opening Tree"), this);
    openingDock->setObjectName("    Dock");
    m_openingTreeWidget = new OpeningTreeWidget(this);
    openingDock->setWidget(m_openingTreeWidget);
    addDockWidget(Qt::RightDockWidgetArea, openingDock);
    m_menuView->addAction(openingDock->toggleViewAction());
    connect(openingDock->toggleViewAction(), SIGNAL(triggered()), SLOT(slotSearchTree()));
    connect(openingDock, SIGNAL(visibilityChanged(bool)), m_openingTreeWidget, SLOT(cancel()));
    connect(m_openingTreeWidget, SIGNAL(signalTreeUpdated()), this, SLOT(slotTreeUpdate()));
    connect(m_openingTreeWidget, SIGNAL(signalSourceChanged()), this, SLOT(slotSearchTree()));
    connect(this, SIGNAL(reconfigure()), m_openingTreeWidget, SLOT(slotReconfigure()));
    openingDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_T);
    openingDock->hide();
    connect(this, SIGNAL(signalDatabaseOpenClose()), this, SLOT(slotUpdateOpeningTreeWidget()));

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
    connect(this, SIGNAL(signalGameLoaded()), analysis, SLOT(slotUciNewGame()));

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
            analysis, SLOT(slotVisibilityChanged(bool)));
    m_menuView->addAction(analysisDock2->toggleViewAction());
    analysisDock2->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_F3);
    analysisDock2->hide();
    connect(this, SIGNAL(signalGameLoaded()), analysis, SLOT(slotUciNewGame()));

    /* Randomize */
    srand(time(0));

    /* Restoring layouts */
    if(!AppSettings->layout(this))
    {
        resize(800, 600);
    }
    m_boardSplitter->restoreState(AppSettings->value("/MainWindow/BoardSplit").toByteArray());

    /* Status */
    m_statusFilter = new QLabel();
    statusBar()->addPermanentWidget(m_statusFilter,1);
    m_statusFilter->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    m_statusApp = new QLabel();
    statusBar()->addPermanentWidget(m_statusApp,1);
    m_statusApp->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QLabel* statusLabel = new QLabel();
    statusBar()->addPermanentWidget(statusLabel,1);
    connect(this, SIGNAL(enterText(QString)), statusLabel, SLOT(setText(QString)));
    statusLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    m_sliderSpeed = new TranslatingSlider(this);
    m_sliderSpeed->setMultiplier(1000);
    m_sliderSpeed->setOrientation(Qt::Horizontal);
    m_sliderSpeed->setMinimum(0);
    m_sliderSpeed->setMaximum(60);
    m_sliderSpeed->setTranslatedValue(AppSettings->getValue("/Board/AutoPlayerInterval").toInt());
    m_sliderSpeed->setTickInterval(1);
    m_sliderSpeed->setTickPosition(QSlider::NoTicks);
    m_sliderSpeed->setMaximumWidth(400);
    connect(m_sliderSpeed, SIGNAL(translatedValueChanged(int)), SLOT(slotMoveIntervalChanged(int)));
    connect(m_sliderSpeed, SIGNAL(translatedValueChanged(int)), m_mainAnalysis, SLOT(setMoveTime(int)));
    connect(m_sliderSpeed, SIGNAL(translatedValueChanged(int)), analysis, SLOT(setMoveTime(int)));
    connect(m_mainAnalysis, SIGNAL(receivedBestMove(const Analysis&)), this, SLOT(slotEngineTimeout(const Analysis&)));

    m_mainAnalysis->setMoveTime(m_sliderSpeed->translatedValue());
    analysis->setMoveTime(m_sliderSpeed->translatedValue());

    statusBar()->addPermanentWidget(new QLabel(tr("Move Interval:"), this));
    statusBar()->addPermanentWidget(m_sliderSpeed);
    m_sliderText = new QLabel(this);
    m_sliderText->setFixedWidth(100);
    statusBar()->addPermanentWidget(m_sliderText);
    connect(m_sliderSpeed, SIGNAL(valueChanged(int)), this, SLOT(slotSetSliderText(int)));
    slotSetSliderText(m_sliderSpeed->value());

    statusBar()->setFixedHeight(statusBar()->height());
    statusBar()->setSizeGripEnabled(true);
    m_progressBar = new QProgressBar();

    /* Reconfigure. */
    slotReconfigure();

    /* Very late as this will update other widgets */
    connect(this, SIGNAL(databaseModified()), SLOT(slotDatabaseModified()));
    connect(this, SIGNAL(signalDatabaseOpenClose()), this, SLOT(updateMenuDatabases()));
    CreateBoardView();

    /* Display main window */
    show();

    /* Load files from command line */
    QStringList args = qApp->arguments();
    for(int i = 1; i < args.count(); i++)
    {
        if(QFile::exists(args[i]))
        {
            openDatabaseUrl(args[i], false);
        }
    }

    qApp->installEventFilter(this);
    /* Activate clipboard */
    updateMenuDatabases();
    slotDatabaseChanged();
    emit signalGameIsEmpty(true);

    setAcceptDrops(true);

    QString dir = AppSettings->commonDataPath();
    QDir().mkpath(dir + "/index");

    /* Load ECO file */
    slotStatusMessage(tr("Loading ECO file..."));
    EcoThread* ecothread = new EcoThread();
    connect(ecothread, SIGNAL(loaded(QObject*, bool)), this, SLOT(ecoLoaded(QObject*, bool)));
    ecothread->start();
    StartCheckUpdate();
    downloadManager = new DownloadManager(this);
}

MainWindow::~MainWindow()
{
    m_autoPlayTimer->stop();
    m_dragTimer->stop();
    m_openingTreeWidget->cancel();

    foreach(DatabaseInfo * database, m_databases)
    {
        // Avoid any GUI action if a database is closed
        disconnect(database,SIGNAL(signalGameModified(bool)), this, SLOT(slotGameChanged()));
        database->close();
    }
    qDeleteAll(m_databases.begin(), m_databases.end());
    delete m_output;
    delete m_progressBar;
    m_boardViews.clear(); // Widgets are deleted by Qt
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::FileOpen)
    {
        openDatabaseUrl(static_cast<QFileOpenEvent*>(event)->file(), false);
        return true;
    }
    else
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent && (keyEvent->key() == Qt::Key_Escape ||
                            keyEvent->key() == Qt::Key_Return ||
                            keyEvent->key() == Qt::Key_Enter))
            {
                if (obj == this || obj == m_boardView || obj == m_gameView)
                {
                    keyPressEvent(keyEvent);
                    return true;
                }
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
    if(confirmQuit())
    {
        m_openingTreeWidget->cancel();

        m_recentFiles.save();
        m_databaseList->save();

        m_gameList->saveConfig();
        m_databaseList->saveConfig();
        m_openingTreeWidget->saveConfig();
        m_gameWindow->saveConfig();
        m_gameView->saveConfig();

        AppSettings->setLayout(this);
        AppSettings->beginGroup("/MainWindow/");
        AppSettings->setValue("BoardSplit", m_boardSplitter->saveState());
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
    evaluateSanNag(e);
    emit enterText(m_nagText);
}

void MainWindow::evaluateSanNag(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
    {
        m_nagText.clear();
        return;
    }

    if ((e->key() == Qt::Key_Backspace) && m_nagText.count())
    {
        m_nagText = m_nagText.left(m_nagText.count()-1);
        return;
    }

    if(e->key() == Qt::Key_Delete)
    {
        if(!game().atGameStart())
        {
            game().clearNags();
        }
        m_nagText.clear();
        return;
    }

    if(e->text().isEmpty())
    {
        return;
    }

    bool enterPressed = ((e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return));

    if(enterPressed)
    {
        // Try to figure out a SAN
        int matches = NagSet::prefixCount(m_nagText);
        if(matches >= 1) // enter forces the current nag
        {
            if(!game().atGameStart())
            {
                game().addNag(NagSet::fromString(m_nagText));
            }
            m_nagText.clear();
            return;
        }
        addVariation(m_nagText);
        m_nagText.clear();
        return;
    }
    else
    {
        m_nagText.append(e->text());
        if (addVariation(m_nagText))
        {
            m_nagText.clear();
            return;
        }
    }

    if (m_nagText == "N")
    {
        // Not a novelty but a Knight
        return;
    }

    if (m_nagText == "n")
    {
        m_nagText = "N";
    }

    int matches = NagSet::prefixCount(m_nagText);
    if(matches == 1)
    {
        if(!game().atGameStart())
        {
            if (!game().addNag(NagSet::fromString(m_nagText)))
            {
                return; // Could be start of NAG or a start of a move (R<->RR)
            }
        }
        m_nagText.clear();
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
    if(index < 0)
    {
        index = m_currentDatabase;
    }
    QString name = m_databases[index]->database() ? m_databases[index]->database()->name() : "";
    if(name.isEmpty())
    {
        return tr("[Clipboard]");
    }
    return name;
}

Database* MainWindow::getDatabaseByPath(QString path)
{
    if(DatabaseInfo* dbInfo = getDatabaseInfoByPath(path))
    {
        if(dbInfo->isValid())
        {
            return dbInfo->database();
        }
    }
    return 0;
}

DatabaseInfo* MainWindow::getDatabaseInfoByPath(QString path)
{
    for(int i = 0; i < m_databases.count(); ++i)
    {
        if(m_databases[i]->filePath() == path)
        {
            return m_databases[i];
        }
    }
    return 0;
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

void MainWindow::gameLoad(int index)
{
    if (index < 0) return;
    if(QuerySaveGame())
    {
        if(databaseInfo()->loadGame(index))
        {
            m_gameList->selectGame(index);
            emit signalGameIsEmpty(true);
            UpdateBoardInformation();
            slotGameChanged();
            m_gameList->setFocus();
            emit signalGameLoaded();
        }
    }
}

bool MainWindow::gameMoveBy(int change)
{
    if(!gameMode() && game().moveByPly(change))
    {
        m_gameView->setFocus();
        return true;
    }
    return false;
}

void MainWindow::updateMenuRecent()
{
    for(int i = 0; (i < m_recentFiles.count()) && (i < MaxRecentFiles); ++i)
    {
        m_recentFileActions[i]->setVisible(true);
        m_recentFileActions[i]->setText(QString("&%1: %2").arg(i + 1).arg(m_recentFiles[i]));
        m_recentFileActions[i]->setData(m_recentFiles[i]);
    }
    for(int i = m_recentFiles.count(); i < MaxRecentFiles; i++)
    {
        m_recentFileActions[i]->setVisible(false);
    }
}

void MainWindow::updateMenuDatabases()
{
    while(m_databases.count() > m_databaseActions.count())
    {
        QAction* action = new QAction(this);
        connect(action, SIGNAL(triggered()), SLOT(slotDatabaseChange()));
        m_databaseActions.append(action);
        m_menuDatabases->addAction(action);
    }
    int n = 1;
    for(int i = 0; i < m_databases.count(); i++)
    {
        if(m_databases[i]->isValid())
        {
            m_databaseActions[i]->setVisible(true);
            m_databaseActions[i]->setData(i);
            m_databaseActions[i]->setText(QString("&%1: %2").arg(i).arg(databaseName(i)));
            if(n < 10)
            {
                int key = Qt::CTRL + Qt::SHIFT + Qt::Key_1 + (n - 1);
                m_databaseActions[i]->setShortcut(key);
                ++n;
            }
        }
    }
    for(int i = m_databases.count(); i < m_databaseActions.count(); i++)
    {
        m_databaseActions[i]->setVisible(false);
        m_databaseActions[i]->setShortcut(0);
    }
}

void MainWindow::setFavoriteDatabase(QString fname)
{
    QUrl url = QUrl::fromUserInput(fname);
    if((url.scheme() == "http") || (url.scheme() == "ftp"))
    {
        m_databaseList->setFileFavorite(fname, true, 0);
    }
    else
    {
        m_databaseList->setFileFavorite(url.toLocalFile(), true, 0);
    }
}

void MainWindow::slotGameMoveFirst()
{
    gameMoveBy(-999);
}

void MainWindow::slotGameMovePrevious()
{
    gameMoveBy(-1);
}

void MainWindow::slotGameMoveLast()
{
    gameMoveBy(999);
}

void MainWindow::slotGameMoveNextN()
{
    gameMoveBy(10);
}

void MainWindow::slotGameMovePreviousN()
{
    gameMoveBy(-10);
}

void MainWindow::openDatabase(QString fname)
{
    openDatabaseUrl(fname, false);
}

void MainWindow::openDatabaseUrl(QString fname, bool utf8)
{
    QUrl url = QUrl::fromUserInput(fname);
    if((url.scheme() == "http") || (url.scheme() == "https") || (url.scheme() == "ftp"))
    {
        connect(downloadManager, SIGNAL(downloadError(QUrl)), this, SLOT(loadError(QUrl)));
        connect(downloadManager, SIGNAL(onDownloadFinished(QUrl, QString)), this, SLOT(loadReady(QUrl, QString)));
        downloadManager->doDownload(url);
        return;
    }

    openDatabaseArchive(url.toLocalFile(), utf8);
}

void MainWindow::openDatabaseArchive(QString fname, bool utf8)
{
    QFileInfo fi = QFileInfo(fname);
    if(fname.isEmpty() || fi.suffix().toLower() == "pgn" || fi.suffix().toLower() == "bin")
    {
        openDatabaseFile(fname, utf8);
    }
    else
    {
        QString dataPath = AppSettings->getUserDataPath();
        QString dir = AppSettings->value("/General/DefaultDataPath", dataPath).toString();

        fname = fi.canonicalFilePath();

        if(!fname.isEmpty())
        {
            QuaZip zip(fname);
            if(zip.open(QuaZip::mdUnzip))
            {
                // first, we need some information about archive itself
                QString comment = zip.getComment();
                // and now we are going to access files inside it
                QuaZipFile file(&zip);
                for(bool more = zip.goToFirstFile(); more; more = zip.goToNextFile())
                {
                    file.open(QIODevice::ReadOnly);
                    QString outName = dir + "/" + file.getActualFileName();
                    QDir pathOut;
                    outName = pathOut.absoluteFilePath(outName);
                    if(!QFile::exists(outName))
                    {
                        QDir().mkpath(dir);

                        QFile out(outName);
                        if(out.open(QIODevice::WriteOnly))
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
    for(int i = 0; i < m_databases.count(); i++)
    {
        if(m_databases[i]->database()->filename() == fname)
        {
            if(m_databases[i]->isValid())
            {
                if (!m_databases[i]->IsBook())
                {
                    if (i != m_currentDatabase)
                    {
                        m_currentDatabase = i;
                        m_databaseList->setFileCurrent(fname);
                        slotDatabaseChanged();
                    }
                    activateBoardViewForDbIndex(m_databases[m_currentDatabase]);
                }
            }
            else
            {
                slotStatusMessage(tr("Database %1 cannot be accessed at the moment.").arg(fi.fileName()));
            }
            return;
        }
    }

    // Create database, connect progress bar and open file
    DatabaseInfo* db = new DatabaseInfo(&m_undoGroup,fname);
    QString basefile = fi.completeBaseName();

    startOperation(tr("Opening %1...").arg(basefile));
    connect(db->database(), SIGNAL(progress(int)), SLOT(slotOperationProgress(int)));
    connect(db, SIGNAL(LoadFinished(DatabaseInfo*)), this, SLOT(slotDataBaseLoaded(DatabaseInfo*)));
    connect(db, SIGNAL(signalRestoreState(Game)), SLOT(slotDbRestoreState(Game)));
    connect(db, SIGNAL(signalGameModified(bool)), SLOT(slotGameChanged()));
    connect(db, SIGNAL(signalMoveChanged()), SLOT(slotMoveChanged()));
    connect(db, SIGNAL(searchProgress(int)), SLOT(slotBoardSearchUpdate(int)), Qt::QueuedConnection);
    connect(db, SIGNAL(searchFinished()), SLOT(slotBoardSearchFinished()), Qt::QueuedConnection);
    connect(db, SIGNAL(signalGameModified(bool)), SIGNAL(signalGameModified(bool)));
    if(!db->open(utf8))
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
    slotStatusMessage(tr("Database %1 cannot be accessed at the moment (%2).").arg(fi.fileName()).arg(url.errorString()));
}

void MainWindow::loadReady(QUrl /*url*/, QString fileName)
{
    openDatabaseArchive(fileName, false);
}

void MainWindow::slotDataBaseLoaded(DatabaseInfo* db)
{
    if(!db->IsLoaded())
    {
        cancelOperation(tr("Cannot open file"));
        m_databases.removeOne(db);
        delete db;
        return;
    }
    QString fname = db->filePath();
    QFileInfo fi = QFileInfo(fname);
    QString basefile = fi.completeBaseName();

    m_databaseList->addFileOpen(fname, db->IsUtf8());

    finishOperation(tr("%1 opened").arg(basefile));

    for(int i = 0; i < m_databases.count(); i++)
    {
        if(m_databases[i]->database()->filename() == fname)
        {
            if (!m_databases[i]->IsBook())
            {
                m_currentDatabase = i;
                CreateBoardView();
            }
        }
    }
    m_databaseList->setFileCurrent(fname);
    m_recentFiles.append(fname);

    updateMenuRecent();
    emit signalDatabaseOpenClose();

    int lastGameIndex = m_databaseList->getLastIndex(fname);
    gameLoad(lastGameIndex);

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
    if(fd.exec() != QDialog::Accepted)
    {
        return QString();
    }
#if QT_VERSION < 0x050000
    if(fd.selectedFilter().contains("*.tex"))
    {
        format = Output::Latex;
    }
    else if(fd.selectedFilter().contains("*.html"))
    {
        format = Output::Html;
    }
#else
    if(fd.selectedNameFilter().contains("*.tex"))
    {
        format = Output::Latex;
    }
    else if(fd.selectedNameFilter().contains("*.html"))
    {
        format = Output::Html;
    }
#endif
    else
    {
        format = Output::Pgn;
    }
    return fd.selectedFiles().first();
}

bool MainWindow::gameEditComment(Output::CommentType type)
{
    QString annotation;
    int moves;
    int comments;
    int nags;
    game().moveCount(&moves, &comments, &nags);

    if((type == Output::Precomment) || (moves <= 0))
    {
        annotation = game().annotation(CURRENT_MOVE, Game::BeforeMove);
    }
    else
    {
        annotation = game().annotation();
    }
    CommentDialog dlg(this);
    dlg.setText(annotation);
    if(!dlg.exec())
    {
        return false;
    }

    if((type == Output::Precomment) || (moves <= 0))
    {
        if(moves > 0)
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
        if (moves > 0)
        {
            game().setAnnotation(dlg.text());
        }
        else
        {
            game().setGameComment(dlg.text());
        }
    }
    return true;
}

QAction* MainWindow::createAction(QObject* parent, QString name, const char* slot, const QKeySequence& key, QToolBar* pToolBar, QString image,
                                  const QString& tip, QAction::MenuRole menuRole)
{
    return createAction(name, slot, key, pToolBar, image, tip, menuRole, parent);
}

QAction* MainWindow::createAction(QString name, const char* slot, const QKeySequence& key, QToolBar* pToolBar, QString image,
                                  const QString& tip, QAction::MenuRole menuRole, QObject* parent)
{
    return createAction(name, slot, key, pToolBar, QIcon(image), tip, menuRole, parent);
}

QAction* MainWindow::createAction(QString name, const char* slot, const QKeySequence& key, QToolBar* pToolBar, QIcon icon,
                                  const QString& tip, QAction::MenuRole menuRole, QObject* parent)
{
    KbAction* action;
    if (!parent)
    {
        parent = this;
    }

    if(icon.isNull())
    {
        action = new KbAction(name, parent, key);
    }
    else
    {
        action = new KbAction(icon, name, parent, key);
    }

    if(!tip.isEmpty())
    {
        action->setStatusTip(tip);
    }

    if(slot)
    {
        connect(action, SIGNAL(triggered()), slot);
    }
    action->setMenuRole(menuRole);
    if(pToolBar)
    {
        pToolBar->addAction(action);
    }
    return action;
}

void MainWindow::setupActions()
{
    /* File menu */
    QMenu* file = menuBar()->addMenu(tr("&File"));
    QToolBar* fileToolBar = addToolBar(tr("File"));
    fileToolBar->setObjectName("FileToolBar");
    file->addAction(createAction(tr("&New database..."), SLOT(slotFileNew()), QKeySequence(), fileToolBar, ":/images/new.png"));
    file->addAction(createAction(tr("&Open..."), SLOT(slotFileOpen()), QKeySequence::Open, fileToolBar, ":/images/folder_open.png"));
    file->addAction(createAction(tr("Open in UTF8..."), SLOT(slotFileOpenUtf8()), QKeySequence()));
    QMenu* menuRecent = file->addMenu(tr("Open &recent..."));

    for(int i = 0; i < MaxRecentFiles; ++i)
    {
        QAction* action = new QAction(this);
        action->setVisible(false);
        connect(action, SIGNAL(triggered()), SLOT(slotFileOpenRecent()));
        m_recentFileActions.append(action);
        menuRecent->addAction(action);
    }
    file->addSeparator();

    QAction* commitAction = createAction(tr("&Save"), SLOT(slotFileSave()), Qt::CTRL + Qt::SHIFT + Qt::Key_S, fileToolBar, ":/images/save.png");
    commitAction->setToolTip(tr("Commit Database to disk"));
    connect(this, SIGNAL(signalCurrentDBisReadWrite(bool)), commitAction, SLOT(setEnabled(bool)));
    file->addAction(commitAction);

    QMenu* exportMenu = file->addMenu(tr("&Export..."));
    connect(this, SIGNAL(signalCurrentDBhasGames(bool)), exportMenu, SLOT(setEnabled(bool)));
    exportMenu->addAction(createAction(exportMenu,tr("Current Game"), SLOT(slotFileExportGame())));
    exportMenu->addAction(createAction(exportMenu,tr("&Games in filter"), SLOT(slotFileExportFilter())));
    exportMenu->addAction(createAction(exportMenu,tr("&All games"), SLOT(slotFileExportAll())));

    QAction* saveImage = createAction(tr("Export Image"), SLOT(slotExportImage()));
    file->addAction(saveImage);
    file->addSeparator();

    QAction* closeFileAction = createAction(tr("&Close"), SLOT(slotFileClose()), QKeySequence::Close, fileToolBar, ":/images/folder.png");
    connect(this, SIGNAL(signalCurrentDBcanBeClosed(bool)), closeFileAction, SLOT(setEnabled(bool)));
    file->addAction(closeFileAction);

    file->addAction(createAction(tr("&Quit"), SLOT(slotFileQuit()), QKeySequence(), 0, QString(), QString(), QAction::QuitRole));

    /* Edit menu */
    QMenu* edit = menuBar()->addMenu(tr("&Edit"));
    QToolBar* editToolBar = addToolBar(tr("Edit"));
    editToolBar->setObjectName("EditToolBar");

    QAction *undoAction = m_undoGroup.createUndoAction(this, tr("Undo"));
    QAction *redoAction = m_undoGroup.createRedoAction(this, tr("Redo"));
    undoAction->setShortcut(Qt::CTRL + Qt::Key_Z);
    redoAction->setShortcut(Qt::CTRL + Qt::Key_Y);
    undoAction->setIcon(QIcon(":/images/undo.png"));
    redoAction->setIcon(QIcon(":/images/redo.png"));

    editToolBar->addAction(undoAction);
    editToolBar->addAction(redoAction);
    editToolBar->addSeparator();

    edit->addAction(undoAction);
    edit->addAction(redoAction);
    edit->addSeparator();

    QAction* commentAfter = createAction(tr("Comment"), SLOT(slotEditComment()),
                                         Qt::CTRL + Qt::Key_A, editToolBar, ":/images/edit_after.png");
    connect(this, SIGNAL(signalGameIsEmpty(bool)), commentAfter, SLOT(setDisabled(bool)));
    edit->addAction(commentAfter);
    QAction* commentBefore = createAction(tr("Comment Before"), SLOT(slotEditCommentBefore()),
                                          Qt::CTRL + Qt::ALT + Qt::Key_A);
    connect(this, SIGNAL(signalGameAtLineStart(bool)), commentBefore, SLOT(setEnabled(bool)));
    edit->addAction(commentBefore);

    QMenu* editVariation = edit->addMenu(tr("Variation"));

    QAction* promoteAction = createAction(editVariation, tr("Promote"), SLOT(slotEditVarPromote()), Qt::CTRL + Qt::Key_J, editToolBar, ":/images/format_indent_less.png");
    promoteAction->setToolTip(tr("Promote Variation"));
    connect(this, SIGNAL(signalMoveHasParent(bool)), promoteAction, SLOT(setEnabled(bool)));
    editVariation->addAction(promoteAction);

    QAction* removeVariationAction = createAction(editVariation, tr("Remove"), SLOT(slotEditVarRemove()),
                                     Qt::CTRL + Qt::Key_Delete, editToolBar, ":/images/edit_cut.png");
    removeVariationAction->setToolTip(tr("Remove Variation"));
    connect(this, SIGNAL(signalMoveHasParent(bool)), removeVariationAction, SLOT(setEnabled(bool)));
    editVariation->addAction(removeVariationAction);

    QMenu* editremove = edit->addMenu(tr("&Remove"));
    editremove->addAction(createAction(editremove, tr("Moves from the beginning"),
                                       SLOT(slotEditTruncateStart()), QKeySequence()));
    editremove->addAction(createAction(editremove, tr("Moves to the end"), SLOT(slotEditTruncateEnd()),
                                       Qt::SHIFT + Qt::Key_Delete));
    edit->addSeparator();
    QAction* setupBoard = createAction(tr("Setup &position..."),
                                       SLOT(slotEditBoard()), Qt::SHIFT + Qt::CTRL + Qt::Key_E,
                                       editToolBar, ":/images/setup_board.png");

    edit->addAction(setupBoard);
    edit->addSeparator();
    edit->addAction(createAction(tr("Copy PGN"), SLOT(slotEditCopyPGN()), Qt::CTRL + Qt::Key_C, editToolBar, ":/images/edit_copy.png"));
    edit->addAction(createAction(tr("Copy FEN"), SLOT(slotEditCopyFEN()), Qt::CTRL + Qt::SHIFT + Qt::Key_C));
    edit->addAction(createAction(tr("Copy Position"), SLOT(slotEditCopyHumanFEN())));
    edit->addAction(createAction(tr("Copy Image"), SLOT(slotEditCopyImage()),
                                 Qt::CTRL + Qt::ALT + Qt::Key_C, editToolBar, ":/images/camera.png"));
    edit->addSeparator();
    edit->addAction(createAction(tr("Paste into new game"), SLOT(slotEditPaste()),
                                 Qt::CTRL + Qt::SHIFT + Qt::Key_V));
    edit->addAction(createAction(tr("Paste"), SLOT(slotEditMergePGN()),
                                 Qt::CTRL + Qt::Key_V, editToolBar, ":/images/edit_paste.png"));
    edit->addSeparator();
    edit->addAction(createAction(tr("&Preferences..."), SLOT(slotConfigure()), QKeySequence(), 0,
                                 QString(), QString(), QAction::PreferencesRole));

    /* View menu */
    m_menuView = menuBar()->addMenu(tr("&View"));

    QMenu* toolbars = m_menuView->addMenu(tr("Toolbars"));
    m_menuView->addSeparator();

#if defined(Q_OS_WIN)
    QAction* stayOnTop = createAction(tr("Stay on Top"), SLOT(slotToggleStayOnTop()));
    stayOnTop->setCheckable(true);
    stayOnTop->setChecked(AppSettings->getValue("/MainWindow/StayOnTop").toBool());
    m_menuView->addAction(stayOnTop);
    m_menuView->addSeparator();
#else
    AppSettings->setValue("/MainWindow/StayOnTop", false);
#endif

    m_menuView->addAction(createAction(tr("Close current board"), SLOT(slotCloseBoardView()), Qt::CTRL + Qt::SHIFT + Qt::Key_W,
                                       0, style()->standardIcon(QStyle::SP_TitleBarCloseButton)));
    m_menuView->addSeparator();

    /* Game menu */
    QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));
    QToolBar* gameToolBar = addToolBar(tr("Game"));
    gameToolBar->setObjectName("GameToolBarMain");
    QToolBar* dbToolBar = addToolBar(tr("Database"));
    dbToolBar->setObjectName("DbToolBarMain");

    QAction* newAction = createAction(tr("&New"), SLOT(slotGameNew()), QKeySequence::New,
                                      dbToolBar, ":/images/new_game.png");
    gameMenu->addAction(newAction);
    connect(this, SIGNAL(signalGameIsEmpty(bool)), newAction, SLOT(setDisabled(bool)));

    QMenu* loadMenu = gameMenu->addMenu(tr("&Load"));

    /* Game->Load submenu */
    QAction * prevAction = createAction(tr("&Previous"), SLOT(slotGameLoadPrevious()), Qt::Key_F3,
                                        dbToolBar, ":/images/game_up.png");
    QAction * nextAction = createAction(tr("&Next"), SLOT(slotGameLoadNext()), Qt::Key_F4,
                                        dbToolBar, ":/images/game_down.png");
    connect(m_gameList, SIGNAL(signalLastGameLoaded(bool)), nextAction, SLOT(setDisabled(bool)));
    connect(m_gameList, SIGNAL(signalFirstGameLoaded(bool)), prevAction, SLOT(setDisabled(bool)));
    loadMenu->addAction(nextAction);
    loadMenu->addAction(prevAction);
    loadMenu->addAction(createAction(tr("&Go to game..."), SLOT(slotGameLoadChosen()), Qt::CTRL + Qt::Key_G));
    loadMenu->addAction(createAction(tr("&Random"), SLOT(slotGameLoadRandom()), Qt::CTRL + Qt::Key_Question));
    QAction* saveAction = createAction(tr("&Save..."), SLOT(slotGameSave()), QKeySequence::Save);
    connect(this, SIGNAL(signalGameModified(bool)), saveAction, SLOT(setEnabled(bool)));
    gameMenu->addAction(saveAction);

    gameMenu->addAction(createAction(tr("Edit tags..."), SLOT(slotGameEditTags())));

    gameMenu->addSeparator();

    QAction* flip = createAction(tr("&Flip board"), SLOT(slotConfigureFlip()), Qt::CTRL + Qt::Key_B, gameToolBar, ":/images/flip_board.png");
    flip->setCheckable(true);
    gameToolBar->addSeparator();

    ExclusiveActionGroup* autoGroup = new ExclusiveActionGroup(this);
    ExclusiveActionGroup* autoGroup2 = new ExclusiveActionGroup(this);

    m_match = createAction(tr("Match"), SLOT(slotToggleGameMode()), Qt::CTRL + Qt::Key_M, gameToolBar, ":/images/black_chess.png");
    m_match->setCheckable(true);
    autoGroup2->addAction(m_match);
    autoGroup->addAction(m_match);
    gameMenu->addAction(m_match );
    gameMenu->addSeparator();
    connect(m_match, SIGNAL(changed()), SLOT(slotToggleGameMode()));

    m_training = createAction(tr("Training"), SLOT(slotToggleTraining()), Qt::CTRL + Qt::Key_R, gameToolBar, ":/images/training.png");
    m_training->setCheckable(true);
    autoGroup2->addAction(m_training);
    gameMenu->addAction(m_training);

    m_autoRespond = createAction(tr("Auto Respond"), SLOT(slotToggleAutoRespond()), Qt::META + Qt::SHIFT + Qt::Key_R, gameToolBar, ":/images/respond.png");
    gameMenu->addAction(m_autoRespond);
    autoGroup->addAction(m_autoRespond);
    m_autoRespond->setCheckable(true);

    m_autoPlay = createAction(tr("Auto Player"), SLOT(slotToggleAutoPlayer()), Qt::CTRL + Qt::SHIFT + Qt::Key_R, gameToolBar, ":/images/replay.png");
    gameMenu->addAction(m_autoPlay);
    autoGroup->addAction(m_autoPlay);
    autoGroup2->addAction(m_autoPlay);
    m_autoPlay->setCheckable(true);

    m_autoAnalysis = createAction(tr("Auto Analysis"), SLOT(slotToggleAutoAnalysis()), Qt::CTRL + Qt::ALT + Qt::Key_R, gameToolBar, ":/images/annotate.png");
    gameMenu->addAction(m_autoAnalysis);
    autoGroup->addAction(m_autoAnalysis);
    autoGroup2->addAction(m_autoAnalysis);
    m_autoAnalysis->setCheckable(true);

    gameToolBar->addSeparator();
    gameMenu->addSeparator();

    gameMenu->addAction(flip);

    /* Game->Go to submenu */
    QMenu* goMenu = gameMenu->addMenu(tr("&Go to"));
    QAction* gotoFirstMove = createAction(goMenu, tr("&Start"), SLOT(slotGameMoveFirst()), Qt::Key_Home, gameToolBar, ":/images/first.png");
    gotoFirstMove->setToolTip(tr("Go to first move"));
    connect(this, SIGNAL(signalMoveHasPreviousMove(bool)), gotoFirstMove, SLOT(setEnabled(bool)));
    goMenu->addAction(gotoFirstMove);

    QAction* actionPrevMove = createAction(goMenu, tr("&Previous move"), SLOT(slotGameMovePrevious()), Qt::Key_Left, gameToolBar, ":/images/prev.png");
    connect(this, SIGNAL(signalMoveHasPreviousMove(bool)), actionPrevMove, SLOT(setEnabled(bool)));
    goMenu->addAction(actionPrevMove);

    QAction* actionNextMove = createAction(goMenu, tr("&Next move"), SLOT(slotGameMoveNext()), Qt::Key_Right, gameToolBar, ":/images/next.png");
    connect(this, SIGNAL(signalMoveHasNextMove(bool)), actionNextMove, SLOT(setEnabled(bool)));
    goMenu->addAction(actionNextMove);

    QAction* gotoLastMove = createAction(goMenu, tr("&End"), SLOT(slotGameMoveLast()), Qt::Key_End, gameToolBar, ":/images/last.png");
    gotoLastMove->setToolTip(tr("Go to last move"));
    connect(this, SIGNAL(signalMoveHasNextMove(bool)), gotoLastMove, SLOT(setEnabled(bool)));
    goMenu->addAction(gotoLastMove);

    QAction* fiveMovesForward = createAction(goMenu, tr("5 moves &forward"), SLOT(slotGameMoveNextN()), Qt::Key_Down);
    connect(this, SIGNAL(signalMoveHasNextMove(bool)), fiveMovesForward, SLOT(setEnabled(bool)));
    goMenu->addAction(fiveMovesForward);

    QAction* fiveMovesPrev = createAction(goMenu, tr("5 moves &backward"), SLOT(slotGameMovePreviousN()), Qt::Key_Up);
    connect(this, SIGNAL(signalMoveHasPreviousMove(bool)), fiveMovesPrev, SLOT(setEnabled(bool)));
    goMenu->addAction(fiveMovesPrev);

    QAction* enterVariation = createAction(goMenu, tr("Enter Variation"), SLOT(slotGameVarEnter()), Qt::CTRL + Qt::Key_Right, gameToolBar, ":/images/go_in.png");
    connect(this, SIGNAL(signalMoveHasVariation(bool)), enterVariation, SLOT(setEnabled(bool)));
    goMenu->addAction(enterVariation);

    QAction* prevVariation = createAction(goMenu, tr("Previous Variation"), SLOT(slotGameVarUp()), Qt::CTRL + Qt::Key_Up, gameToolBar, ":/images/go_up.png");
    connect(this, SIGNAL(signalVariationHasSibling(bool)), prevVariation, SLOT(setEnabled(bool)));
    goMenu->addAction(prevVariation);

    QAction* nextVariation = createAction(goMenu, tr("Next Variation"), SLOT(slotGameVarDown()), Qt::CTRL + Qt::Key_Down, gameToolBar, ":/images/go_down.png");
    connect(this, SIGNAL(signalVariationHasSibling(bool)), nextVariation, SLOT(setEnabled(bool)));
    goMenu->addAction(nextVariation);

    QAction* backToMainLine = createAction(goMenu, tr("Back to main line"), SLOT(slotGameVarExit()), Qt::CTRL + Qt::Key_Left, gameToolBar, ":/images/go_out.png");
    connect(this, SIGNAL(signalMoveHasParent(bool)), backToMainLine, SLOT(setEnabled(bool)));
    goMenu->addAction(backToMainLine);

    gameMenu->addSeparator();

    QMenu* refactorMenu = gameMenu->addMenu(tr("Refactor"));
    refactorMenu->addAction(createAction(refactorMenu, tr("Uncomment"), SLOT(slotGameUncomment())));
    refactorMenu->addAction(createAction(refactorMenu, tr("Remove Variations"), SLOT(slotGameRemoveVariations())));

    /* Search menu */
    QMenu* search = menuBar()->addMenu(tr("Fi&nd"));
    QToolBar* searchToolBar = addToolBar(tr("Search"));
    searchToolBar->setObjectName("SearchToolBar");

    QAction* actionFindTag = createAction(tr("Find tag..."), SLOT(slotSearchTag()), Qt::CTRL + Qt::SHIFT + Qt::Key_T, searchToolBar, ":/images/find_tag.png");
    connect(this, SIGNAL(signalCurrentDBhasGames(bool)), actionFindTag, SLOT(setEnabled(bool)));
    search->addAction(actionFindTag);

    QAction* actionFindBoard = createAction(tr("Find position..."), SLOT(slotSearchBoard()), Qt::CTRL + Qt::SHIFT + Qt::Key_B, searchToolBar, ":/images/find_pos.png");
    connect(this, SIGNAL(signalCurrentDBhasGames(bool)), actionFindBoard, SLOT(setEnabled(bool)));
    search->addAction(actionFindBoard);

    search->addSeparator();

    QAction* filterReset = createAction(tr("&Reset filter"), SLOT(slotSearchReset()), Qt::CTRL + Qt::Key_F, searchToolBar, ":/images/filter_reset.png");
    connect(this, SIGNAL(signalCurrentDBhasGames(bool)), filterReset, SLOT(setEnabled(bool)));
    search->addAction(filterReset);

    QAction* reverseFilter = createAction(tr("&Reverse filter"), SLOT(slotSearchReverse()), Qt::CTRL + Qt::SHIFT + Qt::Key_F, searchToolBar, ":/images/filter_rev.png");
    connect(this, SIGNAL(signalCurrentDBhasGames(bool)), reverseFilter, SLOT(setEnabled(bool)));
    search->addAction(reverseFilter);

    /* Database menu */
    QMenu* menuDatabase = menuBar()->addMenu(tr("&Database"));
    m_menuDatabases = menuDatabase->addMenu(tr("&Switch to"));
    menuDatabase->addAction(createAction(tr("&Copy games..."), SLOT(slotDatabaseCopy()), Qt::Key_F5));
    menuDatabase->addSeparator();
    menuDatabase->addAction(createAction(tr("Clear clipboard"), SLOT(slotDatabaseClearClipboard())));

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
    QAction* helpAction = pHelpDock->toggleViewAction();
    helpAction->setIcon(QIcon(":/images/help.png"));
    helpAction->setShortcut(Qt::Key_F1);
    help->addAction(helpAction);
    pHelpDock->hide();

    help->addAction(createAction(tr("Customize Keyboard..."), SLOT(slotEditActions())));
    help->addSeparator();
    help->addAction(createAction(tr("Load Sample Database"), SLOT(QueryLoadDatabase())));
    QAction* reportBugAction = createAction(tr("Report a bug..."), SLOT(slotHelpBug()));
    reportBugAction->setIcon(QIcon(":/images/bug.png"));
    help->addAction(reportBugAction);
    help->addSeparator();
    help->addAction(createAction(tr("&About ChessX"), SLOT(slotHelpAbout()), QString(), 0, QString(), QString(), QAction::AboutRole));

#ifdef QT_DEBUG
    QMenu* debug = help->addMenu("Debug");
    debug->addAction(createAction("Copy HTML", SLOT(slotGameViewSource())));
    debug->addAction(createAction("Dump Movenodes", SLOT(slotGameDumpMoveNodes())));
    debug->addAction(createAction("Make Screenshot", SLOT(slotScreenShot()), Qt::CTRL + Qt::Key_F12));
    debug->addAction(createAction("Compile ECO", SLOT(slotCompileECO())));
#endif

    fileToolBar->addAction(helpAction);
    toolbars->addAction(fileToolBar->toggleViewAction());
    toolbars->addAction(editToolBar->toggleViewAction());
    toolbars->addAction(dbToolBar->toggleViewAction());
    toolbars->addAction(gameToolBar->toggleViewAction());
    toolbars->addAction(searchToolBar->toggleViewAction());

    KbAction::restoreKeyboardLayoutForObject(this);
}

bool MainWindow::confirmQuit()
{
    QString modified;
    for(int i = 1; i < m_databases.size(); i++)
    {
        if (!QuerySaveGame(m_databases[i]))
        {
            return false;
        }
    }
    for(int i = 1; i < m_databases.size(); i++)
    {
        if(m_databases[i]->isValid() && m_databases[i]->database()->isModified())
        {
            modified += m_databases[i]->database()->name() + '\n';
        }
    }
    if(!modified.isEmpty())
    {
        int response = MessageDialog::yesNoCancel(tr("Following databases are modified:")
                       + '\n' + modified + tr("Save them?"));
        if(response == MessageDialog::Cancel)
        {
            return false;
        }
        if(response == MessageDialog::Yes)
        {
            Output output(Output::Pgn);
            for(int i = 1; i < m_databases.size(); i++)
                if(m_databases[i]->database()->isModified())
                    output.output(m_databases[i]->database()->filename(),
                                  *(m_databases[i]->database()));
        }
    }
    for(int i = 1; i < m_databases.size(); i++)
    {
        slotFileCloseIndex(i);
    }
    return true;
}

void MainWindow::startOperation(const QString& msg)
{
    m_operationTime.start();
    slotStatusMessage(msg);
    m_progressBar->setMaximumHeight(m_statusFilter->height() - 3);
    statusBar()->insertPermanentWidget(0, m_progressBar);
    m_progressBar->setValue(0);
    m_progressBar->show();
}

void MainWindow::finishOperation(const QString& msg)
{
    slotStatusMessage(msg + tr(" (%1 s.)").arg(m_operationTime.elapsed() / 100 / 10.0));
    statusBar()->removeWidget(m_progressBar);
}

void MainWindow::cancelOperation(const QString& msg)
{
    slotStatusMessage(msg);
    statusBar()->removeWidget(m_progressBar);
}

bool MainWindow::QuerySaveGame(DatabaseInfo *dbInfo)
{
    bool shouldNotify = false;
    if (!dbInfo)
    {
        shouldNotify = true;
        dbInfo = databaseInfo();
    }
    if(dbInfo->gameNeedsSaving())
    {
        SaveDialog dlg(this);
        int n = dlg.save(dbInfo->database(), dbInfo->currentGame());
        if(n == QDialog::Accepted)
        {
            saveGame(dbInfo);
        }
        else if(n == SaveDialog::Discard)
        {
            dbInfo->setModified(false, Game(), ""); // Do not notify more than once
        }

        if (shouldNotify)
        {
            emit databaseModified();
        }

        return (n != QDialog::Rejected);
    }
    return true;
}

void MainWindow::restoreRecentFiles()
{
    AppSettings->beginGroup("Favorites");
    QStringList list = AppSettings->value("Files").toStringList();
    QStringList attributes = AppSettings->value("Attributes").toStringList();
    QList<int> indexList;
    AppSettings->list("LastGameIndex", indexList);
    AppSettings->endGroup();

    QStringList::const_iterator it = attributes.begin();
    QList<int>::const_iterator it1 = indexList.begin();
    foreach(QString s, list)
    {
        QString attribute = it != attributes.end() ? *it++ : "";
        m_databaseList->setFileFavorite(s, true, *it1++);
        bool bUtf8 = (attribute.contains("utf8"));
        m_databaseList->setFileUtf8(s, bUtf8);
    }
}

void MainWindow::StartCheckUpdate()
{
    if(AppSettings->getValue("/General/onlineVersionCheck").toBool())
    {
        m_manager = new QNetworkAccessManager(this);
        connect(m_manager, SIGNAL(finished(QNetworkReply*)),
                SLOT(slotHttpDone(QNetworkReply*)));
        connect(this, SIGNAL(signalVersionFound(int, int, int)),
                SLOT(slotVersionFound(int, int, int)));
        QUrl url = QUrl(QString("http://chessx.sourceforge.net/versions/current.txt"));
        QNetworkRequest request(url);
        QByteArray userAgent = QString(QCoreApplication::applicationName() + "/" + STR_VERSION_NET).toLatin1();
        request.setRawHeader("User-Agent",userAgent);
        m_manager->get(request);
    }
}

void MainWindow::slotHttpDone(QNetworkReply *reply)
{
    QUrl url = reply->url();

    if (url.toString().endsWith("current.txt"))
    {
        if(!reply->error())
        {
            QString answer(reply->readAll());
            QRegExp rx("(\\d\\d?)\\.(\\d\\d?)\\.(\\d\\d?)");
            if(answer.indexOf(rx) > -1)
            {
                int major = rx.capturedTexts().at(1).toInt();
                int minor = rx.capturedTexts().at(2).toInt();
                int build = rx.capturedTexts().at(3).toInt();
                emit signalVersionFound(major, minor, build);
            }
        }
        reply->deleteLater();
        StartCheckDatabase();
    }
}

void MainWindow::QueryLoadDatabase()
{
    LoadQuery dlg(this);
    if(dlg.exec() == QDialog::Accepted)
    {
        slotStatusMessage(tr("Start loading database..."));
        if(dlg.largeDB())
        {
            openDatabaseUrl("http://chessx.sourceforge.net/db/bundesliga2000.pgn.zip", false);
        }
        else
        {
            openDatabaseUrl("http://chessx.sourceforge.net/db/SBL1213.pgn.zip", false);
        }
    }
    AppSettings->setValue("/General/BuiltinDbInstalled", QVariant(true));
}

void MainWindow::StartCheckDatabase()
{
    if ((m_recentFiles.count() == 0) && !AppSettings->getValue("/General/BuiltinDbInstalled").toBool())
    {
        QueryLoadDatabase();
    }
}

void MainWindow::slotVersionFound(int major, int minor, int build)
{
    int verInternet = major * 10000 + minor * 100 + build;
    int verCurrent = VERSION_MAJOR * 10000 + VERSION_MINOR * 100 + REVISION;
    if(verInternet > verCurrent)
    {
        slotStatusMessage(tr("A new version is available at chessx.sourceforge.net"));
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    const GameMimeData* gameMimeData = qobject_cast<const GameMimeData*>(mimeData);
    const DbMimeData* dbMimeData = qobject_cast<const DbMimeData*>(mimeData);
    if(mimeData->hasUrls() || gameMimeData || dbMimeData)
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    QTabBar* pTabBar = qobject_cast<QTabBar*>(childAt(event->pos()));
    if (pTabBar == NULL || pTabBar->parent() != this)
    {
        event->ignore();
        return;
    }

    QPoint globalPos = mapToGlobal(event->pos());
    QPoint widgetPos = pTabBar->mapFromGlobal(globalPos);

    int tabIndex = pTabBar->tabAt(widgetPos);
    if (tabIndex != m_tabDragIndex && pTabBar != m_pDragTabBar)
    {
        m_dragTimer->stop();
        m_tabDragIndex = tabIndex;
        m_pDragTabBar = pTabBar;
        m_dragTimer->start(500);
    }
    if (tabIndex == -1)
    {
        event->ignore();
    }
    else
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    m_dragTimer->stop();
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    m_dragTimer->stop();
    event->ignore();
}

void MainWindow::slotAutoSwitchTab()
{
    if (m_pDragTabBar && m_tabDragIndex != -1)
    {
        m_dragTimer->stop();
        m_pDragTabBar->setCurrentIndex(m_tabDragIndex);
        m_pDragTabBar = 0;
        m_tabDragIndex = -1;
    }
}

bool MainWindow::gameMode() const
{
    return m_gameMode;
}

void MainWindow::setGameMode(bool gameMode)
{
    m_gameMode = gameMode;
}
