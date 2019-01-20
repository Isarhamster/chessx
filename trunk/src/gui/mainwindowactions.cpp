/***************************************************************************
 *   (C) 2009 by Michal Rudolf mrudolf@kdewebdev.org                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "actiondialog.h"
#include "analysiswidget.h"
#include "arenabook.h"
#include "board.h"
#include "boardsearchdialog.h"
#include "boardsetup.h"
#include "boardview.h"
#include "boardviewex.h"
#include "copydialog.h"
#include "chessbrowser.h"
#include "clipboarddatabase.h"
#include "compileeco.h"
#include "databaseinfo.h"
#include "databaselist.h"
#include "dlgsavebook.h"
#include "downloadmanager.h"
#include "duplicatesearch.h"
#include "ecolistwidget.h"
#include "editaction.h"
#include "eventlistwidget.h"
#include "exclusiveactiongroup.h"
#include "ficsclient.h"
#include "ficsconsole.h"
#include "ficsdatabase.h"
#include "game.h"
#include "gameid.h"
#include "gamelist.h"
#include "gamewindow.h"
#include "GameMimeData.h"
#include "historylabel.h"
#include "mainwindow.h"
#include "matchparameterdlg.h"
#include "messagedialog.h"
#include "memorydatabase.h"
#include "openingtreewidget.h"
#include "output.h"
#include "pgndatabase.h"
#include "playerlistwidget.h"
#include "polyglotwriter.h"
#include "positionsearch.h"
#include "preferences.h"
#include "promotiondialog.h"
#include "recipientaddressdialog.h"
#include "renametagdialog.h"
#include "shellhelper.h"
#include "settings.h"
#include "streamdatabase.h"
#include "tablebase.h"
#include "tableview.h"
#include "tagdialog.h"
#include "tags.h"
#include "translatingslider.h"
#include "version.h"

#include <time.h>

#include <QtGui>
#include <QAction>
#include <QDesktopServices>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include <QPixmap>
#include <QProgressBar>
#include <QStatusBar>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

void MainWindow::slotFileNew()
{
    QString file = QFileDialog::getSaveFileName(this, tr("New database"),
                   AppSettings->value("/General/DefaultDataPath").toString(),
                   tr("PGN database (*.pgn)"));
    if(file.isEmpty())
    {
        return;
    }
    if(!file.endsWith(".pgn"))
    {
        file += ".pgn";
    }
    QFile pgnfile(file);
    if(!pgnfile.open(QIODevice::WriteOnly))
    {
        MessageDialog::warning(tr("Cannot create ChessX database."), tr("New database"));
    }
    else
    {
        pgnfile.close();
        openDatabase(file);
    }
}

void MainWindow::slotFileOpen()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open database"),
                        AppSettings->value("/General/DefaultDataPath").toString(),
                        tr("PGN databases (*.pgn);;Polyglot books (*.bin);;Arena books (*.abk);;Chessbase books (*.ctg)"));
    foreach(QString file, files)
    {
        if(!file.isEmpty())
        {
            openDatabaseUrl(file, false);
        }
    }
}

void MainWindow::slotFileOpenUtf8()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open database"),
                        AppSettings->value("/General/DefaultDataPath").toString(),
                        tr("PGN databases (*.pgn)"));
    foreach(QString file, files)
    {
        if(!file.isEmpty())
        {
            openDatabaseUrl(file, true);
        }
    }
}

void MainWindow::slotFileOpenRecent()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if(action)
    {
        openDatabase(action->data().toString());
    }
}

void MainWindow::saveDatabase(DatabaseInfo* dbInfo)
{
    if(!dbInfo->database()->isReadOnly() && dbInfo->database()->isModified())
    {
        Database* db = dbInfo->database();
        QMutexLocker m(db->mutex());
        startOperation(tr("Saving %1...").arg(db->name()));
        Output output(Output::Pgn);
        connect(&output, SIGNAL(progress(int)), SLOT(slotOperationProgress(int)));
        output.output(db->filename(), *db);
        finishOperation(tr("%1 saved").arg(db->name()));
    }
}

bool MainWindow::QuerySaveDatabase(DatabaseInfo* dbInfo)
{
    if(QuerySaveGame(dbInfo))
    {
        if(!dbInfo->isClipboard() && qobject_cast<MemoryDatabase*>(dbInfo->database()))
        {
            if(dbInfo->isValid() && dbInfo->database()->isModified())
            {
                int result = MessageDialog::yesNoCancel(tr("The selected database is modified!")
                                                        + " (" + dbInfo->database()->name() + ")\n" + tr("Save it?"));
                if(MessageDialog::Yes == result)
                {
                    saveDatabase(dbInfo);
                    return true;
                }
                return result != MessageDialog::Cancel;
            }
        }
        return true;
    }
    return false;
}

bool MainWindow::QuerySaveDatabase()
{
    return QuerySaveDatabase(databaseInfo());
}

void MainWindow::slotFileSave()
{
    if(database()->isReadOnly())
    {
        MessageDialog::warning(tr("<html>The database <i>%1</i> is read-only and cannot be saved.</html>")
                               .arg(database()->name()));
    }
    else if(!m_currentDatabase->isClipboard() && qobject_cast<MemoryDatabase*>(database()))
    {
        saveDatabase(databaseInfo());
    }
}

bool MainWindow::closeDatabaseInfo(DatabaseInfo* aboutToClose, bool dontAsk)
{
    // Don't remove Clipboard
    if(!aboutToClose->isClipboard() && aboutToClose->IsLoaded())
    {
        if(dontAsk || QuerySaveDatabase(aboutToClose))
        {
            autoGroup->untrigger();

            bool ficsDB = (qobject_cast<FicsDatabase*>(database()));
            if (ficsDB)
            {
                m_ficsClient->exitSession();
            }

            closeBoardViewForDbIndex(aboutToClose);

            m_openingTreeWidget->cancel();
            m_databaseList->setFileClose(aboutToClose->displayName(), aboutToClose->currentIndex());

            m_databases.removeAt(m_databases.indexOf(aboutToClose));
            aboutToClose->close();
            emit signalDatabaseOpenClose();
            delete aboutToClose;
            return true;
        }
    }
    return false;
}

void MainWindow::slotFileClose()
{
    if (closeDatabaseInfo(databaseInfo()))
    {
        SwitchToClipboard();
    }
}

void MainWindow::slotFileCloseIndex(int n, bool dontAsk)
{
    DatabaseInfo* aboutToClose = m_databases[n];
    if(m_currentDatabase == aboutToClose)
    {
        closeDatabaseInfo(aboutToClose, dontAsk);
        SwitchToClipboard();
    }
    else
    {
        closeDatabaseInfo(aboutToClose, dontAsk);
    }
}

void MainWindow::slotFileCloseName(QString fname)
{
    for(int i = 0; i < m_databases.count(); i++)
    {
        if(m_databases[i]->database()->filename() == fname)
        {
            slotFileCloseIndex(i);
            return;
        }
    }
}

void MainWindow::slotFileExportGame()
{
    int format;
    QString filename = exportFileName(format);
    if(!filename.isEmpty())
    {
        Output output((Output::OutputType)format);
        output.output(filename, game());
    }
}

void MainWindow::slotFileExportFilter()
{
    int format;
    QString filename = exportFileName(format);
    if(!filename.isEmpty())
    {
        Output output((Output::OutputType)format);
        output.output(filename, *(databaseInfo()->filter()));
    }
}

void MainWindow::slotFileExportAll()
{
    int format;
    QString filename = exportFileName(format);
    if(!filename.isEmpty())
    {
        Output output((Output::OutputType)format);
        output.output(filename, *database());
    }
}

void MainWindow::slotFileQuit()
{
    qApp->closeAllWindows();
}

void MainWindow::slotConfigure(int page)
{
    PreferencesDialog dlg(this);
    if (page>=0)
    {
        dlg.setCurrentIndex(page);
    }
    connect(&dlg, SIGNAL(reconfigure()), SLOT(slotReconfigure()));
    dlg.exec();
}

void MainWindow::slotReconfigure()
{
    PreferencesDialog::setupIconInMenus(this);

    if(AppSettings->getValue("/MainWindow/VerticalTabs").toBool())
    {
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::VerticalTabs);
    }
    else
    {
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AllowNestedDocks);
    }
#ifdef Q_OS_WINXX
    if(AppSettings->getValue("/MainWindow/StayOnTop").toBool())
    {
        SetWindowPos((HWND)winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    else
    {
        SetWindowPos((HWND)winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
#endif
    m_recentFiles.restore();
    emit reconfigure(); 	// Re-emit for children
    delete m_output;
    m_output = new Output(Output::NotationWidget);
    UpdateGameText();
}

void MainWindow::UpdateGameText()
{
    if(m_gameView)
    {
        m_gameView->setText(m_output->output(&game(), m_training->isChecked()));
    }
}

void MainWindow::UpdateMaterial()
{
    if(m_gameView && databaseInfo())
    {
        m_gameView->slotDisplayMaterial(databaseInfo()->material());
    }
}

void MainWindow::slotToggleStayOnTop()
{
#ifdef Q_OS_WINXX
    QAction* stayOnTop = (QAction*) sender();
    if(stayOnTop)
    {
        AppSettings->setValue("/MainWindow/StayOnTop", stayOnTop->isChecked());
    }

    if(AppSettings->getValue("/MainWindow/StayOnTop").toBool())
    {
        SetWindowPos((HWND)winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    else
    {
        SetWindowPos((HWND)winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    show();
#endif
}

void MainWindow::slotConfigureFlip()
{
    m_boardView->flip();
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

void MainWindow::slotSendMail()
{
    RecipientAddressDialog recAdDialog(this);
    if (databaseInfo()->isClipboard())
    {
        recAdDialog.enableCompleteDatabase(false);
    }
    if (recAdDialog.exec() == QDialog::Accepted)
    {
        QString recipient = recAdDialog.getEmail();

        if (recAdDialog.completeDatabase())
        {
            QString path = database()->filename();
            ShellHelper::sendFileWithMail(path, recipient);
        }
        else
        {
            Game curgame = game();
            Output output(Output::Pgn);
            QString pgn = output.output(&curgame);

            const QString white = curgame.tag(TagNameWhite);
            const QString black = curgame.tag(TagNameBlack);

            QString mailTo("mailto:%1?subject=Game %2-%3&body=%4");
            QUrl url(mailTo.arg(recipient).arg(white).arg(black).arg(pgn), QUrl::TolerantMode);
            QDesktopServices::openUrl(url);
        }
    }
}

void MainWindow::slotEditComment()
{
    gameEditComment(Output::Comment);
}

void MainWindow::slotEditCommentBefore()
{
    gameEditComment(Output::Precomment);
}

void MainWindow::slotEditVarPromote()
{
    if(!game().isMainline())
    {
        game().promoteVariation(game().currentMove());
    }
}

void MainWindow::slotEditVarRemove()
{
    if(!game().isMainline())
    {
        game().removeVariation(game().variationNumber());
    }
}

bool MainWindow::pasteFen(QString& msg, QString fen, bool newGame)
{
    // Prepare Fen - clean up code like this:
    // [FEN "***"] to ***

    if(fen.contains("\""))
    {
        int n1 = fen.indexOf('"');
        int n2 = fen.lastIndexOf('"');
        if(n2 > n1 + 1)
        {
            fen = fen.mid(n1 + 1, n2 - n1 - 1);
        }
    }

    // Another go at Fens copied from Wikis: [FEN]***[/FEN] is reduced to ***
    fen.remove(QRegExp("\\[[^\\]]*\\]"));

    Board board;
    if(!board.isValidFen(fen))
    {
        msg = fen.length() ?
              tr("Text in clipboard does not represent valid FEN:<br><i>%1</i>").arg(fen) :
              tr("There is no text in clipboard.");
        return false;
    }
    board.fromFen(fen);
    if(board.validate() != Valid)
    {
        msg = tr("The clipboard contains FEN, but with illegal position. "
                 "You can only paste such positions in <b>Setup position</b> dialog.");
        return false ;
    }
    if (newGame)
    {
        slotGameNew();
    }
    game().setStartingBoard(board,"Set starting board",board.chess960());
    return true;
}

void MainWindow::slotEditPasteFEN()
{
    QString fen = QApplication::clipboard()->text().simplified();
    QString msg;
    if(!pasteFen(msg, fen))
    {
        MessageDialog::warning(msg);
    }
}

bool MainWindow::slotEditPastePGN()
{
    QString pgn = QApplication::clipboard()->text().trimmed();
    pgn.replace(QChar(0x2013),QChar('-'));
    if(!pgn.isEmpty())
    {
        MemoryDatabase pgnDatabase;
        if(pgnDatabase.openString(pgn))
        {
            Game g;
            if(pgnDatabase.loadGame(0, g))
            {
                slotGameNew();
                game().copyFromGame(g);
                return true;
            }
        }
    }
    return false;
}

void MainWindow::slotEditPaste()
{
    QString fen = QApplication::clipboard()->text().simplified();
    QString dummy;
    if(!pasteFen(dummy, fen, true))
    {
        slotEditPastePGN();
    }
}

void MainWindow::slotEditMergePGN()
{
    if (game().isEmpty())
    {
        QString fen = QApplication::clipboard()->text().simplified();
        QString dummy;
        if(pasteFen(dummy, fen))
        {
            return;
        }
    }
    QString pgn = QApplication::clipboard()->text().trimmed();
    if(!pgn.isEmpty())
    {
        pgn = pgn.trimmed();
        if (pgn.trimmed().startsWith("[")) // looks like something containing tags
        {
            MemoryDatabase pgnDatabase;
            if(pgnDatabase.openString(pgn))
            {
                Game g;
                for (GameId i=0; i<pgnDatabase.count();++i) // pasted text might contain multiple games
                {
                    if(pgnDatabase.loadGame(i, g))
                    {
                        game().mergeWithGame(g);
                    }
                }
            }
        }
        else
        {
            // Perhaps some SAN moves
            addVariationFromSan(pgn);
        }
    }
}

void MainWindow::slotCreateBoardImage(QImage &image, double scaling)
{
    m_boardView->renderImage(image,scaling);
}

void MainWindow::slotEditTruncateEnd()
{
    game().truncateVariation(Game::AfterMove);
}

void MainWindow::slotEditTruncateStart()
{
    game().truncateVariation(Game::BeforeMove);
}

void MainWindow::slotEditBoard()
{
    BoardSetupDialog dlg(this);
    dlg.setBoard(game().board());
    dlg.setFlipped(m_boardView->isFlipped());
    if(dlg.exec() == QDialog::Accepted)
    {
        game().setStartingBoard(dlg.board(),tr("Set starting board"),dlg.board().chess960());
    }
}

bool MainWindow::addRemoteMoveFrom64Char(QString s)
{
    MoveId mId = game().addMoveFrom64Char(s);
    if (mId != NO_MOVE) // move is illegal
    {
        if (mId != CURRENT_MOVE) // my own move
        {
            Move m = game().move(mId);
            m_currentFrom = m.from();
            m_currentTo = m.to();
            moveChanged();
            playSound(":/sounds/move.wav");
        }
        return true;
    }
    return false;
}

BoardViewEx* MainWindow::BoardViewFrame(QWidget* widget)
{
    BoardViewEx* frame;
    QObject* parentWidget = (QObject*)widget;
    do {
        if (parentWidget) parentWidget = parentWidget->parent();
        frame = qobject_cast<BoardViewEx*>(parentWidget);
    } while (!frame && parentWidget);
    return frame;
}

void MainWindow::SlotShowTimer(bool show)
{
    BoardViewEx* frame = BoardViewFrame(m_boardView);
    if (frame) frame->showTime(show);
}

void MainWindow::SlotDisplayTime(int color, QString t)
{
    BoardViewEx* frame = BoardViewFrame(m_boardView);
    if (frame) frame->setTime(color==White, t);
}

void MainWindow::HandleFicsNewGameRequest()
{
    ActivateFICSDatabase();
    newGame();
}

void MainWindow::HandleFicsSaveGameRequest()
{
    ActivateFICSDatabase();
    playSound(":/sounds/fanfare.wav");
    SimpleSaveGame();
}

void MainWindow::HandleFicsCloseRequest()
{
    ActivateFICSDatabase();
    slotFileClose();
}

void MainWindow::HandleFicsResultRequest(QString s)
{
    s = s.remove(QRegExp("\\{[^\\}]*\\}")).trimmed();
    ActivateFICSDatabase();
    game().setResult(ResultFromString(s));
}

void MainWindow::HandleFicsAddTagRequest(QString tag,QString value)
{
    ActivateFICSDatabase();
    game().setTag(tag, value);
}

void MainWindow::HandleFicsRequestRemoveMove()
{
    ActivateFICSDatabase();
    if (game().backward())
    {
        game().dbTruncateVariation();
    }
}

void MainWindow::FicsConnected()
{
    ActivateFICSDatabase();
    m_ficsConsole->setEnabled(true);
    m_ficsConsole->show();
}

void MainWindow::FicsDisconnected()
{
    m_ficsConsole->setEnabled(false);
    slotFileCloseName(ficsPath());
}

void MainWindow::HandleFicsBoardRequest(int cmd,QString s)
{
    ActivateFICSDatabase();
    if ((cmd == FicsClient::BLKCMD_EXAMINE) ||
       (!addRemoteMoveFrom64Char(s)) ||
       (cmd == FicsClient::BLKCMD_OBSERVE))
    {
        // Finally set a new starting board
        Board b;
        if (b.from64Char(s))
        {
            QStringList l = s.split(' ');
            QString nameWhite = l[C64_NAME_WHITE];
            QString nameBlack = l[C64_NAME_BLACK];
            game().setTag(TagNameWhite, nameWhite);
            game().setTag(TagNameBlack, nameBlack);
            if (game().board() != b)
            {
                game().setStartingBoard(b,tr("Set starting board"),b.chess960());
                playSound(":/sounds/ding1.wav");
            }
        }
    }
}

/** Set position's image to clipboard. */
void MainWindow::slotEditCopyImage()
{
    QImage image;
    slotCreateBoardImage(image, -1.0);
    QApplication::clipboard()->setImage(image);
}

