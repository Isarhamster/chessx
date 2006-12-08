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
#include "chessbrowser.h"
#include "enginesetup.h"
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

#include <qapplication.h>
#include <qclipboard.h>
#include <q3filedialog.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qlayout.h>
#include <q3hbox.h>
#include <q3listbox.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <q3popupmenu.h>
#include <qstatusbar.h>
#include <qsplitter.h>
#include <qaction.h>
//Added by qt3to4:
#include <Q3Frame>
#include <Q3VBoxLayout>
#include <QCloseEvent>

MainWindow::MainWindow() : Q3MainWindow(0, "MainWindow", Qt::WDestructiveClose),
  m_playerDialog(0), m_helpWindow(0)
{
  /* Active database */
  m_databases.append(new DatabaseInfo);

  /* Save dialog */
  m_saveDialog = new SaveDialog;

  /* Game List Dialog */
  m_gameList = new GameList;
  connect(m_gameList, SIGNAL(selected(int)), SLOT(slotFilterLoad(int)));

  /* File menu */
  Q3PopupMenu *file = new Q3PopupMenu(this);
  menuBar()->insertItem(tr("&File"), file);
  file->insertItem(tr("&Open..."), this, SLOT(slotFileOpen()), CTRL + Key_O);
  m_menuRecent = new Q3PopupMenu(file);
  file->insertItem(tr("Open &recent..."), m_menuRecent);
  file->insertItem(tr("&Close"), this, SLOT(slotFileClose()), CTRL + Key_W);
  file->insertItem(tr("&Quit"), qApp, SLOT(closeAllWindows()), CTRL + Key_Q);

  /* Edit menu */
  Q3PopupMenu *edit = new Q3PopupMenu(this);
  menuBar()->insertItem(tr("&Edit"), edit);
  edit->insertItem(tr("&Copy FEN"), this, SLOT(slotEditCopyFEN()), CTRL + SHIFT + Key_C);
  edit->insertItem(tr("&Paste FEN"), this, SLOT(slotEditPasteFEN()), CTRL + SHIFT + Key_V);
  edit->insertItem(tr("&Edit board..."), this, SLOT(slotEditBoard()), CTRL + SHIFT + Key_S);

  /* Game menu */
  Q3PopupMenu *gameMenu = new Q3PopupMenu(this);
  menuBar()->insertItem(tr("&Game"), gameMenu);
  Q3PopupMenu* loadMenu = new Q3PopupMenu(this);
  loadMenu->insertItem(tr("&First"), this, SLOT(slotGameLoad(int)), Qt::CTRL + Qt::SHIFT + Qt::Key_Up, IdFirst);
  loadMenu->insertItem(tr("&Last"), this, SLOT(slotGameLoad(int)), Qt::CTRL + Qt::SHIFT + Qt::Key_Down, IdLast);
  loadMenu->insertItem(tr("&Next"), this, SLOT(slotGameLoad(int)), Qt::CTRL + Qt::Key_Down, IdNext);
  loadMenu->insertItem(tr("&Previous"), this, SLOT(slotGameLoad(int)), Qt::CTRL + Qt::Key_Up, IdPrevious);
  loadMenu->insertItem(tr("&Random"), this, SLOT(slotGameLoad(int)), Qt::CTRL + Qt::Key_Question, IdRandom);
  gameMenu->insertItem(tr("&Load..."), loadMenu);
  Q3PopupMenu* goMenu = new Q3PopupMenu(this);
  goMenu->insertItem(tr("&Start"), this, SLOT(slotGameBrowse(int)), Qt::Key_Home, BrowseFirstMove);
  goMenu->insertItem(tr("&End"), this, SLOT(slotGameBrowse(int)), Qt::Key_End, BrowseLastMove);
  goMenu->insertItem(tr("&Next move"), this, SLOT(slotGameBrowse(int)), Qt::Key_Right, BrowseNextMove);
  goMenu->insertItem(tr("&Previous move"), this, SLOT(slotGameBrowse(int)), Qt::Key_Left, BrowsePreviousMove);
  goMenu->insertItem(tr("5 moves &forward"), this, SLOT(slotGameBrowse(int)), Qt::Key_Down, BrowseNextMoves);
  goMenu->insertItem(tr("5 moves &backward"), this, SLOT(slotGameBrowse(int)), Qt::Key_Up, BrowsePreviousMoves);
  gameMenu->insertItem(tr("&Go..."), goMenu);
  gameMenu->insertItem(tr("&Save...."), this, SLOT(slotGameSave()), CTRL + Key_S);

  /* Windows menu */
  Q3PopupMenu *view = new Q3PopupMenu(this);
  menuBar()->insertItem(tr("&View"), view);
  m_menuDatabases = new Q3PopupMenu(view);
  view->insertItem(tr("&Database"), m_menuDatabases);
  view->insertItem(tr("&Game list"), this, SLOT(slotFilterSwitch()), CTRL + Key_L);
  view->insertItem(tr("&Player Database..."), this, SLOT(slotPlayerDialog()), CTRL + SHIFT + Key_P);

  /* Settings menu */
  Q3PopupMenu *settings = new Q3PopupMenu(this);
  menuBar()->insertItem(tr("&Settings"), settings);
  settings ->insertItem(tr("&Configure ChessX..."), this, SLOT(slotConfigure()));
//  settings ->insertItem(tr("Chess &Engines..."), this, SLOT(slotConfigureChessEngines()));
  settings ->insertItem(tr("&Flip board"), this, SLOT(slotConfigureFlip()), CTRL + Key_B);

  /* Help menu */
  menuBar()->insertSeparator();
  Q3PopupMenu *help = new Q3PopupMenu(this);
  menuBar()->insertItem(tr("&Help"), help);
  help->insertItem( tr( "ChessX &help..." ), this, SLOT( slotHelp()), Key_F1);
  help->insertItem(tr("&About..."), this, SLOT(slotAbout()));
  resize(450, 600);

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
  m_output = new Output(Output::NotationWidget);

  /* Layout */
  Q3Frame* frame = new Q3Frame(this);
  setCentralWidget(frame);
  m_layout = new Q3VBoxLayout(frame);

  /* Board */
  Q3HBox* hbox = new Q3HBox(frame, "BGView");
  m_layout->addWidget(hbox, 5);
  m_boardView = new BoardView(hbox, "BoardView");
  hbox->setStretchFactor(m_boardView, 2);
  connect(this, SIGNAL(reconfigure()), m_boardView, SLOT(configure()));
  connect(m_boardView, SIGNAL(moveMade(Square, Square)), SLOT(slotMove(Square, Square)));
  connect(m_boardView, SIGNAL(changed()), SLOT(slotMoveViewUpdate()));
  connect(m_boardView, SIGNAL(wheelScrolled(int)), SLOT(slotGameBrowse(int)));

  /* Game view */
  m_gameView = new ChessBrowser(hbox, "GameView");
  m_gameView->setLinkUnderline(false);
  hbox->setStretchFactor(m_gameView, 2);
  QAction* gameViewToggle = new QAction(tr("Toggle game"), Key_F12, this, "gametoggle");
  connect(gameViewToggle, SIGNAL(activated()), this, SLOT(slotGameViewToggle()));

  /* Move view */
  m_layout->addWidget(m_moveView = new ChessBrowser(frame, "MoveView"));
  m_moveView->setMaximumHeight(100);
  connect(m_moveView, SIGNAL(linkPressed(const QString&)), SLOT(slotMoveViewLink(const QString&)));
  m_boardView->setBoard(game()->board());

  /* Randomize */
  srand(time(0));

  /* Restoring layouts */
  AppSettings->readLayout(this);
  emit reconfigure();

  /* Status */
  slotStatusMessage(tr("Ready."));

}

