/***************************************************************************
 *   (C) 2008-2010 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __ANALYSIS_WIDGET_H__
#define __ANALYSIS_WIDGET_H__

#include "engine.h"
#include "ui_analysiswidget.h"
#include <QtGui>

/** @ingroup GUI
	The Analysis widget which shows engine output
*/

class Analysis;
class Board;
class Tablebase;

class AnalysisWidget : public QWidget
{
    Q_OBJECT
public:
    AnalysisWidget();
    ~AnalysisWidget();

    /** Get the main line */
    Analysis getMainLine() const;
    /** Analysis has a main line */
    bool hasMainLine() const;
    /** Get the name of this widget */
    QString displayName() const;
    /** Unpin the analyis (if pinned) */
    void unPin();
    /** Is any engine running. */
    bool isEngineRunning() const;
    /** Is any engine configured. */
    bool isEngineConfigured() const;

    bool onHold() const;
    void setOnHold(bool onHold);

public slots:
    /** Sets new position. If analysis is active, the current content will be cleared and
    new analysis will be performed. */
    void setPosition(const Board& board);
    /** Called when configuration was changed (either on startup or from Preferences dialog. */
    void slotReconfigure();
    /** Start currently selected engine. */
    void startEngine();
    /** Stop any running  engine. */
    void stopEngine();
    /** Stop game analysis when analysis dock is hidden. */
    void slotVisibilityChanged(bool);
    /** Change the movetime of the engine */
    void setMoveTime(int mt);
    /** Must send ucinewgame next time */
    void slotUciNewGame();

private slots:
    /** Stop if analysis is no longer visible. */
    void toggleAnalysis();
    /** Displays given analysis received from an engine. */
    void showAnalysis(const Analysis& analysis);
    /** The engine is now ready, as requested */
    void engineActivated();
    /** The engine is now deactivated */
    void engineDeactivated();
    /** There was an error while running engine. */
    void engineError(QProcess::ProcessError);
    /** Add variation. */
    void slotLinkClicked(const QUrl& link);
    /** Number of visible lines was changed. */
    void slotMpvChanged(int mpv);
    /** Show tablebase move information. */
    void showTablebaseMove(Move move, int score);
    /** The pin button was pressed or released */
    void slotPinChanged(bool);

signals:
    void addVariation(const Analysis& analysis);
    void addVariation(const QString& san);
    void requestBoard();
    void receivedBestMove(const Analysis& analysis);

private:
    /** Should analysis be running. */
    bool isAnalysisEnabled() const;
    /** Update analysis. */
    void updateAnalysis();
    /** Update complexity. */
    void updateComplexity();

    QList<Analysis> m_analyses;
    Ui::AnalysisWidget ui;
    Engine* m_engine;
    Board m_board;
    Board m_NextBoard;
    QString m_tablebaseEvaluation;
    Tablebase* m_tablebase;
    int m_moveTime;
    bool m_bUciNewGame;

    double m_complexity;
    double m_complexity2;
    Move m_lastBestMove;
    int m_lastDepthAdded;
    bool m_onHold;

};

#endif // __ANALYSIS_WIDGET_H__

