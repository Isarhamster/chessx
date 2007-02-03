/***************************************************************************
                          mainwindow.cpp  -  main window
                             -------------------
    begin                : 10 Oct 2005
    copyright            : (C) 2005-2006 Michal Rudolf <mrudolf@kdewebdev.org>
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

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QDebug>
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QVBoxLayout>

bool yesNo(const QString& question, QMessageBox::Icon icon = QMessageBox::Question)
{
  QMessageBox mb("ChessX", question, icon, QMessageBox::Yes, QMessageBox::No,
     Qt::NoButton);
  return mb.exec() == QMessageBox::Yes;
}


MainWindow::MainWindow() : QMainWindow(),
  m_playerDialog(0), m_helpWindow(0)
{
  setObjectName("MainWindow");

  /* Active database */
  m_databases.append(new DatabaseInfo);
  m_currentDatabase = 0;

  /* Save dialog */
  m_saveDialog = new SaveDialog;

  /* Actions */
  m_actions = new QActionGroup(this);
  setupActions();

  /* Delete on close */
  setAttribute(Qt::WA_DeleteOnClose);

  /* Recent files */
  m_recentFiles.restore("History", "RecentFiles");
  int i = 0;
  while (i < m_recentFiles.count())
  {
    QFileInfo file(m_recentFiles[i]);
    if (file.exists())
      i++;
    else m_recentFiles.remove(m_recentFiles[i]);
  }
  updateMenuRecent();

  /* Output */
  m_output = new Output(Output::NotationWidget,
    AppSettings->dataPath() + "/templates/notation-test.template");

  setDockNestingEnabled(true);

  /* Board */
  QWidget *boardBox = new QWidget;
  setCentralWidget(boardBox);
  m_boardView = new BoardView(boardBox);
  m_boardView->setMinimumSize(200, 200);
  m_boardView->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
  connect(this, SIGNAL(reconfigure()), m_boardView, SLOT(configure()));
  connect(m_boardView, SIGNAL(moveMade(Square, Square)), SLOT(slotMove(Square, Square)));
  connect(m_boardView, SIGNAL(changed()), SLOT(slotMoveViewUpdate()));
  connect(m_boardView, SIGNAL(wheelScrolled(int)), SLOT(slotGameMoveWheel(int)));

  /* Move view */
  m_moveView = new ChessBrowser(boardBox);
  m_moveView->zoomOut();
  m_moveView->setFixedHeight(120);
  connect(m_moveView, SIGNAL(anchorClicked(const QUrl&)), SLOT(slotGameViewLink(const QUrl&)));

  /* Board layout */
  QVBoxLayout* boardLayout = new QVBoxLayout;
  boardLayout->setMargin(0);
  boardLayout->addWidget(m_boardView);
  boardLayout->addWidget(m_moveView);
  boardBox->setLayout(boardLayout);

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
  m_gameList = new GameList(m_databases[m_currentDatabase]->filter(), dock);
  m_gameList->setMinimumSize(150, 100);
  connect(m_gameList, SIGNAL(selected(int)), SLOT(slotFilterLoad(int)));
  dock->setWidget(m_gameList);
  addDockWidget(Qt::BottomDockWidgetArea, dock);
  m_menuView->addAction(dock->toggleViewAction());

  /* Randomize */
  srand(time(0));

  /* Restoring layouts */
  AppSettings->readLayout(this);
  //AppSettings->
  emit reconfigure();

  /* Reset board - not earlier, as all widgets have to be created. */
  m_boardView->setBoard(game()->board());

  /* Status */
  slotStatusMessage(tr("Ready."));

  /* Tip of the day */
  AppSettings->beginGroup("/Tips/");
  m_showTip = AppSettings->readBoolEntry("showTips", true);
  AppSettings->endGroup();
  m_tipDialog = new TipOfDayDialog(this);
}

/*** The QMainWindow::show() slot is overriden to show the tip of the day dialog
aftert the main window has been constructed. 
 */
void MainWindow::show()
{
  this->QMainWindow::show();
   if(m_showTip)
   {
      m_tipDialog->show();
   }
}

MainWindow::~MainWindow()
{
  delete m_saveDialog;
  delete m_playerDialog;
  delete m_helpWindow;
  delete m_output;
  delete m_tipDialog;
}

QString databaseName(const QString& fname)
{
  QString name = fname.section('/', -1);
  int ext = name.findRev('.');
  if (ext > name.length() - 5)
    name = name.left(ext);
  return name;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
  if (yesNo(tr("Do you want to quit?"))) {
    m_recentFiles.save("History", "RecentFiles");
    AppSettings->writeLayout(m_playerDialog);
    AppSettings->writeLayout(m_helpWindow);
    m_gameList->saveConfig();
    AppSettings->writeLayout(this);
    e->accept();
    qApp->quit();
    }
  else
    e->ignore();
}

