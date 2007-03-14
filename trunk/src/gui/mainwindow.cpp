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
#include "filtermodel.h"
#include "chessbrowser.h"
//#include "enginesetup.h"
#include "databaseinfo.h"
#include "game.h"
#include "gamelist.h"
#include "helpwindow.h"
#include "mainwindow.h"
#include "output.h"
#include "pgndatabase.h"
#include "playerdatabase.h"
#include "playerdialog.h"
#include "preferences.h"
#include "savedialog.h"
#include "settings.h"
#include "tipoftheday.h"

#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDockWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>
#include <QStatusBar>
#include <QTime>

bool yesNo(const QString& question, QMessageBox::Icon icon = QMessageBox::Question)
{
  QMessageBox mb("ChessX", question, icon, QMessageBox::Yes, QMessageBox::No,
     Qt::NoButton);
  return mb.exec() == QMessageBox::Yes;
}


MainWindow::MainWindow() : QMainWindow(),
  m_playerDialog(0), m_saveDialog(0), m_helpWindow(0), m_showPgnSource(false)
{
  setObjectName("MainWindow");
  /* Active database */
  m_databases.append(new DatabaseInfo);
  m_currentDatabase = 0;

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
  m_boardView->resize(500, 5400);
  connect(this, SIGNAL(reconfigure()), m_boardView, SLOT(configure()));
  connect(m_boardView, SIGNAL(moveMade(Square, Square)), SLOT(slotMove(Square, Square)));
  connect(m_boardView, SIGNAL(wheelScrolled(int)), SLOT(slotGameMoveWheel(int)));

  /* Move view */
  m_moveView = new ChessBrowser(m_boardSplitter);
  m_moveView->zoomOut();
  m_moveView->setMinimumHeight(80);
  connect(m_moveView, SIGNAL(anchorClicked(const QUrl&)), SLOT(slotGameViewLink(const QUrl&)));

  /* Board layout */
  m_boardSplitter->addWidget(m_boardView);
  m_boardSplitter->addWidget(m_moveView);

 /* Game view */
  QDockWidget* dock = new QDockWidget(tr("Game Text"), this);
  dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  dock->setObjectName("GameText");
  m_gameView = new ChessBrowser(dock);
  m_gameView->setMinimumSize(150, 100);
  connect(m_gameView, SIGNAL(anchorClicked(const QUrl&)), SLOT(slotGameViewLink(const QUrl&)));
  dock->setWidget(m_gameView);
  addDockWidget(Qt::RightDockWidgetArea, dock);
  m_menuView->addAction(dock->toggleViewAction());

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

  /* Randomize */
  srand(time(0));

  /* Restoring layouts */
  if (!AppSettings->layout(this))
    resize(800, 600);
  AppSettings->beginGroup("MainWindow");
  m_boardSplitter->restoreState(AppSettings->value("BoardSplit").toByteArray());
  AppSettings->endGroup();
  emit reconfigure();

  Game::loadEcoFile(AppSettings->dataPath() + "/chessx.eco");

  /* Reset board - not earlier, as all widgets have to be created. */
  slotGameChanged();

  /* Status */
  m_statusFilter = new QLabel(statusBar());
  statusBar()->addPermanentWidget(m_statusFilter);
  slotStatusMessage(tr("Ready."));

  /* Activate clipboard */
  updateMenuDatabases();
  slotDatabaseChanged();

  /* Tip of the day */
  AppSettings->beginGroup("/Tips/");
  m_showTip = AppSettings->value("showTips", true).toBool();
  AppSettings->endGroup();
  m_tipDialog = new TipOfDayDialog(this);

}

void MainWindow::show()
{
  this->QMainWindow::show();
  if(m_showTip)
      m_tipDialog->show();
}

