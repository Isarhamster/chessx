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
#include "enginedata.h"

UCIEngine::UCIEngine(const QString& name,
			  const QString& command,
              bool bTestMode,
			  const QString& directory,
              QTextStream* logStream) : Engine(name, command, bTestMode, directory, logStream)
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

    if (message == "readyok")
    {
		if (m_waitingOn == "uciok") {
			//engine is now initialised and ready to go
			m_waitingOn = "";
			setActive(true);

            if (!m_bTestMode)
            {
                OptionValueList::const_iterator i = m_mapOptionValues.constBegin();
                while (i != m_mapOptionValues.constEnd()) {
                    QString key = i.key();
                    QVariant value = i.value();
                    if (EngineOptionData* dataSpec = EngineOptionData::FindInList(key, m_options))
                    {
                        switch (dataSpec->m_type)
                        {
                        case OPT_TYPE_BUTTON:
                            if (value.toBool())
                            {
                                send(QString("setoption name %1 value %2").arg(key));
                            }
                            break;
                        case OPT_TYPE_CHECK:
                        case OPT_TYPE_SPIN:
                        case OPT_TYPE_STRING:
                        case OPT_TYPE_COMBO:
                            if (dataSpec->m_defVal != value.toString() && !value.toString().isEmpty())
                            {
                                send(QString("setoption name %1 value %2").arg(key).arg(value.toString()));
                            }
                            break;
                        }
                    }
                    ++i;
                }
            }

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

    QString command = message.section(' ', 0, 0);

    if (command == "info" && isAnalyzing())
    {
		parseAnalysis(message);
	}
    else if (command == "option" && !isAnalyzing())
    {
        parseOptions(message);
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

void UCIEngine::parseOptions(const QString& message)
{
    enum ScanPhase { EXPECT_OPTION,
                     EXPECT_NAME,
                     EXPECT_TYPE_TOKEN,
                     EXPECT_TYPE,
                     EXPECT_DEFAULT_VALUE,
                     EXPECT_MIN_MAX_DEFAULT,
                     EXPECT_MIN_VALUE,
                     EXPECT_MAX_VALUE,
                     EXPECT_VAR_TOKEN,
                     EXPECT_VAR } phase;

    phase = EXPECT_OPTION;
    QStringList list = message.split(QRegExp("\\W+"), QString::SkipEmptyParts);

    QStringList nameVals;
    QString defVal;
    QString minVal;
    QString maxVal;
    QStringList varVals;
    OptionType optionType = OPT_TYPE_STRING;
    QString error;
    bool done = false;
    foreach (QString token, list)
    {
        switch (phase)
        {
        case EXPECT_OPTION:
            if (token == "option")
                phase = EXPECT_NAME;
            else
                error = token;
            break;
        case EXPECT_NAME:
            if (token == "name")
                phase = EXPECT_TYPE;
            else
                error = token;
            break;
        case EXPECT_TYPE:
            if (token == "type")
                phase = EXPECT_TYPE_TOKEN;
            else
                nameVals << token;
            break;
        case EXPECT_TYPE_TOKEN:
            if (token == "check")
                optionType = OPT_TYPE_CHECK;
            else if (token == "spin")
                optionType = OPT_TYPE_SPIN;
            else if (token == "combo")
                optionType = OPT_TYPE_COMBO;
            else if (token == "button")
            {
                optionType = OPT_TYPE_BUTTON;
                done = true;
            }
            else if (token == "string")
                optionType = OPT_TYPE_STRING;
            else error = token;

            phase = EXPECT_MIN_MAX_DEFAULT;
            break;
        case EXPECT_DEFAULT_VALUE:
            defVal = token;
            switch (optionType)
            {
            case OPT_TYPE_SPIN:
                phase = EXPECT_MIN_MAX_DEFAULT;
                break;
            case OPT_TYPE_COMBO:
                phase = EXPECT_VAR_TOKEN;
                break;
            case OPT_TYPE_CHECK:
            case OPT_TYPE_STRING:
            default:
                done = true;
                break;
            }
            break;
        case EXPECT_MIN_MAX_DEFAULT:
            if (token == "default")
                phase = EXPECT_DEFAULT_VALUE;
            else if (token == "min")
                phase = EXPECT_MIN_VALUE;
            else if (token == "max")
                phase = EXPECT_MAX_VALUE;
            else
                done = true;
            break;
        case EXPECT_MIN_VALUE:
            minVal = token;
            phase = EXPECT_MIN_MAX_DEFAULT;
            break;
        case EXPECT_MAX_VALUE:
            maxVal = token;
            phase = EXPECT_MIN_MAX_DEFAULT;
            break;
        case EXPECT_VAR_TOKEN:
            if (token == "var")
                phase = EXPECT_VAR;
            else
                done = true;
            break;
        case EXPECT_VAR:
            varVals << token;
            phase = EXPECT_VAR_TOKEN;
            break;
        default:
            error = token;
            return;
        }

        if (done || !error.isEmpty())
        {
            break;
        }
    }
    if (!error.isEmpty())
    {
        qDebug() << "Cannot parse Option string: '"
                 << message
                 << "' looking at token '"
                 << error
                 << "'!";
        return;
    }
    if (done || (phase > EXPECT_DEFAULT_VALUE))
    {
        QString name = nameVals.join(" ");
        EngineOptionData option;
        option.m_name = name;
        option.m_minVal = minVal;
        option.m_maxVal = maxVal;
        option.m_defVal = defVal;
        option.m_varVals = varVals;
        option.m_type = optionType;

        m_options.append(option);
    }
    else
    {
        qDebug() << "Incomplete syntax parsing Option string: '"
                 << message
                 << "'!";
        return;
    }
}
