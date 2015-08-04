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
#include "board.h"
#include "boardsearchdialog.h"
#include "boardsetup.h"
#include "boardview.h"
#include "copydialog.h"
#include "chessbrowser.h"
#include "compileeco.h"
#include "databaseinfo.h"
#include "databaselist.h"
#include "dlgsavebook.h"
#include "ecolistwidget.h"
#include "editaction.h"
#include "eventlistwidget.h"
#include "exclusiveactiongroup.h"
#include "ficsclient.h"
#include "ficsconsole.h"
#include "ficsdatabase.h"
#include "game.h"
#include "gamelist.h"
#include "gamewindow.h"
#include "mainwindow.h"
#include "matchparameterdlg.h"
#include "messagedialog.h"
#include "memorydatabase.h"
#include "openingtreewidget.h"
#include "output.h"
#include "pgndatabase.h"
#include "playerlistwidget.h"
#include "polyglotwriter.h"
#include "preferences.h"
#include "promotiondialog.h"
#include "recipientaddressdialog.h"
#include "renametagdialog.h"
#include "shellhelper.h"
#include "settings.h"
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
                   AppSettings->value("/General/databasePath").toString(),
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
        AppSettings->setValue("/General/databasePath",
                              QFileInfo(file).absolutePath());
    }
}

void MainWindow::slotFileOpen()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open database"),
                        AppSettings->value("/General/databasePath").toString(),
                        tr("PGN databases (*.pgn);;Polyglot books (*.bin)"));
    foreach(QString file, files)
    {
        if(!file.isEmpty())
        {
            AppSettings->setValue("/General/databasePath", QFileInfo(file).absolutePath());
            openDatabaseUrl(file, false);
        }
    }
}

void MainWindow::slotFileOpenUtf8()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open database"),
                        AppSettings->value("/General/databasePath").toString(),
                        tr("PGN databases (*.pgn)"));
    foreach(QString file, files)
    {
        if(!file.isEmpty())
        {
            AppSettings->setValue("/General/databasePath", QFileInfo(file).absolutePath());
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
        startOperation(tr("Saving %1...").arg(dbInfo->database()->name()));
        Output output(Output::Pgn);
        connect(&output, SIGNAL(progress(int)), SLOT(slotOperationProgress(int)));
        output.output(dbInfo->database()->filename(), *database());
        finishOperation(tr("%1 saved").arg(dbInfo->database()->name()));
    }
}