MainWindow::~MainWindow()
{
  qDeleteAll(m_databases.begin(), m_databases.end());
  delete m_saveDialog;
  delete m_playerDialog;
  delete m_helpWindow;
  delete m_output;
  delete m_tipDialog;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
  if (yesNo(tr("Do you want to quit?"))) {
    m_recentFiles.save("History", "RecentFiles");
    AppSettings->setLayout(m_playerDialog);
    AppSettings->setLayout(m_helpWindow);
    m_gameList->saveConfig();
    AppSettings->setLayout(this);
    AppSettings->beginGroup("MainWindow");
    AppSettings->setValue("BoardSplit", m_boardSplitter->saveState());
    AppSettings->endGroup();
    e->accept();
    qApp->quit();
    }
  else
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

Game* MainWindow::game()
{
  return databaseInfo()->currentGame();
}

int MainWindow::gameIndex() const
{
  return databaseInfo()->currentIndex();
}

void MainWindow::gameLoad(int index)
{
  if (index < 0 || index >= database()->count())
    return;
  databaseInfo()->loadGame(index);
  m_gameList->selectGame(index);
  qobject_cast<QWidget*>(m_gameView->parent())->setWindowTitle(tr("Game: %1").arg(index+1));
  slotGameChanged();
}

void MainWindow::gameMoveBy(int change)
{
  if (game()->moveByPly(change))
     slotMoveChanged();
}

void MainWindow::updateMenuRecent()
{
  for (int i = 0; i < m_recentFiles.count(); i++)
  {
    m_recentFileActions[i]->setVisible(true);
    m_recentFileActions[i]->setText(QString("&%1: %2").arg(i+1).arg(m_recentFiles[i]));
    m_recentFileActions[i]->setData(m_recentFiles[i]);
  }
  for (int i = m_recentFiles.count(); i < m_recentFileActions.count(); i++)
    m_recentFileActions[i]->setVisible(false);
}

void MainWindow::updateMenuDatabases()
{
  while (m_databases.count() > m_databaseActions.count())
  {
    QAction* action = new QAction(this);
    connect(action, SIGNAL(triggered()), SLOT(slotDatabaseChange()));
    m_databaseActions.append(action);
    m_menuDatabases->addAction(action);
  }
  for (int i = 0; i < m_databases.count(); i++)
  {
    m_databaseActions[i]->setVisible(true);
    m_databaseActions[i]->setData(i);
    m_databaseActions[i]->setText(QString("&%1: %2").arg(i).arg(databaseName(i)));
    int key = Qt::CTRL + Qt::Key_1 + (i-1);
    if (i < 10)
      m_databaseActions[i]->setShortcut(key);
  }
  for (int i = m_databases.count(); i < m_databaseActions.count(); i++)
  {
    m_databaseActions[i]->setVisible(false);
    m_databaseActions[i]->setShortcut(0);
  }
}

bool MainWindow::openDatabase(const QString& fname)
{
  /* Check if the database isn't already open */
  for (int i = 0; i < m_databases.count(); i++)
    if (m_databases[i]->database()->filename() == fname)
    {
      m_currentDatabase = i;
      slotDatabaseChanged();
      slotStatusMessage(tr("Database %1 already opened.").arg(fname.section('/', -1)));
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
      .arg(fname.section('/', -1)).arg((time.elapsed() + 500)/ 1000));
  slotDatabaseChanged();
  return true;
}

PlayerDialog* MainWindow::playerDialog()
{
  if (!m_playerDialog)
  {
    PlayerDatabase* db = new PlayerDatabase;
    QString path = AppSettings->dataPath().remove("/data") + "/tests/playerdatabase/data/players";
    //if (!db->open(path)); Bug in PlayerDatabase - always returns false
    if (!QFile::exists(path + ".cpm"))
      QMessageBox::information (0, "No player database",
                                QString("Player database was not found at\n%1").arg(path));
    else
      db->open(path);
    m_playerDialog = new PlayerDialog(db, this);
    AppSettings->layout(m_playerDialog);
  }
  return m_playerDialog;
}

SaveDialog * MainWindow::saveDialog()
{
  if (!m_saveDialog)
    m_saveDialog = new SaveDialog;
  return m_saveDialog;
}

HelpWindow* MainWindow::helpWindow()
{
  if (!m_helpWindow)
  {
    m_helpWindow = new HelpWindow();
    AppSettings->layout(m_helpWindow);
  }
  return m_helpWindow;
}


void MainWindow::slotFileOpen()
{
  QString file = QFileDialog::getOpenFileName(this, tr("Open database"), QString::null,
     tr("PGN Database (*.pgn)"));
  if (!file.isEmpty())
    openDatabase(file);
}

void MainWindow::slotFileOpenRecent()
{
  QAction *action = qobject_cast<QAction *>(sender());
  if (action)
    openDatabase(action->data().toString());
}

void MainWindow::slotFileClose()
{
  if (m_currentDatabase) // Clipboard
  {
    m_databases.removeAt(m_currentDatabase);
    if (m_currentDatabase == m_databases.count())
      m_currentDatabase--;
    updateMenuDatabases();
    slotDatabaseChanged();
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
  if (P.exec() == QDialog::Accepted)
    emit reconfigure();
}

void MainWindow::slotConfigureChessEngines()
{
  //EngineSetupDialog dlg;
  //dlg.exec();
}

void MainWindow::slotConfigureFlip()
{
  m_boardView->flip();
}

void MainWindow::slotEditCopyFEN()
{
  QApplication::clipboard()->setText(game()->toFen());
}

void MainWindow::slotEditPasteFEN()
{
  QString fen = QApplication::clipboard()->text();
  if (!game()->board().isValidFEN(fen))
  {
    QString msg = fen.trimmed().length() ?
        tr("Text in clipboard does not represent valid FEN:<br><i>%1</i>").arg(fen) :
        tr("There is no text in clipboard.");
    QMessageBox::warning(0, "Paste FEN", msg);
    return;
  }
  game()->setStartBoard(fen);
  slotGameChanged();
}

void MainWindow::slotEditTruncate()
{
  if (!game()->isMainline() && game()->atEnd())
  {
    int var = game()->currentVariation();
    game()->exitVariation();
    game()->removeVariation(var);
  }
  else
    game()->truncateGameEnd();
  slotGameChanged();
}


void MainWindow::slotEditBoard()
{
  BoardSetupDialog B;
  B.setBoard(game()->board());
  B.setMoveNumber(game()->moveNumber());
  if (B.exec() == QDialog::Accepted)
  {
    game()->setStartBoard(B.board(), B.moveNumber());
    slotGameChanged();
  }
}


void MainWindow::slotHelp()
{
  helpWindow()->show();
}

void MainWindow::slotHelpAbout()
{
  QMessageBox dialog(tr(""), tr("<h1>ChessX</h1>"
    "<p>Free chess database.<br>Version %1</br>"
    "<p>Copyright 2005-2007 ChessX developers:<br>"
     "Database: Marius Roets, William Hoggarth, Ejner Borgbjerg.<br>"
     "GUI: Michal Rudolf, Heinz Hopfgartner."
    "<p>Homepage: <a href=\"http://chessx.sf.net\">http://chessx.sf.net</a><br>"
    "Mailing list: <a href=\"mailto:chessx-users@lists.sourceforge.net\">"
                               "chessx-users@lists.sourceforge.net").arg(ChessXVersion),
    QMessageBox::NoIcon, QMessageBox::Ok, Qt::NoButton, Qt::NoButton, this);
  dialog.exec();
}

void MainWindow::slotHelpBug()
{
  QDesktopServices::openUrl(QUrl("http://sourceforge.net/tracker/?group_id=163833&atid=829300"));
}


void MainWindow::slotMove(Square from, Square to)
{
  Board board = game()->board();
  Move m(board, from, to);
  if ((to < 8 || to > 55) && (board.at(from) == WhitePawn || board.at(from) == BlackPawn))
  {
    QStringList moves;
    moves << tr("Queen") << tr("Rook") << tr("Bishop") << tr("Knight");
    bool ok;
    int index = moves.indexOf(QInputDialog::getItem(0, tr("Promotion"), tr("Promote to:"), 
                              moves, 0, false, &ok));
    if (!ok) return;
    m.setPromotionPiece(Piece(index + (board.toMove() == White ? WhiteQueen : BlackQueen)));
  }
  if (board.isLegal(m))
  {
    if (game()->atEnd())
      game()->addMove(m);
    else
    {
      // Find how way we should add a moveMade
      QMessageBox mbox(QMessageBox::Question, tr("Add move"), 
                      tr("There is already next move in current game. What do you want to do?"));
      QPushButton* addVar = mbox.addButton(tr("Add variation"), QMessageBox::YesRole);
      QPushButton* newMain = mbox.addButton(tr("Add new mainline"), QMessageBox::AcceptRole);
      QPushButton* replaceMain = mbox.addButton(tr("Replace current move"), QMessageBox::DestructiveRole);
      mbox.addButton(tr("Cancel"), QMessageBox::RejectRole);
      mbox.exec();
      if (mbox.clickedButton() == addVar)
        game()->enterVariation(game()->addMove(m));
      else if (mbox.clickedButton() == newMain)
        game()->promoteVariation(game()->addMove(m));
      else if (mbox.clickedButton() == replaceMain)
        game()->replaceMove(m);
      else return;
    }
    game()->forward();
    slotGameChanged();
  }
}

void MainWindow::slotMoveChanged()
{
  Game* g = game();
  // Set board first
  m_boardView->setBoard(g->board());
  // Highlight current move
  int id = game()->currentMoveId();
  if (!id) 
    id = 1;
  m_gameView->selectAnchor(QString("move:%1").arg(id));
  // Finally update game information
  QString white = g->tag("White");
  QString black = g->tag("Black");
  QString eco = m_eco.isNull() ? g->tag("ECO") : m_eco;
  QString players = tr("Game %1: <a href=\"tag:white\">%2</a> %3 - <a href=\"tag:black\">%4</a> %5")
      .arg(gameIndex() + 1).arg(white).arg(g->tag("WhiteElo")).arg(black).arg(g->tag("BlackElo"));
  QString result = tr("%1(%2) %3").arg(g->tag("Result")).arg((g->plyCount() + 1) / 2)
      .arg(eco);
  QString header = tr("%1, %2, %3, round %4").arg(g->tag("Event")).arg(g->tag("Site"))
      .arg(g->tag("Date")).arg(g->tag("Round"));
  QString lastmove, nextmove;
  if (!g->atStart())
    lastmove = QString("<a href=\"move:prev\">%1</a>").arg(g->previousMoveToSan(Game::FullDetail));
  else
    lastmove = tr("(Start of game)");
  if (!g->atEnd())
    nextmove = QString("<a href=\"move:next\">%1</a>").arg(g->moveToSan(Game::FullDetail));
  else
    nextmove = g->isMainline() ? tr("(End of game)") : tr("(End of line)");
  QString move = tr("Last move: %1 &nbsp; &nbsp; Next: %2").arg(lastmove).arg(nextmove);
  if (!g->isMainline())
    move.append(QString(" &nbsp; &nbsp; <a href=\"move:exit\">%1</a>").arg(tr("(&lt;-Var)")));
  QString var;
  if (g->variationCount() > 1)
  {
    var = tr("<br>Variations: &nbsp; ");
    for (int i = 1; i < g->variationCount(); i++)
    {
      var.append(QString("v%1: <a href=\"move:%2\">%3</a>").arg(i).arg(g->moveId(i))
         .arg(g->moveToSan(Game::FullDetail, i)));
      if (i != g->variationCount() - 1)
        var.append(" &nbsp; ");
     }
  }
  m_moveView->setText(QString("<qt>%1<br>%2<br>%3<br>%4%5<qt>").arg(players).arg(result)
      .arg(header).arg(move).arg(var));
}

void MainWindow::slotGameMoveWheel(int wheel)
{
  if (wheel & Qt::AltModifier)
    if (wheel & BoardView::WheelDown) slotGameMoveLast(); else slotGameMoveFirst();
  else if (wheel & Qt::ControlModifier)
    if (wheel & BoardView::WheelDown) slotGameMoveNextN(); else slotGameMovePreviousN();
  else 
    if (wheel & BoardView::WheelDown) slotGameMoveNext(); else slotGameMovePrevious();
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
  int random = rand() % databaseInfo()->filter()->count();
  gameLoad(databaseInfo()->filter()->indexToGame(random));
}

void MainWindow::slotGameLoadChosen()
{
  int index = QInputDialog::getInteger(0, tr("Load Game"), tr("Game number:"), gameIndex()+1,
                                       1, database()->count());
  gameLoad(index - 1);
}

void MainWindow::slotGameSave()
{
  if (saveDialog()->exec(game()) == QMessageBox::Ok)
    slotGameChanged();
}

void MainWindow::slotGameChanged()
{
  if (m_showPgnSource)
    m_gameView->setPlainText(m_output->output(game()));
  else
    m_gameView->setText(m_output->output(game()));
  m_eco = game()->ecoClassify();
  slotMoveChanged();
}

void MainWindow::slotGameViewLink(const QUrl& url)
{
  if (url.scheme() == "move")
  {
    if (url.path() == "prev") game()->backward();
    else if (url.path() == "next") game()->forward();
    else if (url.path() == "exit") game()->exitVariation();
    else 
      game()->moveToId(url.path().toInt());
    slotMoveChanged();
  }
  else if (url.scheme() == "tag")
  {
    if (url.path() == "white")
      playerDialog()->findPlayers(game()->tag("White"));
    else if (url.path() == "black")
      playerDialog()->findPlayers(game()->tag("Black"));
  }
  /*QString command = link.section(':', 0, 0);
  QString arg =  link.section(':', 1);
  else if (command == "var")
  {
    if (arg == "exit")
      game()->exitVariation();
    else
      game()->enterVariation(arg.toInt());
    m_boardView->setBoard(game()->board());
  }
  else if (link.startsWith("player:"))
  {
    playerDialog()->findPlayers(arg);
    playerDialog()->show();
  }*/
}

void MainWindow::slotGameViewToggle(bool toggled)
{
  m_showPgnSource = toggled;
  slotGameChanged();
}

void MainWindow::slotFilterChanged()
{
  int count = databaseInfo()->filter()->count();
  QString f = count == database()->count() ? "all" : QString::number(count);
  m_statusFilter->setText(tr(" %1: %2/%3 ").arg(databaseName())
      .arg(f).arg(database()->count()));
}

void MainWindow::slotFilterLoad(int index)
{
  gameLoad(index);
  activateWindow();
}

void MainWindow::slotStatusMessage(const QString& msg)
{
  statusBar()->showMessage(msg);
}

void MainWindow::slotDatabaseChange()
{
  QAction *action = qobject_cast<QAction *>(sender());
  if (action && m_currentDatabase != action->data().toInt())
  {
    m_currentDatabase = action->data().toInt();
    slotDatabaseChanged();
  }
}

void MainWindow::slotDatabaseChanged()
{
  setWindowTitle(tr("ChessX - %1").arg(databaseName()));
  m_gameList->setFilter(databaseInfo()->filter());
  slotFilterChanged();
  gameLoad(gameIndex());
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
  for (int i = 0; i < MaxRecentFiles; ++i)
  {
    QAction* action = new QAction(this);
    action->setVisible(false);
    connect(action, SIGNAL(triggered()), SLOT(slotFileOpenRecent()));
    m_recentFileActions.append(action);
    menuRecent->addAction(action);
  }
  file->addAction(createAction(tr("&Close"), SLOT(slotFileClose()), Qt::CTRL + Qt::Key_W));
  file->addAction(createAction(tr("&Quit"), SLOT(slotFileQuit()), Qt::CTRL + Qt::Key_Q));

  /* Edit menu */
  QMenu* edit = menuBar()->addMenu(tr("&Edit"));
  edit->addAction(createAction(tr("Truncate moves"), SLOT(slotEditTruncate()),
    Qt::SHIFT + Qt::Key_Delete));
  edit->addSeparator();
  edit->addAction(createAction(tr("&Copy FEN"), SLOT(slotEditCopyFEN()),
    Qt::CTRL + Qt::SHIFT + Qt::Key_C));
  edit->addAction(createAction(tr("&Paste FEN"), SLOT(slotEditPasteFEN()),
    Qt::CTRL + Qt::SHIFT + Qt::Key_V));
  edit->addAction(createAction(tr("Position &Setup..."), SLOT(slotEditBoard()),
    Qt::CTRL + Qt::SHIFT + Qt::Key_S));

  /* Game menu */
  QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));
  QMenu* loadMenu = gameMenu->addMenu(tr("&Load..."));

  /* Game->Load submenu */
  loadMenu->addAction(createAction(tr("&First"), SLOT(slotGameLoadFirst()), Qt::CTRL + Qt::SHIFT + Qt::Key_Up));
  loadMenu->addAction(createAction(tr("&Last"), SLOT(slotGameLoadLast()), Qt::CTRL + Qt::SHIFT + Qt::Key_Down));
  loadMenu->addAction(createAction(tr("&Next"), SLOT(slotGameLoadNext()), Qt::CTRL + Qt::Key_Down));
  loadMenu->addAction(createAction(tr("&Previous"), SLOT(slotGameLoadPrevious()), Qt::CTRL + Qt::Key_Up));
  loadMenu->addAction(createAction(tr("&Go to game..."), SLOT(slotGameLoadChosen()), Qt::CTRL + Qt::Key_G));
  loadMenu->addAction(createAction(tr("&Random"), SLOT(slotGameLoadRandom()), Qt::CTRL + Qt::Key_Question));

  /* Game->Go to submenu */
  QMenu* goMenu = gameMenu->addMenu(tr("&Go..."));
  goMenu->addAction(createAction(tr("&Start"), SLOT(slotGameMoveFirst()), Qt::Key_Home));
  goMenu->addAction(createAction(tr("&End"), SLOT(slotGameMoveLast()), Qt::Key_End));
  goMenu->addAction(createAction(tr("&Next move"), SLOT(slotGameMoveNext()), Qt::Key_Right));
  goMenu->addAction(createAction(tr("&Previous move"), SLOT(slotGameMovePrevious()), Qt::Key_Left));
  goMenu->addAction(createAction(tr("5 moves &forward"), SLOT(slotGameMoveNextN()), Qt::Key_Down));
  goMenu->addAction(createAction(tr("5 moves &backward"), SLOT(slotGameMovePreviousN()), Qt::Key_Up));

  gameMenu->addAction(createAction(tr("&Save...."), SLOT(slotGameSave()), Qt::CTRL + Qt::Key_S));

  /* Search menu */
  QMenu* search = menuBar()->addMenu(tr("&Search"));
  //search->addAction(createAction(tr("&Find position"), SLOT(slotSearchBoard()), Qt::CTRL +
      //Qt::SHIFT + Qt::Key_B));
  search->addAction(createAction(tr("&Reset filter"), SLOT(slotSearchReset()), Qt::CTRL + Qt::Key_F));
  search->addAction(createAction(tr("&Reverse filter"), SLOT(slotSearchReverse()),
                    Qt::CTRL + Qt::SHIFT + Qt::Key_F));

  /* View menu */
  m_menuView = menuBar()->addMenu(tr("&View"));
  m_menuDatabases = m_menuView->addMenu(tr("&Database"));;
  //m_menuView->addAction(createAction(tr("&Game list"), SLOT(slotFilterSwitch()), Qt::CTRL + Qt::Key_L));
  m_menuView->addAction(createAction(tr("&Player Database..."), SLOT(slotPlayerDialog()), Qt::CTRL + Qt::SHIFT + Qt::Key_P));

  /* Settings menu */
  QMenu *settings = menuBar()->addMenu(tr("&Settings"));
  settings->addAction(createAction(tr("&Configure ChessX..."), SLOT(slotConfigure()) ));
//  settings ->addAction(createAction(tr("Chess &Engines..."), this, SLOT(slotConfigureChessEngines()));
  settings ->addAction(createAction(tr("&Flip board"), SLOT(slotConfigureFlip()), Qt::CTRL + Qt::Key_B));

  /* Help menu */
  menuBar()->addSeparator();
  QMenu *help = menuBar()->addMenu(tr("&Help"));
//  help->addAction(createAction(tr("ChessX &help..."), SLOT(slotHelp()), Qt::CTRL + Qt::Key_F10));
  help->addAction(createAction(tr("&About ChessX"), SLOT(slotHelpAbout()) ));
  help->addAction(createAction(tr("&Report a bug"), SLOT(slotHelpBug()) ));
  QMenu* debug = help->addMenu(tr("&Debug"));
#ifndef QT_DEBUG
  debug->setVisible(false);
#endif
  QAction* source;
  debug->addAction(source = createAction("Toggle game view format", 0, Qt::Key_F12));
  source->setCheckable(true);
  connect(source, SIGNAL(toggled(bool)), SLOT(slotGameViewToggle(bool)));
}

void MainWindow::slotSearchBoard()
{
  PositionSearch ps(m_boardView->board());

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
  databaseInfo()->filter()->setAll(true);
  m_gameList->updateFilter();
  slotFilterChanged();
}

