/***************************************************************************
 *   (C) 2008-2010 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ANALYSIS_WIDGET_H_INCLUDED
#define ANALYSIS_WIDGET_H_INCLUDED

#include "engine.h"
#include "movedata.h"
#include "ui_analysiswidget.h"
#include <QtGui>
#include <QPointer>

/** @ingroup GUI
	The Analysis widget which shows engine output
*/

class Analysis;
class Board;
class Tablebase;
class Database;

class AnalysisWidget : public QWidget
{
    Q_OBJECT
public:
    AnalysisWidget(QWidget* parent);
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

    QString engineName() const;
    void updateBookFile(Database*);

public slots:
    /** Sets new position. If analysis is active, the current content will be cleared and
    new analysis will be performed. */
    void setPosition(const Board& board, QString line="");
    /** Called when configuration was changed (either on startup or from Preferences dialog. */
    void slotReconfigure();
    /** Store current configuration. */
    void saveConfig();
    /** Start currently selected engine. */
    void startEngine();
    /** Stop any running  engine. */
    void stopEngine();
    /** Stop game analysis when analysis dock is hidden. */
    void slotVisibilityChanged(bool);
    /** Change the movetime of the engine */
    void setMoveTime(EngineParameter mt);
    /** Change the movetime of the engine */
    void setMoveTime(int);
    /** Change the search depth of the engine */
    void setDepth(int n);
    /** Must send ucinewgame next time */
    void slotUciNewGame(const Board& b);
    /** Called when the list of databases changes */
    void slotUpdateBooks(QStringList);
    /** Called upon entering or leaving game mode */
    void setGameMode(bool);
    /** Restore Book settings */
    void restoreBook();
private slots:
    /** Stop if analysis is no longer visible. */
    void toggleAnalysis();
    /** Displays given analysis received from an engine. */
    void showAnalysis(Analysis analysis);
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
    void showTablebaseMove(QList<Move> move, int score);
    /** The pin button was pressed or released */
    void slotPinChanged(bool);

signals:
    void addVariation(const Analysis& analysis);
    void addVariation(const QString& san);
    void requestBoard();
    void receivedBestMove(const Analysis& analysis);
    void signalSourceChanged(QString);

protected slots:
    void bookActivated(int);
    void sendBookMoveTimeout();

private:
    /** Should analysis be running. */
    bool isAnalysisEnabled() const;
    /** Update analysis. */
    void updateAnalysis();
    /** Update complexity. */
    void updateComplexity();
    void updateBookMoves();
    bool sendBookMove();

    QList<Analysis> m_analyses;
    Ui::AnalysisWidget ui;
    QPointer<Engine> m_engine;
    Board m_board;
    QString m_line;
    Board m_NextBoard;
    QString m_NextLine;
    Board m_startPos;
    QString m_tablebaseEvaluation;
    QString m_tablebaseMove;
    Tablebase* m_tablebase;
    Board m_tbBoard;
    EngineParameter m_moveTime;
    bool m_bUciNewGame;

    double m_complexity;
    double m_complexity2;
    Move m_lastBestMove;
    int m_lastDepthAdded;
    bool m_onHold;

    QTime m_lastEngineStart;
    QPointer<Database> m_pBookDatabase;
    QList<MoveData> moveList;
    int games;

    bool m_gameMode;
 };

#endif // ANALYSIS_WIDGET_H_INCLUDED

