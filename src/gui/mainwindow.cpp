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
#include "enginesetup.h"
#include "game.h"
#include "gamelist.h"
#include "helpwindow.h"
#include "mainwindow.h"
#include "pgndatabase.h"
#include "playerdatabase.h"
#include "playerdialog.h"
#include "preferences.h"
#include "savedialog.h"
#include "settings.h"

#include <qapplication.h>
#include <qclipboard.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qstatusbar.h>
#include <qtextbrowser.h>


MainWindow::MainWindow() : QMainWindow(0, "MainWindow", WDestructiveClose)
{
  /* Database initialization */
  m_playerDatabase = new PlayerDatabase;
  m_playerDatabase->open("../tests/cppunit/data/small/players");
  m_playerDialog = new PlayerDialog(m_playerDatabase, this);
  m_helpWindow = new HelpWindow();

  /* Active database */
  m_database = 0;
  m_gameIndex = -1;

  /* Save dialog */
  m_saveDialog = new SaveDialog;

  /* Game List Dialog */
  m_gameList = new GameList;
  connect(m_gameList, SIGNAL(selected(int)), SLOT(slotFilterLoad(int)));

  /* File menu */
  QPopupMenu *file = new QPopupMenu(this);
  menuBar()->insertItem(tr("&File"), file);
  file->insertItem(tr("&Open..."), this, SLOT(slotFileOpen()), CTRL + Key_O);
  m_menuRecent = new QPopupMenu(file);
  file->insertItem(tr("Open &recent..."), m_menuRecent);
  file->insertItem(tr("&Close"), this, SLOT(slotFileClose()), CTRL + Key_W);
  file->insertItem(tr("&Quit"), qApp, SLOT(closeAllWindows()), CTRL + Key_Q);

  /* Edit menu */
  QPopupMenu *edit = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Edit"), edit);
  edit->insertItem(tr("&Copy FEN"), this, SLOT(slotEditCopyFEN()), CTRL + SHIFT + Key_C);
  edit->insertItem(tr("&Paste FEN"), this, SLOT(slotEditPasteFEN()), CTRL + SHIFT + Key_V);
  edit->insertItem(tr("&Edit board..."), this, SLOT(slotEditBoard()));

  /* Game menu */
  QPopupMenu *gameMenu = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Game"), gameMenu);
  QPopupMenu* loadMenu = new QPopupMenu(this);
  loadMenu->insertItem(tr("&First"), this, SLOT(slotGameLoad(int)), CTRL + SHIFT + Key_Up, IdFirst);
  loadMenu->insertItem(tr("&Last"), this, SLOT(slotGameLoad(int)), CTRL + SHIFT + Key_Down, IdLast);
  loadMenu->insertItem(tr("&Next"), this, SLOT(slotGameLoad(int)), CTRL + Key_Down, IdNext);
  loadMenu->insertItem(tr("&Previous"), this, SLOT(slotGameLoad(int)), CTRL + Key_Up, IdPrevious);
  loadMenu->insertItem(tr("&Random"), this, SLOT(slotGameLoad(int)), CTRL + Key_Question, IdRandom);
  gameMenu->insertItem(tr("&Load..."), loadMenu);
  QPopupMenu* goMenu = new QPopupMenu(this);
  goMenu->insertItem(tr("&Start"), this, SLOT(slotGameBrowse(int)), Key_Home, IdFirst);
  goMenu->insertItem(tr("&End"), this, SLOT(slotGameBrowse(int)), Key_End, IdLast);
  goMenu->insertItem(tr("&Next move"), this, SLOT(slotGameBrowse(int)), Key_Right, IdNext);
  goMenu->insertItem(tr("&Previous move"), this, SLOT(slotGameBrowse(int)), Key_Left, IdPrevious);
  goMenu->insertItem(tr("5 moves &forward"), this, SLOT(slotGameBrowse(int)), Key_Down, IdNext5);
  goMenu->insertItem(tr("5 moves &backward"), this, SLOT(slotGameBrowse(int)), Key_Up, IdPrevious5);
  gameMenu->insertItem(tr("&Go..."), goMenu);
  gameMenu->insertItem(tr("&Save...."), this, SLOT(slotGameSave()), CTRL + Key_S);

  /* Windows menu */
  QPopupMenu *view = new QPopupMenu(this);
  menuBar()->insertItem(tr("&View"), view);
  view->insertItem(tr("&Game list"), this, SLOT(slotFilterSwitch()), CTRL + Key_L);
  view->insertItem(tr("&Player Database..."), this, SLOT(slotPlayerDialog()), CTRL + SHIFT + Key_P);

  /* Settings menu */
  QPopupMenu *settings = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Settings"), settings);
  settings ->insertItem(tr("&Configure ChessX..."), this, SLOT(slotConfigure()));
  settings ->insertItem(tr("Chess &Engines..."), this, SLOT(slotConfigureChessEngines()));
  settings ->insertItem(tr("&Flip board"), this, SLOT(slotConfigureFlip()), CTRL + Key_B);

  /* Help menu */
  menuBar()->insertSeparator();
  QPopupMenu *help = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Help"), help);
  help->insertItem( tr( "ChessX &help..." ), this, SLOT( slotHelp()), Key_F1);
  help->insertItem(tr("&About..."), this, SLOT(slotAbout()));
  resize(450, 600);

  /* Game */
  m_game = new Game;

  /* Layout */
  QFrame* frame = new QFrame(this);
  setCentralWidget(frame);
  m_layout = new QGridLayout(frame, 2, 1);

  /* Board */
  m_layout->addWidget(m_boardView = new BoardView(frame), 0, 0);
  m_layout->setRowStretch(0, 5);
  connect(this, SIGNAL(reconfigure()), m_boardView, SLOT(configure()));
  connect(m_boardView, SIGNAL(moveMade(Square, Square)), SLOT(slotMove(Square, Square)));
  connect(m_boardView, SIGNAL(changed()), SLOT(slotMoveViewUpdate()));

  /* Recent files */
  m_recentFiles.restore("History", "Recent files");
  updateMenuRecent();

  /* Move view */
 m_layout->addWidget(m_moveView = new QTextBrowser(frame), 1, 0);
 m_moveView->setMaximumHeight(100);
 connect(m_moveView, SIGNAL(linkClicked(const QString&)), SLOT(slotMoveViewLink(const QString&)));
 //m_moveView->styleSheet()->item("a")->setFontWeight(QFont::Bold);
 m_boardView->setBoard(m_game->board());

  /* Randomize */
  srandom(time(0));

  /* Restoring layouts */
  AppSettings->readLayout(m_playerDialog, Settings::Show);
  AppSettings->readLayout(m_helpWindow, Settings::Show);
  AppSettings->readLayout(this);
  emit reconfigure();

  /* Status */
  slotStatusMessage(tr("Ready."));

}

