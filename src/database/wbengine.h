/***************************************************************************
                          wbengine.h  -  Winboard engine class
                             -------------------
    begin                : 7 August 2005
    copyright            : (C) 2005 William Hoggarth
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

#ifndef __WBENGINE_H__
#define __WBENGINE_H__  
 
#include <QEvent>
#include <QString>
#include <QTextStream>

#include "engine.h"

/**
	 The WBEngine class provides a simple interface to a Winboard chess engine
 */

class WBEngine : public Engine
{
	public:
		/** Constucts an engine with a given path/command, and log stream */
		WBEngine(const QString& name, const QString& command,
							QTextStream* logStream = NULL);
		
		/** Analyses the the given position */
		bool startAnalysis(const Board& board);
		
		/** Stops any analysis */
		bool stopAnalysis();
		
	protected:
		/** Initialises the winboard protocol */
		void protocolStart();
		
		/** Tells the winboard engine to exit */
		void protocolEnd();
		
		/** Processes messages from the chess engine */
		void processMessage(const QString& message);
		
	private:
		/** Processes the feature command */
		void feature(const QString& command);
		
		/** Processes the "done" feature */
		void featureDone(bool done);
		
		/** Parses analysis */
		void parseAnalysis(const QString& message); 
		
		/** Processes the feature time out */
		void timerEvent(QTimerEvent*);
		
		Board m_board;
		
		bool m_analyze;
		bool m_setboard;
		int m_featureTimer;
};

#endif // __WBENGINE_H__
