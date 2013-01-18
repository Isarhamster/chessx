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
#include "databaselist.h"
#include "editaction.h"
#include "eventlistwidget.h"
#include "filtermodel.h"
#include "game.h"
#include "gamelist.h"
#include "mainwindow.h"
#include "messagedialog.h"
#include "memorydatabase.h"
#include "openingtree.h"
#include "output.h"
#include "pgndatabase.h"
#include "playerlistwidget.h"
#include "preferences.h"
#include "savedialog.h"
#include "settings.h"
#include "tablebase.h"
#include "tableview.h"
#include "analysiswidget.h"
#include "version.h"

#include <time.h>

#include <QtGui>
#include <QAction>
#include <QFileDialog>
#include <QInputDialog>
#include <QPixmap>
#include <QProgressBar>
#include <QStatusBar>


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
		MessageDialog::warning(tr("Cannot create ChessX database."), tr("New database"));
	else {
		pgnfile.close();
		openDatabase(file);
		AppSettings->setValue("/General/databasePath",
				QFileInfo(file).absolutePath());
	}
}

void MainWindow::slotFileOpen()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open database"),
            AppSettings->value("/General/databasePath").toString(),
            tr("PGN databases (*.pgn)"));
    foreach (QString file, files)
    {
        if (!file.isEmpty()) {
            AppSettings->setValue("/General/databasePath", QFileInfo(file).absolutePath());
            openDatabaseUrl(file,false);
        }
    }
}

void MainWindow::slotFileOpenUtf8()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open database"),
            AppSettings->value("/General/databasePath").toString(),
            tr("PGN databases (*.pgn)"));
    foreach (QString file, files)
    {
        if (!file.isEmpty()) {
            AppSettings->setValue("/General/databasePath", QFileInfo(file).absolutePath());
            openDatabaseUrl(file, true);
        }
    }
}

void MainWindow::slotFileOpenRecent()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		openDatabase(action->data().toString());
}

void MainWindow::saveDatabase()
{
    if (!database()->isReadOnly() && database()->isModified())
    {
        Output output(Output::Pgn);
        output.output(database()->filename(), *database());
    }
}

bool MainWindow::QuerySaveDatabase()
{
    if (QuerySaveGame())
    {
        if (m_currentDatabase && qobject_cast<MemoryDatabase*>(database()))
        {
            if (database()->isModified())
            {
                int result = MessageDialog::yesNoCancel(tr("The current database is modified!")
                            + '\n' + tr("Save it?"));
                if (MessageDialog::Yes == result)
                {
                     startOperation(tr("Saving %1...").arg(database()->name()));
                     Output output(Output::Pgn);
                     connect(&output, SIGNAL(progress(int)), SLOT(slotOperationProgress(int)));
                     output.output(database()->filename(), *database());
                     finishOperation(tr("%1 saved").arg(database()->name()));
                     return true;
                }
                return result != MessageDialog::Cancel;
            }
        }
        return true;
    }
    return false;
}

void MainWindow::slotFileSave()
{
	if (database()->isReadOnly())
		MessageDialog::warning(tr("<html>The database <i>%1</i> is read-only and cannot be saved.</html>")
				.arg(database()->name()));
    else if (m_currentDatabase && qobject_cast<MemoryDatabase*>(database())) {
		startOperation(tr("Saving %1...").arg(database()->name()));
		Output output(Output::Pgn);
        connect(&output, SIGNAL(progress(int)), SLOT(slotOperationProgress(int)));
		output.output(database()->filename(), *database());
		finishOperation(tr("%1 saved").arg(database()->name()));
	}
}

void MainWindow::slotFileClose()
{
    if (m_currentDatabase)
    {// Don't remove Clipboard
        if (databaseInfo()->IsLoaded())
        {
            if (QuerySaveDatabase())
            {
                m_openingTree->cancel(false);
                m_databaseList->setFileClose(databaseInfo()->filePath());
                databaseInfo()->close();
                delete databaseInfo();
                m_databases.removeAt(m_currentDatabase);
                m_prevDatabase = 0;
                m_currentDatabase = 0; // Switch to clipboard is always safe
                m_databaseList->setFileCurrent(QString());
                updateMenuDatabases();
                slotDatabaseChanged();
            }
        }
	}
}

