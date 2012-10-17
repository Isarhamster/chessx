/***************************************************************************
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "wbengine.h"

WBEngine::WBEngine(const QString& name,
		   const QString& command,
           bool bTestMode,
		   const QString& directory,
           QTextStream* logStream) : Engine(name, command, bTestMode, directory, logStream)
{
	m_analyze = false;
	m_setboard = false;		// We do not support version 1 xboard protocol, so this _must_ be set true by feature discovery
	m_invertBlack = true;
}

bool WBEngine::startAnalysis(const Board& board, int nv)
{
        m_mpv = nv;
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

    // By spec we must wait up to 2 seconds to receive all features offers from engine
    // Strange bug - if a timer is running, we don't receive messages

    QTimer::singleShot(2000, this, SLOT(featureTimeout()));

	send("protover 2");    
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
		trim = message.mid(12);

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

    if (done)
    {
        setActive(true);
    }

	// The engine will send done=1, when its ready to go,
	//  and done=0 if it needs more than 2 seconds to start.
}

void WBEngine::featureTimeout()
{
    if (!isActive())
    {
        v1TurnOffPondering();
        setActive(true);
    }
}

void WBEngine::parseAnalysis(const QString& message)
{
	QString trimmed = message.simplified();
	Analysis analysis;
	bool ok;
	bool timeInSeconds = false;

	//Depth
	QString depth = trimmed.section(' ', 0, 0);
	analysis.setDepth(depth.toInt(&ok));
	if (!ok) {
		depth.truncate(depth.length() - 1);
		analysis.setDepth(depth.toInt(&ok));
		if (!ok)
			return;
		timeInSeconds = true;
	}

	//Score
	int score = trimmed.section(' ', 1, 1).toInt(&ok);
	if (!ok)
		return;
	if (m_invertBlack && m_board.toMove() == Black)
		score = -score;
	analysis.setScore(score);

	//Time
	int time = trimmed.section(' ', 2, 2).toInt(&ok);
	if (!ok)
		return;
	if (timeInSeconds)
		time = time * 1000;
	else time *= 10;
	analysis.setTime(time);

	//Node
	analysis.setNodes(trimmed.section(' ', 3 , 3).toLongLong(&ok));
	if (!ok)
		return;

	//Variation
	Board board = m_board;
	QString sanMove;
	MoveList moves;
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
			moves.append(move);
		}
		section++;
	}
	analysis.setVariation(moves);

	if (analysis.isValid())
		sendAnalysis(analysis);
}

void WBEngine::v1TurnOffPondering()
{
	send("hard");
	send("easy");
}


