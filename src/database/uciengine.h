/***************************************************************************
 *   (C) 2006-2007 Sean Estabrooks                                         *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef UCIENGINE_H_INCLUDED
#define UCIENGINE_H_INCLUDED

#include <QString>

class QTextStream;

#include "enginex.h"

/** @ingroup Feature
* The UCIEngine class provides an interface to Winboard/XBoard engines.
*/
class UCIEngine : public EngineX
{
public:
    /** Constucts an engine with a given path/command, and log stream */
    UCIEngine(const QString& name,
              const QString& command,
              bool bTestMode,
              const QString& directory = QString(),
              bool log=false,
              bool sendHistory=false);

    /** Set a starting position for a game */
    void setStartPos(const BoardX& startPos);

    /** Analyses the the given position */
    bool startAnalysis(const BoardX& board, int nv, const EngineParameter &mt, bool bNewGame, QString line);

    /** Stops any analysis */
    void stopAnalysis();

    /** Update number of displayed lines. Restarts engine. */
    virtual void setMpv(int mpv);
    virtual void setMoveTime(const EngineParameter &mt);

    virtual bool providesMpv()
    {
        return true;
    }
protected:
    void setPosition();
    /** Performs any initialisation required by the engine protocol */
    void protocolStart();

    /** Performs any shutdown procedure required by the engine protocol */
    void protocolEnd();

    /** Processes messages from the chess engine */
    void processMessage(const QString& message);

private:
    /** Parses analysis */
    void parseAnalysis(const QString& message);
    void parseBestMove(const QString& message);

    /** Parse option string */
    void parseOptions(const QString &message);
    /** Send go command according to currently set timespec */
    void go();

    BoardX m_board;
    BoardX m_startPos;
    QString m_line;
    QString m_name;

    QString m_position;
    bool m_chess960;
    QString m_waitingOn;
    bool m_quitAfterAnalysis;
};

#endif // UCIENGINE_H_INCLUDED