void MainWindow::slotExportImage()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Export Image"),
                   AppSettings->value("/General/DefaultDataPath").toString(),
                   tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    if(!file.isEmpty())
    {
        QPixmap pixmap(m_boardView->size());
        pixmap.fill(Qt::transparent);
        m_boardView->render(&pixmap);
        pixmap.save(file);
    }
}

void MainWindow::slotHelpBug()
{
    QDesktopServices::openUrl(QUrl("http://sourceforge.net/tracker/?group_id=163833&atid=829300"));
}

void MainWindow::slotBoardStoredMove()
{
    const Board& board = game().board();
    Square from, to;
    m_boardView->getStoredMove(from,to);
    if (from != InvalidSquare && to != InvalidSquare)
    {
        Move m(board.prepareMove(from, to));
        doBoardMove(m, 0, from, to);
    }
}

void MainWindow::triggerBoardMove()
{
    if (!game().atLineEnd())
    {
        game().forward();
        Move m = game().move();
        m_currentFrom = m.from();
        m_currentTo = m.to();
        moveChanged(); // The move's currents where set after forward(), thus repair effects
        playSound(":/sounds/move.wav");
    }
    else
    {
        if (!m_elapsedUserTimeValid && !m_mainAnalysis->isEngineRunning())
        {
            m_secondaryAnalysis->stopEngine(); // Avoid interference of second engine
            m_mainAnalysis->unPin();
            m_mainAnalysis->setMoveTime(m_matchParameter);
            m_mainAnalysis->startEngine();
        }

        m_machineHasToMove = true;
    }
}

void MainWindow::slotBoardMove(Square from, Square to, int button)
{
    const Board& board = game().board();
    if (!Move(from,to).isNullMove()) // do not accept null-move via mouse
    {
        Move m(board.prepareMove(from, to));
        doBoardMove(m, button, from, to);
    }
}

void MainWindow::slotEvalRequest(Square from, Square to)
{
    if (!m_mainAnalysis->isVisible())
    {
        return;
    }
    Board b = game().board();
    Piece p = b.pieceAt(from);
    b.removeFrom(from);
    b.setAt(to, p);
    if (b.validate() == Valid)
    {
        m_bEvalRequested = true;
        m_mainAnalysis->setPosition(b);
    }
}

void MainWindow::slotEvalMove(Square from, Square to)
{
    if (!m_mainAnalysis->isVisible())
    {
        return;
    }
    Board b = game().board();
    Piece p = b.pieceAt(from);
    b.removeFrom(from);
    b.setAt(to, p);
    b.swapToMove();
    if (b.validate() == Valid)
    {
        m_bEvalRequested = true;
        m_mainAnalysis->setPosition(b);
    }
}

QString MainWindow::getUCIHistory() const
{
    QString line;
    Game gx = game();
    gx.moveToStart();
    gx.dbMoveToId(game().currentMove(), &line);
    return line;
}

void MainWindow::slotResumeBoard()
{
    if (m_bEvalRequested)
    {
        m_bEvalRequested = false;
        QString line = getUCIHistory();
        m_mainAnalysis->setPosition(game().board(), line);
    }
}

Color MainWindow::UserColor()
{
    if (m_autoRespond->isChecked())
    {
        if (m_machineHasToMove)
        {
            return oppositeColor(game().board().toMove());
        }
    }
    else if (m_engineMatch->isChecked())
    {
        return NoColor;
    }
    else if (gameMode() && qobject_cast<const FicsDatabase*>(database()))
    {
        return m_ficsConsole->playerColor();
    }
    return game().board().toMove();
}

void MainWindow::doBoardMove(Move m, unsigned int button, Square from, Square to)
{
    BoardView::BoardViewAction action = m_boardView->moveActionFromModifier((Qt::KeyboardModifiers) button);
    if ((game().board().toMove() == UserColor()) && (UserColor() == game().board().colorAt(from)))
    {
        if (m.isLegal() || m.isNullMove())
        {
            PieceType promotionPiece = None;
            if(m.isPromotion() && !(button & 0x80000000))
            {
                int index = 0;
                if ((action == BoardView::ActionQuery) || !AppSettings->getValue("/Board/AutoPromoteToQueen").toBool())
                {
                    index = PromotionDialog(0,m.color()).getIndex();
                    if(index<0)
                    {
                        return;
                    }
                }
                promotionPiece = PieceType(Queen + index);
                m.setPromoted(promotionPiece);
            }
            else if (m.isPromotion())
            {
                promotionPiece = pieceType(m.promotedPiece());
            }

            // Use an existing move with the correct promotion piece type if it already exists
            if(game().findNextMove(from, to, promotionPiece))
            {
                if (action == BoardView::ActionAdd)
                {
                    // The move exists but adding a variation was requested anyhow
                    // Take back the move and proceed as if the move does not yet exist
                    game().backward();
                }
                else
                {
                    if (autoRespondActive())
                    {
                        triggerBoardMove();
                    }
                    slotGameChanged(true);
                    return;
                }
            }

            if(m_training->isChecked())
            {
                if (game().atLineEnd())
                {
                    playSound(":/sounds/fanfare.wav");
                    m_training->trigger();
                }
            }
            else
            {
                if(game().atLineEnd())
                {
                    QString annot;
                    if (m_autoRespond->isChecked() || m_match->isChecked())
                    {
                        int t = m_elapsedUserTimeValid ? m_elapsedUserTime.elapsed() : 0;
                        int increment = 0;
                        if (m_autoRespond->isChecked())
                        {
                            increment += m_matchParameter.ms_bonus;
                            increment = std::min(t, increment);
                        }
                        increment += m_matchParameter.ms_increment;
                        Color currentColor = game().board().toMove();
                        m_matchTime[currentColor] += t;

                        EngineParameter par = m_matchParameter;
                        par.ms_white = m_matchParameter.ms_totalTime - m_matchTime[White];
                        par.ms_black = m_matchParameter.ms_totalTime - m_matchTime[Black];

                        bool ok = true;
                        if (par.tm != EngineParameter::TIME_GONG && m_matchTime[currentColor] > (int) m_matchParameter.ms_totalTime)
                        {
                            ok = false;
                            playSound(":/sounds/fanfare.wav");
                            if (m_autoRespond->isChecked())
                            {
                                m_autoRespond->trigger();
                            }
                            else
                            {
                                m_match->trigger();
                            }
                            if (game().isMainline())
                            {
                                annot = tr("Time is over");
                                setResultAgainstColorToMove();
                            }
                        }

                        if (ok)
                        {
                            // Correct move time with increment if move was done in time
                            m_matchTime[currentColor] -= increment;
                            if (currentColor==White)
                            {
                                par.ms_white += increment;
                            }
                            else
                            {
                                par.ms_black += increment;
                            }
                        }

                        QTime tm(0,0);
                        tm = tm.addMSecs(currentColor==White ? par.ms_white : par.ms_black);
                        QString sTime = tm.toString("H:mm:ss");
                        SlotDisplayTime(currentColor, sTime);
                        BoardViewEx* frame = BoardViewFrame(m_boardView);
                        if (frame)
                        {
                            if (ok)
                            {
                                frame->startTime(currentColor==Black);
                            }
                            else
                            {
                                frame->stopTimes();
                            }
                        }

                        if (ok)
                        {
                            if (par.annotateEgt && par.tm != EngineParameter::TIME_GONG)
                            {
                                QString clk = "[%clk %1]";
                                annot = clk.arg(sTime);
                            }

                            if (m_match->isChecked())
                            {
                                m_elapsedUserTimeValid = true;
                                m_elapsedUserTime.start();
                            }
                            else if (m_autoRespond->isChecked())
                            {
                                m_mainAnalysis->unPin();
                                m_mainAnalysis->setMoveTime(par);
                                if (!m_elapsedUserTimeValid && !m_mainAnalysis->isEngineRunning())
                                {
                                    m_mainAnalysis->startEngine();
                                }
                            }
                        }
                    }

                    game().addMove(m, annot);
                    if (qobject_cast<FicsDatabase*>(database()))
                    {
                        m_ficsConsole->SendMove(m.toAlgebraic());
                    }
                    else
                    {
                        if (m_autoRespond->isChecked() || m_match->isChecked())
                        {
                            if (game().board().isStalemate() ||
                                game().board().isCheckmate())
                            {
                                playSound(":/sounds/fanfare.wav");
                                if (m_autoRespond->isChecked())
                                {
                                    m_autoRespond->trigger();
                                }
                                else
                                {
                                    m_match->trigger();
                                }
                                setResultForCurrentPosition();
                            }
                        }
                    }
                }
                else
                {
                    if(action == BoardView::ActionInsert)
                    {
                        game().insertMove(m);
                    }
                    else if(action == BoardView::ActionReplace)
                    {
                        game().replaceMove(m);
                    }
                    else
                    {
                        game().addVariation(m);
                    }
                    game().forward();
                }
            }

            if (m_autoRespond->isChecked() && !m_machineHasToMove)
            {
                m_machineHasToMove = true;
            }
        }
    }
    else if ((game().board().toMove() == UserColor()) && (UserColor() != game().board().colorAt(from)))
    {
        if (UserColor() == game().board().colorAt(to))
        {
            const Board& board = game().board();
            Move m(board.prepareMove(to, from));
            doBoardMove(m, button, to, from);
        }
    }
    else
    {
        if ((game().board().toMove() != UserColor()) && (UserColor() == game().board().colorAt(from)))
        {
            if (premoveAllowed())
            {
                m_boardView->setStoredMove(from,to);
            }
        }
        else if ((game().board().toMove() != UserColor()) && (UserColor() == game().board().colorAt(to)))
        {
            if (premoveAllowed())
            {
                m_boardView->setStoredMove(to,from);
            }
        }
    }
}

