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
 
#include <qobject.h>
#include <qstring.h>
#include <qtextstream.h>
#include <qprocess.h>


#include "board.h"
#include "move.h"
#include "movelist.h"

/**
	 The Engine class provides a simple interface to a chess engine.
 */

class Engine : public QObject
{
	Q_OBJECT
	
	public:
		struct Analysis {
			float time;
			Q_UINT64 nodes;
			int depth;
			float score;
			MoveList variation;
		};
	
		/** Constucts an engine with a given path/command, and log stream */
		Engine(const QString& name, const QString& command, 
						QTextStream* logStream = NULL);
						
		/** Virtual desctructor */
		virtual ~Engine();
		
		/** Set the stream that the debug output goes to */
		void setLogStream(QTextStream* logStream = NULL);
		
		/** Launches and intialises the engine */
		void activate();
		
		/** Shuts down and terminates the engine */
		void deactivate();
		
		/** Returns whether the engine is active or not */
		bool isActive();
		
		/** Analyses the the given position */
		virtual bool startAnalysis(const Board& board) = 0;
		
		/** Stops any analysis */
		virtual bool stopAnalysis() = 0;
		
		/** Returns whether the engine is analyzing or not */
		bool isAnalyzing();
		
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
		void analysisUpdated(const Analysis& analysis);
		
		/** Fired when a log item has been written to the log */
		void logUpdated();
		
	protected:
		/** Performs any initialisation required by the engine protocol */
		virtual void protocolStart() = 0;
		
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
		
	private slots:
		/** Receives notification that there is process output to read */
		void pollProcess();
		
		/** Receives notification that the process has terminated */
		void processExited();
		
	private:
		QString m_name;
		QString	m_command;
		QTextStream* m_logStream;
		QProcess* m_process;
		QTextStream* m_processStream;
		bool m_active;
		bool m_analyzing;
};

#endif // __ENGINE_H__ 
