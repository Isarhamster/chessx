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
#include <QFileInfo>
#include <QMutexLocker>
#include <QRandomGenerator>
#include <algorithm>

using namespace chessx;

AnalysisWidget::AnalysisWidget(QWidget *parent)
    : QWidget(parent), m_moveTime(0), m_bUciNewGame(true), m_onHold(false), m_gameMode(false), m_hideLines(false)
{
    ui.setupUi(this);
    
    QShortcut* spaceShortcut = new QShortcut(QKeySequence(Qt::Key_Space), this);
    spaceShortcut->setContext(Qt::WindowShortcut);
    connect(spaceShortcut, SIGNAL(activated()), this, SLOT(slotSpacebarPressed()));

    connect(ui.engineList, SIGNAL(activated(int)), SLOT(slotSelectEngine()));
    connect(ui.bookList, SIGNAL(currentIndexChanged(int)), SLOT(bookActivated(int)));
    connect(ui.analyzeButton, SIGNAL(clicked(bool)), SLOT(toggleAnalysis()));
    connect(ui.variationText, SIGNAL(anchorClicked(QUrl)), SLOT(slotLinkClicked(QUrl)));
    connect(ui.vpcount, SIGNAL(valueChanged(int)), SLOT(slotMpvChanged(int)));
    connect(ui.btPin, SIGNAL(clicked(bool)), SLOT(slotPinChanged(bool)));
    
    ui.analyzeButton->setFixedHeight(ui.engineList->sizeHint().height());
    m_tablebase = new OnlineTablebase;
    connect(m_tablebase, SIGNAL(bestMove(QList<Move>,int)), this, SLOT(showTablebaseMove(QList<Move>,int)), Qt::QueuedConnection);
    ui.variationText->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.variationText,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint&)));
}

AnalysisWidget::~AnalysisWidget() { stopEngine(); delete m_tablebase; }

void AnalysisWidget::slotSpacebarPressed()
{
    if (isEngineRunning() && !m_analyses.isEmpty() && !m_analyses[0].variation().isEmpty())
        emit addVariation(m_analyses[0].variation().at(0).toAlgebraic());
}

bool AnalysisWidget::hideLines() const { return m_hideLines; }

void AnalysisWidget::setHideLines(bool newHideLines) { m_hideLines = newHideLines; updateAnalysis(); }

void AnalysisWidget::showContextMenu(const QPoint &pt)
{
    QMenu* menu = ui.variationText->createStandardContextMenu(pt);
    QAction* action = new QAction(tr("Hide lines"));
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
        if(parentWidget() && !parentWidget()->isVisible()) parentWidget()->show();
        ui.variationText->clear();
        m_engine = EngineX::newEngine(index);
        ui.vpcount->setEnabled(m_engine->providesMpv());
        if(!m_engine->providesMpv()) ui.vpcount->setValue(1);
        connect(m_engine, SIGNAL(activated()), SLOT(engineActivated()));
        connect(m_engine, SIGNAL(error(QProcess::ProcessError)), SLOT(engineError(QProcess::ProcessError)));
        connect(m_engine, SIGNAL(deactivated()), SLOT(engineDeactivated()));
        connect(m_engine, SIGNAL(analysisUpdated(Analysis)), SLOT(showAnalysis(Analysis)));
        m_engine->setMoveTime(m_moveTime);
        m_engine->activate();
        AppSettings->setValue(QString("/") + objectName() + "/Engine", ui.engineList->itemText(index));
    }
}

void AnalysisWidget::stopEngine()
{
    engineDeactivated();
    if(m_engine) { m_engine->deactivate(); delete m_engine; m_engine.clear(); }
}

void AnalysisWidget::slotVisibilityChanged(bool visible)
{
    if(isEngineRunning() && !visible && !parentWidget()->isVisible()) stopEngine();
}

bool AnalysisWidget::isEngineRunning() const { return m_engine && ui.analyzeButton->isChecked(); }
bool AnalysisWidget::isEngineConfigured() const { return (ui.engineList->currentIndex() != -1); }

void AnalysisWidget::engineActivated()
{
    ui.analyzeButton->setChecked(true);
    m_analyses.clear();
    updateBookMoves(); 
    if (!sendBookMove())
    {
        m_engine->setStartPos(m_startPos);
        m_engine->startAnalysis(m_board, ui.vpcount->value(), m_moveTime, true, m_line);
        m_lastEngineStart.start();
        m_bUciNewGame = false;
    }
}

void AnalysisWidget::engineError(QProcess::ProcessError e)
{
    MessageDialog::warning(tr("Error (%1) running engine <b>%2</b>.").arg(e).arg(ui.engineList->currentText()));
}

void AnalysisWidget::engineDeactivated() { ui.analyzeButton->setChecked(false); }

void AnalysisWidget::slotSelectEngine()
{
    EngineX* ex = EngineX::newEngine(ui.engineList->currentIndex());
    if (ex) { int empv = ex->m_mapOptionValues.value("MultiPV",1).toInt(); if(empv>1) ui.vpcount->setValue(empv); delete ex; }
    toggleAnalysis();
}