void MainWindow::SQAction(QChar c, QAction* action)
{
    action->setIconVisibleInMenu(true);
    if (m_lastColor == c)
    {
        action->setCheckable(true);
        action->setChecked(true);
    }
}

void MainWindow::slotBoardClick(Square s, int button, QPoint pos, Square from)
{
    if(button & Qt::RightButton)
    {
        if(button & Qt::ShiftModifier)
        {
            QMenu* menu = new QMenu(this);
            m_annotationSquare = s;
            m_annotationSquareFrom = from;
            bool twoSquares = (s != from && from != InvalidSquare);
            if (!twoSquares)
            {
                SQAction('R',menu->addAction(QIcon(":/images/square_red.png"),   tr("Red Square"),    this, SLOT(slotRedSquare())));
                SQAction('Y',menu->addAction(QIcon(":/images/square_yellow.png"), tr("Yellow Square"), this, SLOT(slotYellowSquare())));
                SQAction('G',menu->addAction(QIcon(":/images/square_green.png"), tr("Green Square"),  this, SLOT(slotGreenSquare())));
                menu->addSeparator();
                SQAction(0,menu->addAction(QIcon(":/images/square_none.png"),  tr("Remove Color"),  this, SLOT(slotNoColorSquare())));
            }
            else
            {
                SQAction('R',menu->addAction(QIcon(":/images/arrow_red.png"),   tr("Red Arrow to here"),    this, SLOT(slotRedArrowHere())));
                SQAction('Y',menu->addAction(QIcon(":/images/arrow_yellow.png"), tr("Yellow Arrow to here"), this, SLOT(slotYellowArrowHere())));
                SQAction('G',menu->addAction(QIcon(":/images/arrow_green.png"), tr("Green Arrow to here"),  this, SLOT(slotGreenArrowHere())));
                menu->addSeparator();
                SQAction(0,menu->addAction(QIcon(":/images/arrow_none.png"),  tr("Remove Arrow to here"), this, SLOT(slotNoArrowHere())));
            }
            menu->exec(pos);
        }
        else
        {
            bool nextGuess = AppSettings->getValue("/Board/nextGuess").toBool();
            if(button & Qt::ControlModifier)
            {
                nextGuess = !nextGuess;    // CTRL selects the other mapping
            }
            if(!nextGuess)
            {
                bool remove = game().atLineEnd();
                int var = game().variationNumber();
                gameMoveBy(-1);
                if(remove)
                {
                    if(var && game().isMainline())
                    {
                        game().removeVariation(var);
                    }
                    else
                    {
                        game().truncateVariation();
                    }
                }
            }
            else
            {
                m_boardView->nextGuess(s);
            }
        }
    }
    else if (button & Qt::LeftButton)
    {
        const QAction* brushAction = brushGroup->checkedAction();
        if (brushAction)
        {
            bool twoSquares = (s != from && from != InvalidSquare);
            if (twoSquares)
            {
                game().appendArrowAnnotation(s, from, brushAction->data().toChar());
            }
            else
            {
                game().appendSquareAnnotation(s, brushAction->data().toChar());
            }
        }
        else
        {
            BoardView::BoardViewAction action = m_boardView->moveActionFromModifier((Qt::KeyboardModifiers)button);
            if (action == BoardView::ActionPen)
            {
                bool twoSquares = (s != from && from != InvalidSquare);
                if (twoSquares)
                {
                    game().appendArrowAnnotation(s, from, m_lastColor);
                }
                else
                {
                    game().appendSquareAnnotation(s, m_lastColor);
                }
            }
        }
    }
}

void MainWindow::slotMoveChanged()
{
    if(m_training->isChecked())
    {
        UpdateGameText();
    }
    moveChanged();
}

void MainWindow::moveChanged()
{
    const Game& g = game();
    MoveId m = g.currentMove();

    // Set board first
    m_boardView->setBoard(g.board(), m_currentFrom, m_currentTo, game().atLineEnd());

    QString annotation = game().textAnnotation();
    BoardViewEx* frame = BoardViewFrame(m_boardView);
    if (frame)
    {
        frame->setComment(annotation);
    }

    m_currentFrom = InvalidSquare;
    m_currentTo = InvalidSquare;

    emit displayTime(g.timeAnnotation(m, Game::BeforeMove), g.board().toMove(), g.timeAnnotation(m, Game::AfterMove));

    // Highlight current move
    m_gameView->showMove(m);
    if (g.isMainline())
    {
        m_gameView->slotDisplayPly(g.ply());
    }

    displayVariations();

    slotSearchTree();

    QString line = getUCIHistory();
    emit boardChange(g.board(), line);

    // Clear  entries
    m_nagText.clear();

    emit signalMoveHasNextMove(!gameMode() && !game().atLineEnd());
    emit signalMoveHasPreviousMove(!gameMode() && !game().atGameStart());
    emit signalMoveHasVariation(!gameMode() && game().variationCount() > 0);
    emit signalMoveHasParent(!gameMode() && !game().isMainline());
    emit signalVariationHasSibling(!gameMode() && game().variationHasSiblings(m));
    emit signalGameIsEmpty(false);
    emit signalGameAtLineStart(!gameMode() && game().atLineStart());
}

void MainWindow::displayVariations()
{
    QList<MoveId> listVariations = game().variations();
    if (listVariations.size() && !game().atLineEnd())
    {
        listVariations.push_front(game().nextMove());
    }

    QStringList textVariations;
    foreach(MoveId id, listVariations)
    {
        Move move = game().move(id);
        textVariations << game().board().moveToSan(move, true, true);
    }

    if (textVariations.isEmpty())
    {
        QString placeHolder;
        if (game().atLineEnd())
        {
            placeHolder = (game().isMainline() ? tr("End of game") : tr("End of line"));
        }
        else
        {
            placeHolder = (game().isMainline() ? tr("Main line") : tr("Line"));
        }

        MoveId start = game().variationStartMove();
        if (start != NO_MOVE)
        {
            Game g(game());
            g.dbMoveToId(start);
            Move startMove = g.move();
            g.backward();
            QString startSan = g.board().moveToSan(startMove, true, true);
            placeHolder += " ";
            placeHolder += startSan;
        }
        textVariations << placeHolder;
    }

    BoardViewFrame(m_boardView)->showVariations(listVariations, textVariations);
}

void MainWindow::slotGameFilterUpdate(int index, int value)
{
    m_gameList->updateFilter(index,value);
}

void MainWindow::slotSearchTree()
{
    if (databaseInfo()->isValid())
    {
        updateOpeningTree(game().board(), false);
    }
}

void MainWindow::slotBoardMoveWheel(int wheel)
{
    if(wheel & Qt::AltModifier)
    {
        if(wheel & BoardView::WheelDown)
        {
            slotGameMoveLast();
        }
        else
        {
            slotGameMoveFirst();
        }
    }
    else if(wheel & Qt::ControlModifier)
    {
        if(wheel & BoardView::WheelDown)
        {
            slotGameMoveNextN();
        }
        else
        {
            slotGameMovePreviousN();
        }
    }
    else if(wheel & BoardView::WheelDown)
    {
        slotGameMoveNext();
    }
    else
    {
        slotGameMovePrevious();
    }
}

void MainWindow::slotGameVarEnter(int index)
{
    if(index)
    {
        game().moveToId(game().variations().at(index-1));
    }
    else
    {
        if (!slotGameMoveNext())
        {
            game().moveToId(game().parentMove());
        }
    }
}

void MainWindow::slotGameVarEnter()
{
    if(game().variationCount(game().currentMove()))
    {
        game().moveToId(game().variations().first());
    }
    else
    {
        slotGameMoveNext();
    }
}

void MainWindow::slotGameVarUp()
{
    if(!game().isMainline())
    {
        while(!game().atLineStart())
        {
            game().backward();
        }
        MoveId currentVar = game().currentMove();
        game().backward();
        int n = game().variations().indexOf(currentVar) - 1;
        if(n >= 0)
        {
            game().moveToId(game().variations().at(n));
        }
    }
}

void MainWindow::slotGameVarDown()
{
    if(!game().isMainline())
    {
        while(!game().atLineStart())
        {
            game().backward();
        }
        MoveId currentVar = game().currentMove();
        game().backward();
        int n = game().variations().indexOf(currentVar) + 1;
        if(n < game().variations().count())
        {
            game().moveToId(game().variations().at(n));
        }
        else
        {
            if(!m_training->isChecked())
            {
                // Do not show next move in training mode
                game().forward();
            }
        }
    }
}

void MainWindow::slotGameVarExit()
{
    if(!game().isMainline())
    {
        while(!game().atLineStart())
        {
            game().backward();
        }
        game().backward();
    }
    else
    {
        slotGameMovePrevious();
    }
}

void MainWindow::slotGameLoadPrevious()
{
    m_gameList->selectPreviousGame();
}

bool MainWindow::loadNextGame()
{
    return (m_gameList->selectNextGame());
}

void MainWindow::slotGameLoadNext()
{
    loadNextGame();
}

void MainWindow::slotGameLoadRandom()
{
    m_gameList->selectRandomGame();
}

void MainWindow::slotGameLoadChosen()
{
    int index = QInputDialog::getInt(this, tr("Load Game"), tr("Game number:"), gameIndex() + 1,
                                     1, database()->count());
    if (index != gameIndex() + 1)
    {
        gameLoad(index - 1);
    }
}

void MainWindow::newGame()
{
    m_boardView->setStoredMove(InvalidSquare, InvalidSquare);
    databaseInfo()->newGame();
    m_gameList->removeSelection();
    emit signalGameIsEmpty(true); // repair effect of slotGameChanged
    m_gameList->setFocus();
    UpdateBoardInformation();
    emit signalGameLoaded(game().startingBoard());
}

void MainWindow::slotGameNew()
{
    if(database()->isReadOnly())
    {
        MessageDialog::error(tr("This database is read only."));
    }
    else
    {
        if(QuerySaveGame())
        {
           newGame();
        }
    }
}

void MainWindow::saveGame(DatabaseInfo* dbInfo)
{
    if(!dbInfo->database()->isReadOnly())
    {
        if (dbInfo->saveGame())
        {
            if (dbInfo == databaseInfo())
            {
                m_gameList->updateFilter(dbInfo->currentIndex(), 1);
                slotFilterChanged();
                slotGameChanged(true);
                updateLastGameList();
                UpdateBoardInformation();
            }

            if(AppSettings->getValue("/General/autoCommitDB").toBool())
            {
                saveDatabase(dbInfo);
                emit databaseChanged(dbInfo);
            }
        }
    }
}

