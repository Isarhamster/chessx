/***************************************************************************
 *   (C) 2008-2010 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "QtWidgets/qmenu.h"
#include "database.h"
#include "settings.h"
#include "analysis.h"
#include "analysiswidget.h"
#include "board.h"
#include "databaseinfo.h"
#include "enginelist.h"
#include "messagedialog.h"
#include "move.h"
#include "movedata.h"
#include "tablebase.h"

#include <assert.h>
#include <QMutexLocker>
#include <QRandomGenerator>

#include <algorithm>

using namespace chessx;

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

AnalysisWidget::AnalysisWidget(QWidget *parent)
    : QWidget(parent),
      m_moveTime(0),
      m_bUciNewGame(true),
      m_onHold(false),
      m_gameMode(false),
      m_hideLines(false)
{
    ui.setupUi(this);
    connect(ui.engineList, SIGNAL(activated(int)), SLOT(slotSelectEngine()));
    connect(ui.bookList, SIGNAL(currentIndexChanged(int)), SLOT(bookActivated(int)));
    connect(ui.analyzeButton, SIGNAL(clicked(bool)), SLOT(toggleAnalysis()));

    connect(ui.variationText, SIGNAL(anchorClicked(QUrl)),
            SLOT(slotLinkClicked(QUrl)));
    connect(ui.vpcount, SIGNAL(valueChanged(int)), SLOT(slotMpvChanged(int)));
    connect(ui.btPin, SIGNAL(clicked(bool)), SLOT(slotPinChanged(bool)));
    ui.analyzeButton->setFixedHeight(ui.engineList->sizeHint().height());

    m_tablebase = new OnlineTablebase;
    connect(m_tablebase, SIGNAL(bestMove(QList<Move>,int)), this, SLOT(showTablebaseMove(QList<Move>,int)), Qt::QueuedConnection);

    ui.variationText->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.variationText,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint&)));
}

AnalysisWidget::~AnalysisWidget()
{
    stopEngine();
    delete m_tablebase;
}


bool AnalysisWidget::hideLines() const
{
    return m_hideLines;
}

void AnalysisWidget::setHideLines(bool newHideLines)
{
    m_hideLines = newHideLines;
    updateAnalysis();
}

void AnalysisWidget::showContextMenu(const QPoint &pt)
{
    QMenu* menu = ui.variationText->createStandardContextMenu(pt);
    QAction* action = new QAction(tr("Hide lines"));
    action->setVisible(true);
    action->setCheckable(true);
    action->setChecked(m_hideLines);
    connect(action, SIGNAL(triggered(bool)),this,SLOT(setHideLines(bool)));
    menu->addAction(action);
    menu->exec(ui.variationText->mapToGlobal(pt));
    delete menu;
}

void AnalysisWidget::startEngine()
{
    updateBookMoves();

    int index = ui.engineList->currentIndex();
    stopEngine();
    m_onHold = false;
    if(index != -1)
    {
        if(parentWidget() && !parentWidget()->isVisible())
        {
            parentWidget()->show();
        }
        ui.variationText->clear();
        assert(m_engine.isNull());
        m_engine = EngineX::newEngine(index);
        assert(!m_engine.isNull());
        ui.vpcount->setEnabled(m_engine->providesMpv());
        ui.label->setEnabled(m_engine->providesMpv());
        if(!m_engine->providesMpv())
        {
            ui.vpcount->setValue(1);
        }

        connect(m_engine, SIGNAL(activated()), SLOT(engineActivated()));
        connect(m_engine, SIGNAL(error(QProcess::ProcessError)), SLOT(engineError(QProcess::ProcessError)));
        connect(m_engine, SIGNAL(deactivated()), SLOT(engineDeactivated()));
        connect(m_engine, SIGNAL(analysisUpdated(Analysis)),
                SLOT(showAnalysis(Analysis)));
        m_engine->setMoveTime(m_moveTime);
        m_engine->activate();
        QString key = QString("/") + objectName() + "/Engine";
        AppSettings->setValue(key, ui.engineList->itemText(index));
    }
}

void AnalysisWidget::stopEngine()
{
    engineDeactivated();
    if(m_engine)
    {
        m_engine->deactivate();
        delete m_engine;
        m_engine.clear();
    }
}

void AnalysisWidget::slotVisibilityChanged(bool visible)
{
    if(isEngineRunning() && !visible && !parentWidget()->isVisible())
    {
        stopEngine();
    }
}

bool AnalysisWidget::isEngineRunning() const
{
    return m_engine && ui.analyzeButton->isChecked();
}

bool AnalysisWidget::isEngineConfigured() const
{
    int index = ui.engineList->currentIndex();
    return (index != -1);
}

void AnalysisWidget::engineActivated()
{
    ui.analyzeButton->setChecked(true);
    m_analyses.clear();
    updateBookMoves(); // Delay this to here so that engine process is up
    if (!sendBookMove())
    {
        assert(!m_engine.isNull());
        m_engine->setStartPos(m_startPos);
        m_engine->startAnalysis(m_board, ui.vpcount->value(), m_moveTime, true, m_line);
        m_lastEngineStart.start();
        m_bUciNewGame = false;
    }
}

void AnalysisWidget::engineError(QProcess::ProcessError e)
{
    MessageDialog::warning(tr("There was an error (%1) running engine <b>%2</b>.")
                           .arg(e)
                           .arg(ui.engineList->currentText()));
}

void AnalysisWidget::engineDeactivated()
{
    ui.analyzeButton->setChecked(false);
}

void AnalysisWidget::slotSelectEngine()
{
    EngineX* ex = EngineX::newEngine(ui.engineList->currentIndex());
    if (ex)
    {
        int empv = ex->m_mapOptionValues.value("MultiPV",1).toInt();
        if(empv>1) ui.vpcount->setValue(empv);
    }
    delete ex;
    toggleAnalysis();
}

void AnalysisWidget::toggleAnalysis()
{
    if(!isAnalysisEnabled())
    {
        stopEngine();
    }
    else
    {
        startEngine();
    }
}

void AnalysisWidget::bookActivated(int index)
{
    m_pBookDatabase.clear();
    emit signalSourceChanged(index>=0 ? ui.bookList->itemData(index).toString() : "");
    updateBookMoves();
    updateAnalysis();
}

void AnalysisWidget::slotPinChanged(bool pinned)
{
    if (pinned && isEngineRunning())
    {
        assert(!m_engine.isNull());
        m_engine->setMoveTime(0);
    }
    if (m_board != m_NextBoard)
    {
        setPosition(m_NextBoard, m_NextLine);
    }
}

void AnalysisWidget::slotReconfigure()
{
    QString oldEngineName = ui.engineList->currentText();
    if(oldEngineName.isEmpty())
    {
        QString key = QString("/") + objectName() + "/Engine";
        oldEngineName = AppSettings->getValue(key).toString();
    }

    EngineList enginesList;
    enginesList.restore();
    QStringList names = enginesList.names();
    ui.engineList->clear();
    ui.engineList->insertItems(0, names);
    int index = names.indexOf(oldEngineName);
    if(index != -1)
    {
        ui.engineList->setCurrentIndex(index);
    }
    else
    {
        ui.engineList->setCurrentIndex(0);
        stopEngine();
    }

    // Choose MPV from GUI
    QString key = QString("/") + objectName() + "/mpv";
    int mpv = AppSettings->value(key, 1).toInt();

    // Look into engine settings if mpv != 1
    EngineX* ex = EngineX::newEngine(index);
    if (ex)
    {
        int empv = ex->m_mapOptionValues.value("MultiPV",1).toInt();
        if (empv > 1) mpv = empv;
    }
    delete ex;
    ui.vpcount->setValue(mpv);

    int fontSize = AppSettings->getValue("/General/ListFontSize").toInt();
    fontSize = std::max(fontSize, 8);
    QFont f = ui.variationText->font();
    f.setPointSize(fontSize);
    setFont(f);
    ui.variationText->setFont(f);
}

void AnalysisWidget::saveConfig()
{
    AppSettings->beginGroup(objectName());
    AppSettings->setValue("LastBook", ui.bookList->currentText());
    AppSettings->endGroup();
}

void AnalysisWidget::restoreBook()
{
    AppSettings->beginGroup(objectName());
    QString lastBook = AppSettings->value("LastBook", "").toString();
    AppSettings->endGroup();
    int index = ui.bookList->findText(lastBook);
    if (index >= 0)
    {
        ui.bookList->setCurrentIndex(index);
    }
}

void AnalysisWidget::slotUpdateBooks(QStringList files)
{
    QString current = ui.bookList->currentText();
    ui.bookList->clear();
    ui.bookList->addItem("-",QVariant(QString()));
    foreach(QString filename, files)
    {
        QFileInfo fi(filename);
        QString baseName = fi.baseName();
        if (DatabaseInfo::IsBook(filename))
        {
            ui.bookList->addItem(baseName,QVariant(filename));
        }
    }
    int index = ui.bookList->findText(current);
    if (index < 0) index = 0;
    ui.bookList->setCurrentIndex(index);
}

void AnalysisWidget::setGameMode(bool gameMode)
{
    m_gameMode = gameMode;
    if (!m_gameMode)
    {
        updateBookMoves();
    }
}

void AnalysisWidget::showAnalysis(Analysis analysis)
{
    Move m;
    if (m_analyses.count() && m_analyses[0].variation().count())
    {
        m = m_analyses[0].variation().at(0);
    }
    int elapsed = m_lastEngineStart.elapsed();
    int mpv = analysis.mpv() - 1;
    bool bestMove = analysis.bestMove();
    if (bestMove)
    {
        if (m_analyses.count() && m_analyses.last().bestMove())
        {
            m_analyses.removeLast();
        }
        m_analyses.append(analysis);
    }
    else if(mpv < 0 || mpv > m_analyses.count() || mpv >= ui.vpcount->value())
    {
        return;
    }
    else if(mpv == m_analyses.count())
    {
        m_analyses.append(analysis);
    }
    else
    {
        m_analyses[mpv] = analysis;
    }
    updateComplexity();
    updateAnalysis();
    Analysis c = analysis;
    if (bestMove && analysis.variation().count()) // Some weird engines send a move twice - find the first (usually longer) line
    {
        foreach (Analysis a, m_analyses)
        {
            if (a.variation().count())
            {
                if (a.variation().at(0)==analysis.variation().at(0))
                {
                    c = a;
                    c.setBestMove(true);
                    break;
                }
            }
        }
    }

    if (bestMove)
    {
        c.setElapsedTimeMS(elapsed);
        emit receivedBestMove(c);
    }

    if (m_tb.isNullMove())
    {
        if (!bestMove && !c.getEndOfGame())
        {
            if (m_analyses.count()) // First line mostly is the best line
            {
                c = m_analyses.at(0);
            }
            else
            {
                return;
            }
        }

        emit currentBestMove(c); // Do not overwrite TB move
    }
}

void AnalysisWidget::setPosition(const BoardX& board, QString line)
{
    if (ui.btPin->isChecked())
    {
        m_NextBoard = board;
        m_NextLine = line;
        return;
    }
    if(m_board != board)
    {
        m_board = board;
        m_NextBoard = board;
        m_NextLine = line;
        m_line = line;
        m_analyses.clear();
        m_tablebase->abortLookup();
        m_tablebaseEvaluation.clear();
        m_tablebaseMove.clear();
        m_tb.setNullMove();
        m_score_tb = 0;

        updateBookMoves();

        if(AppSettings->getValue("/General/onlineTablebases").toBool())
        {
            if (!(m_board.isStalemate() || m_board.isCheckmate() || m_board.chess960()))
            {
                if(objectName() == "Analysis")
                {
                    m_tbBoard = m_board;
                    m_tablebase->getBestMove(m_board.toFen());
                }
            }
        }

        m_lastDepthAdded = 0;
        updateAnalysis();
        if (m_engine && m_engine->isActive() && !onHold() && !sendBookMove())
        {
            if (m_bUciNewGame)
            {
                m_engine->setStartPos(m_startPos);
            }
            m_engine->startAnalysis(m_board, ui.vpcount->value(), m_moveTime, m_bUciNewGame, line);
            m_lastEngineStart.start();
            m_bUciNewGame = false;
        }
    }
}

bool AnalysisWidget::sendBookMove()
{
    if (moveList.count() && m_moveTime.allowBook)
    {
        QTimer::singleShot(500, this, SLOT(sendBookMoveTimeout()));
        return true;
    }
    return false;
}

void AnalysisWidget::sendBookMoveTimeout()
{
    if (moveList.count() && m_moveTime.allowBook)
    {
        Analysis analysis;
        analysis.setElapsedTimeMS(0);
        Move::List moves;
        int index = 0;
        if (m_moveTime.bookMove == 1)
        {
            index = QRandomGenerator::global()->bounded(0,moveList.count()-1);
        }
        else if (m_moveTime.bookMove == 2)
        {
            index = moveList.count() - 1;
            int randomPos = QRandomGenerator::global()->bounded(0,games-1);
            for (int i=0; i<moveList.count();++i)
            {
                randomPos -= moveList.at(i).results.count();
                if (randomPos<0)
                {
                    index = i;
                    break;
                }
            }
        }
        moves.append(moveList.at(index).move);
        analysis.setVariation(moves);
        analysis.setBestMove(true);
        analysis.setBookMove(true);
        analysis.setTb(m_tb);
        analysis.setScoreTb(m_score_tb);
        emit receivedBestMove(analysis);
    }
}

void AnalysisWidget::slotLinkClicked(const QUrl& url)
{
    if (m_NextBoard != m_board)
    {
        return; // Pinned and user moved somewhere else
    }
    int mpv = url.toString().toInt() - 1;
    if(mpv >= 0 && mpv < m_analyses.count())
    {
        emit addVariation(m_analyses[mpv], "");
    }
    else if(mpv == -1)
    {
        emit addVariation(m_tablebaseMove);
    }
    else
    {
        mpv = (-mpv) - 2;
        if(mpv < m_analyses.count())
        {
            if (!m_analyses[mpv].variation().isEmpty())
            {
                emit addVariation(m_analyses[mpv].variation().at(0).toAlgebraic());
            }
        }
    }
}

void AnalysisWidget::setMoveTime(EngineParameter mt)
{
    m_moveTime = mt;
    if(isEngineRunning() && !ui.btPin->isChecked())
    {
        assert(!m_engine.isNull());
        m_engine->setMoveTime(mt);
    }
}

void AnalysisWidget::setMoveTime(int n)
{
    EngineParameter par(n);
    par.analysisMode = true;
    setMoveTime(par);
}

void AnalysisWidget::setDepth(int n)
{
    m_moveTime.searchDepth = n;
    m_moveTime.analysisMode = true;
    setMoveTime(m_moveTime);
}

void AnalysisWidget::slotMpvChanged(int mpv)
{
    if(isEngineRunning())
    {
        while(m_analyses.count() > mpv)
        {
            m_analyses.removeLast();
        }
        assert(!m_engine.isNull());
        m_engine->setMpv(mpv);
    }
    QString key = QString("/") + objectName() + "/mpv";
    AppSettings->setValue(key, mpv);
}

bool AnalysisWidget::isAnalysisEnabled() const
{
    if(!parentWidget())
    {
        return false;
    }
    if(!parentWidget()->isVisible() || !ui.analyzeButton->isChecked())
    {
        return false;
    }
    return true;
}

void AnalysisWidget::showTablebaseMove(QList<Move> bestMoves, int score)
{
    if (m_tbBoard == m_board)
    {
        bool first = true;
        QStringList also;
        if (!bestMoves.isEmpty()) foreach(Move move, bestMoves)
        {
            if (first)
            {
                first = false;
                QString result;

                bool dtm = false;
                if (score > 0x800)
                {
                    score -= 0x800;
                    dtm = true;
                }
                else if (score < -0x800)
                {
                    score += 0x800;
                    dtm = true;
                }

                m_score_tb = 0;
                if(score == 0)
                {
                    result = tr("Draw");
                }
                else
                {
                    if (dtm)
                    {
                        if((score < 0) == (m_board.toMove() == Black))
                        {
                            result = tr("White mates in %n move(s)", "", (qAbs(score)+1)/2);
                            m_score_tb = 1;
                        }
                        else
                        {
                            result = tr("Black mates in %n move(s)", "", (qAbs(score)+1)/2);
                            m_score_tb = -1;
                        }
                    }
                    else
                    {
                        if((score < 0) == (m_board.toMove() == Black))
                        {
                            result = tr("White wins (reset in %n move(s))", "", (qAbs(score)+1)/2);
                            m_score_tb = 1;
                        }
                        else
                        {
                            result = tr("Black wins (reset in %n move(s))", "", (qAbs(score)+1)/2);
                            m_score_tb = -1;
                        }
                    }
                }
                Move move1 = m_board.prepareMove(move.from(), move.to());
                if (!m_hideLines)
                {
                     if(move.isPromotion())
                    {
                        move1.setPromoted(pieceType(move.promotedPiece()));
                    }
                    m_tablebaseEvaluation = QString("%1 - %2").arg(m_board.moveToFullSan(move1,true), result);
                }
                else
                {
                    m_tablebaseEvaluation = result;
                }
                m_tablebaseMove = m_board.moveToFullSan(move1);
                m_tb = move1;
                m_lastDepthAdded = 0;
            }
            else if (!m_hideLines)
            {
                Move move1 = m_board.prepareMove(move.from(), move.to());
                if(move.isPromotion())
                {
                    move1.setPromoted(pieceType(move.promotedPiece()));
                }
                also.append(m_board.moveToFullSan(move1,true));
            }
        }
        else if (score)
        {
            QString result;
            if((score < 0) == (m_board.toMove() == Black))
            {
                result = tr("White wins");
                m_score_tb = 1;
            }
            else if (score>0)
            {
                result = tr("Black wins");
                m_score_tb = -1;
            }
            else
            {
                result = tr("Draw");
            }
            m_tablebaseEvaluation = result;
        }
        if (!also.isEmpty())
        {
            m_tablebaseEvaluation.append(QString(" === %1").arg(also.join(" ")));
        }
        updateAnalysis();
        Analysis tbAnalysis;
        tbAnalysis.setTb(m_tb);
        tbAnalysis.setScoreTb(m_score_tb);
        emit currentBestMove(tbAnalysis);
    }
}

void AnalysisWidget::clear()
{
    ui.variationText->clear();
}

void AnalysisWidget::updateAnalysis()
{
    QString text;
    if (ui.btPin->isChecked())
    {
        unsigned int moveNr = m_board.moveNumber();
        text = tr("Analysis pinned to move %1").arg(moveNr) + "<br>";
    }
    foreach(Analysis a, m_analyses)
    {
        QString s = a.toString(m_board, m_hideLines);
        if (!s.isEmpty()) text.append(s + "<br>");
    }
    if(!m_tablebaseEvaluation.isEmpty())
    {
        text.append(QString("<a href=\"0\" title=\"%1\">[+]</a> <b>%2:</b> ").arg(tr("Click to add move to game"), tr("Tablebase")) + m_tablebaseEvaluation);
    }
    if (m_lastDepthAdded == 17)
    {
        text.append(QString("<br><b>%1:</b> %2/%3<br>").arg(tr("Complexity")).arg(m_complexity).arg(m_complexity2));
    }
    else if (m_lastDepthAdded >= 12)
    {
        text.append(tr("<br><b>Complexity:</b> %1<br>").arg(m_complexity));
    }

    if (moveList.count())
    {
        QString bookLine = tr("<i>Book:</i>");
        foreach (MoveData move, moveList)
        {
            bookLine.append(" ");
            bookLine.append(move.localsan);
        }
        text.append(bookLine);
    }
    ui.variationText->setText(text);
}

void AnalysisWidget::updateComplexity()
{
    if (!m_analyses[0].variation().isEmpty())
    {
        Move bestMove = m_analyses[0].variation().first();
        if (m_analyses[0].depth() == 2)
        {
            m_lastBestMove = bestMove;
            m_complexity = 0.0;
            m_lastDepthAdded = 2;
        }
        if ((m_analyses.size() >= 2) && !m_analyses[1].bestMove())
        {
            if ((m_lastDepthAdded+1 == m_analyses[0].depth()) && !m_analyses[0].isMate())
            {
                if (m_analyses[0].depth() <= 12)
                {
                    m_lastDepthAdded = m_analyses[0].depth();
                    if (m_lastBestMove != bestMove)
                    {
                        if (abs(m_analyses[0].score()) < 200)
                        {
                            m_lastBestMove = bestMove;
                            m_complexity += fabs(double(m_analyses[0].score()-m_analyses[1].score()))/100.0;
                        }
                    }
                    m_complexity2 = m_complexity;
                }
                else if ((m_analyses[0].depth() > 12) && (m_analyses[0].depth() <= 17))
                {
                    m_lastDepthAdded = m_analyses[0].depth();
                    if (m_lastBestMove != bestMove)
                    {
                        if (abs(m_analyses[0].score()) < 200)
                        {
                            m_lastBestMove = bestMove;
                            m_complexity2 += fabs(double(m_analyses[0].score()-m_analyses[1].score()))/100.0;
                        }
                    }
                }
            }
        }
    }
    else
    {
        m_lastDepthAdded = 0;
    }
}

Analysis AnalysisWidget::getMainLine() const
{
    Analysis a;
    if(!m_analyses.isEmpty())
    {
        a = m_analyses.first();
    }
    return a;
}

bool AnalysisWidget::hasMainLine() const
{
    return (!m_analyses.isEmpty());
}

QString AnalysisWidget::displayName() const
{
    return ui.engineList->currentText();
}

void AnalysisWidget::unPin()
{
    if (ui.btPin->isChecked())
    {
        ui.btPin->setChecked(false);
    }
}

void AnalysisWidget::slotUciNewGame(const BoardX& b)
{
    m_bUciNewGame = true;
    m_startPos = b;
}

bool AnalysisWidget::onHold() const
{
    return m_onHold;
}

void AnalysisWidget::setOnHold(bool onHold)
{
    m_onHold = onHold;
    if (!onHold && (!m_engine || !m_engine->isActive()))
    {
        startEngine();
    }
}

QString AnalysisWidget::engineName() const
{
    return ui.engineList->currentText();
}

void AnalysisWidget::updateBookFile(Database *pgdb)
{
    m_pBookDatabase = pgdb;
}

void AnalysisWidget::updateBookMoves()
{
    QMap<Move, MoveData> moves;
    games = 0;

    if (m_pBookDatabase && !m_gameMode)
    {
        games = m_pBookDatabase->getMoveMapForBoard(m_board, moves);
    }

    moveList.clear();
    for(QMap<Move, MoveData>::iterator it = moves.begin(); it != moves.end(); ++it)
    {
        moveList.append(it.value());
    }

    std::sort(moveList.begin(), moveList.end());
}