Database* MainWindow::database()
{
  return m_databases[m_currentDatabase]->database();
}

Game* MainWindow::game()
{
  return m_databases[m_currentDatabase]->currentGame();
}

int MainWindow::gameIndex() const
{
  return m_databases[m_currentDatabase]->currentIndex();
}

void MainWindow::gameLoad(int index)
{
  if (index < 0) index = 0;
  if (index >= database()->count())
    index = database()->count() - 1;
  m_databases[m_currentDatabase]->loadGame(index);
  m_boardView->setBoard(game()->board());
  qobject_cast<QWidget*>(m_boardView->parent())->setWindowTitle(tr("Game: %1").arg(index+1));
  slotMoveViewUpdate();
  slotGameView();
}

void MainWindow::gameMoveBy(int change)
{
  if (game()->moveByPly(change))
    m_boardView->setBoard(game()->board());
}

void MainWindow::updateMenuRecent()
{
  m_menuRecent->clear();
  for (int i = 0; i < 5 && i < m_recentFiles.count(); i++)
    m_menuRecent->insertItem(QString("&%1: %2").arg(i+1).arg(m_recentFiles[i]),
                             this, SLOT(slotFileOpenRecent(int)), 0, i);
}

void MainWindow::updateMenuDatabases()
{
  int i = 0;
  m_menuDatabases->clear();
  QListIterator<DatabaseInfo*> it(m_databases);
  while (it.hasNext() && i < 10)
  {
    int key = Qt::CTRL + Qt::Key_1 + (i-1);
    m_menuDatabases->insertItem(QString("&%1: %2").arg(i).arg(databaseName(it.next()->database()->filename())),
        this, SLOT(slotDatabaseChange(int)), key, i);
    i++;
  }
}

bool MainWindow::openDatabase(const QString& fname)
{
  /* Check if the database isn't already open */
  for (int i = 0; i < m_databases.count(); i++)
    if (m_databases[i]->filename() == fname)
    {
      slotDatabaseChange(i);
      return false;
    }

  m_databases.append(new DatabaseInfo(fname));
  m_currentDatabase = m_databases.count() - 1;
  m_recentFiles.append(fname);
  updateMenuRecent();
  updateMenuDatabases();
  slotStatusMessage(tr("Database %1 opened successfully.").arg(fname.section('/', -1)));
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
    AppSettings->readLayout(m_playerDialog, Settings::Show);
  }
  return m_playerDialog;
}