MainWindow::~MainWindow()
{
  /* saving layouts */
  delete m_saveDialog;
  delete m_playerDialog;
  delete m_playerDatabase;
  delete m_helpWindow;
  delete m_game;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
  if (yesNo(tr("Do you want to quit?"))) {
    m_recentFiles.save("History", "Recent files");
    AppSettings->writeLayout(m_playerDialog);
    AppSettings->writeLayout(m_helpWindow);
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
     QMessageBox::NoButton);
  return mb.exec() == QMessageBox::Yes;
}

void MainWindow::loadGame(int index)
{
  if (database() && activeGameIndex() != index)
  {
    database()->load(index, *m_game);
    m_gameIndex = index;
    m_game->moveToStart();
    m_boardView->setBoard(m_game->board());
    slotMoveViewUpdate();
  }
}

void MainWindow::updateMenuRecent()
{
  m_menuRecent->clear();
  for (int i = 0; i < 5 && i < m_recentFiles.count(); i++)
    m_menuRecent->insertItem(QString("&%1: %2").arg(i+1).arg(m_recentFiles[i]), 
                             this, SLOT(slotFileOpenRecent(int)), 0, i);
}

void MainWindow::slotAbout()
{
  QMessageBox dialog(tr(""), tr("<h1>ChessX</h1>"
    "<p>Copyright 2005-2006 ChessX developers</p>"
     "<p>"
        "William Hoggarth, Ejner Borgbjerg, Marius Roets (database)<br>"
        "Michal Rudolf, Heinz Hopfgartner (GUI)<br>"
    "</p>"),
    QMessageBox::NoIcon, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton, this); 
  dialog.exec();
}

void MainWindow::slotPlayerDialog()
{
  m_playerDialog->show();
}

void MainWindow::slotConfigure()
{
  PreferencesDialog P;
  if (P.exec() == QDialog::Accepted)
    emit reconfigure();
}

void MainWindow::slotConfigureChessEngines()
{
	EngineSetupDialog* dialog = new EngineSetupDialog();
	dialog->exec();
	delete dialog;
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
  if(!m_helpWindow->winId())
    m_helpWindow = new HelpWindow();
  m_helpWindow->show();
}

void MainWindow::slotMove(Square from, Square to)
{
  Board board = m_game->board();
  Move m(board, from, to);
  if (board.isLegal(m))
  {
    m_game->replaceMove(m);
    m_game->moveTo(1);
    m_boardView->setBoard(m_game->board());
  }
}

