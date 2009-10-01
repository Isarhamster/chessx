/***************************************************************************
 *   (C) 2009 by Michal Rudolf mrudolf@kdewebdev.org                                        *
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
#include "version.h"

#include <time.h>

#include <QtGui>

void MainWindow::slotFileNew()
{
	QString file = QFileDialog::getSaveFileName(this, tr("New database"),
			AppSettings->value("/General/databasePath").toString(),
			tr("PGN database (*.pgn)"));
	if (file.isEmpty())
		return;
	if (!file.endsWith(".pgn"))
		file += ".pgn";
	QFile pgnfile(file);
	if (!pgnfile.open(QIODevice::WriteOnly)) 
		MessageDialog::warning(this, tr("Cannot create ChessX database."), tr("New database"));
	else {
		pgnfile.close();
		openDatabase(file);
		AppSettings->setValue("/General/databasePath",
				QFileInfo(file).absolutePath());
	}
}

void MainWindow::slotFileOpen()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open database"),
			AppSettings->value("/General/databasePath").toString(),
			tr("PGN databases (*.pgn)"));
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
	if (database()->isReadOnly())
		MessageDialog::warning(this, tr("<html>The database <i>%1</i> is read-only and cannot be saved.</html>")
				.arg(database()->name()));
	else if (m_currentDatabase && dynamic_cast<MemoryDatabase*>(database())) {
		startOperation(tr("Saving %1...").arg(database()->name()));
		Output output(Output::Pgn);
		connect(&output, SIGNAL(progress(int)), SLOT(slotOperationProgress(int)));
		output.output(database()->filename(), *database());
		finishOperation(tr("%1 saved").arg(database()->name()));
	}
}

void MainWindow::slotFileClose()
{
	if (m_currentDatabase) {// Don't remove Clipboard
		delete m_databases[m_currentDatabase];
		m_databases.removeAt(m_currentDatabase);
		if (m_currentDatabase == m_databases.count())
			m_currentDatabase--;
		updateMenuDatabases();
		slotDatabaseChanged();
	}
}

void MainWindow::slotFileExportFilter()
{
	int format;
	QString filename = exportFileName(format);
	if (!filename.isEmpty()) {
		Output output((Output::OutputType)format);
		output.output(filename, *(databaseInfo()->filter()));
	}
}

void MainWindow::slotFileExportAll()
{
	int format;
	QString filename = exportFileName(format);
	if (!filename.isEmpty()) {
		Output output((Output::OutputType)format);
		output.output(filename, *database());
	}
}

void MainWindow::slotFileQuit()
{
	qApp->closeAllWindows();
}

void MainWindow::slotPlayerDialog()
{
	playerDialog()->setDatabase(database());
	playerDialog()->show();
}

void MainWindow::slotConfigure()
{
	PreferencesDialog P(this);
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
	Board board;
	if (!board.isValidFen(fen)) {
		QString msg = fen.length() ?
			      tr("Text in clipboard does not represent valid FEN:<br><i>%1</i>").arg(fen) :
			      tr("There is no text in clipboard.");
		MessageDialog::warning(this, msg);
		return;
	}
	board.fromFen(fen);
	if (board.validate() != Valid) {
		MessageDialog::warning(this, tr("The clipboard contains FEN, but with illegal position. "
						"You can only paste such positions in <b>Setup position</b> dialog."));
		return;
	}
	game().setStartingBoard(board);
	slotGameChanged();
}

void MainWindow::slotEditTruncateEnd()
{
	game().truncateVariation(Game::AfterMove);
	slotGameChanged();
}

void MainWindow::slotEditTruncateStart()
{
	game().truncateVariation(Game::BeforeMove);
	slotGameChanged();
}

void MainWindow::slotEditBoard()
{
	BoardSetupDialog B(this);
	B.setBoard(game().board());
	B.setFlipped(m_boardView->isFlipped());
	if (B.exec() == QDialog::Accepted) {
		game().setStartingBoard(B.board());
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
	QString fastbits = Board::fastbitsOption ? tr("<br>Compiled with 'fastbits' option") : "";
	QMessageBox dialog(tr(""), tr("<h1>ChessX</h1>"
				      "<p>Free chess database available under GPLv2.<br>Version %1%2<br>"
				      "Copyright 2005-2009 ChessX developers"
				      "<p>Current developer and maintainer: Michal Rudolf <a href=\"mailto:mrudolf"
				      "@kdewebdev.org\">&lt;mrudolf"
				      "@kdewebdev.org&gt;</a>"
				      "<p>Additional coding: Marius Roets, Sean Estabrooks, Rico Zenklusen, "
				      "Ejner Borgbjerg, Heinz Hopfgartner, William Hoggarth."
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
		if (game().atLineEnd())
			game().addMove(m);
		else {
			// Find how way we should add new move
			QMessageBox mbox(QMessageBox::Question, tr("Add move"),
					 tr("There is already next move in current game. What do you want to do?"), QMessageBox::Cancel, this);
			QPushButton* addVar = mbox.addButton(tr("Add variation"), QMessageBox::YesRole);
			QPushButton* newMain = mbox.addButton(tr("Add new mainline"), QMessageBox::AcceptRole);
			QPushButton* replaceMain = mbox.addButton(tr("Replace current move"), QMessageBox::DestructiveRole);
			mbox.exec();
			if (mbox.clickedButton() == addVar)
				game().addVariation(m);
			else if (mbox.clickedButton() == newMain)
				game().promoteVariation(game().addVariation(m));
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
	bool remove = game().atLineEnd();
	int var = game().variationNumber();
	gameMoveBy(-1);
	if (remove) {
		if (var && game().isMainline())
			game().removeVariation(var);
		else
			game().truncateVariation();
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
	m_gameView->showMove(g.currentMove());

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
	if (!g.atGameStart())
		lastmove = QString("<a href=\"move:prev\">%1</a>").arg(g.moveToSan(Game::FullDetail, Game::PreviousMove));
	else
		lastmove = tr("(Start of game)");
	if (!g.atGameEnd())
		nextmove = QString("<a href=\"move:next\">%1</a>").arg(g.moveToSan(Game::FullDetail, Game::NextMove));
	else
		nextmove = g.isMainline() ? tr("(End of game)") : tr("(End of line)");
	QString move = tr("Last move: %1 &nbsp; &nbsp; Next: %2").arg(lastmove).arg(nextmove);
	if (!g.isMainline())
		move.append(QString(" &nbsp; &nbsp; <a href=\"move:exit\">%1</a>").arg(tr("(&lt;-Var)")));
	QString var;
	if (g.variationCount()) {
		var = tr("<br>Variations: &nbsp; ");
		QList <int> variations = g.variations();
		for (int i = 1; i <= variations.size(); i++) {
			var.append(QString("v%1: <a href=\"move:%2\">%3</a>").arg(i).arg(variations[i-1])
				   .arg(g.moveToSan(Game::FullDetail, Game::PreviousMove, variations[i-1])));
			if (i != variations.size())
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
	m_gameList->setFocus();
}

void MainWindow::slotGameLoadLast()
{
	gameLoad(databaseInfo()->filter()->indexToGame(databaseInfo()->filter()->count() - 1));
	m_gameList->setFocus();
}

void MainWindow::slotGameLoadPrevious()
{
	int game = m_gameList->currentIndex().row();
	game = databaseInfo()->filter()->indexToGame(game);
	game = databaseInfo()->filter()->previousGame(game);
	if (game != -1) {
		m_gameList->selectGame(game);
		m_gameList->setFocus();
		m_pending = PendingLoad(database(), game);
		m_timer->start();
	}
}

void MainWindow::slotGameLoadNext()
{
	int game = m_gameList->currentIndex().row();
	game = databaseInfo()->filter()->indexToGame(game);
	game = databaseInfo()->filter()->nextGame(game);
	if (game != -1) {
		m_gameList->selectGame(game);
		m_gameList->setFocus();
		m_pending = PendingLoad(database(), game);
		m_timer->start();
	}
}

void MainWindow::slotGameLoadPending()
{
	if (m_pending.database == database())
		gameLoad(m_pending.game);
}


void MainWindow::slotGameLoadRandom()
{
	if (databaseInfo()->filter()->count()) {
		int random = rand() % databaseInfo()->filter()->count();
		gameLoad(databaseInfo()->filter()->indexToGame(random));
		m_gameList->setFocus();
	}
}

void MainWindow::slotGameLoadChosen()
{
	int index = QInputDialog::getInteger(this, tr("Load Game"), tr("Game number:"), gameIndex() + 1,
					     1, database()->count());
	gameLoad(index - 1);
	m_gameList->setFocus();
}

void MainWindow::slotGameNew()
{
	if (database()->isReadOnly())
		MessageDialog::error(this, tr("This database is read only."));
	else {
		databaseInfo()->newGame();
		slotGameChanged();
	}
}

void MainWindow::slotGameSave()
{
	if (database()->isReadOnly())
		MessageDialog::error(this, tr("This database is read only."));
	else if (saveDialog()->exec(database(), game()) == QDialog::Accepted) {
		databaseInfo()->saveGame();
		slotDatabaseChanged();
	}
}

void MainWindow::slotGameModify(int action, int move)
{
	game().moveToId(move);
	slotMoveChanged();
	switch (action) {
	case ChessBrowser::RemoveNextMoves:
		game().truncateVariation();
		break;
	case ChessBrowser::RemovePreviousMoves:
		game().truncateVariation(Game::BeforeMove);
		break;
	case ChessBrowser::RemoveVariation: {
		game().removeVariation(game().variationNumber());
		break;
	}
	case ChessBrowser::EditPrecomment:
		if (!gameEditComment(Output::Precomment))
			return;
		break;
	case ChessBrowser::EditComment:
		if (!gameEditComment(Output::Comment))
			return;
		break;
	default:
		;
	}
	slotGameChanged();
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
		else if (url.path() == "exit") game().moveToId(game().parentMove());
		else
			game().moveToId(url.path().toInt());
		slotMoveChanged();
	} else if (url.scheme() == "precmt" || url.scheme() == "cmt") {
		game().moveToId(url.path().toInt());
		slotMoveChanged();
		Output::CommentType type = url.scheme() == "cmt" ? Output::Comment : Output::Precomment;
		if (gameEditComment(type))
			slotGameChanged();
	} else if (url.scheme() == "egtb") {
		if (!game().atGameEnd())
			game().addVariation(url.path());
		else
			game().addMove(url.path());
		game().forward();
		slotGameChanged();
	} else if (url.scheme() == "tag") {
		playerDialog()->setDatabase(database());
		if (url.path() == "white")
			playerDialog()->showPlayer(game().tag("White"));
		else if (url.path() == "black")
			playerDialog()->showPlayer(game().tag("Black"));
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
	gameLoad(index);
	activateWindow();
}

void MainWindow::slotStatusMessage(const QString& msg)
{
	statusBar()->showMessage(msg);
}

void MainWindow::slotOperationProgress(int progress)
{
	m_progressBar->setValue(progress);
	qApp->processEvents();
}


void MainWindow::slotDatabaseChange()
{
	QAction* action = qobject_cast<QAction*>(sender());
	if (action && m_currentDatabase != action->data().toInt()) {
		m_currentDatabase = action->data().toInt();
		slotDatabaseChanged();
	}
}

void MainWindow::slotDatabaseCopy()
{
	if (m_databases.count() < 2) {
		MessageDialog::error(this, tr("You need at least two open databases to copy games"));
		return;
	}
	CopyDialog dlg(this);
	QStringList db;
	for (int i = 0; i < m_databases.count(); i++)
		if (i != m_currentDatabase)
			db.append(tr("%1. %2 (%3 games)").arg(i).arg(databaseName(i))
				  .arg(m_databases[i]->database()->count()));
	dlg.setDatabases(db);
	if (dlg.exec() != QDialog::Accepted)
		return;
	int target = dlg.getDatabase();
	if (target >= m_currentDatabase)
		target++;
	Game g;
	switch (dlg.getMode()) {
	case CopyDialog::SingleGame:
		m_databases[target]->database()->appendGame(game());
		break;
	case CopyDialog::Filter:
		for (int i = 0; i < database()->count(); i++)
			if (databaseInfo()->filter()->contains(i) && database()->loadGame(i, g))
				m_databases[target]->database()->appendGame(g);
		break;
	case CopyDialog::AllGames:
		for (int i = 0; i < database()->count(); i++)
			if (database()->loadGame(i, g))
				m_databases[target]->database()->appendGame(g);
		break;
	default:
		;
	}
	m_databases[target]->filter()->resize(m_databases[target]->database()->count(), 1);
}


void MainWindow::slotDatabaseChanged()
{
	setWindowTitle(tr("ChessX - %1").arg(databaseName()));
	m_gameList->setFilter(databaseInfo()->filter());
	slotFilterChanged();
	gameLoad(gameIndex(), true, true);
	if (m_playerDialog && playerDialog()->isVisible())
		playerDialog()->setDatabase(database());
	
	emit databaseChanged(databaseInfo());
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
	if (!m_openingTree->isVisible())
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
	if (!m.isLegal())
		return;
	else if (m == game().move(game().nextMove()))
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

void MainWindow::slotDatabaseDeleteGame()
{
	database()->remove(gameIndex());
	m_gameList->updateFilter();
}

void MainWindow::slotDatabaseDeleteFilter()
{
	database()->remove(*databaseInfo()->filter());
	m_gameList->updateFilter();
}

void MainWindow::slotDatabaseCompact()
{
	database()->compact();
	databaseInfo()->resetFilter();
	slotDatabaseChanged();
	m_gameList->updateFilter();
}


