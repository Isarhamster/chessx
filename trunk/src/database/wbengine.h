/***************************************************************************
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __WBENGINE_H__
#define __WBENGINE_H__

#include <QString>

class QTextStream;

#include "engine.h"

/** @ingroup Feature
  The WBEngine class provides an interface to a Winboard chess engine.
*/

class WBEngine : public Engine
{
    Q_OBJECT

public:
    /** Constucts an engine with a given path/command, and log stream */
    WBEngine(const QString& name,
             const QString& command,
             bool bTestMode,
             const QString& directory,
             bool log,
             bool sendHistory);

    /** Set a starting position for a game */
   void setStartPos(const Board& startPos);

    /** Analyses the the given position */
    bool startAnalysis(const Board& board, int nv, const EngineParameter &mt, bool bNewGame, QString line);

    /** Stops any analysis */
    void stopAnalysis();

protected:
    /** Initialises the winboard protocol */
    void protocolStart();

    /** Tells the winboard engine to exit */
    void protocolEnd();

    /** Processes messages from the chess engine */
    void processMessage(const QString& message);

protected slots:
    void featureTimeout();

private:
    /** Processes the feature command */
    void feature(const QString& command);

    /** Processes the "done" feature */
    void featureDone(bool done);

    /** Parses analysis */
    void parseAnalysis(const QString& message);
    void parseBestMove(const QString& message);
    void parseEndOfGame(const QString &command, const QString &message);

    /** Instruct version 1 engine to stop pondering */
    void v1TurnOffPondering();
    void go();

    Board m_board;

    bool m_analyze;
    bool m_setboard;
    bool m_bHasSentAnalyze;

};

#endif // __WBENGINE_H__