void MainWindow::slotFileCloseIndex(int n)
{
    if (m_currentDatabase == n)
    {
        slotFileClose();
    }
    else if (n) // Don't remove Clipboard
    {
        if (m_databases[n]->IsLoaded())
        {
            m_databaseList->setFileClose(m_databases[n]->filePath());
            m_databases[n]->close();
            delete m_databases[n];
            m_databases.removeAt(n);
            if (m_currentDatabase > n)
            {
                // hack as we have just moved the index by one
                m_currentDatabase--;
                m_prevDatabase = 0;
            }
            updateMenuDatabases();
        }
    }
}

void MainWindow::slotFileCloseName(QString fname)
{
    for (int i = 0; i < m_databases.count(); i++)
    {
        if (m_databases[i]->database()->filename() == fname)
        {
            slotFileCloseIndex(i);
            return;
        }
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

void MainWindow::slotPlayerListWidget()
{
    m_playerList->setDatabase(database());
}

void MainWindow::slotEventListWidget()
{
    m_eventList->setDatabase(database());
}

void MainWindow::slotConfigure()
{
	PreferencesDialog P(this);
	connect(&P, SIGNAL(reconfigure()), SLOT(slotReconfigure()));
	P.exec();
}

void MainWindow::slotReconfigure()
{
    if (AppSettings->getValue("/MainWindow/VerticalTabs").toBool())
    {
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::VerticalTabs);
    }
    else
    {
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AllowNestedDocks);
    }
    m_recentFiles.restore();
    updateMenuRecent();
    emit reconfigure(); 	// Re-emit for children
}

void MainWindow::slotConfigureFlip()
{
	m_boardView->setFlipped(!m_boardView->isFlipped());
}

void MainWindow::slotEditCopyFEN()
{
	QApplication::clipboard()->setText(game().toFen());
}

void MainWindow::slotEditCopyHumanFEN()
{
    QApplication::clipboard()->setText(game().toHumanFen());
}

void MainWindow::slotEditCopyPGN()
{
    Output output(Output::Pgn);
    QString pgn = output.output(&game());
    QApplication::clipboard()->setText(pgn);
}


void MainWindow::slotEditComment()
{
	if (gameEditComment(Output::Comment))
		slotGameChanged();
}

void MainWindow::slotEditCommentBefore()
{
    if (gameEditComment(Output::Precomment))
        slotGameChanged();
}

void MainWindow::slotEditVarPromote()
{
	if (!game().isMainline()) {
		game().promoteVariation(game().currentMove());
		slotGameChanged();
	}
}

void MainWindow::slotEditVarRemove()
{
	if (!game().isMainline()) {
		game().removeVariation(game().currentMove());
		slotGameChanged();
	}
}

void MainWindow::slotEditPasteFEN()
{
	QString fen = QApplication::clipboard()->text().trimmed();
	Board board;
	if (!board.isValidFen(fen)) {
		QString msg = fen.length() ?
					tr("Text in clipboard does not represent valid FEN:<br><i>%1</i>").arg(fen) :
					tr("There is no text in clipboard.");
		MessageDialog::warning(msg);
		return;
	}
	board.fromFen(fen);
	if (board.validate() != Valid) {
		MessageDialog::warning(tr("The clipboard contains FEN, but with illegal position. "
						"You can only paste such positions in <b>Setup position</b> dialog."));
		return;
	}
	game().setStartingBoard(board);
	slotGameChanged();
}

