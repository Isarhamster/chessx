/***************************************************************************
                          wbengine.cpp  -  Winboard engine class
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

#include "wbengine.h" 

WBEngine::WBEngine(const QString& name, const QString& command,
										QTextStream* logStream) : Engine(name, command, logStream)
{
	m_analyze = false;
	m_setboard = false;
	m_featureTimer = 0;
}

bool WBEngine::startAnalysis(const Board& board)
{
	if(!m_analyze || !isActive()) {
		return false;
	}
	
	m_board = board;
	
	//stop any current analysis
	if(isAnalyzing()) {
		stopAnalysis();
	}
	
	//determine method of setting up the board
	if(m_setboard) {
		send("setboard " + board.toFEN());
		send("post");
		send("analyze");
	}
	setAnalyzing(true);
	
	return true;
}
		
bool WBEngine::stopAnalysis()
{
	if(!isAnalyzing()) {
		return false;
	}
	
	//exit analysis mode
	send("exit");
	setAnalyzing(false);
	
	return true;
}

void WBEngine::protocolStart()
{
	send("xboard");
	send("protover 2");
	
	//set feature timeout
	m_featureTimer = startTimer(2000);
}

void WBEngine::protocolEnd()
{
	if(isAnalyzing()) {
		stopAnalysis();
	}
	send("quit");
	setActive(false);
}

void WBEngine::processMessage(const QString& message)
{
	//determine command
	QString command = message.section(" ", 0, 0);
	
	//identify and process the command
	if(command == "feature") {
		feature(message);
		return;
	}
	
	if(isAnalyzing()) {
		parseAnalysis(message);
	}
}

void WBEngine::feature(const QString& command)
{
	//break up command into individual features
	int index = command.find(' ') + 1;
	int equalsIndex;
	int endIndex;
	QString feature;
	QString value;
	
	while(index < command.length()) {
		
		//feature name terminates with an =
		equalsIndex = command.find('=', index);
		feature = command.mid(index, equalsIndex - index);
		
		//string values are delimited by ", others end with whitespace
		if(command[equalsIndex + 1] == '"') {
			endIndex = command.find('"', equalsIndex + 2);
			value = command.mid(equalsIndex + 2,  (endIndex - equalsIndex) - 2);
			index = endIndex + 2;
		} else {
			endIndex = command.find(' ', equalsIndex + 1);
			value = command.mid(equalsIndex + 1,  (endIndex - equalsIndex) - 1);
			index = endIndex + 1;
		}
		
		//if endIndex is -1 then the end of the string has been reached
		if(endIndex == -1) {   
			index = command.length();                 
		}                             
		
		//process feature/value pair
		if(feature == "setboard") {
			m_setboard = (bool)value.toInt();
			send("accepted " + feature);
			continue;
		}
		
		if(feature == "analyze") {
			m_analyze = (bool)value.toInt();
			send("accepted " + feature);
			continue;
		}
		
		if(feature == "done") {
			featureDone((bool)value.toInt());
			send("accepted " + feature);
			continue;
		}
		
		//unknown feature, reject it
		send("rejected " + feature);
	}
}

void WBEngine::featureDone(bool done)
{
	if(done) {
		send("hard");
		send("easy");
		setActive(true);
		killTimer(m_featureTimer);
	} else {
		killTimer(m_featureTimer);
		m_featureTimer = startTimer(60 * 60 * 1000);
	}
}

void WBEngine::parseAnalysis(const QString& message)
{
	QString trimmed = message.simplifyWhiteSpace();
	Analysis analysis;
	bool ok;
	bool timeInSeconds = false;
	
	//Depth
	QString depth = trimmed.section(' ', 0, 0);
	analysis.depth = depth.toInt(&ok);
	if(!ok) {
		depth.truncate(depth.length() - 1);
		analysis.depth = depth.toInt(&ok);
		if(!ok) {
			return;
		}
		timeInSeconds = true;
	}
	
	//Score
	analysis.score = (float)trimmed.section(' ', 1, 1).toInt(&ok) / 100;
	if(!ok) {
		return;
	}
	
	//Time
	analysis.time = trimmed.section(' ', 2, 2).toInt(&ok);
	if(!ok) {
		return;
	}
	if(!timeInSeconds) {
		analysis.time /= 100;
	}
	
	//Node
	analysis.nodes = trimmed.section(' ',3 ,3).toLongLong(&ok);
	if(!ok) {
		return;
	}
	
	//Variation
	Board board = m_board;
	QString sanMove;
	int section = 4;
	while((sanMove = trimmed.section(' ', section, section)) != "") {
		if(sanMove.startsWith('(')) {
			break;
		}
		if(!sanMove.endsWith('.')) {
			qWarning("! move: |%s|", sanMove.latin1());
			Move move = board.singleMove(sanMove);
			if(!move.isValid()) {
				qWarning("Variation parsing failed\n");
				break;
			} 
			board.doMove(move);
			analysis.variation.append(move);
		}
		section++;
	}
	
	sendAnalysis(analysis);
	qWarning("! depth = %d\n", analysis.depth);
	qWarning("! score = %g\n", analysis.score);
	qWarning("! time = %g\n", analysis.time);
	qWarning("! nodes = %ld\n", (long)analysis.nodes);
}

void WBEngine::timerEvent(QTimerEvent*)
{
	//assuming pre version 2 engine
	send("hard");
	send("easy");
	setActive(true);
}