void AnalysisWidget::toggleAnalysis() { if(!isAnalysisEnabled()) stopEngine(); else startEngine(); }

void AnalysisWidget::bookActivated(int)
{
    m_pBookDatabase.clear();
    emit signalSourceChanged(ui.bookList->currentIndex()>=0 ? ui.bookList->itemData(ui.bookList->currentIndex()).toString() : "");
    updateBookMoves();
    updateAnalysis();
}

void AnalysisWidget::slotPinChanged(bool pinned)
{
    if (pinned && isEngineRunning()) m_engine->setMoveTime(0);
    if (m_board != m_NextBoard) setPosition(m_NextBoard, m_NextLine);
}

void AnalysisWidget::slotReconfigure()
{
    QString oldEngineName = ui.engineList->currentText();
    if(oldEngineName.isEmpty()) oldEngineName = AppSettings->getValue(QString("/") + objectName() + "/Engine").toString();
    EngineList enginesList; enginesList.restore();
    QStringList names = enginesList.names();
    ui.engineList->clear(); ui.engineList->insertItems(0, names);
    int index = names.indexOf(oldEngineName);
    ui.engineList->setCurrentIndex(index != -1 ? index : 0);
    if(index == -1) stopEngine();
    int mpv = AppSettings->value(QString("/") + objectName() + "/mpv", 1).toInt();
    ui.vpcount->setValue(mpv);
    int fontSize = std::max(AppSettings->getValue("/General/ListFontSize").toInt(), 8);
    QFont f = ui.variationText->font(); f.setPointSize(fontSize);
    ui.variationText->setFont(f);
}

void AnalysisWidget::saveConfig() { AppSettings->beginGroup(objectName()); AppSettings->setValue("LastBook", ui.bookList->currentText()); AppSettings->endGroup(); }

void AnalysisWidget::restoreBook()
{
    AppSettings->beginGroup(objectName());
    QString lastBook = AppSettings->value("LastBook", "").toString();
    AppSettings->endGroup();
    int index = ui.bookList->findText(lastBook);
    if (index >= 0) ui.bookList->setCurrentIndex(index);
}

void AnalysisWidget::slotUpdateBooks(QStringList files)
{
    QString current = ui.bookList->currentText();
    ui.bookList->clear(); ui.bookList->addItem("-",QVariant(QString()));
    foreach(QString filename, files) { if (DatabaseInfo::IsBook(filename)) ui.bookList->addItem(QFileInfo(filename).baseName(), QVariant(filename)); }
    int index = ui.bookList->findText(current); ui.bookList->setCurrentIndex(index < 0 ? 0 : index);
}

void AnalysisWidget::setGameMode(bool gameMode) { m_gameMode = gameMode; if (!m_gameMode) updateBookMoves(); }

void AnalysisWidget::showAnalysis(Analysis analysis)
{
    int elapsed = m_lastEngineStart.elapsed();
    int mpv = analysis.mpv() - 1;
    if (analysis.bestMove()) { if (m_analyses.count() && m_analyses.last().bestMove()) m_analyses.removeLast(); m_analyses.append(analysis); }
    else if(mpv < 0 || mpv >= ui.vpcount->value()) return;
    else if(mpv == m_analyses.count()) m_analyses.append(analysis);
    else m_analyses[mpv] = analysis;
    updateComplexity(); updateAnalysis();
    if (analysis.bestMove()) { analysis.setElapsedTimeMS(elapsed); emit receivedBestMove(analysis); }
}

void AnalysisWidget::setPosition(const BoardX& board, QString line)
{
    if (ui.btPin->isChecked()) { m_NextBoard = board; m_NextLine = line; return; }
    if(m_board != board)
    {
        m_board = board; m_line = line; m_analyses.clear();
        m_tablebase->abortLookup(); m_tablebaseEvaluation.clear();
        updateBookMoves();
        if(AppSettings->getValue("/General/onlineTablebases").toBool() && !m_board.chess960()) m_tablebase->getBestMove(m_board.toFen());
        updateAnalysis();
        if (m_engine && m_engine->isActive() && !onHold() && !sendBookMove())
        {
            if (m_bUciNewGame) m_engine->setStartPos(m_startPos);
            m_engine->startAnalysis(m_board, ui.vpcount->value(), m_moveTime, m_bUciNewGame, line);
            m_lastEngineStart.start(); m_bUciNewGame = false;
        }
    }
}

bool AnalysisWidget::sendBookMove() { if (moveList.count() && m_moveTime.allowBook) { QTimer::singleShot(500, this, SLOT(sendBookMoveTimeout())); return true; } return false; }

void AnalysisWidget::sendBookMoveTimeout()
{
    if (moveList.count() && m_moveTime.allowBook)
    {
        Analysis analysis; Move::List moves;
        moves.append(moveList.at(0).move);
        analysis.setVariation(moves); analysis.setBestMove(true); analysis.setBookMove(true);
        emit receivedBestMove(analysis);
    }
}

