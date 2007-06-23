/***************************************************************************
                          mainwindow.cpp  -  main window
                             -------------------
    begin                : 10 Oct 2005
    copyright            : (C) 2005-2007 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "boardsetup.h"
#include "boardview.h"
#include "chessbrowser.h"
#include "databaseinfo.h"
#include "ecothread.h"
#include "filtermodel.h"
#include "game.h"
#include "gamelist.h"
#include "helpwindow.h"
#include "mainwindow.h"
#include "memorydatabase.h"
#include "openingtree.h"
#include "output.h"
#include "pgndatabase.h"
#include "playerdatabase.h"
#include "playerdialog.h"
#include "preferences.h"
#include "savedialog.h"
#include "settings.h"
#include "tablebase.h"
#include "tableview.h"
#include "tipoftheday.h"
#include "analysiswidget.h"

#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDockWidget>
#include <QFileDialog>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>
#include <QStatusBar>
#include <QTime>



MainWindow::MainWindow() : QMainWindow(),
		m_playerDialog(0), m_saveDialog(0), m_helpWindow(0), m_tipDialog(0),
		m_showPgnSource(false)
{
	setObjectName("MainWindow");
	/* Active database */
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
	m_output = new Output(Output::NotationWidget,
			      AppSettings->dataPath() + "/templates/notation-test.template");

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
	//m_moveView->zoomOut();
	m_moveView->setMinimumHeight(80);
	connect(m_moveView, SIGNAL(anchorClicked(const QUrl&)), SLOT(slotGameViewLink(const QUrl&)));

	/* Board layout */
	m_boardSplitter->addWidget(m_boardView);
	m_boardSplitter->addWidget(m_moveView);

	/* Game view */
	QDockWidget* dock = new QDockWidget(tr("Game Text"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dock->setObjectName("GameTextDock");
	m_gameView = new ChessBrowser(dock);
	m_gameView->setMinimumSize(150, 100);
	connect(m_gameView, SIGNAL(anchorClicked(const QUrl&)), SLOT(slotGameViewLink(const QUrl&)));
	dock->setWidget(m_gameView);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	m_menuView->addAction(dock->toggleViewAction());
	dock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_P);

	/* Game List */
	dock = new QDockWidget(tr("Game List"), this);
	dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
	dock->setObjectName("GameList");
	dock->setFloating(true);
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
	m_openingTree = new TableView(dock);
	m_openingTree->setObjectName("OpeningTree");
	m_openingTree->setMinimumSize(150, 100);
	m_openingTree->setSortingEnabled(true);
	m_openingTree->setModel(new OpeningTree);
	m_openingTree->sortByColumn(1, Qt::DescendingOrder);
	m_openingTree->slotReconfigure();
	connect(m_openingTree, SIGNAL(clicked(const QModelIndex&)),
			  SLOT(slotSearchTreeMove(const QModelIndex&)));
	dock->setWidget(m_openingTree);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	m_menuView->addAction(dock->toggleViewAction());
	connect(dock->toggleViewAction(), SIGNAL(triggered()), SLOT(slotSearchTree()));
	dock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_T);

	/* Analysis Dock */
	dock = new QDockWidget(tr("Analysis"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dock->setObjectName("AnalysisDock");
	m_analysis = new AnalysisWidget();
	m_analysis->setMinimumSize(150, 100);
	dock->setWidget(m_analysis);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	QAction *action = dock->toggleViewAction();
	m_menuView->addAction(action);
	connect(this, SIGNAL(boardChange(const Board&)), m_analysis, SLOT(setPosition(const Board&)));
	connect(this, SIGNAL(reconfigure()), m_analysis, SLOT(slotReconfigure()));
	// Make sure engine is disabled if dock is hidden
	connect(action, SIGNAL(toggled(bool)), m_analysis, SLOT(setShown(bool)));
	dock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_A);

	/* Randomize */
	srand(time(0));

	/* Restoring layouts */
	if (!AppSettings->layout(this))
		resize(800, 600);
	AppSettings->beginGroup("MainWindow");
	m_boardSplitter->restoreState(AppSettings->value("BoardSplit").toByteArray());
	AppSettings->endGroup();
	slotReconfigure();

	/* Reset board - not earlier, as all widgets have to be created. */
	slotGameChanged();

	/* Status */
	m_statusFilter = new QLabel(statusBar());
	statusBar()->addPermanentWidget(m_statusFilter);

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
}

