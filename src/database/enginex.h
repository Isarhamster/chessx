/***************************************************************************
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2008-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ENGINE_H_DEFINED
#define ENGINE_H_DEFINED

#include <QObject>
#include <QString>
#include <QTextStream>
#include <QProcess>

#include "analysis.h"
#include "board.h"
#include "move.h"
#include "enginelist.h"

#include "engineoptiondata.h"
#include "engineparameter.h"

/**
 * @defgroup Feature Feature - assorted feature classes of ChessX
 **/

/**
 * @ingroup Feature
 *
 *	Provides a simple interface to a chess engine.
 **/

class EngineX : public QObject
{
    Q_OBJECT

public:

    /** Constructs an engine with a given path/command, and log stream */
    EngineX(const QString& name,
           const QString& command,
           bool  bTestMode,
           const QString& directory = QString(),
           bool log=false,
           bool sendHistory=false);

    /** Virtual destructor */
    virtual ~EngineX();

    void removeProcess();

    /** Launch and initialize engine, fire activated() signal when done*/
    void activate();

    /** Destroy engine process */
    void deactivate();

    /** Returns whether the engine is active or not */
    bool isActive();

    /** Set a starting position for a game */
    virtual void setStartPos(const BoardX& startPos) = 0;

    /** Analyzes the given position */
    virtual bool startAnalysis(const BoardX& board, int nv, const EngineParameter &mt, bool bNewGame, QString line) = 0;

    /** Stops any analysis */
    virtual void stopAnalysis() = 0;

    /** Returns whether the engine is analyzing or not */
    bool isAnalyzing();

    /** Create a new engine, pass index into engine settings list */
    static EngineX* newEngine(int index);
    static EngineX* newEngine(int index, bool bTestMode);
    static EngineX* newEngine(EngineList& engineList, int index, bool bTestMode);

    /** Set number of lines. */
    virtual void setMpv(int mpv);
    virtual int defaultMpv() const;

    /** Set new move time */
    virtual void setMoveTime(const EngineParameter &mt);

    virtual bool providesMpv()
    {
        return false;
    }
signals:
    /** Fired when the engine is activated */
    void activated();

    /** Fired when the engine is deactivated */
    void deactivated();

    /** Fired when the engine is deactivated */
    void error(QProcess::ProcessError);

    /** Fired when analysis starts */
    void analysisStarted();

    /** Fired when analysis stops */
    void analysisStopped();

    /** Fired when the engine has produced some analysis */
    void analysisUpdated(const Analysis& analysis);

    /** Fired when a log item has been written to the log */
    void logUpdated();


protected:
    /** Performs any shutdown procedure required by the engine protocol */
    virtual void protocolEnd() = 0;

    /** Processes messages from the chess engine */
    virtual void processMessage(const QString& message) = 0;

    /** Log error message into the logging stream */
    virtual void logError(const QString &errMsg);

    /** Sends a message to the chess engine */
    void send(const QString& message);

    /** Sets whether the engine is active or not */
    void setActive(bool active);

    /** Sets whether the engine is analysing or not */
    void setAnalyzing(bool analyzing);

    /** Sends an analysis signal */
    void sendAnalysis(const Analysis& analysis);

    int m_mpv;
    EngineParameter m_moveTime;
    bool m_bTestMode;
    bool m_sendHistory;

private slots:
    /** Receives notification that there is process output to read */
    void pollProcess();

    /** Receives notification that the process has terminated */
    void processExited();

    /** Performs any initialisation required by the engine protocol */
    virtual void protocolStart() = 0;

    /** Processes messages from the chess engine */
    void processError(QProcess::ProcessError);

public:
    QList<EngineOptionData> m_options;
    OptionValueMap m_mapOptionValues;

    bool hasOption(const QString &name) const;
    bool getOption(const QString &name, EngineOptionData &result);

private:
    QString m_name;
    QString	m_command;
    QString	m_directory;
    QTextStream* m_logStream;
    QFile m_logFile;
    QProcess* m_process;
    bool m_active;
    bool m_analyzing;

public:
    static void setAllowEngineOutput(bool allow);
    static bool engineOutputAllowed();
    bool getSendHistory() const;

protected:
    static bool s_allowEngineOutput;
};

#endif // ENGINE_H_DEFINED
