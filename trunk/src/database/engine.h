/***************************************************************************
                          engine.h  -  Chess engine class
                             -------------------
    begin                : 25 July 2005
    copyright            : (C) 2005, 2006 William Hoggarth
                           <whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <QObject>
#include <QString>
#include <QTextStream>
#include <QProcess>


#include "board.h"
#include "move.h"
#include "movelist.h"

/**
 * @defgroup Feature Feature - assorted feature classes of ChessX
 **/

/**
 * @ingroup Feature
 *
 *	Provides a simple interface to a chess engine.
 **/

class Engine : public QObject
{
	Q_OBJECT

public:
	struct Analysis
	{
		float time;
		quint64 nodes;
		int depth;
		float score;
		MoveList variation;
	};

	/** Constructs an engine with a given path/command, and log stream */
	Engine(const QString& name,
		const QString& command,
		const QString& directory = QString(),
		QTextStream* logStream = NULL);

	/** Virtual destructor */
	virtual ~Engine();

	/** Set the stream that the debug output goes to */
	void setLogStream(QTextStream* logStream = NULL);

	/** Launch and initialize engine, fire activated() signal when done*/
	void activate();

	/** Destroy engine process */
	void deactivate();

	/** Returns whether the engine is active or not */
	bool isActive();

	/** Analyzes the given position */
	virtual bool startAnalysis(const Board& board) = 0;

	/** Stops any analysis */
	virtual void stopAnalysis() = 0;

	/** Returns whether the engine is analyzing or not */
	bool isAnalyzing();

	/** Create a new engine, pass index into engine settings list */
	static Engine* newEngine(int index);

signals:
	/** Fired when the engine is activated */
	void activated();

	/** Fired when the engine is deactivated */
	void deactivated();

	/** Fired when analysis starts */
	void analysisStarted();

	/** Fired when analysis stops */
	void analysisStopped();

	/** Fired when the engine has produced some analysis */
	void analysisUpdated(const Engine::Analysis& analysis);

	/** Fired when a log item has been written to the log */
	void logUpdated();

protected:
	/** Performs any shutdown procedure required by the engine protocol */
	virtual void protocolEnd() = 0;

	/** Processes messages from the chess engine */
	virtual void processMessage(const QString& message) = 0;

	/** Sends a message to the chess engine */
	void send(const QString& message);

	/** Sets whether the engine is active or not */
	void setActive(bool active);

	/** Sets whether the engine is analysing or not */
	void setAnalyzing(bool analyzing);

	/** Sends an analysis signal */
	void sendAnalysis(const Analysis& analysis);

	bool m_invertBlack;

private slots:
	/** Receives notification that there is process output to read */
	void pollProcess();

	/** Receives notification that the process has terminated */
	void processExited();

	/** Performs any initialisation required by the engine protocol */
	virtual void protocolStart() = 0;

private:
	QString m_name;
	QString	m_command;
	QString	m_directory;
	QTextStream* m_logStream;
	QProcess* m_process;
	QTextStream* m_processStream;
	bool m_active;
	bool m_analyzing;
};

#endif // __ENGINE_H__