void MainWindow::slotDatabaseModified()
{
    slotFilterChanged();
    emit signalCurrentDBisReadWrite((!m_currentDatabase->isClipboard()) && !databaseInfo()->database()->isReadOnly() && databaseInfo()->database()->isModified());
    emit signalCurrentDBcanBeClosed(!m_currentDatabase->isClipboard());
    emit signalCurrentDBhasGames(database()->index()->count() > 0);
    emit signalGameModified(databaseInfo()->modified());
}

bool MainWindow::slotGameSave()
{
    if(database()->isReadOnly())
    {
        MessageDialog::error(tr("This database is read only."));
        databaseInfo()->setModified(false, Game(), ""); // Do not notify more than once
        return true;
    }

    return QuerySaveGame();
}

void MainWindow::slotGameSaveOnly()
{
    if(database()->isReadOnly())
    {
        MessageDialog::error(tr("This database is read only."));
        databaseInfo()->setModified(false, Game(), ""); // Do not notify more than once
    }
    else
    {
        SimpleSaveGame();
    }
}

void MainWindow::slotGameEditTags()
{
    DatabaseInfo* dbInfo = databaseInfo();
    if (dbInfo->currentIndex()>=0)
    {
        TagDialog dlg(this);
        if (dlg.editTags(database()->index(), game(),dbInfo->currentIndex()))
        {
            saveGame(dbInfo);
            emit databaseModified();
        }
    }
}

void MainWindow::slotGameMoveToPly(int ply)
{
    if (!gameMode())
    {
        game().moveToStart();
        game().moveByPly(ply);
    }
}

void MainWindow::slotGameModify(const EditAction& action)
{
    if((action.type() != EditAction::CopyHtml) &&
       (action.type() != EditAction::CopyText))
    {
        game().moveToId(action.move());
    }
    switch(action.type())
    {
    case EditAction::RemoveNextMoves:
        game().truncateVariation();
        break;
    case EditAction::RemovePreviousMoves:
        game().truncateVariation(Game::BeforeMove);
        break;
    case EditAction::RemoveVariation:
    {
        game().removeVariation(game().variationNumber());
        break;
    }
    case EditAction::PromoteVariation:
    {
        game().promoteVariation(action.move());
        break;
    }
    case EditAction::EnumerateVariations1:
    {
        game().enumerateVariations(action.move(), 'A');
        break;
    }
    case EditAction::EnumerateVariations2:
    {
        game().enumerateVariations(action.move(), 'a');
        break;
    }
    case EditAction::VariationUp:
    {
        game().moveVariationUp(action.move());
        break;
    }
    case EditAction::VariationDown:
    {
        game().moveVariationDown(action.move());
        break;
    }
    case EditAction::EditPrecomment:
        gameEditComment(Output::Precomment);
        break;
    case EditAction::EditGameComment:
        game().moveToId(0);
        gameEditComment(Output::Comment);
        break;
    case EditAction::EditComment:
        gameEditComment(Output::Comment);
        break;
    case EditAction::AddNag:
        game().addNag(Nag(action.data().toInt()), action.move());
        break;
    case EditAction::ClearNags:
        game().clearNags(action.move());
        break;
    case EditAction::AddNullMove:
        if (game().atLineEnd())
        {
            game().addMove(game().board().nullMove());
        }
        else
        {
            game().addVariation(game().board().nullMove());
        }
        break;
    case EditAction::CopyHtml:
        QApplication::clipboard()->setText(m_gameView->toHtml());
        break;
    case EditAction::CopyText:
        QApplication::clipboard()->setText(m_gameView->toPlainText());
        break;
    case EditAction::Uncomment:
        slotGameUncomment();
        break;
    case EditAction::RemoveVariations:
        slotGameRemoveVariations();
        break;
    default:
        break;
    }
}

void MainWindow::slotMergeActiveGame(QList<GameId> gameIndexList)
{
    if (gameIndexList.size())
    {
        Game state = game();
        foreach(GameId gameIndex, gameIndexList)
        {
            if(gameIndex != databaseInfo()->currentIndex())
            {
                Game g;
                if(database()->loadGame(gameIndex, g))
                {
                    game().dbMergeWithGame(g);
                }
            }
        }
        databaseInfo()->setModified(true,state,tr("Merge selected games"));
    }
}

void MainWindow::slotMergeActiveGame(GameId gameIndex)
{
    Game g;
    if(gameIndex != databaseInfo()->currentIndex())
    {
        if(database()->loadGame(gameIndex, g))
        {
            game().mergeWithGame(g);
        }
    }
}

void MainWindow::slotMergeAllGames()
{
    Game g;
    for(GameId i = 0; i < database()->index()->count(); ++i)
    {
        if(i != databaseInfo()->currentIndex())
        {
            if(database()->loadGame(i, g))
            {
                game().mergeWithGame(g);
            }
        }
    }
}

void MainWindow::slotMergeFilter()
{
    Game g;
    for(GameId i = 0; i < database()->count(); ++i)
    {
        if(databaseInfo()->filter()->contains(i) && database()->loadGame(i, g))
        {
            game().mergeWithGame(g);
        }
    }
}

void MainWindow::slotGetActiveGame(const Game** g)
{
    *g = &game();
}

void MainWindow::slotGameChanged(bool /*bModified*/)
{
    UpdateMaterial();
    UpdateGameText();
    UpdateGameTitle();
    moveChanged();
}

void MainWindow::slotGameViewLink(const QUrl& url)
{
    if (gameMode())
    {
        return;
    }
    if(url.scheme() == "move")
    {
        if(url.path() == "prev")
        {
            game().backward();
        }
        else if(url.path() == "next")
        {
            game().forward();
        }
        else if(url.path() == "exit")
        {
            game().moveToId(game().parentMove());
        }
        else
        {
            game().moveToId(url.path().toInt());
        }
    }
    else if(url.scheme() == "precmt" || url.scheme() == "cmt")
    {
        game().moveToId(url.path().toInt());
        Output::CommentType type = url.scheme() == "cmt" ? Output::Comment : Output::Precomment;
        gameEditComment(type);
    }
    else if(url.scheme() == "egtb")
    {
        if(!game().atGameEnd())
        {
            game().addVariation(url.path());
        }
        else
        {
            game().addMove(url.path());
        }
        game().forward();
    }
    else if(url.scheme() == "tag")
    {
        m_playerList->setDatabase(databaseInfo());
        if(url.path() == "white")
        {
            m_playerList->slotSelectPlayer(game().tag(TagNameWhite));
        }
        else if(url.path() == "black")
        {
            m_playerList->slotSelectPlayer(game().tag(TagNameBlack));
        }
    }
    else if (url.scheme().startsWith("eco"))
    {
        m_ecoList->slotSelectECO(url.path());
    }
    else if (url.scheme().startsWith("event"))
    {
        m_eventList->slotSelectEvent(url.path());
    }
}

void MainWindow::slotGameViewLink(const QString& url)
{
    slotGameViewLink(QUrl(url));
}

void MainWindow::slotGameViewSource()
{
    QString text = m_output->output(&game(), m_training->isChecked());
    QApplication::clipboard()->setText(text);
}

void MainWindow::slotGameDumpMoveNodes()
{
    game().dumpAllMoveNodes();
}

void MainWindow::slotGameDumpBoard()
{
    qDebug() << "View: " << m_boardView->board().toFen() << "//" << m_boardView->board().getHashValue();
    qDebug() << "Board:" << game().board().toFen() << "//" << game().board().getHashValue();
}

void MainWindow::slotGameAddVariation(const Analysis& analysis)
{
    QString score;
    if (!analysis.bestMove())
    {
        score = QString::number(analysis.fscore(), 'f', 2);
    }
    if (game().atLineEnd())
    {
        game().addLine(analysis.variation(), score);
    }
    else
    {
        game().addVariation(analysis.variation(), score);
    }
}

bool MainWindow::addVariationFromSan(const QString& san)
{
    bool added = false;
    Move m = game().board().parseMove(san);
    if(m.isLegal() || (!gameMode() && m.isNullMove()))
    {
        added = true;
        doBoardMove(m, 0x80000000, m.from(), m.to());
    }
    return added;
}

bool MainWindow::slotGameAddVariation(const QString& san)
{
    QString s = san;
    s = s.remove(QRegExp("-.*"));
    s = s.remove(QRegExp("[0-9]*\\."));   
    return addVariationFromSan(s);
}

void MainWindow::slotGameUncomment()
{
    game().removeComments();
}

void MainWindow::slotGameRemoveVariations()
{
    game().removeVariations();
}

void MainWindow::slotGameSetComment(QString annotation)
{
    if (game().textAnnotation() != annotation)
    {
        game().editAnnotation(annotation);
        UpdateGameText();
    }
}

bool MainWindow::autoRespondActive() const
{
    return (m_autoRespond->isChecked() ||
            (m_training->isChecked() && m_bTrainigAutoRespond));
}

void MainWindow::slotToggleTraining()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (AppSettings->getValue("/Board/noHints").toBool())
    {    
        enterNoHintMode(action->isChecked());
    }
    if (action->isChecked())
    {
        QMenu* menu = new QMenu(this);
        QAction *autoRespond = new QAction(tr("Automatic responses"), this);
        QAction *noAutoRespond = new QAction(tr("Play both sides"), this);
        menu->addAction(autoRespond);
        menu->addAction(noAutoRespond);
        m_bTrainigAutoRespond = (autoRespond == menu->exec(QCursor::pos()));
    }
    UpdateGameText();
}

void MainWindow::slotToggleAutoRespond()
{
    if (AppSettings->getValue("/Board/noHints").toBool())
    {
        Guess::setGuessAllowed(!m_autoRespond->isChecked());
    }

    if (m_autoRespond->isChecked())
    {
        if (!MatchParameterDlg::getParametersForEngineGame(m_matchParameter))
        {
            m_autoRespond->setChecked(false);
            return;
        }

        m_matchParameter.reset();

        QTime tmWhite(0,0);
        tmWhite = tmWhite.addMSecs(m_matchParameter.ms_white);
        SlotDisplayTime(White, tmWhite.toString("H:mm:ss"));

        QTime tmBlack(0,0);
        tmBlack = tmBlack.addMSecs(m_matchParameter.ms_black);
        SlotDisplayTime(Black, tmBlack.toString("H:mm:ss"));

        SlotShowTimer(true);

        m_mainAnalysis->setMoveTime(m_matchParameter);
        m_machineHasToMove = m_matchParameter.engineStarts;
        m_boardView->setFlags(m_boardView->flags() | BoardView::IgnoreSideToMove);
        if (m_matchParameter.engineStarts)
        {
            m_boardView->setFlipped(game().board().toMove() == White);
            triggerBoardMove();
            slotGameChanged(true);
        }
        else
        {
            m_boardView->setFlipped(game().board().toMove() == Black);
        }
    }
    else
    {
        SlotShowTimer(false);
        m_mainAnalysis->stopEngine();
        // Reset to value from slider
        m_mainAnalysis->setMoveTime(m_sliderSpeed->translatedValue());
        m_boardView->setFlags(m_boardView->flags() & ~BoardView::IgnoreSideToMove);
    }

    m_matchTime[0] = 0;
    m_matchTime[1] = 0;
    m_elapsedUserTimeValid = false;
}

void MainWindow::slotToggleAutoAnalysis()
{
    if(m_autoAnalysis->isChecked())
    {
        m_mainAnalysis->setMoveTime(m_sliderSpeed->translatedValue());
        m_AutoInsertLastBoard.clear();
        if(!m_mainAnalysis->isEngineConfigured())
        {
            MessageDialog::information(tr("Analysis Pane 1 is not running an engine for automatic analysis."), tr("Auto Analysis"));
        }
        else
        {
            if (game().atGameStart()) // Prevent surprising inactivity
            {
                if (AppSettings->getValue("/Board/BackwardAnalysis").toBool())
                {
                    game().moveToEnd();
                }
            }
            QString line = getUCIHistory();
            m_mainAnalysis->unPin();
            m_mainAnalysis->setPosition(game().board(), line);
            m_mainAnalysis->startEngine();
        }
    }
    else
    {
        if (!autoGroup->checkedAction())
        {
            m_mainAnalysis->stopEngine();
        }
    }
}

