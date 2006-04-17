/***************************************************************************
                          mainwindow.cpp  -  main window
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
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
#include "helpwindow.h"
#include "mainwindow.h"
#include "playerdatabase.h"
#include "playerdialog.h"
#include "preferences.h"
#include "savedialog.h"
#include "settings.h"

#include <qapplication.h>
#include <qclipboard.h>
#include <qlabel.h>
#include <qlayout.h>
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

  /* Save dialog */
  m_saveDialog = new SaveDialog;
  
  /* File menu */
  QPopupMenu *file = new QPopupMenu(this);
  menuBar()->insertItem(tr("&File"), file);
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
  QPopupMenu* goMenu = new QPopupMenu(this);
  goMenu->insertItem(tr("&Start"), this, SLOT(slotMoveToStart()), Key_Home);
  goMenu->insertItem(tr("&End"), this, SLOT(slotMoveToEnd()), Key_End);
  goMenu->insertItem(tr("&Next move"), this, SLOT(slotMoveForward()), Key_Right);
  goMenu->insertItem(tr("&Previous move"), this, SLOT(slotMoveBackward()), Key_Left);
  gameMenu->insertItem(tr("&Go..."), goMenu);
  gameMenu->insertItem(tr("&Save...."), this, SLOT(slotGameSave()), CTRL + Key_S);

  /* Windows menu */
  QPopupMenu *windows = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Windows"), windows);
  windows ->insertItem(tr("&Player Database..."), this, SLOT(slotPlayerDialog()), CTRL + SHIFT + Key_P);

  /* Settings menu */
  QPopupMenu *settings = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Settings"), settings);
  settings ->insertItem(tr("&Configure ChessX..."), this, SLOT(slotConfigure()));
	settings ->insertItem(tr("Configure Chess &Engines..."), this, SLOT(slotConfigureChessEngines()));
  settings ->insertItem(tr("&Flip board"), this, SLOT(slotConfigureFlip()), CTRL + Key_B);

  /* Help menu */
  menuBar()->insertSeparator();
  QPopupMenu *help = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Help"), help);
  help->insertItem( tr( "ChessX &help..." ), this, SLOT( slotHelp()), Key_F4 );
  help->insertItem(tr("&About..."), this, SLOT(slotAbout()), Key_F1);
  statusBar()->message(tr("Ready"), 2000);
  resize(450, 600);

  /* Game */
  m_game = new Game;
  m_game->setTag("White", "Botvinnik");
  m_game->setTag("Black", "Polugaevsky");

  /* Layout */
  QFrame* frame = new QFrame(this);
  setCentralWidget(frame);
  QGridLayout* layout = new QGridLayout(frame, 2, 1);

  /* Board */
  layout->addWidget(m_boardView = new BoardView(frame), 0, 0);
  layout->setRowStretch(0, 5);
  connect(this, SIGNAL(reconfigure()), m_boardView, SLOT(configure()));
  connect(m_boardView, SIGNAL(moveMade(Square, Square)), SLOT(slotMove(Square, Square)));
  connect(m_boardView, SIGNAL(changed()), SLOT(slotMoveViewUpdate()));

  /* Move view */
 layout->addWidget(m_moveView = new QTextBrowser(frame), 1, 0);
 connect(m_moveView, SIGNAL(linkClicked(const QString&)), SLOT(slotMoveViewLink(const QString&)));
 layout->setRowStretch(1, 1);
 //m_moveView->styleSheet()->item("a")->setFontWeight(QFont::Bold);
 m_boardView->setBoard(m_game->board());

  /* Restoring layouts */
  AppSettings->readLayout(m_playerDialog, Settings::Show);
  AppSettings->readLayout(m_helpWindow, Settings::Show);
  AppSettings->readLayout(this);
  emit reconfigure();

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

void MainWindow::slotAbout()
{
  QMessageBox::about(this, tr("ChessX"),
      tr("Chess Database\n(C) 2005 William Hoggarth, Michal Rudolf and Ejner Borgbjerg"));
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
    m_game->forward(1);
    m_boardView->setBoard(m_game->board());
  }
}

void MainWindow::slotMoveForward()
{
  if (m_game->forward())
    m_boardView->setBoard(m_game->board());
}

void MainWindow::slotMoveBackward()
{
  if (m_game->backward())
    m_boardView->setBoard(m_game->board());
}

void MainWindow::slotMoveToStart()
{
  if (m_game->backward(9999))
    m_boardView->setBoard(m_game->board());
}

void MainWindow::slotMoveToEnd()
{
  if (m_game->forward(9999))
    m_boardView->setBoard(m_game->board());
}

void MainWindow::slotMoveViewUpdate()
{
  QString white = m_game->tag("White");
  QString black = m_game->tag("Black");
  QString players = tr("Game %1: <a href=\"player_%2\">%3</a> %4 - <a href=\"player_%5\">%6</a> %7").arg(1).arg(white).arg(white)
      .arg(m_game->tag("WhiteElo")).arg(black).arg(black).arg(m_game->tag("BlackElo"));
  QString result = tr("%1(%2) %3").arg(m_game->tag("Result")).arg(m_game->plyCount() / 2)
      .arg(m_game->tag("ECO"));
  QString header = tr("%1, %2, %3, round %4").arg(m_game->tag("Event")).arg(m_game->tag("Site"))
      .arg(m_game->tag("Date")).arg(m_game->tag("Round"));
  QString lastmove, nextmove;
  if (!m_game->atStart())
  {
    m_game->backward();
    lastmove = QString("<a href=\"backward\">%1%2 %3</a>").arg(m_game->ply() / 2 + 1)
        .arg(m_game->ply() % 2 ? "..." : ".")
        .arg(m_game->board().moveToSAN(m_game->move(0)));
    m_game->forward();
  }
  else
    lastmove = tr("(Start of game)");
  if (!m_game->atEnd())
  {
    nextmove = QString("<a href=\"forward\">%1%2 %3</a>")
        .arg(m_game->ply() / 2 + 1)
        .arg(m_game->ply() % 2 ? "..." : ".")
        .arg(m_game->board().moveToSAN(m_game->move(0)));
  }
  else
    nextmove = tr("(End of game)");
  QString move = tr("Last move: %1 &nbsp; &nbsp; Next: %2").arg(lastmove).arg(nextmove);
  m_moveView->setText(QString("<qt>%1<br>%2<br>%3<br>%4<qt>").arg(players).arg(result)
      .arg(header).arg(move));
}

void MainWindow::slotMoveViewLink(const QString& link)
{
  if (link == "backward")
    slotMoveBackward();
  else if (link == "forward")
    slotMoveForward();
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
