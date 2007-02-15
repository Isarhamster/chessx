/***************************************************************************
                          uciengine.h  -  UCI engine class
                             -------------------
    begin                : 12 August 2005
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
 
#ifndef __UCIENGINE_H__
#define __UCIENGINE_H__ 
 
#include <QString>
#include <QTextStream>

#include "engine.h"

/** @ingroup Database  
The UCIEngine class provides an interface to Winboard/XBoard engines.
*/ 
class UCIEngine : public Engine
{
	public:
		/** Constucts an engine with a given path/command, and log stream */
		UCIEngine(const QString& name, const QString& command,
								QTextStream* logStream = NULL);
			
		/** Analyses the the given position */
		bool startAnalysis(const Board& board);
		
		/** Stops any analysis */
		bool stopAnalysis();
				
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
		
		Board m_board;
		
		QString m_position;
		QString m_waitingOn;
		bool m_quitAfterAnalysis;
};

#endif // __UCIENGINE_H__