bool MainWindow::QuerySaveDatabase()
{
    if(QuerySaveGame())
    {
        if(m_currentDatabase && qobject_cast<MemoryDatabase*>(database()))
        {
            if(databaseInfo()->isValid() && database()->isModified())
            {
                int result = MessageDialog::yesNoCancel(tr("The current database is modified!")
                                                        + '\n' + tr("Save it?"));
                if(MessageDialog::Yes == result)
                {
                    saveDatabase(databaseInfo());
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
    if(database()->isReadOnly())
    {
        MessageDialog::warning(tr("<html>The database <i>%1</i> is read-only and cannot be saved.</html>")
                               .arg(database()->name()));
    }
    else if(m_currentDatabase && qobject_cast<MemoryDatabase*>(database()))
    {
        saveDatabase(databaseInfo());
    }
}

void MainWindow::slotFileClose()
{
    if(m_currentDatabase)
    {
        // Don't remove Clipboard
        if(databaseInfo()->IsLoaded())
        {
            if(QuerySaveDatabase())
            {
                autoGroup->untrigger();

                bool ficsDB = (qobject_cast<FicsDatabase*>(database()));
                if (ficsDB)
                {
                    m_ficsClient->exitSession();
                }

                closeBoardViewForDbIndex(databaseInfo());

                DatabaseInfo* aboutToClose = databaseInfo();
                m_openingTreeWidget->cancel();
                m_databaseList->setFileClose(aboutToClose->filePath(), aboutToClose->currentIndex());

                m_databases.removeAt(m_currentDatabase);
                aboutToClose->close();
                emit signalDatabaseOpenClose();
                delete aboutToClose;

                if (m_currentDatabase != 0)
                {
                    m_currentDatabase = 0; // Switch to clipboard is always safe
                    activateBoardViewForDbIndex(databaseInfo());
                    m_databaseList->setFileCurrent("Clipboard");
                    slotDatabaseChanged();
                }
            }
        }
    }
}

void MainWindow::slotFileCloseIndex(int n)
{
    if(m_currentDatabase == n)
    {
        slotFileClose();
    }
    else if(n)  // Don't remove Clipboard
    {
        if(m_databases[n]->IsLoaded())
        {
            if (!QuerySaveGame(m_databases[n]))
            {
                return;
            }

            bool ficsDB = (qobject_cast<FicsDatabase*>(m_databases[n]->database()));
            if (ficsDB)
            {
                m_ficsClient->exitSession();
            }

            closeBoardViewForDbIndex(m_databases[n]);

            m_openingTreeWidget->cancel();

            m_databaseList->setFileClose(m_databases[n]->filePath(), m_databases[n]->currentIndex());

            m_databases[n]->close();

            delete m_databases[n];
            m_databases.removeAt(n);

            if(m_currentDatabase > n)
            {
                // hack as we have just moved the index by one
                m_currentDatabase--;
            }

            emit signalDatabaseOpenClose();
        }
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

void MainWindow::slotConfigure()
{
    PreferencesDialog dlg(this);
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
    updateMenuRecent();
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
    Game curgame = game();
    Output output(Output::Pgn);
    QString pgn = output.output(&curgame);

    RecipientAddressDialog recAdDialog(this);
    if (recAdDialog.exec() == QDialog::Accepted)
    {
        const QString white = curgame.tag(TagNameWhite);
        const QString black = curgame.tag(TagNameBlack);

        QString recipient = recAdDialog.getEmail();
        QString mailTo("mailto:%1?subject=Game %2-%3&body=%4");
        QUrl url = mailTo.arg(recipient).arg(white).arg(black).arg(pgn);
        QDesktopServices::openUrl(url);
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
        MemoryDatabase pgnDatabase;
        if (pgn.trimmed().startsWith("[")) // looks like something containing tags
        {
            if(pgnDatabase.openString(pgn))
            {
                Game g;
                for (quint64 i=0; i<pgnDatabase.count();++i) // pasted text might contain multiple games
                {
                    if(pgnDatabase.loadGame(i, g))
                    {
                        game().mergeWithGame(g);
                    }
                }
            }
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
    slotCreateBoardImage(image, 1.0);
    QApplication::clipboard()->setImage(image);
}

void MainWindow::slotExportImage()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Export Image"),
                   AppSettings->value("/General/databasePath").toString(),
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

void MainWindow::slotMoveStarted()
{
    if (premoveAllowed())
    {
        m_ficsConsole->SendStoredMove(InvalidSquare,InvalidSquare);
    }
}

void MainWindow::slotMoveFinished()
{
}

void MainWindow::slotBoardMove(Square from, Square to, int button)
{
    const Board& board = game().board();
    Move m(board.prepareMove(from, to));

    if(m.isLegal())
    {
        PieceType promotionPiece = None;
        if(m.isPromotion())
        {
            int index = 0;
            if ((button & Qt::MetaModifier) || !AppSettings->getValue("/Board/AutoPromoteToQueen").toBool())
            {
                index = PromotionDialog(0,m.color()).getIndex();
                if(index<0)
                {
                    return;
                }
            }
            promotionPiece = PieceType(Queen + index);
            m.setPromotionPiece(promotionPiece);
        }

        // Use an existing move with the correct promotion piece type if it already exists
        if(game().findNextMove(from, to, promotionPiece))
        {
            if((button & Qt::AltModifier) && (!(button & Qt::ControlModifier)))
            {
                // The move exists but adding a variation was requested anyhow
                // Take back the move and proceed as if the move does not yet exist
                game().backward();
            }
            else
            {
                if (m_autoRespond->isChecked())
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
                slotGameChanged();
                return;
            }
        }

        if(!m_training->isChecked())
        {
            if(game().atLineEnd())
            {
                QString annot;
                if (m_autoRespond->isChecked())
                {
                    int t = m_elapsedUserTimeValid ? m_elapsedUserTime.elapsed() : 0;
                    t = t - m_matchParameter.ms_bonus;
                    if (t<0) t = 0;
                    t = t - m_matchParameter.ms_increment;
                    m_matchTime[game().board().toMove()] += t;

                    EngineParameter par = m_matchParameter;
                    par.ms_white = m_matchParameter.ms_totalTime - m_matchTime[White];
                    par.ms_black = m_matchParameter.ms_totalTime - m_matchTime[Black];

                    m_mainAnalysis->unPin();
                    m_mainAnalysis->setMoveTime(par);

                    if (!m_elapsedUserTimeValid && !m_mainAnalysis->isEngineRunning())
                    {
                        m_mainAnalysis->startEngine();
                    }

                    if (par.tm != EngineParameter::TIME_GONG && m_matchTime[game().board().toMove()] > (int) m_matchParameter.ms_totalTime)
                    {
                        playSound(":/sounds/fanfare.wav");
                        // Game is drawn by repetition or 50 move rule
                        m_autoRespond->trigger();
                        if (game().isMainline())
                        {
                            setResultAgainstColorToMove();
                        }
                    }
                    else if (par.annotateEgt && par.tm != EngineParameter::TIME_GONG)
                    {
                        QString clk = "[%clk %1]";
                        QTime t(0,0);
                        t = t.addMSecs(game().board().toMove()==White ? par.ms_white : par.ms_black);
                        annot = clk.arg(t.toString("H:mm:ss"));
                    }
                }

                game().addMove(m, annot);
                if (qobject_cast<FicsDatabase*>(database()))
                {
                    m_ficsConsole->SendMove(m.toAlgebraic());
                }
                else
                {
                    if (m_autoRespond->isChecked())
                    {
                        if (game().board().isStalemate() ||
                            game().board().isCheckmate())
                        {
                            playSound(":/sounds/fanfare.wav");
                            m_autoRespond->trigger();
                            setResultForCurrentPosition();
                        }
                    }
                }
            }
            else
            {
                if(button & Qt::ControlModifier)
                {
                    if(button & Qt::AltModifier)
                    {
                        game().insertMove(m);
                    }
                    else
                    {
                        game().replaceMove(m);
                    }
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
    else
    {
        // Move is not legal, but could become legal, so store as premove and try later
        if (premoveAllowed())
        {
            m_ficsConsole->SendStoredMove(from,to);
            m_boardView->setStoredMove(from,to);
        }
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
                menu->addAction(QIcon(":/images/square_red.png"),   tr("Red Square"),    this, SLOT(slotRedSquare()));
                menu->addAction(QIcon(":/images/square_yellow.png"), tr("Yellow Square"), this, SLOT(slotYellowSquare()));
                menu->addAction(QIcon(":/images/square_green.png"), tr("Green Square"),  this, SLOT(slotGreenSquare()));
                menu->addSeparator();
                menu->addAction(QIcon(":/images/square_none.png"),  tr("Remove Color"),  this, SLOT(slotNoColorSquare()));
            }
            else
            {
                menu->addAction(QIcon(":/images/arrow_red.png"),   tr("Red Arrow to here"),    this, SLOT(slotRedArrowHere()));
                menu->addAction(QIcon(":/images/arrow_yellow.png"), tr("Yellow Arrow to here"), this, SLOT(slotYellowArrowHere()));
                menu->addAction(QIcon(":/images/arrow_green.png"), tr("Green Arrow to here"),  this, SLOT(slotGreenArrowHere()));
                menu->addSeparator();
                menu->addAction(QIcon(":/images/arrow_none.png"),  tr("Remove Arrow to here"), this, SLOT(slotNoArrowHere()));
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
    else if ((button & Qt::LeftButton) && (button & Qt::ShiftModifier))
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
    m_currentFrom = InvalidSquare;
    m_currentTo = InvalidSquare;

    emit displayTime(g.timeAnnotation(m, Game::BeforeMove), g.board().toMove(), g.timeAnnotation(m, Game::AfterMove));

    // Highlight current move
    m_gameView->showMove(m);
    if (g.isMainline())
    {
        m_gameView->slotDisplayPly(g.ply());
    }

    QList<MoveId> listVariations = g.variations();
    if (listVariations.size() && !game().atLineEnd())
    {
        listVariations.push_front(game().nextMove());
    }
    m_gameWindow->showVariations(listVariations);

    slotSearchTree();
    emit boardChange(g.board());

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

void MainWindow::slotSearchTree()
{
    updateOpeningTree(game().board(), false);
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
        slotGameMoveNext();
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
    if(databaseInfo()->filter()->count())
    {
        int random = rand() % databaseInfo()->filter()->count();
        gameLoad(databaseInfo()->filter()->indexToGame(random));
    }
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
    databaseInfo()->newGame();
    m_gameList->removeSelection();
    emit signalGameIsEmpty(true); // repair effect of slotGameChanged
    m_gameList->setFocus();
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
        dbInfo->saveGame();
        dbInfo->database()->index()->setTag(TagNameLength, QString::number((dbInfo->currentGame().plyCount() + 1) / 2), dbInfo->currentIndex());
        m_gameList->updateFilter();
        slotFilterChanged();
        slotGameChanged();
        UpdateBoardInformation();

        if(AppSettings->getValue("/General/autoCommitDB").toBool())
        {
            saveDatabase(dbInfo);
            emit databaseChanged(dbInfo);
        }
    }
}

void MainWindow::slotDatabaseModified()
{
    m_gameList->updateFilter();
    slotFilterChanged();
    emit signalCurrentDBisReadWrite((m_currentDatabase > 0) && !databaseInfo()->database()->isReadOnly() && databaseInfo()->database()->isModified());
    emit signalCurrentDBcanBeClosed(m_currentDatabase > 0);
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

void MainWindow::slotMergeActiveGame(QList<int> gameIndexList)
{
    if (gameIndexList.size())
    {
        Game state = game();
        foreach(int gameIndex, gameIndexList)
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

void MainWindow::slotMergeActiveGame(int gameIndex)
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
    for(int i = 0; i < database()->index()->count(); ++i)
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
    for(int i = 0; i < (int)database()->count(); ++i)
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

void MainWindow::slotGameChanged()
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
    qDebug() << m_boardView->board().toFen();
    qDebug() << game().board().toFen();
}

void MainWindow::slotGameAddVariation(const Analysis& analysis)
{
    QString score;
    if (!analysis.bestMove())
    {
        score = QString::number(analysis.score() / 100.0, 'f', 2);
    }
    game().addVariation(analysis.variation(), score);
}

bool MainWindow::addVariation(const QString& s)
{
    bool added = false;
    if(game().atLineEnd())
    {
        added = game().addMove(s) != NO_MOVE;
    }
    else
    {
        added = game().addVariation(s) != NO_MOVE;
    }
    return added;
}

bool MainWindow::slotGameAddVariation(const QString& san)
{
    QString s = san;
    s = s.remove(QRegExp("-.*"));
    s = s.remove(QRegExp("[0-9]*\\."));   
    return addVariation(s);
}

void MainWindow::slotGameUncomment()
{
    game().removeComments();
}

void MainWindow::slotGameRemoveVariations()
{
    game().removeVariations();
}

void MainWindow::slotToggleTraining()
{
    if (AppSettings->getValue("/Board/noHints").toBool())
    {
        QAction* match = qobject_cast<QAction*>(sender());
        enterGameMode(match->isChecked());
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
        if (!MatchParameterDlg::getParameters(m_matchParameter))
        {
            m_autoRespond->setChecked(false);
            return;
        }

        m_matchParameter.reset();
        m_mainAnalysis->setMoveTime(m_matchParameter);
        m_machineHasToMove = true;
    }
    else
    {
        m_mainAnalysis->stopEngine();
        // Reset to value from slider
        m_mainAnalysis->setMoveTime(m_sliderSpeed->translatedValue());
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
            m_mainAnalysis->unPin();
            m_mainAnalysis->setPosition(game().board());
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
        if (!MatchParameterDlg::getParameters(m_matchParameter))
        {
            m_engineMatch->setChecked(false);
            return;
        }
        m_AutoInsertLastBoard.clear();
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

            m_mainAnalysis->unPin();
            m_mainAnalysis->setOnHold(true);
            m_mainAnalysis->setPosition(game().board());
            m_mainAnalysis->setMoveTime(m_matchParameter);

            m_secondaryAnalysis->unPin();
            m_secondaryAnalysis->setOnHold(true);
            m_secondaryAnalysis->setPosition(game().board());
            m_secondaryAnalysis->setMoveTime(m_matchParameter);

            if (game().tag(TagNameWhite).isEmpty() &&
                game().tag(TagNameBlack).isEmpty())
            {
                QString engine1 = m_mainAnalysis->engineName();
                QString engine2 = m_secondaryAnalysis->engineName();

                game().setTag(TagNameWhite, engine1);
                game().setTag(TagNameBlack, engine2);
            }

            m_matchTime[0] = 0;
            m_matchTime[1] = 0;

            if (game().board().toMove() == White)
                m_mainAnalysis->startEngine();
            else
                m_secondaryAnalysis->startEngine();
        }
    }
    else
    {
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

bool MainWindow::doEngineMove(Move m, EngineParameter e)
{
    m_currentFrom = m.from();
    m_currentTo = m.to();

    QString annot;
    if (e.annotateEgt && e.tm != EngineParameter::TIME_GONG)
    {
        QString clk = "[%clk %1]";
        QTime t(0,0);
        t = t.addMSecs(game().board().toMove()==White ? e.ms_white : e.ms_black);
        annot = clk.arg(t.toString("H:mm:ss"));
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

void MainWindow::slotEngineTimeout(const Analysis& analysis)
{
    if (m_boardView->dragged() == Empty) // Do not interfer with moving a piece
    {
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
                    // Game is terminated by end of time
                    m_autoRespond->trigger();
                    if (game().isMainline())
                    {
                        setResultAgainstColorToMove();
                    }
                }
                else if (game().board().insufficientMaterial() ||
                    game().positionRepetition3(game().board()) ||
                    game().board().halfMoveClock() > 101)
                {
                    playSound(":/sounds/fanfare.wav");
                    // Game is drawn by repetition or 50 move rule
                    m_autoRespond->trigger();
                    if (game().isMainline())
                    {
                        game().setResult(Draw);
                    }
                }
                else if(analysis.getEndOfGame())
                {
                    if (analysis.score() == 0.0)
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

            if (m_matchParameter.tm != EngineParameter::TIME_GONG && m_matchTime[game().board().toMove()] > m_matchParameter.ms_totalTime)
            {
                playSound(":/sounds/fanfare.wav");
                // Game is terminated by end of time

                m_engineMatch->trigger();
                if (game().isMainline())
                {
                    setResultAgainstColorToMove();
                }
            }
            else if (game().board().insufficientMaterial() ||
                game().positionRepetition3(game().board()) ||
                game().board().halfMoveClock() > 101)
            {
                playSound(":/sounds/fanfare.wav");
                // Game is drawn by repetition or 50 move rule
                m_engineMatch->trigger();
                if (game().isMainline())
                {
                    game().setResult(Draw);
                }
            }
            else if(analysis.getEndOfGame())
            {
                if (analysis.score() == 0.0)
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
                    m_mainAnalysis->setOnHold(sender()==m_mainAnalysis);
                    m_secondaryAnalysis->setOnHold(sender()==m_secondaryAnalysis);
                    if (!doEngineMove(m,par))
                    {
                        m_engineMatch->trigger();
                    }
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
        done = false;
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

void MainWindow::slotFilterChanged()
{
    if(gameIndex() >= 0)
    {
        m_gameList->selectGame(gameIndex());
        m_gameList->setFocus();
    }
    int count = databaseInfo()->filter() ? databaseInfo()->filter()->count() : 0;
    QString f = count == (int)database()->count() ? tr("all") : QString::number(count);
    m_statusFilter->setText(QString(" %1: %2/%3 ").arg(databaseName())
                            .arg(f).arg(database()->count()));
}

void MainWindow::slotFilterLoad(int index)
{
    if(index != gameIndex())
    {
        gameLoad(index);
    }
}

void MainWindow::slotStatusMessage(const QString& msg)
{
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
        int n = action->data().toInt();
        if (m_currentDatabase != n && !m_databases[n]->IsBook())
        {
            autoGroup->untrigger();
            m_currentDatabase = action->data().toInt();
            activateBoardViewForDbIndex(m_databases[m_currentDatabase]);
            m_databaseList->setFileCurrent(databaseInfo()->filePath());
            slotDatabaseChanged();
            if(database()->isReadOnly())
            {
                for(int i = 0; i < m_databases.count(); ++i)
                {
                    if(i != m_currentDatabase)
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

void MainWindow::copyGame(int target, int index)
{
    if(m_databases[target]->isValid())
    {
        Game g;
        if(database()->loadGame(index, g))
        {
            QString fileName = m_databases[target]->database()->name();
            QString msg;
            msg = tr("Append game %1 to %2.").arg(index + 1).arg(fileName);
            slotStatusMessage(msg);

            // The database is open and accessible
            m_databases[target]->database()->appendGame(g);
            if(index == m_currentDatabase)
            {
                emit databaseModified();
            }
        }
    }
}

void MainWindow::copyGames(QString fileName, QList<int> indexes)
{
    for(int i = 0; i < m_databases.count(); ++i)
    {
        if(m_databases[i]->filePath() == fileName)
        {
            if(m_databases[i]->isValid())
            {
                foreach (int index, indexes)
                {
                    copyGame(i, index);
                }
                m_databases[i]->filter()->resize(m_databases[i]->database()->count(), true);
                QString msg = tr("Appended %1 games to %2.").arg(indexes.count()).arg(fileName);
                slotStatusMessage(msg);
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
    if(target != src)
    {
        Database* pSrcDB = getDatabaseByPath(src);
        Database* pDestDB = getDatabaseByPath(target);
        DatabaseInfo* pDestDBInfo = getDatabaseInfoByPath(target);

        if(pDestDBInfo && pSrcDB && pDestDB && (pSrcDB != pDestDB))
        {
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
        else if(!pSrcDB && !pDestDB && (src != target))
        {
            QFile fSrc(src);
            QFile fDest(target);

            if(fSrc.open(QIODevice::ReadOnly) &&
                    fDest.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
            {
                QFileInfo fiSrc(fSrc);
                QFileInfo fiDest(fDest);
                QString msg = tr("Append games from %1 to %2.").arg(fiSrc.fileName()).arg(fiDest.fileName());
                slotStatusMessage(msg);

                while(!fSrc.atEnd())
                {
                    QByteArray line = fSrc.readLine();
                    fDest.write(line);
                }
            }
            fDest.close();
            m_databaseList->update(target);
        }

        if(databaseInfo() == pDestDBInfo)
        {
            emit databaseModified();
        }
    }
}

void MainWindow::slotDatabaseCopy(int preselect)
{
    QList<int> gameIndexList = m_gameList->selectedGames();
    copyFromDatabase(preselect, gameIndexList);
}

void MainWindow::copyFromDatabase(int preselect, QList<int> gameIndexList)
{
    if(m_databases.count() < 2)
    {
        MessageDialog::error(tr("You need at least two open databases to copy games"));
        return;
    }
    Game g;
    QString players;
    int index = -1;
    if (gameIndexList.size())
    {
        index = gameIndexList.at(0);
    }
    if (index == -1)
    {
        players = game().tag(TagNameWhite)+"-"+game().tag(TagNameBlack);
    }
    else if(databaseInfo()->filter()->contains(index) && database()->loadGame(index, g))
    {
        players = g.tag(TagNameWhite)+"-"+g.tag(TagNameBlack);
    }
    else
    {
        return;
    }
    CopyDialog dlg(this);
    dlg.setCurrentGame(players);
    dlg.setMode((CopyDialog::SrcMode)preselect);
    QStringList db;
    for(int i = 0; i < m_databases.count(); ++i)
        if(i != m_currentDatabase)
            db.append(tr("%1. %2 (%3 games)").arg(i).arg(databaseName(i))
                      .arg(m_databases[i]->database()->count()));
    dlg.setDatabases(db);
    if(dlg.exec() != QDialog::Accepted)
    {
        return;
    }
    int target = dlg.getDatabase();
    if(target >= m_currentDatabase)
    {
        target++;
    }

    switch(dlg.getMode())
    {
    case CopyDialog::SingleGame:
        if (index == -1)
        {
            m_databases[target]->database()->appendGame(game());
        }
        else
        {
            m_databases[target]->database()->appendGame(g);
        }
        break;
    case CopyDialog::Selection:
        foreach (int i, gameIndexList)
        {
            if(database()->loadGame(i, g))
            {
                m_databases[target]->database()->appendGame(g);
            }
        }
        break;
    case CopyDialog::Filter:
        for(int i = 0; i < (int)database()->count(); ++i)
        {
            if(databaseInfo()->filter()->contains(i) && database()->loadGame(i, g))
            {
                m_databases[target]->database()->appendGame(g);
            }
        }
        break;
    case CopyDialog::AllGames:
        for(int i = 0; i < (int)database()->count(); ++i)
        {
            if(database()->loadGame(i, g))
            {
                m_databases[target]->database()->appendGame(g);
            }
        }
        break;
    default:
        ;
    }
    m_databases[target]->filter()->resize(m_databases[target]->database()->count(), true);
}

void MainWindow::slotDatabaseClearClipboard()
{
    if (!m_currentDatabase)
    {
        autoGroup->untrigger();
    }

    ((MemoryDatabase*)(m_databases[0]->database()))->clear();
    m_databases[0]->filter()->resize(0, false);
    m_databases[0]->newGame();

    if (!m_currentDatabase)
    {
        emit databaseChanged(databaseInfo());
        emit databaseModified();
    }
}

void MainWindow::slotDatabaseCopySingle(QList<int> gameIndexList)
{
    copyFromDatabase(0, gameIndexList);
}

void MainWindow::slotDatabaseChanged()
{
    m_undoGroup.setActiveStack(databaseInfo()->undoStack());
    database()->index()->calculateCache();
    setWindowTitle(tr("%1 - ChessX").arg(databaseName()));
    m_gameList->setFilter(databaseInfo()->filter());
    slotFilterChanged();
    slotGameChanged();
    emit databaseChanged(databaseInfo());
    emit databaseModified();
    emit signalGameModified(databaseInfo()->modified());
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

    for(int i = 0; i < m_boardViews.count(); ++i)
    {
        if(i != m_tabWidget->currentIndex())
        {
            const Board& b = m_boardViews.at(i)->board();
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
        databaseInfo()->filter()->executeSearch(ps, Search::Operator(dlg.mode()));
    }
}

void MainWindow::slotBoardSearchUpdate(int progress)
{
    m_gameList->updateFilter();
    slotFilterChanged();
    slotOperationProgress(progress);
    m_gameList->repaint(); // workaround issue with Qt
}

void MainWindow::slotBoardSearchFinished()
{
    finishOperation(tr("Search ended"));
}

void MainWindow::slotBoardSearchStarted()
{
    startOperation(tr("Searching..."));
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

void MainWindow::slotTreeUpdate(bool dbIsFilterSource)
{
    if (!gameMode())
    {
        if (dbIsFilterSource)
        {
            m_gameList->updateFilter();
            slotFilterChanged();
        }
        else
        {
            Search* ps = new PositionSearch (databaseInfo()->filter()->database(), m_openingTreeWidget->board());
            m_openingTreeWidget->cancel();
            slotBoardSearchStarted();
            databaseInfo()->filter()->executeSearch(ps);
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

void MainWindow::slotDatabaseDeleteGame(QList<int> gameIndexList)
{
    foreach(int n, gameIndexList)
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
    m_gameList->updateFilter();
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
    if(database()->index()->replaceTagValue(tag, newValue, oldValue))
    {
        if(game().tag(tag) == oldValue)
        {
            game().setTag(tag, newValue);
        }
        database()->setModified(true);
        if(tag == TagNameWhite)
        {
            database()->index()->replaceTagValue(TagNameBlack, newValue, oldValue);
            if(game().tag(TagNameBlack) == oldValue)
            {
                game().setTag(TagNameBlack, newValue);
            }
        }
        m_eventList->setDatabase(databaseInfo());
        m_playerList->setDatabase(databaseInfo());
        slotGameChanged();
        UpdateBoardInformation();
    }
}

void MainWindow::slotDatabaseDeleteFilter()
{
    database()->remove(*databaseInfo()->filter());
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
        BoardView* boardView = new BoardView(m_tabWidget);

        boardView->setMinimumSize(200, 200);
        boardView->configure();
        boardView->setBoard(Board::standardStartBoard);
        boardView->setDbIndex(m_databases[m_currentDatabase]);

        connect(this, SIGNAL(reconfigure()), boardView, SLOT(configure()));
        connect(boardView, SIGNAL(moveMade(Square, Square, int)), SLOT(slotBoardMove(Square, Square, int)));
        connect(boardView, SIGNAL(clicked(Square, int, QPoint, Square)), SLOT(slotBoardClick(Square, int, QPoint, Square)));
        connect(boardView, SIGNAL(wheelScrolled(int)), SLOT(slotBoardMoveWheel(int)));
        connect(boardView, SIGNAL(moveStarted()), SLOT(slotMoveStarted()));
        connect(boardView, SIGNAL(moveFinished()), SLOT(slotMoveFinished()));

        m_boardViews.push_back(boardView);
        m_tabWidget->addTab(boardView, QString("%1").arg(m_boardViews.count()));
        m_tabWidget->setCurrentIndex(m_boardViews.count() - 1);

        UpdateBoardInformation();

        m_boardView = boardView;
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
}

void MainWindow::closeBoardViewForDbIndex(void* dbIndex)
{
    int index = findBoardView(dbIndex);
    if (index >= 0)
    {
        slotCloseBoardView(index);
    }
}

int MainWindow::findBoardView(void* dbIndex) const
{
    for(int i = 0; i < m_boardViews.count(); ++i)
    {
        if(m_boardViews.at(i)->dbIndex() == dbIndex)
        {
            return i;
        }
    }
    return -1;
}

void MainWindow::activateBoardView(int n)
{
    BoardView* boardView = m_boardViews.at(n);
    m_boardView = boardView;
    m_tabWidget->setCurrentIndex(n);
}

void MainWindow::slotActivateBoardView(int n)
{
    activateBoardView(n);

    BoardView* boardView = m_boardViews.at(n);
    void* db = boardView->dbIndex();
    for (int i=0; i<m_databases.size(); ++i)
    {
        if (m_databases[i]==db)
        {
            m_currentDatabase = i;
            break;
        }
    }
    Q_ASSERT(!databaseInfo()->IsBook());

    emit signalGameModified(databaseInfo()->modified());
    slotGameChanged();
    m_databaseList->setFileCurrent(databaseInfo()->filePath());
    database()->index()->calculateCache();
    setWindowTitle(tr("%1 - ChessX").arg(databaseName()));
    m_gameList->setFilter(databaseInfo()->filter());
    slotFilterChanged();
    m_undoGroup.setActiveStack(databaseInfo()->undoStack());
    emit databaseChanged(databaseInfo());
}

void MainWindow::slotCloseBoardView(int n)
{
    if(n < 0)
    {
        n = m_tabWidget->currentIndex();
    }
    if(m_boardViews.count() > 1)
    {
        m_boardViews.removeAt(n);
        m_tabWidget->removeTab(n);
    }
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

        QString tabName = databaseName();
        m_tabWidget->setTabText(m_tabWidget->currentIndex(), tabName);
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
        m_sliderText->setText(QString::number(interval)+"s");
    }
}

void MainWindow::slotUpdateOpeningTreeWidget()
{
    QStringList files; // List of all open files excluding ClipBoard
    for(int i = 1; i < m_databases.count(); i++)
    {
        QString displayName = m_databases[i]->filePath();
        files << displayName;
    }
    m_openingTreeWidget->updateFilterIndex(files);
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
                int maxPly, minGame;
                bool uniform;
                dlg.getBookParameters(out, maxPly, minGame, uniform);
                PolyglotWriter* polyglotWriter = new PolyglotWriter();
                connect(polyglotWriter, SIGNAL(bookBuildError(QString)), SLOT(slotBookBuildError(QString)));
                connect(polyglotWriter, SIGNAL(bookBuildFinished(QString)), SLOT(slotShowInFinder(QString)));
                polyglotWriter->writeBookForDatabase(m_databases[i]->database(), out, maxPly, minGame, uniform);
            }
            return;
        }
    }
}

void MainWindow::slotShowInFinder(QString path)
{
    ShellHelper::showInFinder(path);
}

void MainWindow::slotBookBuildError(QString /*path*/)
{
    MessageDialog::warning(tr("Could not build book"), tr("Polyglot Error"));
}

void MainWindow::slotToggleGameMode()
{
    enterGameMode(qobject_cast<QAction*>(sender())->isChecked());
}

void MainWindow::slotFlipView(bool flip)
{
    m_boardView->setFlipped(flip);
}

void MainWindow::enterGameMode(bool gameMode)
{
    m_boardView->setDragged(Empty);
    Guess::setGuessAllowed(!gameMode);
    Engine::setAllowEngineOutput(!gameMode);
    Tablebase::setAllowEngineOutput(!gameMode);
    if (gameMode)
    {
        m_openingTreeWidget->cancel();
        m_boardView->setFlags(m_boardView->flags() | BoardView::IgnoreSideToMove);
    }
    else
    {
        m_boardView->setFlags(m_boardView->flags() & ~BoardView::IgnoreSideToMove);
    }
    setGameMode(gameMode);
    m_ficsConsole->SlotGameModeChanged(gameMode);
}

bool MainWindow::premoveAllowed() const
{
    return (gameMode() && m_ficsConsole->canUsePremove() && qobject_cast<const FicsDatabase*>(database()));
}