HelpWindow* MainWindow::helpWindow()
{
  if (!m_helpWindow)
  {
    m_helpWindow = new HelpWindow();
    AppSettings->readLayout(m_helpWindow, Settings::Show);
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

void MainWindow::slotFileOpenRecent(int id)
{
  QString file = m_recentFiles[id];
  if (!file.isEmpty())
    openDatabase(file);
}

void MainWindow::slotFileClose()
{
  if (m_currentDatabase) // Clipboard
  {
    m_databases.removeAt(m_currentDatabase);
    m_currentDatabase--;
    updateMenuDatabases();
    slotDatabaseChanged();
  }
}

void MainWindow::slotFileQuit()
{
  qApp->closeAllWindows();
}

void MainWindow::slotAbout()
{
  QMessageBox dialog(tr(""), tr("<h1>ChessX</h1>"
    "<p>Copyright 2005-2006 ChessX developers</p>"
     "<p>"
        "William Hoggarth, Ejner Borgbjerg, Marius Roets (database)<br>"
        "Michal Rudolf, Heinz Hopfgartner (GUI)<br>"
    "</p>"),
    QMessageBox::NoIcon, QMessageBox::Ok, Qt::NoButton, Qt::NoButton, this); 
  dialog.exec();
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
  Board b = m_boardView->board();
  QApplication::clipboard()->setText(b.toFEN());
}

void MainWindow::slotEditPasteFEN()
{
  Board b = m_boardView->board();
  QString fen = QApplication::clipboard()->text();
  b.fromFEN(fen);
  m_boardView->setBoard(b);
}

void MainWindow::slotEditBoard()
{
  BoardSetupDialog B;
  B.setBoard(m_boardView->board());
  if (B.exec() == QDialog::Accepted)
    m_boardView->setBoard(B.board());
}


void MainWindow::slotHelp()
{
  helpWindow()->show();
}

void MainWindow::slotMove(Square from, Square to)
{
  Board board = game()->board();
  Move m(board, from, to);
  if (board.isLegal(m))
  {
    game()->replaceMove(m);
    game()->forward();
    m_boardView->setBoard(game()->board());
  }
}

void MainWindow::slotMoveViewUpdate()
{
  Game* g = game();
  QString white = g->tag("White");
  QString black = g->tag("Black");
  QString players = tr("Game %1: <a href=\"tag:white\">%2</a> %3 - <a href=\"tag:black\">%4</a> %5")
      .arg(gameIndex() + 1).arg(white).arg(g->tag("WhiteElo")).arg(black).arg(g->tag("BlackElo"));
  QString result = tr("%1(%2) %3").arg(g->tag("Result")).arg(g->plyCount() / 2)
      .arg(g->tag("ECO"));
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
  if (wheel & Qt::AltButton)
    if (wheel & BoardView::WheelDown) slotGameMoveLast(); else slotGameMoveFirst();
  else if (wheel & Qt::ControlButton)
    if (wheel & BoardView::WheelDown) slotGameMoveNextN(); else slotGameMovePreviousN();
  else 
    if (wheel & BoardView::WheelDown) slotGameMoveNext(); else slotGameMovePrevious();
}

void MainWindow::slotGameLoadRandom()
{
   gameLoad(rand() % database()->count());
}

void MainWindow::slotGameSave()
{
  if (m_saveDialog->exec(game()) == QMessageBox::Ok)
    slotMoveViewUpdate();
}

void MainWindow::slotGameView()
{
  int ply = game()->ply();
  m_gameView->setText(m_output->output(game()));
  game()->moveToPly(ply);
}

void MainWindow::slotGameViewLink(const QUrl& url)
{
  if (url.protocol() == "move")
  {
    if (url.path() == "prev") game()->backward();
    else if (url.path() == "next") game()->forward();
    else if (url.path() == "exit") game()->exitVariation();
    else game()->moveToId(url.path().toInt());
    m_boardView->setBoard(game()->board());
  }
  else if (url.protocol() == "tag")
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

void MainWindow::slotGameViewToggle()
{
  if (m_gameView->textFormat() != Qt::PlainText)
    m_gameView->setTextFormat(Qt::PlainText);
  else
    m_gameView->setTextFormat(Qt::RichText);
  slotGameView();
}

void MainWindow::slotFilterUpdate()
{
  m_gameList->setFilter(m_databases[m_currentDatabase]->filter());
}

void MainWindow::slotFilterLoad(int index)
{
  gameLoad(index);
  setActiveWindow();
}

void MainWindow::slotStatusMessage(const QString& msg)
{
  statusBar()->message(msg, 5000);
}

void MainWindow::slotDatabaseChanged()
{
  setCaption(tr("ChessX - %1").arg(databaseName(database()->filename())));
  slotFilterUpdate();
  gameLoad(gameIndex());
}

void MainWindow::slotDatabaseChange(int current)
{
  m_currentDatabase = current;
  slotDatabaseChanged();
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
  m_menuRecent = file->addMenu(tr("&Recent files..."));
  file->addAction(createAction(tr("&Close"), SLOT(slotFileClose()), Qt::CTRL + Qt::Key_W));
  file->addAction(createAction(tr("&Quit"), SLOT(slotFileQuit()), Qt::CTRL + Qt::Key_Q));

  /* Edit menu */
  QMenu* edit = menuBar()->addMenu(tr("&Edit"));
  edit->addAction(createAction(tr("&Copy FEN"), SLOT(slotEditCopyFEN()), Qt::CTRL + Qt::SHIFT + Qt::Key_C));
  edit->addAction(createAction(tr("&Paste FEN"), SLOT(slotEditPasteFEN()), Qt::CTRL + Qt::SHIFT + Qt::Key_V));
  edit->addAction(createAction(tr("&Edit board..."), SLOT(slotEditBoard()), Qt::CTRL + Qt::SHIFT + Qt::Key_S));

  /* Game menu */
  QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));
  QMenu* loadMenu = gameMenu->addMenu(tr("&Load..."));

  /* Game->Load submenu */
  loadMenu->addAction(createAction(tr("&First"), SLOT(slotGameLoadFirst()), Qt::CTRL + Qt::SHIFT + Qt::Key_Up));
  loadMenu->addAction(createAction(tr("&Last"), SLOT(slotGameLoadLast()), Qt::CTRL + Qt::SHIFT + Qt::Key_Down));
  loadMenu->addAction(createAction(tr("&Next"), SLOT(slotGameLoadNext()), Qt::CTRL + Qt::Key_Down));
  loadMenu->addAction(createAction(tr("&Previous"), SLOT(slotGameLoadPrevious()), Qt::CTRL + Qt::Key_Up));
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
  menuBar()->insertSeparator();
  QMenu *help = menuBar()->addMenu(tr("&Help"));
  help->addAction(createAction(tr("ChessX &help..."), SLOT(slotHelp()), Qt::CTRL + Qt::Key_F10));
  help->addAction(createAction(tr("&About..."), SLOT(slotAbout()) ));
  help->insertSeparator();
  QMenu* debug = help->addMenu(tr("&Debug"));
  debug->addAction(createAction("Toggle game view format", SLOT(slotGameViewToggle()),
                                        Qt::Key_F12));

}


