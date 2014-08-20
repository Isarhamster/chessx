/***************************************************************************
 *   (C) 2006-2007 Sean Estabrooks                                         *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __UCIENGINE_H__
#define __UCIENGINE_H__

#include <QString>

class QTextStream;

#include "engine.h"

/** @ingroup Feature
* The UCIEngine class provides an interface to Winboard/XBoard engines.
*/
class UCIEngine : public Engine
{
public:
    /** Constucts an engine with a given path/command, and log stream */
    UCIEngine(const QString& name,
              const QString& command,
              bool bTestMode,
              const QString& directory = QString(),
              QTextStream* logStream = NULL);

    /** Analyses the the given position */
    bool startAnalysis(const Board& board, int nv, int mt, bool bNewGame);

    /** Stops any analysis */
    void stopAnalysis();

    /** Update number of displayed lines. Restarts engine. */
    virtual void setMpv(int mpv);
    virtual void setMoveTime(int mt);

    virtual bool providesMvp()
    {
        return true;
    }
    void setPosition();
protected:
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

    Board m_board;

    QString m_position;
    QString m_waitingOn;
    bool m_quitAfterAnalysis;
};

#endif // __UCIENGINE_H__