void AnalysisWidget::slotLinkClicked(const QUrl& url)
{
    QString linkData = url.toString();
    if (linkData.startsWith("first:")) {
        int idx = linkData.mid(6).toInt();
        if(idx >= 0 && idx < m_analyses.count() && !m_analyses[idx].variation().isEmpty())
            emit addVariation(m_analyses[idx].variation().at(0).toAlgebraic());
    } else if (linkData.contains(',')) {
        QStringList p = linkData.split(',');
        int lIdx = p[0].toInt(), mIdx = p[1].toInt();
        if (lIdx >= 0 && lIdx < m_analyses.count()) {
            Analysis a = m_analyses[lIdx]; Move::List m;
            for (int i=0; i<=mIdx && i<a.variation().count(); ++i) m.append(a.variation()[i]);
            a.setVariation(m); emit addVariation(a, "");
        }
    }
}

void AnalysisWidget::setMoveTime(EngineParameter mt) { m_moveTime = mt; if(isEngineRunning() && !ui.btPin->isChecked()) m_engine->setMoveTime(mt); }
void AnalysisWidget::setMoveTime(int n) { EngineParameter p(n); p.analysisMode = true; setMoveTime(p); }
void AnalysisWidget::setDepth(int n) { m_moveTime.searchDepth = n; m_moveTime.analysisMode = true; setMoveTime(m_moveTime); }

void AnalysisWidget::slotMpvChanged(int mpv)
{
    if(isEngineRunning()) { while(m_analyses.count() > mpv) m_analyses.removeLast(); m_engine->setMpv(mpv); }
    AppSettings->setValue(QString("/") + objectName() + "/mpv", mpv);
}

bool AnalysisWidget::isAnalysisEnabled() const { return (parentWidget() && parentWidget()->isVisible() && ui.analyzeButton->isChecked()); }

void AnalysisWidget::showTablebaseMove(QList<Move> bestMoves, int score)
{
    if (!bestMoves.isEmpty())
    {
        Move m = m_board.prepareMove(bestMoves[0].from(), bestMoves[0].to());
        if(bestMoves[0].isPromotion()) m.setPromoted(pieceType(bestMoves[0].promotedPiece()));
        m_tablebaseEvaluation = QString("%1 (Score: %2)").arg(m_board.moveToFullSan(m,true)).arg(score);
        updateAnalysis();
    }
}

void AnalysisWidget::clear() { ui.variationText->clear(); }

void AnalysisWidget::updateAnalysis()
{
    QString text;
    int lineIdx = 0;
    foreach(Analysis a, m_analyses) {
        Move::List moves = a.variation();
        if (!moves.isEmpty()) {
            QString score = (a.score() > 0 ? "+" : "") + QString::number(a.score()/100.0, 'f', 2);
            QString s = QString("<a href=\"first:%1\">[+]</a> <b>%2</b> (%3) ").arg(lineIdx).arg(score).arg(a.depth());
            BoardX temp = m_board;
            for(int i=0; i<moves.count(); ++i) {
                s += QString("<a href=\"%1,%2\">%3</a> ").arg(lineIdx).arg(i).arg(temp.moveToSan(moves[i]));
                temp.doMove(moves[i]);
            }
            text.append(s + "<br>");
        }
        lineIdx++;
    }
    if(!m_tablebaseEvaluation.isEmpty()) text.append("<b>TB:</b> " + m_tablebaseEvaluation);
    ui.variationText->setText(text);
}

void AnalysisWidget::updateComplexity() {}
Analysis AnalysisWidget::getMainLine() const { return m_analyses.isEmpty() ? Analysis() : m_analyses.first(); }
bool AnalysisWidget::hasMainLine() const { return !m_analyses.isEmpty(); }
QString AnalysisWidget::displayName() const { return ui.engineList->currentText(); }
void AnalysisWidget::unPin() { if (ui.btPin->isChecked()) ui.btPin->setChecked(false); }
void AnalysisWidget::slotUciNewGame(const BoardX& b) { m_bUciNewGame = true; m_startPos = b; }
bool AnalysisWidget::onHold() const { return m_onHold; }
void AnalysisWidget::setOnHold(bool onHold) { m_onHold = onHold; if (!onHold && (!m_engine || !m_engine->isActive())) startEngine(); }
QString AnalysisWidget::engineName() const { return ui.engineList->currentText(); }
void AnalysisWidget::updateBookFile(Database *pgdb) { m_pBookDatabase = pgdb; }

void AnalysisWidget::updateBookMoves()
{
    QMap<Move, MoveData> moves;
    games = (m_pBookDatabase && !m_gameMode) ? m_pBookDatabase->getMoveMapForBoard(m_board, moves) : 0;
    moveList.clear();
    for(auto it = moves.begin(); it != moves.end(); ++it) moveList.append(it.value());
    std::sort(moveList.begin(), moveList.end());
}