void MainWindow::slotToggleEngineMatch()
{
    Guess::setGuessAllowed(!m_engineMatch->isChecked());
    if(m_engineMatch->isChecked())
    {
        if (!MatchParameterDlg::getParametersForEngineMatch(m_matchParameter))
        {
            m_engineMatch->setChecked(false);
            return;
        }

        if(!m_mainAnalysis->isEngineConfigured())
        {
            MessageDialog::information(tr("Analysis Pane 1 is not running an engine for automatic analysis."), tr("Engine Match"));
        }
        else if(!m_secondaryAnalysis->isEngineConfigured())
        {
            MessageDialog::information(tr("Analysis Pane 2 is not running an engine for automatic analysis."), tr("Engine Match"));
        }
        else
        {
            m_matchParameter.reset();

            QTime tmWhite(0,0);
            tmWhite = tmWhite.addMSecs(m_matchParameter.ms_white);
            SlotDisplayTime(White, tmWhite.toString("H:mm:ss"));

            QTime tmBlack(0,0);
            tmBlack = tmBlack.addMSecs(m_matchParameter.ms_black);
            SlotDisplayTime(Black, tmBlack.toString("H:mm:ss"));

            SlotShowTimer(true);

            m_AutoInsertLastBoard.clear();

            QString line = getUCIHistory();

            m_mainAnalysis->unPin();
            m_mainAnalysis->setOnHold(true);
            m_mainAnalysis->setPosition(game().board(), line);
            m_mainAnalysis->setMoveTime(m_matchParameter);

            m_secondaryAnalysis->unPin();
            m_secondaryAnalysis->setOnHold(true);
            m_secondaryAnalysis->setPosition(game().board(), line);
            m_secondaryAnalysis->setMoveTime(m_matchParameter);

            if (game().tag(TagNameWhite).isEmpty() &&
                game().tag(TagNameBlack).isEmpty())
            {
                QString engine1 = m_mainAnalysis->engineName();
                QString engine2 = m_secondaryAnalysis->engineName();

                game().setTag(TagNameWhite, engine1);
                game().setTag(TagNameBlack, engine2);
                game().setTag(TagNameDate, PartialDate::today().asString());
                game().setTag(TagNameTimeControl, m_matchParameter.timeAsString());
            }

            m_matchTime[0] = 0;
            m_matchTime[1] = 0;

            if (game().board().toMove() == White)
            {
                m_mainAnalysis->startEngine();
            }
            else
            {
                m_secondaryAnalysis->startEngine();
            }
        }
    }
    else
    {
        SlotShowTimer(false);

        if (!autoGroup->checkedAction())
        {
            m_mainAnalysis->stopEngine();
            m_secondaryAnalysis->stopEngine();
        }
        m_mainAnalysis->setMoveTime(m_sliderSpeed->translatedValue());
        m_secondaryAnalysis->setMoveTime(m_sliderSpeed->translatedValue());
    }
}

void MainWindow::slotToggleAutoPlayer()
{
    QAction* autoPlayAction = (QAction*) sender();
    if(autoPlayAction)
    {
        if(autoPlayAction->isChecked())
        {
            int interval = AppSettings->getValue("/Board/AutoPlayerInterval").toInt();
            if(m_autoPlayTimer->interval() != interval)
            {
                m_autoPlayTimer->setInterval(interval);
            }
            m_mainAnalysis->setMoveTime(interval);
            m_autoPlayTimer->start();
        }
        else
        {
            m_autoPlayTimer->stop();
        }
    }
}

void MainWindow::setResultForCurrentPosition()
{
    if (game().isMainline())
    {
        if (!game().board().isCheckmate())
        {
            game().setResult(Draw);
        }
        else
        {
            setResultAgainstColorToMove();
        }
    }
}

void MainWindow::setResultAgainstColorToMove()
{
    if (game().isMainline())
    {
        if (game().board().toMove()==Black)
        {
            game().setResult(WhiteWin);
        }
        else
        {
            game().setResult(BlackWin);
        }
    }
}

bool MainWindow::doEngineMove(Move m, EngineParameter matchParameter)
{
    m_currentFrom = m.from();
    m_currentTo = m.to();

    QTime tm(0,0);
    tm = tm.addMSecs(game().board().toMove()==White ? matchParameter.ms_white : matchParameter.ms_black);
    QString sTime = tm.toString("H:mm:ss");
    SlotDisplayTime(game().board().toMove(), sTime);
    BoardViewEx* frame = BoardViewFrame(m_boardView);
    if (frame) frame->startTime(game().board().toMove()==Black);

    QString annot;
    if (matchParameter.annotateEgt && matchParameter.tm != EngineParameter::TIME_GONG)
    {
        QString clk = "[%clk %1]";
        annot = clk.arg(sTime);
    }

    game().addMove(m,annot);
    if (game().board().isStalemate() ||
        game().board().isCheckmate())
    {
        playSound(":/sounds/fanfare.wav");
        // The game is terminated immediately
        setResultForCurrentPosition();
        return false;
    }
    else
    {
        playSound(":/sounds/move.wav");
    }
    return true;
}

bool MainWindow::gameIsDraw() const
{
    return (game().board().insufficientMaterial() ||
           game().positionRepetition3(game().board()) ||
           game().board().halfMoveClock() > 99);
}

QString MainWindow::drawAnnotation() const
{
    QString reason;

    if (game().board().insufficientMaterial()) reason = tr("Game is drawn by insufficient material");
    else if (game().positionRepetition3(game().board())) reason = tr("Game is drawn by repetition");
    else if (game().board().halfMoveClock() > 99) reason = tr("Game is drawn by 50 move rule");

    return reason;
}

void MainWindow::slotEngineTimeout(const Analysis& analysis)
{
    if (m_boardView->dragged() == Empty || m_autoRespond->isChecked()) // Do not interfer with moving a piece, unless it might be a premove
    {
        // todo - if premove, make it possible to change the board "underneath" the piece that is moved
        if(m_autoAnalysis->isChecked() && (m_AutoInsertLastBoard != game().board()))
        {
            m_AutoInsertLastBoard = game().board();

            Analysis a = m_mainAnalysis->getMainLine();
            if(!a.variation().isEmpty())
            {
                Move m = a.variation().first();
                if(!game().currentNodeHasMove(m.from(), m.to()))
                {
                    if(!game().isEcoPosition())
                    {
                        slotGameAddVariation(a);
                    }
                }
            }            
            slotAutoPlayTimeout();
        }
        else if (game().atLineEnd() && m_autoRespond->isChecked())
        {
            if (m_machineHasToMove)
            {
                int t = analysis.elapsedTimeMS() - m_matchParameter.ms_increment;
                m_matchTime[game().board().toMove()] += t;

                if (m_matchParameter.tm != EngineParameter::TIME_GONG && m_matchTime[game().board().toMove()] > m_matchParameter.ms_totalTime)
                {
                    playSound(":/sounds/fanfare.wav");
                    m_autoRespond->trigger();
                    if (game().isMainline())
                    {
                        game().dbSetAnnotation(tr("Time is over"));
                        setResultAgainstColorToMove();
                    }
                }
                else if (gameIsDraw())
                {
                    playSound(":/sounds/fanfare.wav");
                    // Game is drawn by repetition or 50 move rule
                    m_autoRespond->trigger();
                    if (game().isMainline())
                    {
                        game().dbSetAnnotation(drawAnnotation());
                        game().setResult(Draw);
                    }
                }
                else if(analysis.getEndOfGame())
                {
                    if (analysis.score() == 0)
                    {
                        game().setResult(Draw);
                    }
                    else
                    {
                        setResultAgainstColorToMove();
                    }
                    playSound(":/sounds/fanfare.wav");
                    // Game is terminated by engine
                    m_autoRespond->trigger();
                }
                else if(!analysis.variation().isEmpty() && analysis.bestMove())
                {
                    Move m = analysis.variation().first();
                    if (m.isLegal())
                    {
                        m_machineHasToMove = false;
                        EngineParameter par = m_matchParameter;
                        par.ms_white = m_matchParameter.ms_totalTime - m_matchTime[White];
                        par.ms_black = m_matchParameter.ms_totalTime - m_matchTime[Black];
                        m_mainAnalysis->setMoveTime(par);
                        if (!doEngineMove(m, par))
                        {
                            m_autoRespond->trigger();
                        }
                        else
                        {
                            m_elapsedUserTime.start();
                            m_elapsedUserTimeValid = true;
                            slotBoardStoredMove();
                        }
                    }
                }
            }
        }
        else if (game().atLineEnd() && m_engineMatch->isChecked() && (m_AutoInsertLastBoard != game().board()))
        {
            m_AutoInsertLastBoard = game().board();

            int t = analysis.elapsedTimeMS() - m_matchParameter.ms_increment;
            m_matchTime[game().board().toMove()] += t;

            QTime tm(0,0);
            tm = tm.addMSecs(game().board().toMove()==White ? m_matchParameter.ms_white : m_matchParameter.ms_black);
            QString sTime = tm.toString("H:mm:ss");
            SlotDisplayTime(game().board().toMove(), sTime);
            BoardViewEx* frame = BoardViewFrame(m_boardView);
            if (frame) frame->startTime(game().board().toMove()==Black);

            if (m_matchParameter.tm != EngineParameter::TIME_GONG && m_matchTime[game().board().toMove()] > m_matchParameter.ms_totalTime)
            {
                playSound(":/sounds/fanfare.wav");
                m_engineMatch->trigger();
                if (game().isMainline())
                {
                    game().dbSetAnnotation(tr("Time is over"));
                    setResultAgainstColorToMove();
                }
            }
            else if (gameIsDraw())
            {
                playSound(":/sounds/fanfare.wav");
                m_engineMatch->trigger();
                if (game().isMainline())
                {
                    game().dbSetAnnotation(drawAnnotation());
                    game().setResult(Draw);
                }
            }
            else if(analysis.getEndOfGame())
            {
                if (analysis.score() == 0)
                {
                    game().setResult(Draw);
                }
                else
                {
                    setResultAgainstColorToMove();
                }
                playSound(":/sounds/fanfare.wav");
                // Game is terminated by engine
                m_engineMatch->trigger();
            }
            else if(!analysis.variation().isEmpty() && analysis.bestMove())
            {
                Move m = analysis.variation().first();
                if (m.isLegal())
                {
                    EngineParameter par = m_matchParameter;
                    par.ms_white = m_matchParameter.ms_totalTime - m_matchTime[White];
                    par.ms_black = m_matchParameter.ms_totalTime - m_matchTime[Black];
                    m_mainAnalysis->setMoveTime(par);
                    m_secondaryAnalysis->setMoveTime(par);
                    m_bMainAnalyisIsNextEngine = (sender()==m_mainAnalysis);
                    m_MoveForMatch = m;
                    m_EngineParameterForMatch = par;
                    QTimer::singleShot(0, this, SLOT(slotRestartAnalysis()));
                }
            }
            else
            {
                playSound(":/sounds/fanfare.wav");
                // Engines did not send something useful - could be drawn or something else
                m_engineMatch->trigger();
            }
        }
        else if (m_engineMatch->isChecked())
        {
            playSound(":/sounds/fanfare.wav");
            // Engines did not make progress or user intervention
            m_engineMatch->trigger();
        }
        else if (m_autoRespond->isChecked())
        {
            playSound(":/sounds/fanfare.wav");
            // Engines did not make progress or user intervention
            m_autoRespond->trigger();
        }
    }
}

void MainWindow::slotRestartAnalysis()
{
    m_mainAnalysis->setOnHold(m_bMainAnalyisIsNextEngine);
    m_secondaryAnalysis->setOnHold(!m_bMainAnalyisIsNextEngine);
    if (!doEngineMove(m_MoveForMatch,m_EngineParameterForMatch))
    {
        m_engineMatch->trigger();
    }
}

void MainWindow::slotAutoPlayTimeout()
{
    bool done = false;
    if (m_autoAnalysis->isChecked() && AppSettings->getValue("/Board/BackwardAnalysis").toBool())
    {
        done = game().atGameStart();
    }
    else
    {
        done = game().atGameEnd();
    }

    if(done)
    {
        AutoMoveAtEndOfGame();
    }
    else
    {
        //done = false;
        if (m_autoAnalysis->isChecked() && AppSettings->getValue("/Board/BackwardAnalysis").toBool())
        {
            slotGameMovePrevious();
        }
        else
        {
            slotGameMoveNext();
        }
        if (game().board().isCheckmate() ||
            game().board().isStalemate())
        {
            AutoMoveAtEndOfGame();
        }
    }
    if (m_autoPlay->isChecked())
    {
        m_autoPlayTimer->start();
    }
}

void MainWindow::AutoMoveAtEndOfGame()
{
    if(m_autoAnalysis->isChecked())
    {
        QString engineAnnotation = tr("Engine %1").arg(m_mainAnalysis->displayName());
        game().dbSetAnnotation(engineAnnotation, game().lastMove());

        if (AppSettings->getValue("/Board/AutoSaveAndContinue").toBool())
        {
            saveGame(databaseInfo());
            if (loadNextGame())
            {
                if (AppSettings->getValue("/Board/BackwardAnalysis").toBool())
                {
                    game().moveToEnd();
                }
            }
            else
            {
                // All games finished
                m_autoAnalysis->setChecked(false);
            }
        }
        else
        {
            // Game finished
            m_autoAnalysis->setChecked(false);
        }
    }
}

