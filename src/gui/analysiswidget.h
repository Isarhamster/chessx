/***************************************************************************
 * (C) 2008-2010 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 * *
 * This program is free software; you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 2 of the License, or     *
 * (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ANALYSIS_WIDGET_H_INCLUDED
#define ANALYSIS_WIDGET_H_INCLUDED

#include "enginex.h"
#include "movedata.h"
#include "ui_analysiswidget.h"
#include <QtGui>
#include <QElapsedTimer>
#include <QObject>
#include <QPointer>
#include <QShortcut>

/** @ingroup GUI
    The Analysis widget which shows engine output
*/

class Tablebase;
class Database;

class AnalysisWidget : public QWidget
{
    Q_OBJECT
public:
    AnalysisWidget(QWidget* parent);
    ~AnalysisWidget();

    Analysis getMainLine() const;
    bool hasMainLine() const;
    QString displayName() const;
    void unPin();
    bool isEngineRunning() const;
    bool isEngineConfigured() const;
    bool onHold() const;
    void setOnHold(bool onHold);
    QString engineName() const;
    void updateBookFile(Database*);
    void clear();

public slots:
    void setPosition(const BoardX& board, QString line="");
    void slotReconfigure();
    void saveConfig();
    void startEngine();
    void stopEngine();
    void slotVisibilityChanged(bool);
    void setMoveTime(EngineParameter mt);
    void setMoveTime(int);
    void setDepth(int n);
    void slotUciNewGame(const BoardX& b);
    void slotUpdateBooks(QStringList);
    void setGameMode(bool);
    void restoreBook();

private slots:
    void toggleAnalysis();
    void slotSelectEngine();
    void showAnalysis(Analysis analysis);
    void engineActivated();
    void engineDeactivated();
    void engineError(QProcess::ProcessError);
    void slotLinkClicked(const QUrl& link);
    void slotMpvChanged(int mpv);
    void showTablebaseMove(QList<Move> move, int score);
    void slotPinChanged(bool);
    bool hideLines() const;
    void setHideLines(bool newHideLines);
    
    /** Handle the Spacebar shortcut */
    void slotSpacebarPressed();

signals:
    void addVariation(const Analysis& analysis, const QString&);
    void addVariation(const QString& san);
    void requestBoard();
    void receivedBestMove(const Analysis& analysis);
    void currentBestMove(const Analysis& analysis);
    void signalSourceChanged(QString);

protected slots:
    void bookActivated(int);
    void sendBookMoveTimeout();
    void showContextMenu(const QPoint &pt);

private:
    bool isAnalysisEnabled() const;
    void updateAnalysis();
    void updateComplexity();
    void updateBookMoves();
    bool sendBookMove();

    QList<Analysis> m_analyses;
    Ui::AnalysisWidget ui;
    QPointer<EngineX> m_engine;
    BoardX m_board;
    QString m_line;
    BoardX m_NextBoard;
    QString m_NextLine;
    BoardX m_startPos;
    QString m_tablebaseEvaluation;
    QString m_tablebaseMove;
    Move m_tb;
    int m_score_tb;
    Tablebase* m_tablebase;
    BoardX m_tbBoard;
    EngineParameter m_moveTime;
    bool m_bUciNewGame;

    double m_complexity;
    double m_complexity2;
    Move m_lastBestMove;
    int m_lastDepthAdded;
    bool m_onHold;

    QElapsedTimer m_lastEngineStart;
    QPointer<Database> m_pBookDatabase;
    QList<MoveData> moveList;
    int games;

    bool m_gameMode;
    bool m_hideLines;
};

#endif // ANALYSIS_WIDGET_H_INCLUDED
