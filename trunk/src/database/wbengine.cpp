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

WBEngine::WBEngine(const QString& name,
		const QString& command,
		const QString& directory,
		QTextStream* logStream) : Engine(name, command, directory, logStream)
{
	m_analyze = false;
	m_setboard = false;		// We do not support version 1 xboard protocol, so this _must_ be set true by feature discovery
	m_featureTimer = 0;
	m_invertBlack = true;
}

bool WBEngine::startAnalysis(const Board& board)
{
	if (isActive() && m_board == board)
		return true;
	stopAnalysis();
	m_board = board;
	if (m_analyze && isActive() && m_setboard) {
		send("setboard " + board.toFen());
		send("post");
		send("analyze");
		setAnalyzing(true);
		return true;
	}
	return false;
}

void WBEngine::stopAnalysis()
{
	if (isAnalyzing()) {
		send("exit");
		setAnalyzing(false);
	}
}

void WBEngine::protocolStart()
{
	send("xboard");
	send("protover 2");

	// By spec we must wait up to 2 seconds to receive all features offers from engine
	m_featureTimer = startTimer(2000);
}

void WBEngine::protocolEnd()
{
	stopAnalysis();
	send("quit");
	setActive(false);
	m_board.clear();
}

void WBEngine::processMessage(const QString& message)
{
	QString trim(message);

	// GNU Chess always prompts ...
	if (trim.startsWith("White (1) : "))
		trim=message.mid(12);

	trim = trim.trimmed();

	if (!isActive() && trim.startsWith("Crafty v"))
		m_invertBlack = false;

	//determine command
	QString command = trim.section(" ", 0, 0);

	//identify and process the command
	if (command == "feature") {
		feature(trim);
		return;
	}

	if (isAnalyzing()) {
		parseAnalysis(trim);
	}
}

void WBEngine::feature(const QString& command)
{
	//break up command into individual features
	int index = command.indexOf(' ') + 1;
	int equalsIndex;
	int endIndex;
	QString feature;
	QString value;

	while (index < (int)command.length()) {

		//feature name terminates with an =
		equalsIndex = command.indexOf('=', index);
		feature = command.mid(index, equalsIndex - index);

		//string values are delimited by ", others end with whitespace
		if (command[equalsIndex + 1] == '"') {
			endIndex = command.indexOf('"', equalsIndex + 2);
			value = command.mid(equalsIndex + 2, (endIndex - equalsIndex) - 2);
			index = endIndex + 2;
		} else {
			endIndex = command.indexOf(' ', equalsIndex + 1);
			value = command.mid(equalsIndex + 1, (endIndex - equalsIndex) - 1);
			index = endIndex + 1;
		}

		//if endIndex is -1 then the end of the string has been reached
		if (endIndex == -1) {
			index = command.length();
		}

		//process feature/value pair
		if (feature == "setboard") {
			m_setboard = (bool)value.toInt();
			send("accepted " + feature);
			continue;
		}

		if (feature == "analyze") {
			m_analyze = (bool)value.toInt();
			send("accepted " + feature);
			continue;
		}

		if (feature == "done") {
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
	// We've received a "done" feature offer from engine,
	// so it supports V2 or better of the xboard protocol

	// No need to wait any longer wondering if we're talking to a V1 engine
	if (m_featureTimer) {
		killTimer(m_featureTimer);
		m_featureTimer=0;
	}

	// The engine will send done=1, when its ready to go,
	//  and done=0 if it needs more than 2 seconds to start.
	if (done) {
		send("hard");
		send("easy");
		setActive(true);
	}

}

void WBEngine::parseAnalysis(const QString& message)
{
	QString trimmed = message.simplified();
	Analysis analysis;
	analysis.mateIn = false;
	bool ok;
	bool timeInSeconds = false;

	//Depth
	QString depth = trimmed.section(' ', 0, 0);
	analysis.depth = depth.toInt(&ok);
	if (!ok) {
		depth.truncate(depth.length() - 1);
		analysis.depth = depth.toInt(&ok);
		if (!ok) {
			return;
		}
		timeInSeconds = true;
	}

	//Score
	analysis.score = (float)trimmed.section(' ', 1, 1).toInt(&ok) / 100;
	if (!ok) {
		return;
	}
	if (m_invertBlack && m_board.toMove() == Black)
		analysis.score *= -1;

	//Time
	analysis.time = trimmed.section(' ', 2, 2).toInt(&ok);
	if (!ok) {
		return;
	}
	if (!timeInSeconds) {
		analysis.time /= 100;
	}

	//Node
	analysis.nodes = trimmed.section(' ', 3 , 3).toLongLong(&ok);
	if (!ok) {
		return;
	}

	//Variation
	Board board = m_board;
	QString sanMove;
	int section = 4;
	while ((sanMove = trimmed.section(' ', section, section)) != "") {
		if (sanMove.startsWith("(")) {
			break;
		}
		// SBE -- What the heck is "<HT>" and why does crafty send it?
		if (!sanMove.endsWith(".") && sanMove != "<HT>") {
			Move move = board.parseMove(sanMove);
			if (!move.isLegal()) {
//				qWarning("! move: |%s|", sanMove.toLatin1().constData());
//				qWarning("Variation parsing failed");
				break;
			}
			board.doMove(move);
			analysis.variation.append(move);
		}
		section++;
	}

	sendAnalysis(analysis);
//	qWarning("! depth = %d", analysis.depth);
//	qWarning("! score = %g", analysis.score);
//	qWarning("! time = %g", analysis.time);
//	qWarning("! nodes = %ld", (long)analysis.nodes);
}

void WBEngine::v1TurnOffPondering()
{
	send("hard");
	send("easy");
}

void WBEngine::timerEvent(QTimerEvent*)
{
	// Two seconds passed and we didn't get a "done" feature from engine
	//   So, we'll assume we're talking to a version 1 engine
	//     version 1 support is not yet complete (ie. we need "edit" command instead of setboard)

	// Make sure timer doesn't fire again
	killTimer(m_featureTimer);
	m_featureTimer=0;

	v1TurnOffPondering();
	setActive(true);
}