MainWindow::~MainWindow()
{
  delete m_saveDialog;
  delete m_playerDialog;
  delete m_helpWindow;
  delete m_output;
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

bool MainWindow::yesNo(const QString& question, QMessageBox::Icon icon) const
{
  QMessageBox mb("ChessX", question, icon, QMessageBox::Yes, QMessageBox::No,
     Qt::NoButton);
  return mb.exec() == QMessageBox::Yes;
}

Database* MainWindow::database()
{
  return m_databases.current()->database();
}

Game* MainWindow::game()
{
  return m_databases.current()->currentGame();
}

int MainWindow::gameIndex() const
{
  return m_databases.current()->currentIndex();
}

void MainWindow::loadGame(int index)
{
  m_databases.current()->loadGame(index);
  m_boardView->setBoard(game()->board());
  slotMoveViewUpdate();
  slotGameView();
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
  Q3PtrListIterator<DatabaseInfo> it(m_databases);
  DatabaseInfo* db;
  m_menuDatabases->clear();
  while ((db = it.current()))
  {
    int key = i < 10 ? CTRL + Key_1 + (i-1) : 0;
    if (i)
      m_menuDatabases->insertItem(QString("&%1: %2").arg(i).arg(databaseName(db->database()->name())),
         this, SLOT(slotDatabaseChange(int)), key, i);
    i++;
    ++it;
  }
}

bool MainWindow::openDatabase(const QString& fname)
{
  m_databases.append(new DatabaseInfo(fname));
  m_recentFiles.append(fname);
  updateMenuRecent();
  updateMenuDatabases();
  slotGameLoad(0);
  slotStatusMessage(tr("Database %1 opened successfully.").arg(fname.section('/', -1)));
  slotDatabaseChanged();
  return true;
}

PlayerDialog* MainWindow::playerDialog()
{
  if (!m_playerDialog)
  {
    PlayerDatabase* db = new PlayerDatabase;
    db->open("../tests/cppunit/data/small/players");
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
  QString file = Q3FileDialog::getOpenFileName(QString::null,
     tr("PGN Database (*.pgn)"), this, "open database", tr("Open database"));
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
  if (m_databases.current() != m_databases.getFirst()) // Clipboard
  {
    m_databases.remove();
    updateMenuDatabases();
    slotDatabaseChanged();
  }
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
  EngineSetupDialog dlg;
  dlg.exec();
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
    game()->moveTo(1);
    m_boardView->setBoard(game()->board());
  }
}

void MainWindow::slotMoveViewUpdate()
{
  Game* g = game();
  QString white = g->tag("White");
  QString black = g->tag("Black");
  QString players = tr("Game %1: <a href=\"player:%2\">%3</a> %4 - <a href=\"player:%5\">%6</a> %7")
      .arg(gameIndex() + 1).arg(white).arg(white)
      .arg(g->tag("WhiteElo")).arg(black).arg(black).arg(g->tag("BlackElo"));
  QString result = tr("%1(%2) %3").arg(g->tag("Result")).arg(g->plyCount() / 2)
      .arg(g->tag("ECO"));
  QString header = tr("%1, %2, %3, round %4").arg(g->tag("Event")).arg(g->tag("Site"))
      .arg(g->tag("Date")).arg(g->tag("Round"));
  QString lastmove, nextmove;
  if (!g->atStart())
    lastmove = QString("<a href=\"backward\">%1</a>").arg(g->previousMoveToSan(Game::FullDetail));
  else
    lastmove = tr("(Start of game)");
  if (!g->atEnd())
    nextmove = QString("<a href=\"forward\">%1</a>").arg(g->moveToSan(Game::FullDetail));
  else
    nextmove = g->isMainline() ? tr("(End of game)") : tr("(End of line)");
  QString move = tr("Last move: %1 &nbsp; &nbsp; Next: %2").arg(lastmove).arg(nextmove);
  if (!g->isMainline())
    move.append(QString(" &nbsp; &nbsp; <a href=\"var:exit\">%1</a>").arg(tr("(&lt;-Var)")));
  QString var;
  if (g->variationCount() > 1)
  {
    var = tr("<br>Variations: &nbsp; ");
    for (int i = 1; i < g->variationCount(); i++)
    {
      var.append(QString("v%1: <a href=\"var:%2\">%3</a>").arg(i).arg(i)
         .arg(g->moveToSan(Game::FullDetail, i)));
      if (i != g->variationCount() - 1)
        var.append(" &nbsp; ");
     }
  }
  m_moveView->setText(QString("<qt>%1<br>%2<br>%3<br>%4%5<qt>").arg(players).arg(result)
      .arg(header).arg(move).arg(var));
}

void MainWindow::slotMoveViewLink(const QString& link)
{
  QString command = link.section(':', 0, 0);
  QString arg =  link.section(':', 1);
  if (command == "backward")
    slotGameBrowse(IdPrevious);
  else if (command == "forward")
    slotGameBrowse(IdNext);
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
  }
}