void MainWindow::slotFilterChanged(bool selectGame)
{
    GameId n = gameIndex();
    m_gameList->endSearch();
    if (selectGame)
    {
        m_gameList->selectGame(n);
    }
    if(VALID_INDEX(n))
    {
        m_gameList->setFocus();
    }
    int count = databaseInfo()->filter() ? databaseInfo()->filter()->count() : 0;
    QString f = count == (int)database()->count() ? tr("all") : QString::number(count);
    m_statusFilter->setText(QString(" %1: %2/%3 ").arg(databaseName())
                            .arg(f).arg(database()->count()));
}

void MainWindow::slotFilterLoad(GameId index)
{
    if(index != gameIndex())
    {
        gameLoad(index);
    }
}

void MainWindow::slotStatusMessageHint(const QString& msg)
{
    if (!msg.isEmpty() || m_lastMessageWasHint)
    {
        m_statusApp->setText(msg, true);
        m_lastMessageWasHint = !msg.isEmpty();
        if (!msg.isEmpty())
        {
            m_messageTimer->stop();
            m_messageTimer->start();
        }
    }
}

void MainWindow::slotStatusMessage(const QString& msg)
{
    m_lastMessageWasHint = false;
    m_statusApp->setText(msg);
    if (!msg.isEmpty())
    {
        m_messageTimer->stop();
        m_messageTimer->start();
    }
}

void MainWindow::slotOperationProgress(int progress)
{
    m_progressBar->setValue(progress);
}

void MainWindow::slotDbRestoreState(const Game& game)
{
    DatabaseInfo* pDb = qobject_cast<DatabaseInfo*>(sender());
    if (pDb)
    {
        pDb->replaceGame(game);
    }
}

