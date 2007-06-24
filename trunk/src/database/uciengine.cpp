/***************************************************************************
                          uciengine.cpp  -  UCI engine class
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

#include "uciengine.h"

UCIEngine::UCIEngine(const QString& name,
		const QString& command,
		const QString& directory,
		QTextStream* logStream) : Engine(name, command, directory, logStream)
{
	m_position = "";
	m_waitingOn = "";
	m_quitAfterAnalysis = false;
	m_invertBlack = true;
}

bool UCIEngine::startAnalysis(const Board& board)
{
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
	if (isAnalyzing()) {
		send("stop");
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
	/*
			float time;
			Q_UINT64 nodes;
			int depth;
			float score;
			QString variation;

			info score cp 20  depth 3 nodes 423 time 15 pv f1c4 g8f6 b1c3
	*/
	Analysis analysis;
	analysis.mateIn = false;
	bool timeFound, nodesFound, depthFound, scoreFound, variationFound;
	timeFound = nodesFound = depthFound = scoreFound = variationFound = false;

	QString info = message.section(' ', 1, -1);
	int section = 0;
	QString name, value;
	bool ok;

	//loop around the name value tuples
	while (info.section(' ', section, section + 1) != "") {
		name = info.section(' ', section, section);

		//Time
		if (name == "time") {
			analysis.time = info.section(' ', section + 1, section + 1).toInt(&ok);
			analysis.time /= 1000;
			section += 2;
			if (ok) {
				timeFound = true;
				continue;
			}
		}

		//Nodes
		if (name == "nodes") {
			analysis.nodes = info.section(' ', section + 1,
						      section + 1).toLongLong(&ok);
			section += 2;
			if (ok) {
				nodesFound = true;
				continue;
			}
		}

		//Depth
		if (name == "depth") {
			analysis.depth = info.section(' ', section + 1, section + 1).toInt(&ok);
			section += 2;
			if (ok) {
				depthFound = true;
				continue;
			}
		}

		//Score
		if (name == "score") {
			QString type = info.section(' ', section + 1, section + 1);
			if (type == "cp" || type == "mate") {
				analysis.score = info.section(' ', section + 2, section + 2).toInt(&ok);
				if (type == "mate") {
					analysis.mateIn = true;
				} else	analysis.score /= 100;
				if (m_invertBlack && m_board.toMove() == Black)
					analysis.score *= -1;
				section += 3;
				if (ok) {
					scoreFound = true;
					continue;
				}
			} else {
				section += 3;
			}
		}

		//PV
		if (name == "pv") {

			Board board = m_board;

			QString lanMove;
			section++;
			while ((lanMove = info.section(' ', section, section)) != "") {
//				qWarning("! move: |%s|", lanMove.toLatin1().constData());
				Move move = board.parseMove(lanMove);
				if (!move.isLegal()) {
//					qWarning("Variation parsing failed\n");
					break;
				}
				board.doMove(move);
				analysis.variation.append(move);
				section++;
			}
			// break;
			//analysis.variation.append();
		}

		//not understood, skip
		section += 2;
	}

	if (timeFound && nodesFound && depthFound && scoreFound) {
		sendAnalysis(analysis);
//		qWarning("! depth = %d\n", analysis.depth);
//		qWarning("! score = %g\n", analysis.score);
//		qWarning("! time = %g\n", analysis.time);
//		qWarning("! nodes = %ld\n", (long)analysis.nodes);
	}
}