MainWindow::~MainWindow()
{
	qDeleteAll(m_databases.begin(), m_databases.end());
	delete m_saveDialog;
	delete m_playerDialog;
	delete m_helpWindow;
	delete m_output;
	delete m_tipDialog;
	delete m_tablebase;
	delete m_analysis;
}

void MainWindow::ecoLoaded()
{
	qApp->restoreOverrideCursor();
	slotStatusMessage(tr("ECO Loaded."));
	m_ecothread->wait();
	delete m_ecothread;
	m_ecothread = NULL;
}

bool MainWindow::confirm(const QString& title, const QString& question,
			 const QString& proceed)
{
	QMessageBox mb(this);
	mb.setWindowTitle(title);
	mb.setText(question);
	mb.setIcon(QMessageBox::Question);
	QPushButton* okButton = mb.addButton(proceed, QMessageBox::ActionRole);
	mb.addButton(QMessageBox::Cancel);
	mb.exec();
	return mb.clickedButton() == okButton;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
	if (!AppSettings->value("/General/confirmQuit", true).toBool() ||
			confirm(tr("Quit"), tr("Do you want to quit?"))) {
		m_recentFiles.save("History", "RecentFiles");
		AppSettings->setLayout(m_playerDialog);
		AppSettings->setLayout(m_helpWindow);
		m_gameList->saveConfig();
		m_openingTree->saveConfig();
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
	QString name = m_databases[index]->name();
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

void MainWindow::gameLoad(int index, bool force)
{
	if (index >= 0 && index < database()->count()) {
		databaseInfo()->loadGame(index);
		m_gameList->selectGame(index);
	}
	else if (!force)
		return;
	else
		databaseInfo()->newGame();
	slotGameChanged();
}

void MainWindow::gameMoveBy(int change)
{
	if (game().moveByPly(change))
		slotMoveChanged();
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

	QTime time;
	time.start();
	m_databases.append(new DatabaseInfo(fname));
	m_currentDatabase = m_databases.count() - 1;
	m_recentFiles.append(fname);
	updateMenuRecent();
	updateMenuDatabases();
	slotStatusMessage(tr("Database %1 opened successfully (%2 seconds).")
			  .arg(fname.section('/', -1)).arg((time.elapsed() / 100 / 10.0)));
	slotDatabaseChanged();
	return true;
}

PlayerDialog* MainWindow::playerDialog()
{
	if (!m_playerDialog) {
		PlayerDatabase* db = new PlayerDatabase;
		QString path = AppSettings->dataPath() + "/players";
		//if (!db->open(path)); Bug in PlayerDatabase - always returns false
		if (!QFile::exists(path + ".cpm"))
			QMessageBox::information(0, "No player database",
						 QString("Player database was not found at\n%1").arg(path));
		else
			db->open(path);
		m_playerDialog = new PlayerDialog(db, this);
		AppSettings->layout(m_playerDialog);
	}
	return m_playerDialog;
}

SaveDialog* MainWindow::saveDialog()
{
	if (!m_saveDialog)
		m_saveDialog = new SaveDialog;
	return m_saveDialog;
}

HelpWindow* MainWindow::helpWindow()
{
	if (!m_helpWindow) {
		m_helpWindow = new HelpWindow();
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


void MainWindow::slotFileOpen()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open database"),
			AppSettings->value("/General/databasePath").toString(),
			tr("PGN Database (*.pgn)"));
	if (!file.isEmpty()) {
		AppSettings->setValue("/General/databasePath", QFileInfo(file).absolutePath());
		openDatabase(file);
	}
}

void MainWindow::slotFileOpenRecent()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		openDatabase(action->data().toString());
}


void MainWindow::slotFileSave()
{
	if (m_currentDatabase && dynamic_cast<MemoryDatabase*>(database())) {
		Output output(Output::Pgn);
		output.output(database()->filename(), *database());
		slotStatusMessage(tr("Database %1 successfully saved.")
				.arg(database()->filename().section('/', -1)));
	}
}

void MainWindow::slotFileClose()
{
	if (m_currentDatabase) {// Don't remove Clipboard
		m_databases.removeAt(m_currentDatabase);
		if (m_currentDatabase == m_databases.count())
			m_currentDatabase--;
		updateMenuDatabases();
		slotDatabaseChanged();
	}
}

void MainWindow::slotFileExportFilter()
{
	QString file = QFileDialog::getSaveFileName(this, tr("Export games"), QString(),
															  tr("PGN Database (*.pgn)"));
	if (!file.isEmpty()) {
		Output output(Output::Pgn);
		output.output(file, *databaseInfo()->filter());
	}
}

void MainWindow::slotFileExportAll()
{
	QString file = QFileDialog::getSaveFileName(this, tr("Export games"), QString(),
															  tr("PGN Database (*.pgn)"));
	if (!file.isEmpty()) {
		Output output(Output::Pgn);
		output.output(file, *database());
	}
}

void MainWindow::slotFileQuit()
{
	qApp->closeAllWindows();
}

void MainWindow::slotPlayerDialog()
{
	playerDialog()->show();
}

void MainWindow::slotConfigure()
{
	PreferencesDialog P;
	connect(&P, SIGNAL(reconfigure()), SLOT(slotReconfigure()));
	P.exec();
}

void MainWindow::slotReconfigure()
{
	// Re-emit for children
	emit reconfigure();
}

void MainWindow::slotConfigureFlip()
{
	m_boardView->setFlipped(!m_boardView->isFlipped());
}

void MainWindow::slotEditCopyFEN()
{
	QApplication::clipboard()->setText(game().toFen());
}

void MainWindow::slotEditPasteFEN()
{
	QString fen = QApplication::clipboard()->text().trimmed();
	if (!game().board().isValidFen(fen)) {
		QString msg = fen.length() ?
			      tr("Text in clipboard does not represent valid FEN:<br><i>%1</i>").arg(fen) :
			      tr("There is no text in clipboard.");
		QMessageBox::warning(0, "Paste FEN", msg);
		return;
	}
	game().setStartBoard(fen);
	slotGameChanged();
}

void MainWindow::slotEditTruncateEnd()
{
	game().truncateGameEnd();
	slotGameChanged();
}

void MainWindow::slotEditTruncateStart()
{
	game().truncateGameStart();
	slotGameChanged();
}

void MainWindow::slotEditBoard()
{
	BoardSetupDialog B;
	B.setBoard(game().board());
	B.setFlipped(m_boardView->isFlipped());
	if (B.exec() == QDialog::Accepted) {
		game().setStartBoard(B.board());
		slotGameChanged();
	}
}


void MainWindow::slotHelp()
{
	helpWindow()->show();
}

void MainWindow::slotHelpTip()
{
	tipDialog()->show();
}

void MainWindow::slotHelpAbout()
{
	QString fastbits = Board::fastbitsOption ? tr("Compiled with 'fastbits' option<br>") : tr("");
	QMessageBox dialog(tr(""), tr("<h1>ChessX</h1>"
				      "<p>Free chess database.<br>Version %1<br>%2"
				      "<p>Copyright 2005-2007 ChessX developers<br>"
				      "Core developers: Sean Estabrooks, Marius Roets, Michal Rudolf.<br>"
				      "Additional coding: Ejner Borgbjerg, Heinz Hopfgartner, William Hoggarth.<br>"
				      "<p>Homepage: <a href=\"http://chessx.sf.net\">http://chessx.sf.net</a><br>"
				      "Mailing list: <a href=\"mailto:chessx-users@lists.sourceforge.net\">"
				      "chessx-users@lists.sourceforge.net").arg(ChessXVersion).arg(fastbits),
			   QMessageBox::NoIcon, QMessageBox::Ok, Qt::NoButton, Qt::NoButton, this);
	dialog.exec();
}

void MainWindow::slotHelpBug()
{
	QDesktopServices::openUrl(QUrl("http://sourceforge.net/tracker/?group_id=163833&atid=829300"));
}


void MainWindow::slotBoardMove(Square from, Square to)
{
	const Board& board = game().board();
	Move m(board.prepareMove(from, to));
	if (m.isLegal()) {
		if (m.isPromotion()) {
			bool ok;
			QStringList moves;
			moves << tr("Queen") << tr("Rook") << tr("Bishop") << tr("Knight");
			int index = moves.indexOf(QInputDialog::getItem(0, tr("Promotion"), tr("Promote to:"),
						  moves, 0, false, &ok));
			if (!ok)
				return;
			m.setPromotionPiece(PieceType(Queen + index));
		}
		if (game().atEnd())
			game().addMove(m);
		else {
			// Find how way we should add new move
			QMessageBox mbox(QMessageBox::Question, tr("Add move"),
					 tr("There is already next move in current game. What do you want to do?"));
			QPushButton* addVar = mbox.addButton(tr("Add variation"), QMessageBox::YesRole);
			QPushButton* newMain = mbox.addButton(tr("Add new mainline"), QMessageBox::AcceptRole);
			QPushButton* replaceMain = mbox.addButton(tr("Replace current move"), QMessageBox::DestructiveRole);
			mbox.addButton(tr("Cancel"), QMessageBox::RejectRole);
			mbox.exec();
			if (mbox.clickedButton() == addVar)
				game().enterVariation(game().addMove(m));
			else if (mbox.clickedButton() == newMain)
				game().promoteVariation(game().addMove(m));
			else if (mbox.clickedButton() == replaceMain)
				game().replaceMove(m);
			else return;
		}
		game().forward();
		slotGameChanged();
	}
}

void MainWindow::slotBoardClick(Square, int button)
{
	if (button != Qt::RightButton)
		return;
	bool remove = game().atEnd();
	int var = game().currentVariation();
	gameMoveBy(-1);
	if (remove)
	{
		if (var && game().isMainline())
			game().removeVariation(var);
		else
			game().truncateGameEnd();
		slotGameChanged();
	}
}

void MainWindow::slotMoveChanged()
{
	Game& g = game();

	// Set board first
	m_tablebase->abortLookup();
	m_boardView->setBoard(g.board());

	// Highlight current move
	m_gameView->showMove(g.currentMoveId());

	// Finally update game information
	QString white = g.tag("White");
	QString black = g.tag("Black");
	QString eco = m_eco.isNull() ? g.tag("ECO") : m_eco;
	if (!eco.isEmpty()) {
		int comma = eco.lastIndexOf(',');
		if (comma != -1 && eco.at(comma + 2).isNumber())
			eco.truncate(comma);
	}
	QString whiteElo = g.tag("WhiteElo");
	QString blackElo = g.tag("BlackElo");
	if (whiteElo == "?")
		whiteElo = QString();
	if (blackElo == "?")
		blackElo = QString();
	QString players = tr("Game %1: <b><a href=\"tag:white\">%2</a> %3 - <a href=\"tag:black\">%4</a> %5</b>")
			  .arg(gameIndex() + 1).arg(white).arg(whiteElo).arg(black).arg(blackElo);
	QString result = tr("%1(%2) %3").arg(g.tag("Result")).arg((g.plyCount() + 1) / 2)
			 .arg(eco);
	QString header = tr("<i>%1(%2), %3, %4</i>").arg(g.tag("Event")).arg(g.tag("Round"))
			 .arg(g.tag("Site")).arg(g.tag("Date"));
	QString lastmove, nextmove;
	if (!g.atStart())
		lastmove = QString("<a href=\"move:prev\">%1</a>").arg(g.previousMoveToSan(Game::FullDetail));
	else
		lastmove = tr("(Start of game)");
	if (!g.atEnd())
		nextmove = QString("<a href=\"move:next\">%1</a>").arg(g.moveToSan(Game::FullDetail));
	else
		nextmove = g.isMainline() ? tr("(End of game)") : tr("(End of line)");
	QString move = tr("Last move: %1 &nbsp; &nbsp; Next: %2").arg(lastmove).arg(nextmove);
	if (!g.isMainline())
		move.append(QString(" &nbsp; &nbsp; <a href=\"move:exit\">%1</a>").arg(tr("(&lt;-Var)")));
	QString var;
	if (g.variationCount() > 1) {
		var = tr("<br>Variations: &nbsp; ");
		for (int i = 1; i < g.variationCount(); i++) {
			var.append(QString("v%1: <a href=\"move:%2\">%3</a>").arg(i).arg(g.moveId(i))
				   .arg(g.moveToSan(Game::FullDetail, i)));
			if (i != g.variationCount() - 1)
				var.append(" &nbsp; ");
		}
	}
	m_moveView->setText(QString("<qt>%1<br>%2<br>%3<br>%4%5</qt>").arg(players).arg(result)
			    .arg(header).arg(move).arg(var));
	if (AppSettings->value("/General/onlineTablebases", true).toBool())
		m_tablebase->getBestMove(g.toFen());

	slotSearchTree();
	emit boardChange(g.board());
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

void MainWindow::slotBoardMoveWheel(int wheel)
{
	if (wheel & Qt::AltModifier)
		if (wheel & BoardView::WheelDown) slotGameMoveLast();
		else slotGameMoveFirst();
	else if (wheel & Qt::ControlModifier)
		if (wheel & BoardView::WheelDown) slotGameMoveNextN();
		else slotGameMovePreviousN();
	else
		if (wheel & BoardView::WheelDown) slotGameMoveNext();
		else slotGameMovePrevious();
}

void MainWindow::slotGameLoadFirst()
{
	gameLoad(databaseInfo()->filter()->indexToGame(0));
}

void MainWindow::slotGameLoadLast()
{
	gameLoad(databaseInfo()->filter()->indexToGame(databaseInfo()->filter()->count() - 1));
}

void MainWindow::slotGameLoadPrevious()
{
	gameLoad(databaseInfo()->filter()->previousGame(gameIndex()));
}

void MainWindow::slotGameLoadNext()
{
	gameLoad(databaseInfo()->filter()->nextGame(gameIndex()));
}

void MainWindow::slotGameLoadRandom()
{
	if (databaseInfo()->filter()->count()) {
		int random = rand() % databaseInfo()->filter()->count();
		gameLoad(databaseInfo()->filter()->indexToGame(random));
	}
}

void MainWindow::slotGameLoadChosen()
{
	int index = QInputDialog::getInteger(0, tr("Load Game"), tr("Game number:"), gameIndex() + 1,
					     1, database()->count());
	gameLoad(index - 1);
}

void MainWindow::slotGameNew()
{
	if (database()->isReadOnly())
		QMessageBox::critical(this, tr("New game"), tr("This database is read only."));
	else {
		databaseInfo()->newGame();
		slotGameChanged();
	}
}

void MainWindow::slotGameSave()
{
	if (database()->isReadOnly())
		QMessageBox::critical(this, tr("Save game"), tr("This database is read only."));
	else if (saveDialog()->exec(game()) == QDialog::Accepted) {
		databaseInfo()->saveGame();
		slotDatabaseChanged();
	}
}

void MainWindow::slotGameChanged()
{
	if (m_showPgnSource)
		m_gameView->setPlainText(m_output->output(&game()));
	else
		m_gameView->setText(m_output->output(&game()));
	m_eco = game().ecoClassify();
	slotMoveChanged();
}

void MainWindow::slotGameViewLink(const QUrl& url)
{
	if (url.scheme() == "move") {
		if (url.path() == "prev") game().backward();
		else if (url.path() == "next") game().forward();
		else if (url.path() == "exit") game().exitVariation();
		else
			game().moveToId(url.path().toInt());
		slotMoveChanged();
	} else if (url.scheme() == "egtb") {
		if (!game().atEnd())
			game().enterVariation(game().addMove(url.path()));
		else
			game().addMove(url.path());
		game().forward();
		slotGameChanged();
	} else if (url.scheme() == "tag") {
		if (url.path() == "white")
			playerDialog()->findPlayers(game().tag("White"));
		else if (url.path() == "black")
			playerDialog()->findPlayers(game().tag("Black"));
	}
}

void MainWindow::slotGameViewToggle(bool toggled)
{
	m_showPgnSource = toggled;
	slotGameChanged();
}

void MainWindow::slotFilterChanged()
{
	if (gameIndex() >= 0)
		m_gameList->selectGame(gameIndex());
	int count = databaseInfo()->filter()->count();
	QString f = count == database()->count() ? "all" : QString::number(count);
	m_statusFilter->setText(tr(" %1: %2/%3 ").arg(databaseName())
				.arg(f).arg(database()->count()));
}

void MainWindow::slotFilterLoad(int index)
{
	if (gameIndex() != index)
		gameLoad(index);
	activateWindow();
}

void MainWindow::slotStatusMessage(const QString& msg)
{
	statusBar()->showMessage(msg);
}

void MainWindow::slotDatabaseChange()
{
	QAction* action = qobject_cast<QAction*>(sender());
	if (action && m_currentDatabase != action->data().toInt()) {
		m_currentDatabase = action->data().toInt();
		slotDatabaseChanged();
	}
}

void MainWindow::slotDatabaseChanged()
{
	setWindowTitle(tr("ChessX - %1").arg(databaseName()));
	m_gameList->setFilter(databaseInfo()->filter());
	slotFilterChanged();
	gameLoad(gameIndex(), true);
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
	file->addAction(createAction(tr("&Open..."), SLOT(slotFileOpen()), Qt::CTRL + Qt::Key_O));
	QMenu* menuRecent = file->addMenu(tr("&Recent files..."));
	const int MaxRecentFiles = 10;
	for (int i = 0; i < MaxRecentFiles; ++i) {
		QAction* action = new QAction(this);
		action->setVisible(false);
		connect(action, SIGNAL(triggered()), SLOT(slotFileOpenRecent()));
		m_recentFileActions.append(action);
		menuRecent->addAction(action);
	}
#ifdef QT_DEBUG
	file->addAction(createAction(tr("&Save"), SLOT(slotFileSave()), Qt::CTRL + Qt::SHIFT + Qt::Key_S));
#endif
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

#ifdef QT_DEBUG
	gameMenu->addAction(createAction(tr("&New"), SLOT(slotGameNew()), Qt::CTRL + Qt::Key_N));
	gameMenu->addAction(createAction(tr("&Save...."), SLOT(slotGameSave()), Qt::CTRL + Qt::Key_S));
#endif

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

	/* View menu */
	m_menuView = menuBar()->addMenu(tr("&View"));
	m_menuDatabases = m_menuView->addMenu(tr("&Database"));
	;
	m_menuView->addAction(createAction(tr("&Player Database..."), SLOT(slotPlayerDialog()),
					   Qt::CTRL + Qt::SHIFT + Qt::Key_P));

	/* Settings menu */
	QMenu *settings = menuBar()->addMenu(tr("&Settings"));
	settings->addAction(createAction(tr("&Configure ChessX..."), SLOT(slotConfigure())));
	QAction* flip = createAction(tr("&Flip board"), SLOT(slotConfigureFlip()), Qt::CTRL + Qt::Key_B);
	flip->setCheckable(true);
	settings->addAction(flip);

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

void MainWindow::slotSearchTag()
{
	m_gameList->simpleSearch(1);
}

void MainWindow::slotSearchBoard()
{
	PositionSearch ps(databaseInfo()->filter()->database(), m_boardView->board());

	databaseInfo()->filter()->executeSearch(ps);
	m_gameList->updateFilter();
	slotFilterChanged();
}

void MainWindow::slotSearchReverse()
{
	databaseInfo()->filter()->reverse();
	m_gameList->updateFilter();
	slotFilterChanged();
}

void MainWindow::slotSearchReset()
{
	databaseInfo()->filter()->setAll(1);
	m_gameList->updateFilter();
	slotFilterChanged();
}

void MainWindow::slotSearchTree()
{
	if (!database()->count() || !m_openingTree->isVisible())
		return;
	QTime time;
	time.start();
	dynamic_cast<OpeningTree*>(m_openingTree->model())->update(*databaseInfo()->filter(), m_boardView->board());
	m_gameList->updateFilter();
	slotFilterChanged();
	slotStatusMessage(tr("Tree updated (%1 s.)").arg(time.elapsed() / 100 / 10.0));
}

void MainWindow::slotSearchTreeMove(const QModelIndex& index)
{
	QString move = dynamic_cast<OpeningTree*>(m_openingTree->model())->move(index);
	Move m = m_boardView->board().parseMove(move);
	if (m == game().move())
		slotGameMoveNext();
	else if (game().isModified())
		slotBoardMove(m.from(), m.to());
	else {
		Board board = m_boardView->board();
		board.doMove(m);
		m_boardView->setBoard(board);
		slotSearchTree();
		slotGameLoadFirst();
	}
}