void MainWindow::slotDatabaseChange()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
    {
        DatabaseInfo* db = action->data().value<DatabaseInfo*>();
        if (m_currentDatabase != db && !db->IsBook())
        {
            autoGroup->untrigger();
            m_currentDatabase = db;
            activateBoardViewForDbIndex(m_currentDatabase);
            m_databaseList->setFileCurrent(databaseInfo()->displayName());
            slotDatabaseChanged();
            if(database()->isReadOnly())
            {
                for(int i = 0; i < m_databases.count(); ++i)
                {
                    if(m_databases[i] != m_currentDatabase)
                    {
                        if(m_databases[i]->isValid() && m_databases[i]->database()->isReadOnly())
                        {
                            m_databases[i]->database()->index()->clearCache();
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::copyGame(int target, GameId index)
{
    DatabaseInfo* pTargetDB = m_databases[target];
    if(pTargetDB->isValid())
    {
        Game g;
        if(database()->loadGame(index, g))
        {
            QString fileName = pTargetDB->database()->name();
            QString msg;
            msg = tr("Append game %1 to %2.").arg(index + 1).arg(fileName);
            slotStatusMessage(msg);

            // The database is open and accessible
            pTargetDB->database()->appendGame(g);
            if(pTargetDB == m_currentDatabase)
            {
                emit databaseModified();
            }
        }
    }
}

void MainWindow::copyGames(QString fileName, QList<GameId> indexes)
{
    for(int i = 0; i < m_databases.count(); ++i)
    {
        if(m_databases[i]->displayName() == fileName)
        {
            if(m_databases[i]->isValid())
            {
                if (m_databases[i]==m_currentDatabase)
                {
                    m_gameList->startUpdate();
                }
                m_databases[i]->filter()->resize(m_databases[i]->database()->count() + static_cast<quint64>(indexes.count()), true);
                foreach (GameId index, indexes)
                {
                    copyGame(i, index);
                }
                QString msg = tr("Appended %1 games to %2.").arg(indexes.count()).arg(fileName);
                slotStatusMessage(msg);
                if (m_databases[i]==m_currentDatabase)
                {
                    m_gameList->endUpdate();
                }
            }
            return;
        }
    }

    // The database is closed
    Output writer(Output::Pgn);
    Game g;
    bool success = true;
    foreach (int index, indexes)
    {
        if(database()->loadGame(index, g))
        {
            QString msg;
            msg = tr("Append game %1 to %2.").arg(index + 1).arg(fileName);
            slotStatusMessage(msg);

            success = writer.append(fileName, g);
            m_databaseList->update(fileName);
            if (!success) break;
        }
    }
    QString msg = success ? tr("Appended %1 games to %2.").arg(indexes.count()).arg(fileName) :
                            tr("Error appending games to %1").arg(fileName);
    slotStatusMessage(msg);
}

void MainWindow::copyDatabase(QString target, QString src)
{
    if(QDir::cleanPath(target) != QDir::cleanPath(src))
    {
        QFileInfo fiSrc(src);
        QFileInfo fiDest(target);
        Database* pSrcDB = getDatabaseByPath(src);
        Database* pDestDB = getDatabaseByPath(target);
        DatabaseInfo* pDestDBInfo = getDatabaseInfoByPath(target);
        DatabaseInfo* pSrcDBInfo = getDatabaseInfoByPath(src);

        bool done = false;
        if(pDestDBInfo && pSrcDB && pDestDB && !pDestDB->isReadOnly() && (pSrcDB != pDestDB) && !pDestDBInfo->IsBook() && !pSrcDBInfo->IsBook())
        {
            // Both databases are open
            done = true;
            for(int i = 0; i < (int)pSrcDB->count(); ++i)
            {
                Game g;
                if(pSrcDB->loadGame(i, g))
                {
                    pDestDB->appendGame(g);
                }
            }
            QString msg = tr("Append games from %1 to %2.").arg(pSrcDB->name()).arg(pDestDB->name());
            slotStatusMessage(msg);

            pDestDBInfo->filter()->resize(pDestDB->count(), true);
        }
        else if((!pSrcDB || (pSrcDBInfo && !pSrcDBInfo->IsBook() && !pSrcDBInfo->modified())) && !pDestDB)
        {
            // Both databases are closed or the target is closed and the source unmodified (so that file can be used)
            QFile fSrc(src);
            QFile fDest(target);

            if(fiDest.exists() && fiDest.suffix()=="pgn"
                    && fiSrc.exists() && fiSrc.suffix()=="pgn"
                    && fSrc.open(QIODevice::ReadOnly) &&
                    fDest.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
            {
                done = true;
                QString msg = tr("Append games from %1 to %2.").arg(fiSrc.fileName()).arg(fiDest.fileName());
                slotStatusMessage(msg);
                fDest.write("\n");
                while(!fSrc.atEnd())
                {
                    QByteArray line = fSrc.readLine();
                    fDest.write(line);
                }
                fSrc.close();
                fDest.close();
            }
            if (done)
            {
                m_databaseList->update(target);
            }
        }
        else if (pSrcDB && pSrcDBInfo && !pSrcDBInfo->IsBook() && pSrcDBInfo->modified() && !pDestDB && fiDest.exists() && fiDest.suffix()=="pgn")
        {
            // Src is open and modified, target is closed
            QFile fDest(target);

            if(fDest.isWritable())
            {
                done = true;
                Output output(Output::Pgn);
                output.append(target, *pSrcDB);

                QString msg = tr("Append games from %1 to %2.").arg(pSrcDB->name()).arg(fiDest.fileName());
                slotStatusMessage(msg);
                m_databaseList->update(target);
            }
        }
        else if (!pSrcDB && fiSrc.exists() && fiSrc.suffix()=="pgn" && pDestDB)
        {
            // Source is closed, target is open
            StreamDatabase streamDb;
            if (streamDb.open(src, false))
            {
                Game g;
                while (streamDb.loadNextGame(g))
                {
                    pDestDB->appendGame(g);
                }
                QString msg = tr("Append games from %1 to %2.").arg(fiSrc.fileName()).arg(pDestDB->name());
                slotStatusMessage(msg);
                done = true;
                if (pDestDBInfo)
                {
                    pDestDBInfo->filter()->resize(pDestDB->count(), true);
                }
            }
        }
        else if (!pSrcDB && fiSrc.exists() && fiSrc.suffix()=="abk" && pDestDB)
        {
            // Source is closed, target is open
            ArenaBook abk;
            if (abk.open(src, false) && abk.parseFile())
            {
                QString msg = tr("Append games from %1 to %2.").arg(fiSrc.fileName()).arg(pDestDB->name());
                slotStatusMessage(msg);

                for (quint64 i=0; i<abk.count(); ++i)
                {
                    Game g;
                    if (abk.loadGame(i,g))
                    {
                        pDestDB->appendGame(g);
                    }
                }
                if (pDestDBInfo)
                {
                    pDestDBInfo->filter()->resize(pDestDB->count(), true);
                }
                done = true;
            }
        }

        if(done && (databaseInfo() == pDestDBInfo))
        {
            m_gameList->startUpdate();
            m_gameList->endUpdate();
            emit databaseChanged(databaseInfo());
            emit databaseModified();
        }
    }
}

void MainWindow::slotDatabaseDroppedFailed(QUrl url)
{
    m_mapDatabaseToDroppedUrl.remove(url);
}

void MainWindow::slotDatabaseDroppedHandler(QUrl url, QString filename)
{
    QString target = m_mapDatabaseToDroppedUrl[url];
    m_mapDatabaseToDroppedUrl.remove(url);
    copyDatabase(target, filename);
    if (QFile::exists(filename))
    {
        QFile::remove(filename);
    }
}

void MainWindow::slotDatabaseDropped(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    const DbMimeData* dbMimeData = qobject_cast<const DbMimeData*>(mimeData);

    if(dbMimeData && mimeData->hasUrls())
    {
        foreach (QUrl url, mimeData->urls())
        {
            copyDatabase(databaseInfo()->displayName(), url.toString());
        }
    }
    else if(mimeData && mimeData->hasUrls())
    {
        foreach (QUrl url, mimeData->urls())
        {
            if ((url.scheme() == "http") || (url.scheme() == "https") || (url.scheme() == "ftp") || (url.scheme() == "sftp"))
            {
                m_mapDatabaseToDroppedUrl[url] = databaseInfo()->displayName();
                downloadManager2->doDownloadToPath(url, "");
            }
            else
            {
                copyDatabase(databaseInfo()->displayName(), url.path());
            }
        }
    }
    event->accept();
}

void MainWindow::slotDatabaseCopy(int preselect)
{
    QList<GameId> gameIndexList = m_gameList->selectedGames();
    copyFromDatabase(preselect, gameIndexList);
}

void MainWindow::filterDuplicates(int mode)
{
    FilterOperator oper = FilterOperator::NullOperator;
    if (mode == DuplicateSearch::DS_Both_All)
    {
        oper = FilterOperator::Or;
    }
    if (mode == DuplicateSearch::DS_Tags && !databaseInfo()->filter()->database()->isReadOnly())
    {
        mode = DuplicateSearch::DS_Tags_BestGame;
    }
    Search* ds = (mode != DuplicateSearch::DS_Both_All) ?
            new DuplicateSearch (databaseInfo()->filter()->database(), DuplicateSearch::DSMode(mode)) :
            new DuplicateSearch (databaseInfo()->filter(), DuplicateSearch::DSMode(mode));
    m_openingTreeWidget->cancel();
    slotBoardSearchStarted();
    m_gameList->executeSearch(ds, oper);
}

void MainWindow::slotDatabaseFilterDuplicateGames()
{
    filterDuplicates(DuplicateSearch::DS_Both);
}

void MainWindow::slotDatabaseFilterDuplicateTags()
{
    filterDuplicates(DuplicateSearch::DS_Tags);
}

void MainWindow::copyFromDatabase(int preselect, QList<GameId> gameIndexList)
{
    QStringList db;
    int cc = 1;
    QList<DatabaseInfo*> targets;
    for (int i = 0; i < m_databases.count(); ++i)
    {
        if ((m_databases[i] != m_currentDatabase) && m_databases[i]->isNative())
        {
            db.append(tr("%1. %2 (%3 games)").arg(cc++).arg(databaseName(i))
                      .arg(m_databases[i]->database()->count()));
            targets.append(m_databases[i]);
        }
    }
    if (db.isEmpty())
    {
        MessageDialog::error(tr("You need at least two open databases to copy games"));
        return;
    }

    QString players = game().tag(TagNameWhite)+"-"+game().tag(TagNameBlack);

    CopyDialog dlg(this);
    dlg.setCurrentGame(players, gameIndexList.count(), m_currentDatabase->filter()->count(), m_currentDatabase->database()->count());
    dlg.setMode((CopyDialog::SrcMode)preselect);
    dlg.setDatabases(db);
    if(dlg.exec() != QDialog::Accepted)
    {
        return;
    }
    int targetIndex = dlg.getDatabase();
    if (targetIndex<0) return;
    DatabaseInfo* targetDb = targets.at(targetIndex);
    if (!targetDb) return;

    int n = 0;
    switch(dlg.getMode())
    {
    case CopyDialog::SingleGame:
        targetDb->database()->appendGame(game());
        n = 1;
        break;
    case CopyDialog::Selection:
        foreach (GameId i, gameIndexList)
        {
            Game g;
            if(database()->loadGame(i, g))
            {
                ++n;
                targetDb->database()->appendGame(g);
            }
        }
        break;
    case CopyDialog::Filter:
        for(GameId i = 0; i < (int)database()->count(); ++i)
        {
            Game g;
            if(databaseInfo()->filter()->contains(i) && database()->loadGame(i, g))
            {
                ++n;
                targetDb->database()->appendGame(g);
            }
        }
        break;
    case CopyDialog::AllGames:
        for(GameId i = 0; i < (int)database()->count(); ++i)
        {
            Game g;
            if(database()->loadGame(i, g))
            {
                ++n;
                targetDb->database()->appendGame(g);
            }
        }
        break;
    default:
        break;
    }
    targetDb->filter()->resize(targetDb->database()->count(), true);
    QString msg = tr("Append %1 games from %2 to %3.").arg(n).arg(database()->name()).arg(targetDb->database()->name());
    slotStatusMessage(msg);
}

void MainWindow::slotDatabaseClearClipboard()
{
    if (!m_currentDatabase->isClipboard())
    {
        autoGroup->untrigger();
    }

    m_databases[0]->database()->clear();
    m_databases[0]->clearLastGames();
    m_databases[0]->filter()->resize(0, false);
    m_databases[0]->newGame();

    if (m_currentDatabase->isClipboard())
    {
        emit databaseChanged(databaseInfo());
        emit databaseModified();
    }
}

void MainWindow::slotDatabaseCopySingle(QList<GameId> gameIndexList)
{
    copyFromDatabase(0, gameIndexList);
}

void MainWindow::slotDatabaseFindDuplicates(QList<GameId> gameIndexList)
{
    databaseInfo()->filter()->setAll(0);
    foreach(GameId i, gameIndexList)
    {
        databaseInfo()->filter()->set(i,1);
    }
    filterDuplicates(DuplicateSearch::DS_Both_All);
}

void MainWindow::slotDatabaseChanged()
{
    m_undoGroup.setActiveStack(databaseInfo()->undoStack());
    database()->index()->calculateCache();
    setWindowTitle(tr("%1 - ChessX").arg(databaseName()));
    m_gameList->setFilter(databaseInfo()->filter());
    updateLastGameList();
    slotFilterChanged();
    slotGameChanged(true);
    emit databaseChanged(databaseInfo());
    emit databaseModified();
}

void MainWindow::updateLastGameList()
{
    const CircularBuffer<GameId>& gameList = databaseInfo()->lastGames();
    const Index* index = databaseInfo()->database()->index();
    m_recentGames->clear();
    if (index) foreach(GameId n, gameList)
    {
        QString white = index->tagValue(TagNameWhite,n);
        QString black = index->tagValue(TagNameBlack,n);
        QAction* action = m_recentGames->addAction(QString::number(n)+" : "+white+"-"+black, this, SLOT(slotLoadRecentGame()));
        action->setData(n);
    }
}

void MainWindow::slotLoadRecentGame()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
    {
        GameId n = static_cast<GameId>(action->data().toInt());
        gameLoad(n);
    }
}

void MainWindow::slotSearchTag()
{
    m_gameList->simpleSearch(1);
}

void MainWindow::slotSearchBoard()
{
    BoardSearchDialog dlg(this);
    QList<Board> boardList;
    boardList.append(game().board());

    for(int i = 0; i < m_tabWidget->count(); ++i)
    {
        if(i != m_tabWidget->currentIndex())
        {
            BoardViewEx* boardViewEx = qobject_cast<BoardViewEx*>(m_tabWidget->widget(i));
            BoardView* boardView = boardViewEx->boardView();
            const Board& b = boardView->board();
            if (b != Board::standardStartBoard)
            {
                boardList.append(b);
            }
        }
    }

    dlg.setBoardList(boardList);

    if (dlg.exec() == QDialog::Accepted)
    {
        Search* ps = new PositionSearch (databaseInfo()->filter()->database(), boardList.at(dlg.boardIndex()));
        m_openingTreeWidget->cancel();
        slotBoardSearchStarted();
        m_gameList->executeSearch(ps, FilterOperator(dlg.mode()));
    }
}

void MainWindow::slotBoardSearchUpdate(int progress)
{
    slotFilterChanged(false);
    slotOperationProgress(progress);
}

void MainWindow::slotBoardSearchFinished()
{
    slotFilterChanged();
    finishOperation(tr("Search ended"));
}

void MainWindow::slotBoardSearchStarted()
{
    startOperation(tr("Searching..."));
}

void MainWindow::slotSearchReverse()
{
    m_gameList->filterInvert();
    slotFilterChanged();
}

void MainWindow::slotSearchReset()
{
    m_gameList->filterSetAll(1);
    slotFilterChanged();
}

void MainWindow::slotTreeUpdate(bool dbIsFilterSource)
{
    if (!gameMode())
    {
        if (dbIsFilterSource)
        {
            slotFilterChanged();
        }
        else
        {
            Search* ps = new PositionSearch (databaseInfo()->filter()->database(), m_openingTreeWidget->board());
            m_openingTreeWidget->cancel();
            slotBoardSearchStarted();
            m_gameList->executeSearch(ps);
        }
    }
}

void MainWindow::slotSearchTreeMove(const QModelIndex& index)
{
    Qt::KeyboardModifiers modifiers = QApplication::keyboardModifiers();
    bool addMove = m_openingTreeWidget->shouldAddMove() || (modifiers & Qt::ShiftModifier);

    QString move = m_openingTreeWidget->move(index);
    bool bEnd = (move == "[end]");
    Board b = m_openingTreeWidget->board();

    if(!bEnd)
    {
        Move m = b.parseMove(move);
        if (addMove && b==game().board())
        {
            if (!game().findNextMove(m))
            {
                if(game().atLineEnd())
                {
                    game().addMove(m);
                }
                else
                {
                    game().addVariation(m);
                    game().forward();
                }
            }
        }
        b.doMove(m);
    }

    updateOpeningTree(b, bEnd);
}

void MainWindow::updateOpeningTree(const Board& b, bool atEnd)
{
    if(!gameMode() && m_openingTreeWidget->isVisible())
    {
        QString name;
        DatabaseInfo* dbInfo;
        int index = m_openingTreeWidget->getFilterIndex(name);
        if (index > 1)
        {
            dbInfo = getDatabaseInfoByPath(name);
        }
        else
        {
            dbInfo = databaseInfo();
        }
        if (dbInfo && dbInfo->isValid())
        {
            m_openingTreeWidget->updateFilter(*dbInfo->filter(), b, atEnd);
        }
    }
}

void MainWindow::slotUpdateOpeningBook(QString name)
{
    AnalysisWidget* analysis = qobject_cast<AnalysisWidget*>(sender());
    if (analysis)
    {
        DatabaseInfo* dbInfo = getDatabaseInfoByPath(name);
        if (dbInfo && dbInfo->IsBook())
        {
            analysis->updateBookFile(dbInfo->database());
        }
        else
        {
            analysis->updateBookFile(0);
        }
    }
}

void MainWindow::slotDatabaseDeleteGame(QList<GameId> gameIndexList)
{
    foreach(GameId n, gameIndexList)
    {
        if(database()->deleted(n))
        {
            database()->undelete(n);
        }
        else
        {
            database()->remove(n);
        }
    }
}

void MainWindow::slotRenameEvent(QString ts)
{
    RenameTagDialog dlg(this, ts, TagNameEvent);
    connect(&dlg, SIGNAL(renameRequest(QString, QString, QString)), SLOT(slotRenameRequest(QString, QString, QString)));
    dlg.exec();
}

void MainWindow::slotRenamePlayer(QString ts)
{
    RenameTagDialog dlg(this, ts, TagNameWhite);
    connect(&dlg, SIGNAL(renameRequest(QString, QString, QString)), SLOT(slotRenameRequest(QString, QString, QString)));
    dlg.exec();
}

void MainWindow::slotRenameRequest(QString tag, QString newValue, QString oldValue)
{
    QStringList l;
    l << tag;
    if(tag == TagNameWhite)
    {
        l << TagNameBlack;
    }

    if(database()->index()->replaceTagValue(l, newValue, oldValue))
    {
        if(game().tag(tag) == oldValue)
        {
            game().setTag(tag, newValue);
        }
        if(tag == TagNameWhite)
        {
            if(game().tag(TagNameBlack) == oldValue)
            {
                game().setTag(TagNameBlack, newValue);
            }
        }
        database()->setModified(true);
        m_eventList->setDatabase(databaseInfo());
        m_playerList->setDatabase(databaseInfo());
        slotGameChanged(true);
        UpdateBoardInformation();
    }
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
    m_lastColor = 0;
    game().appendSquareAnnotation(m_annotationSquare, 0);
}

void MainWindow::slotGreenSquare()
{
    m_lastColor = 'G';
    game().appendSquareAnnotation(m_annotationSquare, 'G');
}

void MainWindow::slotYellowSquare()
{
    m_lastColor = 'Y';
    game().appendSquareAnnotation(m_annotationSquare, 'Y');
}

void MainWindow::slotRedSquare()
{
    m_lastColor = 'R';
    game().appendSquareAnnotation(m_annotationSquare, 'R');
}

void MainWindow::slotNoArrowHere()
{
    m_lastColor = 0;
    game().appendArrowAnnotation(m_annotationSquare, m_annotationSquareFrom, 0);
}

void MainWindow::slotGreenArrowHere()
{
    m_lastColor = 'G';
    game().appendArrowAnnotation(m_annotationSquare, m_annotationSquareFrom, 'G');
}

void MainWindow::slotYellowArrowHere()
{
    m_lastColor = 'Y';
    game().appendArrowAnnotation(m_annotationSquare, m_annotationSquareFrom, 'Y');
}

void MainWindow::slotRedArrowHere()
{
    m_lastColor = 'R';
    game().appendArrowAnnotation(m_annotationSquare, m_annotationSquareFrom, 'R');
}

BoardView* MainWindow::CreateBoardView()
{
    if (!databaseInfo()->IsBook())
    {
        BoardViewEx* boardViewEx = new BoardViewEx(m_tabWidget);
        BoardView* boardView = boardViewEx->boardView();
        boardView->configure();
        boardView->setBoard(Board::standardStartBoard);
        boardView->setDbIndex(m_currentDatabase);

        connect(this, SIGNAL(reconfigure()), boardView, SLOT(configure()));
        connect(boardView, SIGNAL(moveMade(Square, Square, int)), SLOT(slotBoardMove(Square, Square, int)));
        connect(boardView, SIGNAL(clicked(Square, int, QPoint, Square)), SLOT(slotBoardClick(Square, int, QPoint, Square)));
        connect(boardView, SIGNAL(wheelScrolled(int)), SLOT(slotBoardMoveWheel(int)));
        connect(boardView, SIGNAL(actionHint(QString)), SLOT(slotStatusMessageHint(QString)));
        connect(boardView, SIGNAL(evalRequest(Square, Square)), SLOT(slotEvalRequest(Square, Square)));
        connect(boardView, SIGNAL(evalMove(Square, Square)), SLOT(slotEvalMove(Square, Square)));
        connect(boardView, SIGNAL(evalModeDone()), SLOT(slotResumeBoard()));
        connect(boardViewEx, SIGNAL(signalNewAnnotation(QString)), SLOT(slotGameSetComment(QString)));
        connect(boardViewEx, SIGNAL(enterVariation(int)), this, SLOT(slotGameVarEnter(int)));
        connect(this, SIGNAL(signalGameIsEmpty(bool)), boardViewEx, SLOT(setAnnotationPlaceholder(bool)));


        if (databaseInfo()->IsFicsDB())
        {
            connect(m_ficsConsole, SIGNAL(SignalPlayerIsBlack(bool,bool)), boardViewEx, SLOT(configureTime(bool,bool)));
            connect(m_ficsConsole, SIGNAL(SignalStartTime(bool)), boardViewEx, SLOT(startTime(bool)));
        }

        m_tabWidget->addTab(boardViewEx, databaseName());
        m_tabWidget->setCurrentWidget(boardViewEx);
        UpdateBoardInformation();

        m_boardView = boardView;
        boardViewEx->slotReconfigure();

        return boardView;
    }
    return 0;
}

void MainWindow::activateBoardViewForDbIndex(void* dbIndex)
{
    int index = findBoardView(dbIndex);
    if (index >= 0)
    {
        activateBoardView(index);
    }
    else
    {
        CreateBoardView();
    }
    emit signalGameLoaded(game().startingBoard());
}

void MainWindow::closeBoardViewForDbIndex(void* dbIndex)
{
    int index = findBoardView(dbIndex);
    if (index >= 0)
    {
        QWidget* w = m_tabWidget->widget(index);
        m_tabWidget->removeTab(index);
        delete w;
    }
}

int MainWindow::findBoardView(void* dbIndex) const
{
    for(int i = 0; i < m_tabWidget->count(); ++i)
    {
        BoardViewEx* boardViewEx = qobject_cast<BoardViewEx*>(m_tabWidget->widget(i));
        BoardView* boardView = boardViewEx->boardView();
        if(boardView->dbIndex() == dbIndex)
        {
            return i;
        }
    }
    return -1;
}

void MainWindow::activateBoardView(int n)
{
    QWidget *w = m_tabWidget->widget(n);
    BoardViewEx* boardViewEx = qobject_cast<BoardViewEx*>(w);
    m_boardView = boardViewEx->boardView();
    m_tabWidget->setCurrentIndex(n);
}

void MainWindow::slotActivateBoardView(int n)
{
    if (m_tabWidget->isTabEnabled(n))
    {
        BoardViewEx* currentView = qobject_cast<BoardViewEx*>(m_tabWidget->currentWidget());
        if (currentView) currentView->saveConfig();

        activateBoardView(n);

        BoardViewEx* boardViewEx = qobject_cast<BoardViewEx*>(m_tabWidget->widget(n));
        boardViewEx->slotReconfigure();

        BoardView* boardView = boardViewEx->boardView();
        m_currentDatabase = qobject_cast<DatabaseInfo*>(boardView->dbIndex());

        Q_ASSERT(!databaseInfo()->IsBook());

        m_databaseList->setFileCurrent(databaseInfo()->displayName());
        slotDatabaseChanged();
        emit signalGameLoaded(game().startingBoard());
    }
}

void MainWindow::slotCloseTabWidget(int n)
{
    if(n < 0)
    {
        n = m_tabWidget->currentIndex();
    }

    if(m_tabWidget->count() > 1)
    {
        QWidget* w = m_tabWidget->widget(n);
        m_tabWidget->removeTab(n);
        BoardViewEx* view = qobject_cast<BoardViewEx*>(w);
        if (view) view->saveConfig();
        delete w;
    }

    slotActivateBoardView(m_tabWidget->currentIndex());
}

void MainWindow::UpdateGameTitle()
{
    QString white = game().tag(TagNameWhite);
    QString black = game().tag(TagNameBlack);
    QString eco = game().tag(TagNameECO).left(3);
    if(eco == "?")
    {
        eco.clear();
    }

    QString actualEco = game().ecoClassify();
    if(eco.isEmpty())
    {
        eco = actualEco.left(3);
    }
    eco = QString("<a href='eco:%1'>%2</a>").arg(eco).arg(eco);

    QString opName = actualEco.section(" ",1);
    if (!opName.isEmpty())
    {
        eco.append(" (" + opName + ")");
    }

    QString whiteElo = game().tag(TagNameWhiteElo);
    QString blackElo = game().tag(TagNameBlackElo);
    if(whiteElo == "?")
    {
        whiteElo = QString();
    }
    if(blackElo == "?")
    {
        blackElo = QString();
    }
    QString players = QString("<b><a href=\"tag:white\">%1</a></b> %2 - <b><a href=\"tag:black\">%3</a></b> %4")
                      .arg(white).arg(whiteElo).arg(black).arg(blackElo);
    QString result = QString("<b>%1</b> &nbsp;").arg(game().tag(TagNameResult));

    QString eventInfo = game().eventInfo();
    QString event = QString("<a href='event:%1'>%2</a>").arg(game().tag(TagNameEvent)).arg(eventInfo);
    QString title;
    if(!white.isEmpty() || !black.isEmpty())
    {
        title.append(players);
    }
    else
    {
        title.append(tr("<b>New game</b>"));
    }
    if(game().result() != ResultUnknown)
    {
        title.append(QString(", ") + result);
    }
    if(eco.length() <= 3)
    {
        title.append(QString(" ") + eco);
    }
    if(event.length() > 8)
    {
        title.append(QString("<br>") + event);
    }
    if(eco.length() > 3)
    {
        title.append(QString("<br>") + eco);
    }
    m_gameTitle->setText(QString("<qt>%1</qt>").arg(title));
}

void MainWindow::UpdateBoardInformation()
{
    if (!databaseInfo()->IsBook())
    {
        QString name = "<div align='center'><p>" + databaseName() + "</p>";
        QString nameWhite = game().tag(TagNameWhite);
        QString nameBlack = game().tag(TagNameBlack);
        if(!(nameWhite.isEmpty() && nameBlack.isEmpty()))
        {
            name += "<p align='center'><font color='midnightblue'>" +
                    nameWhite  + "-" +
                    nameBlack + "</font></p>";
        }
        name += "</div>";
        m_tabWidget->setTabToolTip(m_tabWidget->currentIndex(), name);
    }
}

void MainWindow::slotScreenShot()
{
    QPixmap pixmap = QPixmap::grabWindow(effectiveWinId());

    QString shotDir = AppSettings->shotsPath();
    QDir().mkpath(shotDir);

    QString fileName = shotDir + QDir::separator() + "shot-" + QDateTime::currentDateTime().toString() + ".png";

    pixmap.save(fileName);
}

void MainWindow::slotCompileECO()
{
    QString filepath = QFileDialog::getOpenFileName(0, "Compile ECO", QDir::currentPath(), "ECO Text File (*.txt);;All Files(*.*)");
    if (!filepath.isEmpty())
    {
        (void) compileAsciiEcoFile(filepath, "chessx.eco", "chessx.gtm");
    }
}

void MainWindow::slotEditActions()
{
    ActionDialog actionsDialog(this);
    actionsDialog.exec();
}

void MainWindow::slotMoveIntervalChanged(int interval)
{
    AppSettings->setValue("/Board/AutoPlayerInterval", interval);
    if(m_autoPlayTimer->interval() != interval)
    {
        m_autoPlayTimer->setInterval(interval);
    }

    if (!m_autoRespond->isChecked() && !m_engineMatch->isChecked())
    {
        m_mainAnalysis->setMoveTime(interval);
        m_secondaryAnalysis->setMoveTime(interval);
    }
}

void MainWindow::slotSetSliderText(int interval)
{
    if (!interval)
    {
        m_sliderText->setText(QString("0s/")+tr("Infinite"));
    }
    else
    {
        QTime t = QTime::fromMSecsSinceStartOfDay(interval);
        m_sliderText->setText(t.toString("mm:ss"));
    }
}

void MainWindow::slotUpdateOpeningTreeWidget()
{
    QStringList files; // List of all open files excluding ClipBoard
    for(int i = 1; i < m_databases.count(); i++)
    {
        QString displayName = m_databases[i]->displayName();
        files << displayName;
    }
    emit signalUpdateDatabaseList(files);
}

void MainWindow::slotMakeBook(QString pathIn)
{
    for(int i = 0; i < m_databases.count(); i++)
    {
        if(m_databases[i]->database()->filename() == pathIn)
        {
            DlgSaveBook dlg(pathIn);
            connect(&dlg, SIGNAL(bookWritten(QString)), SLOT(slotShowInFinder(QString)));
            if (dlg.exec() == QDialog::Accepted)
            {
                QString out;
                int maxPly, minGame, result, filterResult;
                bool uniform;
                dlg.getBookParameters(out, maxPly, minGame, uniform, result, filterResult);
                PolyglotWriter* polyglotWriter = new PolyglotWriter(this);
                connect(polyglotWriter, SIGNAL(bookBuildError(QString, PolyglotWriter*)), SLOT(slotBookBuildError(QString, PolyglotWriter*)));
                connect(polyglotWriter, SIGNAL(bookBuildFinished(QString, PolyglotWriter*)), SLOT(slotBookDone(QString, PolyglotWriter*)), Qt::QueuedConnection);
                connect(polyglotWriter, SIGNAL(progress(int)), SLOT(slotOperationProgress(int)), Qt::QueuedConnection);
                startOperation(tr("Build book"));
                m_polyglotWriters.append(polyglotWriter);
                polyglotWriter->writeBookForDatabase(m_databases[i]->database(), out, maxPly, minGame, uniform, result, filterResult);
            }
            return;
        }
    }
}

void MainWindow::cancelPolyglotWriters()
{
    foreach (PolyglotWriter* writer, m_polyglotWriters)
    {
        writer->cancel();
    }
}

void MainWindow::slotBookDone(QString path, PolyglotWriter* writer)
{
    finishOperation(tr("Book built"));
    slotShowInFinder(path);
    if (!m_polyglotWriters.removeOne(writer))
    {
        qDebug() << "Missing writer";
    }
}

void MainWindow::slotShowInFinder(QString path)
{
    ShellHelper::showInFinder(path);
}

void MainWindow::slotBookBuildError(QString /*path*/, PolyglotWriter* writer)
{
    MessageDialog::warning(tr("Could not build book"), tr("Polyglot Error"));
    finishOperation(tr("Book build finished with Error"));
    if (!m_polyglotWriters.removeOne(writer))
    {
        qDebug() << "Missing writer";
    }
}

void MainWindow::slotToggleGameMode()
{
    if (m_match->isChecked() != gameMode())
    {
        enterGameMode(m_match->isChecked());
        if(m_match->isChecked())
        {
            if (!MatchParameterDlg::getParametersForMatch(m_matchParameter))
            {
                m_match->setChecked(false);
                return;
            }
        }
        m_matchParameter.reset();
        m_matchTime[0] = 0;
        m_matchTime[1] = 0;
        m_elapsedUserTimeValid = false;

        QTime tmWhite(0,0);
        tmWhite = tmWhite.addMSecs(m_matchParameter.ms_white);
        SlotDisplayTime(White, tmWhite.toString("H:mm:ss"));

        QTime tmBlack(0,0);
        tmBlack = tmBlack.addMSecs(m_matchParameter.ms_black);
        SlotDisplayTime(Black, tmBlack.toString("H:mm:ss"));

        SlotShowTimer(gameMode());
    }
}

void MainWindow::slotFlipView(bool flip)
{
    m_boardView->setFlipped(flip);
}

void MainWindow::enterNoHintMode(bool gameMode)
{
    Guess::setGuessAllowed(!gameMode);
    Engine::setAllowEngineOutput(!gameMode);
    Tablebase::setAllowEngineOutput(!gameMode);
}

void MainWindow::enterGameMode(bool gameMode)
{
    m_boardView->setDragged(Empty);
    enterNoHintMode(gameMode);
    if (gameMode)
    {
        m_openingTreeWidget->cancel();
        m_boardView->setFlags(m_boardView->flags() | BoardView::IgnoreSideToMove);
    }
    else
    {
        m_boardView->setFlags(m_boardView->flags() & ~BoardView::IgnoreSideToMove);
    }

    databaseInfo()->undoStack()->setActive(!gameMode);
    int currentTab = m_tabWidget->currentIndex();
    for (int i=0; i<m_tabWidget->count();++i)
    {
        if (i != currentTab || !gameMode)
        {
            m_tabWidget->setTabEnabled(i, !gameMode);
        }
    }
    setGameMode(gameMode);
    m_ficsConsole->SlotGameModeChanged(gameMode);
}

bool MainWindow::premoveAllowed() const
{
    return ((gameMode() && m_ficsConsole->canUsePremove() && qobject_cast<const FicsDatabase*>(database()))
            || m_autoRespond->isChecked());
}

void MainWindow::slotToggleBrush()
{
    if (gameMode())
    {
        // Not allowed
        brushGroup->untrigger();
    }
    else
    {
        m_boardView->setBrushMode(brushGroup->checkedAction());
    }
}

void MainWindow::slotShowTargetFields()
{
    QAction* bt = qobject_cast<QAction*>(sender());
    AppSettings->setValue("/Board/showTargets", bt->isChecked());
    m_boardView->configure();
}

void MainWindow::slotShowThreat()
{
    QAction* bt = qobject_cast<QAction*>(sender());
    AppSettings->setValue("/Board/showThreat", bt->isChecked());
    m_boardView->configure();
}

void MainWindow::slotShowWhiteAttacks()
{
    QAction* bt = qobject_cast<QAction*>(sender());
    m_boardView->setShowAttacks(bt->isChecked() ? White:NoColor);
 }

void MainWindow::slotShowBlackAttacks()
{
    QAction* bt = qobject_cast<QAction*>(sender());
    m_boardView->setShowAttacks(bt->isChecked() ? Black:NoColor);
}

void MainWindow::slotShowUnderprotectedWhite()
{
    QAction* bt = qobject_cast<QAction*>(sender());
    m_boardView->setShowUnderProtection(bt->isChecked() ? White:NoColor);
}

void MainWindow::slotShowUnderprotectedBlack()
{
    QAction* bt = qobject_cast<QAction*>(sender());
    m_boardView->setShowUnderProtection(bt->isChecked() ? Black:NoColor);
}
