/***************************************************************************
 *   (C) 2006-2007 Sean Estabrooks                                         *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "uciengine.h"

UCIEngine::UCIEngine(const QString& name,
			  const QString& command,
			  const QString& directory,
			  QTextStream* logStream) : Engine(name, command, directory, logStream)
{
	m_quitAfterAnalysis = false;
	m_invertBlack = true;
}

bool UCIEngine::startAnalysis(const Board& board, int nv)
{
	m_mpv = nv;
	if (!isActive()) {
		return false;
	}

	if (m_board == board)
		return true;
	m_board = board;

	m_position = board.toFen();
	m_waitingOn = "ucinewgame";
	send("stop");
	send("ucinewgame");
	send("isready");
	setAnalyzing(true);

	return true;
}

void UCIEngine::stopAnalysis()
{
	if (isAnalyzing())
		send("stop");
}

void UCIEngine::setMpv(int mpv)
{
	m_mpv = mpv;
	if (isAnalyzing()) {
		send("stop");
		send(QString("setoption name MultiPV value %1").arg(m_mpv));
		send("go infinite");
	}
}


void UCIEngine::protocolStart()
{
	//tell the engine we are using the uci protocol
	send("uci");
}

void UCIEngine::protocolEnd()
{
	send("quit");
	setActive(false);
	m_board.clear();
}

void UCIEngine::processMessage(const QString& message)
{
	if (message == "uciok") {
		//once the engine is running wait for it to initialise
		m_waitingOn = "uciok";
		send("isready");
	}

	if (message == "readyok") {
		if (m_waitingOn == "uciok") {
			//engine is now initialised and ready to go
			m_waitingOn = "";
			setActive(true);
			send("setoption name UCI_AnalyseMode value true");
		}

		if (m_waitingOn == "ucinewgame") {
			//engine is now ready to analyse a new position
			m_waitingOn = "";
			send(QString("setoption name MultiPV value %1").arg(m_mpv));
			send("position fen " + m_position);
			send("go infinite");
		}
	}

//	if (message.section(' ', 0, 0) == "bestmove") {
//		//analysis finished
//		setAnalyzing(false);
//
//		if (m_quitAfterAnalysis) {
//			send("quit");
//			setActive(false);
//		}
//	}

	if (message.section(' ', 0, 0) == "info" && isAnalyzing()) {
		parseAnalysis(message);
	}
}

void UCIEngine::parseAnalysis(const QString& message)
{
	// Sample: info score cp 20  depth 3 nodes 423 time 15 pv f1c4 g8f6 b1c3
	Analysis analysis;
		  bool multiPVFound, timeFound, nodesFound, depthFound, scoreFound, variationFound;
		  multiPVFound = timeFound = nodesFound = depthFound = scoreFound = variationFound = false;

	QString info = message.section(' ', 1, -1, QString::SectionSkipEmpty);
	int section = 0;
	QString name;
	bool ok;

	//loop around the name value tuples
	while (!info.section(' ', section, section + 1, QString::SectionSkipEmpty).isEmpty()) {
		name = info.section(' ', section, section, QString::SectionSkipEmpty);

		if (name == "multipv") {
			analysis.setNumpv(info.section(' ', section + 1, section + 1, QString::SectionSkipEmpty).toInt(&ok));
			section += 2;
			if (ok) {
				multiPVFound = true;
				continue;
			}
		}

		if (name == "time") {
			analysis.setTime(info.section(' ', section + 1, section + 1, QString::SectionSkipEmpty).toInt(&ok));
			section += 2;
			if (ok) {
				timeFound = true;
				continue;
			}
		}

		if (name == "nodes") {
			analysis.setNodes(info.section(' ', section + 1, section + 1, QString::SectionSkipEmpty).toLongLong(&ok));
			section += 2;
			if (ok) {
				nodesFound = true;
				continue;
			}
		}

		if (name == "depth") {
			analysis.setDepth(info.section(' ', section + 1, section + 1, QString::SectionSkipEmpty).toInt(&ok));
			section += 2;
			if (ok) {
				depthFound = true;
				continue;
			}
		}

		if (name == "score") {
			QString type = info.section(' ', section + 1, section + 1, QString::SectionSkipEmpty);
			if (type == "cp" || type == "mate") {
				int score = info.section(' ', section + 2, section + 2).toInt(&ok);
				if (type == "mate")
					analysis.setMovesToMate(abs(score));
				else if (m_invertBlack && m_board.toMove() == Black)
					analysis.setScore(-score);
				else analysis.setScore(score);
				section += 3;
				if (ok) {
					scoreFound = true;
					continue;
				}
			}
			else section += 3;
		}

		if (name == "pv") {
			Board board = m_board;
			MoveList moves;
			QString moveText;
			section++;
			while ((moveText = info.section(' ', section, section, QString::SectionSkipEmpty)) != "") {
				Move move = board.parseMove(moveText);
				if (!move.isLegal())
					break;
				board.doMove(move);
				moves.append(move);
				section++;
			}
			analysis.setVariation(moves);
		}

		//not understood, skip
		section += 2;
	}

	if (timeFound && nodesFound && scoreFound && analysis.isValid()) {
		if (!multiPVFound)
			analysis.setNumpv(1);
		sendAnalysis(analysis);
	}
}