void MainWindow::slotEditPastePGN()
{
    QString pgn = QApplication::clipboard()->text().trimmed();
    if (!pgn.isEmpty())
    {
        MemoryDatabase pgnDatabase;
        pgnDatabase.openString(pgn);
        Game g;
        if (pgnDatabase.loadGame(0,g))
        {
            slotGameNew();
            game() = g;
            slotGameChanged();
        }
    }
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

/** Set position's PGN to clipboard. */
void MainWindow::slotEditCopyImage()
{
    QPixmap pixmap(m_boardView->size());
    m_boardView->render(&pixmap);
    QImage image = pixmap.toImage();
    QApplication::clipboard()->setImage(image);
}

void MainWindow::slotHelpBug()
{
	QDesktopServices::openUrl(QUrl("http://sourceforge.net/tracker/?group_id=163833&atid=829300"));
}


void MainWindow::slotBoardMove(Square from, Square to, int button)
{
    const Board& board = game().board();
    Move m(board.prepareMove(from, to));
    if (m.isLegal())
    {
        PieceType promotionPiece = None;
        if (m.isPromotion())
        {
            bool ok;
            QStringList moves;
            moves << tr("Queen") << tr("Rook") << tr("Bishop") << tr("Knight");
            int index = moves.indexOf(QInputDialog::getItem(0, tr("Promotion"), tr("Promote to:"),
                          moves, 0, false, &ok));
            if (!ok)
                return;
            promotionPiece = PieceType(Queen + index);
            m.setPromotionPiece(promotionPiece);
        }

        // Use an existing move with the correct promotion piece type if it already exists
        if( game().findNextMove(from,to,promotionPiece))
        {
            if (button & Qt::AltModifier)
            {
                // The move exists but adding a variation was requested anyhow
                // Take back the move and proceed as if the move does not yet exist
                game().backward();
            }
            else
            {
                slotGameChanged();
                return;
            }
        }

        if (game().atLineEnd())
        {
            game().addMove(m);
        }
        else
        {
            if (button & Qt::ControlModifier)
            {
                // TODO: Make a insertMove algorithm start from here
                // -> need to understand the moving stuff before!
                game().replaceMove(m);
            }
            else
            {
                game().addVariation(m);
            }
            game().forward();
        }
		slotGameChanged();
	}
}

void MainWindow::slotBoardClick(Square s, int button, QPoint pos)
{
    if (button & Qt::RightButton)
    {
        if (button & Qt::ShiftModifier)
        {
            QMenu* menu = new QMenu(this);
            m_annotationSquare = s;
            menu->addAction(QIcon(":/images/square_red.png"),   tr("Red Square"),    this, SLOT(slotRedSquare()));
            menu->addAction(QIcon(":/images/square_yellow.png"),tr("Yellow Square"), this, SLOT(slotYellowSquare()));
            menu->addAction(QIcon(":/images/square_green.png"), tr("Green Square"),  this, SLOT(slotGreenSquare()));
            menu->addAction(QIcon(":/images/square_none.png"),  tr("Remove Color"),  this, SLOT(slotNoColorSquare()));
            menu->addSeparator();
            menu->addAction(tr("Red Arrow to here"));
            menu->addAction(tr("Yellow Arrow to here"));
            menu->addAction(tr("Green Arrow to here"));
            menu->addAction(tr("Remove Arrow to here"));
            menu->exec(pos);
        }
        else
        {
            bool nextGuess = AppSettings->getValue("/Board/nextGuess").toBool();
            if (button & Qt::ControlModifier) nextGuess = !nextGuess; // CTRL selects the other mapping
            if (!nextGuess)
            {
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
            else
            {
                m_boardView->nextGuess(s);
            }
        }
    }
}

void MainWindow::slotMoveChanged()
{
    const Game& g = game();

	// Set board first
    QString fen = m_boardView->board().toFen();
    m_boardView->setBoard(g.board(), m_currentFrom, m_currentTo);
    m_currentFrom = InvalidSquare;
    m_currentTo = InvalidSquare;

    emit displayTime(g.timeAnnotation(), g.board().toMove());

	// Highlight current move
	m_gameView->showMove(g.currentMove());

	slotSearchTree();
	emit boardChange(g.board());

	// Clear  entries
	m_nagText.clear();
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

void MainWindow::slotGameVarEnter()
{
    if (game().variationCount(game().currentMove()))
    {
		game().moveToId(game().variations().first());
		slotMoveChanged();
	}
}

void MainWindow::slotGameVarExit()
{
    if (!game().isMainline())
    {
		while (!game().atLineStart())
        {
			game().backward();
        }
		game().backward();
		slotMoveChanged();
	}
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
    if (QuerySaveGame())
    {
        int game = m_gameList->currentIndex().row();
        game = databaseInfo()->filter()->indexToGame(game);
        game = databaseInfo()->filter()->previousGame(game);
        if (game != -1)
        {
            gameLoad(game);
            m_gameList->setFocus();
        }
    }
}

void MainWindow::loadNextGame()
{
    int game = m_gameList->currentIndex().row();
    game = databaseInfo()->filter()->indexToGame(game);
    game = databaseInfo()->filter()->nextGame(game);
    if (game != -1)
    {
        gameLoad(game);
        m_gameList->setFocus();
    }
}

void MainWindow::slotGameLoadNext()
{
    if (QuerySaveGame())
    {
        loadNextGame();
    }
}

void MainWindow::slotGameLoadRandom()
{
    if (databaseInfo()->filter()->count())
    {
		int random = rand() % databaseInfo()->filter()->count();
		gameLoad(databaseInfo()->filter()->indexToGame(random));
		m_gameList->setFocus();
	}
}

void MainWindow::slotGameLoadChosen()
{
    int index = QInputDialog::getInt(this, tr("Load Game"), tr("Game number:"), gameIndex() + 1,
						  1, database()->count());
	gameLoad(index - 1);
	m_gameList->setFocus();
}

void MainWindow::slotGameNew()
{
	if (database()->isReadOnly())
    {
		MessageDialog::error(tr("This database is read only."));
    }
    else
    {
        if (QuerySaveGame())
        {
            databaseInfo()->newGame();
            slotGameChanged();
        }
	}
}

void MainWindow::saveGame()
{
    if (!database()->isReadOnly())
    {
        databaseInfo()->saveGame();
        database()->index()->setTag("Length", QString::number((game().plyCount() + 1) / 2), gameIndex() );
        m_gameList->updateFilter();
        slotFilterChanged();
        slotGameChanged();
        game().setModified(false);

        if (AppSettings->getValue("/General/autoCommitDB").toBool())
        {
            saveDatabase();
        }
    }
}

bool MainWindow::slotGameSave()
{
	if (database()->isReadOnly())
    {
		MessageDialog::error(tr("This database is read only."));
        game().setModified(false); // Do not notify more than once
        return true;
    }

    int n = saveDialog()->exec(database(), game());
    if (n==QDialog::Accepted)
    {
        saveGame();
    }
    return (n!=QDialog::Rejected);
}

void MainWindow::slotGameModify(const EditAction& action)
{
	game().moveToId(action.move());
	slotMoveChanged();
	switch (action.type()) {
	case EditAction::RemoveNextMoves:
		game().truncateVariation();
		break;
	case EditAction::RemovePreviousMoves:
		game().truncateVariation(Game::BeforeMove);
		break;
	case EditAction::RemoveVariation: {
		game().removeVariation(game().variationNumber());
		break;
	}
	case EditAction::PromoteVariation: {
		game().promoteVariation(action.move());
		break;
	}
    case EditAction::VariationUp: {
        game().moveVariationUp(action.move());
        break;
    }
    case EditAction::VariationDown: {
        game().moveVariationDown(action.move());
        break;
    }
	case EditAction::EditPrecomment:
		if (!gameEditComment(Output::Precomment))
			return;
		break;
	case EditAction::EditComment:
		if (!gameEditComment(Output::Comment))
			return;
		break;
	case EditAction::AddNag:
		game().addNag(Nag(action.data().toInt()), action.move());
		break;
	case EditAction::ClearNags:
        game().clearNags(action.move());
		break;
    case EditAction::AddNullMove:
        game().addMove(m_boardView->board().nullMove());
        break;
	default:
		break;
	}
	slotGameChanged();
}


void MainWindow::slotGameChanged()
{
	if (m_showPgnSource)
		m_gameView->setPlainText(m_output->output(&game()));
	else
		m_gameView->setText(m_output->output(&game()));

	// Finally update game information
	QString white = game().tag("White");
	QString black = game().tag("Black");
	QString eco = game().tag("ECO").left(3);
	if (eco == "?")
        eco.clear();

    if (eco.isEmpty())
    {
        eco = game().ecoClassify().left(3);
    }

	QString whiteElo = game().tag("WhiteElo");
	QString blackElo = game().tag("BlackElo");
	if (whiteElo == "?")
		whiteElo = QString();
	if (blackElo == "?")
		blackElo = QString();
	QString players = QString("<b><a href=\"tag:white\">%1</a></b> %2 - <b><a href=\"tag:black\">%3</a></b> %4")
			  .arg(white).arg(whiteElo).arg(black).arg(blackElo);
	QString result = QString("<b>%1</b> &nbsp; %2").arg(game().tag("Result")).arg(eco);
	QString site = game().tag("Site").left(30);
	QString event = game().tag("Event").left(30);
	QString header = "<i>";
	if (!event.isEmpty()) {
		header.append(site);
		if (game().result() != Unknown)
			header.append(QString(" (%1)").arg(game().tag("Round")));
		if (!site.isEmpty())
			header.append(", ");
	}
	header.append(site);
	if (!game().tag("Date").startsWith("?")) {
		if (header.length() > 4)
			header.append(", ");
		header.append(game().tag("Date"));
	}
	header.append("</i>");

	QString title;
	if (!white.isEmpty() || !black.isEmpty())
		title.append(players);
	else title.append(tr("<b>New game</b>"));
	if (game().result() != Unknown || !eco.isEmpty())
		title.append(QString(", ") + result);
	if (header.length() > 8)
		title.append(QString("<br>") + header);
    m_gameTitle->setText(QString("<qt>%1</qt>").arg(title));
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
        m_playerList->setDatabase(database());
		if (url.path() == "white")
        {
            m_playerList->slotSelectPlayer(game().tag(TagNameWhite));
        }
		else if (url.path() == "black")
        {
            m_playerList->slotSelectPlayer(game().tag(TagNameBlack));
        }
	}
}

void MainWindow::slotGameViewLink(const QString& url)
{
	slotGameViewLink(QUrl(url));
}

void MainWindow::slotGameViewToggle(bool toggled)
{
	m_showPgnSource = toggled;
	slotGameChanged();
}

void MainWindow::slotGameDumpMoveNodes()
{
    game().dumpAllMoveNodes();
}

void MainWindow::slotGameAddVariation(const Analysis& analysis)
{
	game().addVariation(analysis.variation(),
								QString::number(analysis.score() / 100.0, 'f', 2));
	slotGameChanged();
}

void MainWindow::slotGameAddVariation(const QString& san)
{
    QString s = san;
    s = s.remove(QRegExp("-.*"));
    s = s.remove(QRegExp("[0-9]*\\."));
    if (game().atLineEnd())
        game().addMove(s);
    else
        game().addVariation(s);
    slotGameChanged();
}

void MainWindow::slotGameUncomment()
{
    game().removeComments();
    slotGameChanged();
}

void MainWindow::slotGameRemoveVariations()
{
    game().removeVariations();
    slotGameChanged();
}

void MainWindow::slotToggleAutoAnalysis()
{
    slotToggleAutoPlayer();
    m_bAutoInsertAnalysis = m_autoPlayTimer->isActive();
    if (m_bAutoInsertAnalysis && !m_mainAnalysis->isEngineRunning())
    {
        MessageDialog::information(tr("Analysis Pane 1 is not running an engine for automatic analysis."), tr("Auto Analysis"));
    }
}

void MainWindow::slotToggleAutoPlayer()
{
    QAction* autoPlayAction = (QAction*) sender();
    if (autoPlayAction)
    {
        if (autoPlayAction->isChecked())
        {
            int interval = AppSettings->getValue("/Board/AutoPlayerInterval").toInt();
            if (m_autoPlayTimer->interval() != interval)
            {
                m_autoPlayTimer->setInterval(interval);
            }
            m_autoPlayTimer->start();
        }
        else
        {
            m_autoPlayTimer->stop();
        }
    }
}

void MainWindow::slotAutoPlayTimeout()
{
    if (m_bAutoInsertAnalysis && m_mainAnalysis->isEngineRunning() && (m_AutoInsertLastBoard != m_boardView->board()))
    {
        Analysis a = m_mainAnalysis->getMainLine();
        if (!a.variation().isEmpty())
        {
            Move m = a.variation().first();
            if (!game().currentNodeHasMove(m.from(), m.to()) || game().atLineEnd())
            {
                slotGameAddVariation(a);
            }
        }
        m_AutoInsertLastBoard = m_boardView->board();
    }
    if (game().atGameEnd() && AppSettings->getValue("/Board/AutoSaveAndContinue").toBool())
    {
        saveGame();
        loadNextGame();
    }
    else
    {
        slotGameMoveNext();
    }
    m_autoPlayTimer->start();
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
}

void MainWindow::slotDatabaseChange()
{
	QAction* action = qobject_cast<QAction*>(sender());
	if (action && m_currentDatabase != action->data().toInt()) {
        m_prevDatabase = m_currentDatabase;
        database()->index()->clearCache();
		m_currentDatabase = action->data().toInt();
        m_databaseList->setFileCurrent(m_databases[m_currentDatabase]->filePath());
		slotDatabaseChanged();
	}
}

void MainWindow::copyGame(int target, const Game& game)
{
    if (m_databases[target]->isValid())
    {
        // The database is open and accessible
        m_databases[target]->database()->appendGame(game);
    }
}

void MainWindow::copyGame(QString fileName, const Game& game)
{
    for (int i=0; i < m_databases.count(); ++i)
    {
        if (m_databases[i]->filePath() == fileName)
        {
            if (m_databases[i]->isValid())
            {
                copyGame(i,game);
                m_databases[i]->filter()->resize(m_databases[i]->database()->count(), true);
            }
            return;
        }
    }

    // The database is closed
    Output writer(Output::Pgn);
    Game g = game;
    writer.append(fileName, g);
    m_databaseList->update(fileName);
}

void MainWindow::slotDatabaseCopy(int preselect)
{
	if (m_databases.count() < 2) {
		MessageDialog::error(tr("You need at least two open databases to copy games"));
		return;
	}
	CopyDialog dlg(this);
    dlg.setMode((CopyDialog::SrcMode)preselect);
	QStringList db;
    for (int i = 0; i < m_databases.count(); ++i)
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
        for (int i = 0; i < database()->count(); ++i)
			if (databaseInfo()->filter()->contains(i) && database()->loadGame(i, g))
				m_databases[target]->database()->appendGame(g);
		break;
	case CopyDialog::AllGames:
        for (int i = 0; i < database()->count(); ++i)
			if (database()->loadGame(i, g))
				m_databases[target]->database()->appendGame(g);
		break;
	default:
		;
	}
    m_databases[target]->filter()->resize(m_databases[target]->database()->count(), true);
}

void MainWindow::slotDatabaseCopySingle()
{
    slotDatabaseCopy(0);
}

void MainWindow::slotDatabaseChanged()
{
    database()->index()->calculateCache();
	setWindowTitle(tr("%1 - ChessX").arg(databaseName()));
	m_gameList->setFilter(databaseInfo()->filter());
	slotFilterChanged();
    gameLoad(gameIndex()>=0 ? gameIndex() : 0, true, true);
    m_playerList->setDatabase(database());
    m_eventList->setDatabase(database());
	emit databaseChanged(databaseInfo());
}

void MainWindow::slotSearchTag()
{
	m_gameList->simpleSearch(1);
}

void MainWindow::slotSearchBoard()
{
	PositionSearch ps(databaseInfo()->filter()->database(), m_boardView->board());
    m_openingTree->cancel(false);
    slotBoardSearchStarted();
	databaseInfo()->filter()->executeSearch(ps);
    slotBoardSearchUpdate();
}

void MainWindow::slotBoardSearchUpdate()
{
    m_gameList->updateFilter();
    slotFilterChanged();
    finishOperation(tr("Search ended"));
}

void MainWindow::slotBoardSearchStarted()
{
    startOperation(tr("Updating tree..."));
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

void MainWindow::slotToggleFilter()
{
    m_gameList->m_FilterActive = m_toggleFilter->isChecked();
    m_gameList->updateFilter();
}

void MainWindow::slotTreeUpdate()
{
    if (m_gameList->m_FilterActive)
    {
        m_gameList->updateFilter();
        slotFilterChanged();
    }
    finishOperation(tr("Tree updated."));
    if (m_bGameChange)
    {
        slotGameLoadFirst();
        m_bGameChange = false;
    }
}

void MainWindow::slotTreeUpdateStarted()
{
    startOperation(tr("Updating tree..."));
}

void MainWindow::slotSearchTree()
{
    if (m_openingTreeView->isVisible() )
    {
        m_openingTree->update(*databaseInfo()->filter(), m_boardView->board(), m_gameList->m_FilterActive);
	}
}

void MainWindow::slotSearchTreeMove(const QModelIndex& index)
{
    m_bGameChange = false;
    QString move = qobject_cast<OpeningTree*>(m_openingTreeView->model())->move(index);
	Move m = m_boardView->board().parseMove(move);
	if (!m.isLegal())
		return;
	else if (m == game().move(game().nextMove()))
		slotGameMoveNext();
    else
        slotBoardMove(m.from(), m.to(), 0);
}

void MainWindow::slotDatabaseDeleteGame()
{
    int n = gameIndex();
    if (database()->deleted(n))
    {
        database()->undelete(gameIndex());
    }
    else
    {
        database()->remove(gameIndex());
    }
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

void MainWindow::slotGetGameData(Game& g)
{
    g = game();
}

bool MainWindow::slotGameMoveNext()
{
    Move m = game().move(game().nextMove());
    m_currentFrom = m.from();
    m_currentTo = m.to();
    return gameMoveBy(1);
}

void MainWindow::slotNoColorSquare()
{
    game().appendSquareAnnotation(m_annotationSquare, 0);
    slotGameChanged();
}

void MainWindow::slotGreenSquare()
{
    game().appendSquareAnnotation(m_annotationSquare, 'G');
    slotGameChanged();
}

void MainWindow::slotYellowSquare()
{
    game().appendSquareAnnotation(m_annotationSquare, 'Y');
    slotGameChanged();
}

void MainWindow::slotRedSquare()
{
    game().appendSquareAnnotation(m_annotationSquare, 'R');
    slotGameChanged();
}