void MainWindow::slotMoveViewUpdate()
{
  QString white = m_game->tag("White");
  QString black = m_game->tag("Black");
  QString players = tr("Game %1: <a href=\"player_%2\">%3</a> %4 - <a href=\"player_%5\">%6</a> %7")
      .arg(activeGameIndex() + 1).arg(white).arg(white)
      .arg(m_game->tag("WhiteElo")).arg(black).arg(black).arg(m_game->tag("BlackElo"));
  QString result = tr("%1(%2) %3").arg(m_game->tag("Result")).arg(m_game->plyCount() / 2)
      .arg(m_game->tag("ECO"));
  QString header = tr("%1, %2, %3, round %4").arg(m_game->tag("Event")).arg(m_game->tag("Site"))
      .arg(m_game->tag("Date")).arg(m_game->tag("Round"));
  QString lastmove, nextmove;
  if (!m_game->atStart())
    lastmove = QString("<a href=\"backward\">%1</a>").arg(m_game->previousMoveToSan(Game::FullDetail));
  else
    lastmove = tr("(Start of game)");
  if (!m_game->atEnd())
    nextmove = QString("<a href=\"forward\">%1</a>").arg(m_game->moveToSan(Game::FullDetail));
  else
    nextmove = m_game->isMainline() ? tr("(End of game)") : tr("(End of line)");
  QString move = tr("Last move: %1 &nbsp; &nbsp; Next: %2").arg(lastmove).arg(nextmove);
  if (!m_game->isMainline())
    move.append(QString(" &nbsp; &nbsp; <a href=\"varexit\">%1</a>").arg(tr("(&lt;-Var)")));
  QString var;
  if (m_game->variationCount() > 1)
  {
    var = tr("<br>Variations: &nbsp; ");
    for (int i = 1; i < m_game->variationCount(); i++)
    {
      var.append(QString("v%1: <a href=\"var_%2\">%3</a>").arg(i).arg(i).arg(m_game->moveToSan(Game::FullDetail, i)));
      if (i != m_game->variationCount() - 1)
        var.append(" &nbsp; ");
     }
  }
  m_moveView->setText(QString("<qt>%1<br>%2<br>%3<br>%4%5<qt>").arg(players).arg(result)
      .arg(header).arg(move).arg(var));
}

void MainWindow::slotMoveViewLink(const QString& link)
{
  if (link == "backward")
    slotGameBrowse(IdPrevious);
  else if (link == "forward")
    slotGameBrowse(IdNext);
  else if (link == "varexit")
  {
    m_game->exitVariation();
    m_boardView->setBoard(m_game->board());
  }
  else if (link.startsWith("var_"))
  {
    m_game->enterVariation(link.section('_', 1).toInt());
    m_boardView->setBoard(m_game->board());
  }
  else if (link.startsWith("player_"))
  {
    m_playerDialog->findPlayers(link.section('_', 1));
    m_playerDialog->show();
  }
}

void MainWindow::slotGameSave()
{
  if (m_saveDialog->exec(m_game) == QMessageBox::Ok)
    slotMoveViewUpdate();
}

void MainWindow::slotFileOpen()
{
  QString file = QFileDialog::getOpenFileName(QString::null,
     tr("PGN Database (*.pgn)"), this, "open database", tr("Open database"));
  if (!file.isEmpty())
  {
    slotFileClose();
    m_database = new PgnDatabase(file);
    m_recentFiles.append(file);
    slotGameLoad(0);
    slotFilterUpdate();
    updateMenuRecent();
    slotStatusMessage(tr("File %1 opened successfully.").arg(file.section('/', -1)));
  }
}

void MainWindow::slotFileOpenRecent(int id)
{
  QString file = m_recentFiles[id];
  if (!file.isEmpty())
  {
    slotFileClose();
    m_database = new PgnDatabase(file);
    m_recentFiles.append(file);
    updateMenuRecent();
    slotGameLoad(0);
    slotFilterUpdate();
    slotStatusMessage(tr("File %1 opened successfully.").arg(file.section('/', -1)));
  }
}


void MainWindow::slotFileClose()
{
  delete m_database;
  m_database = 0;
}

const int IdChange[7] = {-99999999, 99999999, 1, -1, 10, -10, 0};

void MainWindow::slotGameLoad(int id)
{
  if (!database())
    return;
  int index = activeGameIndex() + IdChange[id];
  if (id == IdRandom)
    index = random() % database()->count();
  if (index < 0) index = 0;
  if (index >= database()->count() - 1) index = database()->count() - 1;
  loadGame(index);
}

void MainWindow::slotGameBrowse(int id)
{
  int change = IdChange[id];
  if (m_game->moveTo(change))
    m_boardView->setBoard(m_game->board());
}

void MainWindow::slotFilterSwitch()
{
  if (m_gameList->isVisible()) {
    m_gameList->hide();
  }
  else {
    m_gameList->show();
  }
}

void MainWindow::slotFilterUpdate()
{
  if (database())
    m_gameList->setDatabase(database());
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