const int IdChange[7] = {-99999999, 99999999, 1, -1, 10, -10, 0};
void MainWindow::slotGameLoad(int id)
{
  if (!database())
    return;
  int index = gameIndex() + IdChange[id];
  if (id == IdRandom)
    index = rand() % database()->count();
  if (index < 0) index = 0;
  if (index >= database()->count() - 1) index = database()->count() - 1;
  loadGame(index);
}

void MainWindow::slotGameBrowse(int id)
{
  int change = BoardView::movesBrowsed(id);
  if (game()->moveTo(change))
  {
    m_boardView->setBoard(game()->board());
  }
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

void MainWindow::slotGameViewToggle()
{
  if (m_gameView->textFormat() != Qt::PlainText)
    m_gameView->setTextFormat(Qt::PlainText);
  else
    m_gameView->setTextFormat(Qt::RichText);
  m_gameView->clear();
  m_gameView->setText(m_output->output(game()));
}

void MainWindow::slotFilterSwitch()
{
  if (m_gameList->isVisible())
    m_gameList->hide();
  else 
    m_gameList->show();
}

void MainWindow::slotFilterUpdate()
{
  m_gameList->setDatabase(m_databases.current());
}

void MainWindow::slotFilterLoad(int index)
{
  loadGame(index);
  setActiveWindow();
}

void MainWindow::slotStatusMessage(const QString& msg)
{
  statusBar()->message(msg, 5000);
}

void MainWindow::slotDatabaseChanged()
{
  setCaption(tr("ChessX - %1").arg(databaseName(m_databases.current()->database()->name())));
  slotFilterUpdate();
  loadGame(gameIndex());
}

void MainWindow::slotDatabaseChange(int current)
{
  m_databases.find(m_databases.at(current));
  slotDatabaseChanged();
}


