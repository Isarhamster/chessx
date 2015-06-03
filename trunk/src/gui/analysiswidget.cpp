/***************************************************************************
 *   (C) 2008-2010 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "settings.h"
#include "analysis.h"
#include "board.h"
#include "analysiswidget.h"
#include "enginelist.h"
#include "messagedialog.h"
#include "tablebase.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

AnalysisWidget::AnalysisWidget()
    : m_engine(0),
      m_moveTime(0),
      m_bUciNewGame(true),
      m_onHold(false)
{
    ui.setupUi(this);
    connect(ui.engineList, SIGNAL(activated(int)), SLOT(toggleAnalysis()));
    connect(ui.analyzeButton, SIGNAL(clicked(bool)), SLOT(toggleAnalysis()));
    connect(ui.variationText, SIGNAL(anchorClicked(QUrl)),
            SLOT(slotLinkClicked(QUrl)));
    connect(ui.vpcount, SIGNAL(valueChanged(int)), SLOT(slotMpvChanged(int)));
    connect(ui.btPin, SIGNAL(clicked(bool)), SLOT(slotPinChanged(bool)));
    ui.analyzeButton->setFixedHeight(ui.engineList->sizeHint().height());

    m_tablebase = new OnlineTablebase;
    connect(m_tablebase, SIGNAL(bestMove(Move, int)), this, SLOT(showTablebaseMove(Move, int)));
}

AnalysisWidget::~AnalysisWidget()
{
    stopEngine();
    delete m_tablebase;
}

void AnalysisWidget::startEngine()
{
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
        m_engine = Engine::newEngine(index);
        ui.vpcount->setEnabled(m_engine->providesMvp());
        ui.label->setEnabled(m_engine->providesMvp());
        if(!m_engine->providesMvp())
        {
            ui.vpcount->setValue(1);
        }

        connect(m_engine, SIGNAL(activated()), SLOT(engineActivated()));
        connect(m_engine, SIGNAL(error(QProcess::ProcessError)), SLOT(engineError(QProcess::ProcessError)));
        connect(m_engine, SIGNAL(deactivated()), SLOT(engineDeactivated()));
        connect(m_engine, SIGNAL(analysisUpdated(const Analysis&)),
                SLOT(showAnalysis(const Analysis&)));
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
        m_engine->deleteLater();
        m_engine = 0;
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
    ui.analyzeButton->setText(tr("Stop"));
    m_analyses.clear();
    m_engine->startAnalysis(m_board, ui.vpcount->value(), m_moveTime, true);
}

void AnalysisWidget::engineError(QProcess::ProcessError e)
{
    MessageDialog::warning(tr("There was an error (%1) running engine <b>%2</b>.")
                           .arg(e)
                           .arg(ui.engineList->currentText()));
    stopEngine();
}

void AnalysisWidget::engineDeactivated()
{
    ui.analyzeButton->setChecked(false);
    ui.analyzeButton->setText(tr("Analyze"));
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

void AnalysisWidget::slotPinChanged(bool pinned)
{
    if (!pinned && isAnalysisEnabled())
    {
        if (m_board != m_NextBoard)
        {
            setPosition(m_NextBoard);
        }
    }
    else
    {
        if(isEngineRunning())
        {
            m_engine->setMoveTime(0);
        }
    }
}

void AnalysisWidget::slotReconfigure()
{
    QString oldEngineName = ui.engineList->currentText();
    if(oldEngineName.isEmpty())
    {
        QString key = QString("/") + objectName() + "/Engine";
        oldEngineName = AppSettings->value(key).toString();
    }

    EngineList enginesList;
    enginesList.restore();
    QStringList names = enginesList.names();
    ui.engineList->clear();
    ui.engineList->insertItems(0,	names);
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

    int fontSize = AppSettings->getValue("/General/ListFontSize").toInt();
    QFont f = ui.variationText->font();
    f.setPointSize(fontSize);
    setFont(f);
    ui.variationText->setFont(f);
}

void AnalysisWidget::showAnalysis(const Analysis& analysis)
{
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
    if (bestMove)
    {
        emit receivedBestMove(analysis);
    }
}

void AnalysisWidget::setPosition(const Board& board)
{
    if (ui.btPin->isChecked())
    {
        m_NextBoard = board;
        return;
    }
    if(m_board != board)
    {
        m_board = board;
        m_NextBoard = board;
        m_analyses.clear();
        m_tablebase->abortLookup();
        m_tablebaseEvaluation.clear();

        if(AppSettings->getValue("/General/onlineTablebases").toBool())
        {
            if (!(m_board.isStalemate() || m_board.isCheckmate()))
            {
                if(objectName() == "Analysis")
                {
                    m_tablebase->getBestMove(m_board.toFen());
                }
            }
        }
        m_lastDepthAdded = 0;
        updateAnalysis();
        if(m_engine && m_engine->isActive() && !onHold())
        {
            m_engine->startAnalysis(m_board, ui.vpcount->value(), m_moveTime, m_bUciNewGame);
            m_bUciNewGame = false;
        }
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
        emit addVariation(m_analyses[mpv]);
    }
    else if(mpv == -1)
    {
        emit addVariation(m_tablebaseEvaluation);
    }
    else
    {
        mpv = (-mpv) - 2;
        if(mpv < m_analyses.count())
        {
            if (m_analyses[mpv].variation().size())
            {
                emit addVariation(m_analyses[mpv].variation().at(0).toAlgebraic());
            }
        }
    }
}

void AnalysisWidget::setMoveTime(int mt)
{
    m_moveTime = mt;
    if(isEngineRunning() && !ui.btPin->isChecked())
    {
        m_engine->setMoveTime(mt);
    }
}

void AnalysisWidget::slotMpvChanged(int mpv)
{
    if(isEngineRunning())
    {
        while(m_analyses.count() > mpv)
        {
            m_analyses.removeLast();
        }
        m_engine->setMpv(mpv);
    }
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

void AnalysisWidget::showTablebaseMove(Move move, int score)
{
    QString result;
    if(score == 0)
    {
        result = tr("Draw");
    }
    else if((score < 0) == (m_board.toMove() == Black))
    {
        result = tr("White wins in %n moves", "", qAbs(score));
    }
    else
    {
        result = tr("Black wins in %n moves", "", qAbs(score));
    }

    Move move1 = m_board.prepareMove(move.from(), move.to());
    if(move.isPromotion())
    {
        move1.setPromoted(pieceType(move.promotedPiece()));
    }
    m_tablebaseEvaluation = QString("%1 - %2").arg(m_board.moveToFullSan(move1)).arg(result);
    m_lastDepthAdded = 0;
    updateAnalysis();
}

void AnalysisWidget::updateAnalysis()
{
    QString text;
    if (ui.btPin->isChecked())
    {
        int moveNr = m_board.moveNumber();
        text = tr("Analysis pinned to move %1").arg(moveNr) + "<br>";
    }
    foreach(Analysis a, m_analyses)
    {
        QString s = a.toString(m_board);
        if (!s.isEmpty()) text.append(s + "<br>");
    }
    if(!m_tablebaseEvaluation.isEmpty())
    {
        text.append(QString("<a href=\"0\" title=\"%1\">[+]</a> <b>%2:</b> ").arg(tr("Click to add move to game")).arg(tr("Tablebase")) + m_tablebaseEvaluation);
    }
    if (m_lastDepthAdded == 17)
    {
        text.append(QString("<br><b>%1:</b> %2/%3<br>").arg(tr("Complexity")).arg(m_complexity).arg(m_complexity2));
    }
    else if (m_lastDepthAdded >= 12)
    {
        text.append(tr("<br><b>Complexity:</b> %1<br>").arg(m_complexity));
    }
    ui.variationText->setText(text);
}

void AnalysisWidget::updateComplexity()
{
    if (m_analyses[0].variation().size())
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

void AnalysisWidget::slotUciNewGame()
{
    m_bUciNewGame = true;
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
