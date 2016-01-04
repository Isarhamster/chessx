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

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

WBEngine::WBEngine(const QString& name,
                   const QString& command,
                   bool bTestMode,
                   const QString& directory,
                   bool log) : Engine(name, command, bTestMode, directory, log)
{
    m_analyze = false;
    m_setboard = false;		// We do not support version 1 xboard protocol, so this _must_ be set true by feature discovery
    m_bHasSentAnalyze = false;
    m_invertBlack = true;
}

void WBEngine::go()
{
    send("post");
    if (m_moveTime.tm == EngineParameter::TIME_GONG)
    {
        if (m_moveTime.ms_totalTime > 0)
        {
            send (QString("st %1").arg(m_moveTime.ms_totalTime/1000));
            send ("go");
            m_bHasSentAnalyze = false;
        }
        else
        {
            send("analyze");
            m_bHasSentAnalyze = true;
        }
    }
    else if (m_moveTime.tm == EngineParameter::TIME_SUDDEN_DEATH)
    {
        QTime t(0,0);
        t = t.addMSecs(m_board.toMove()==White ? m_moveTime.ms_white : m_moveTime.ms_black);
        send (QString("level 0 %1 %2").arg(t.toString("HH:mm:ss")).arg(m_moveTime.ms_increment/1000));
        send ("go");
        m_bHasSentAnalyze = false;
    }
}

bool WBEngine::startAnalysis(const Board& board, int nv, const EngineParameter &mt, bool /*bNewGame*/)
{
    Engine::setMoveTime(mt);
    m_mpv = nv;

    if(isActive() && m_board == board)
    {
        return true;
    }
    stopAnalysis();
    m_board = board;
    if(m_analyze && isActive() && m_setboard)
    {
        send("setboard " + board.toFen());
        go();
        setAnalyzing(true);
        return true;
    }
    return false;
}

void WBEngine::stopAnalysis()
{
    if(isAnalyzing())
    {
        if (m_bHasSentAnalyze) send("exit");
        setAnalyzing(false);
    }
}

void WBEngine::protocolStart()
{
    send("xboard");
    send("protover 2");
    QTimer::singleShot(2000, this, SLOT(featureTimeout()));
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
    if(trim.startsWith("White (1) : "))
    {
        trim = message.mid(12);
    }

    trim = trim.trimmed();

    if(!isActive() && trim.startsWith("Crafty v"))
    {
        m_invertBlack = false;
    }

    //determine command
    QString command = trim.section(" ", 0, 0);

    //identify and process the command
    if(command == "feature")
    {
        feature(trim);
    }
    else if(command == "move")
    {
        parseBestMove(trim);
    }
    else if ((command == "resign") || (command == "result") || (command == "1-0") || (command == "0-1") || (command == "1/2-1/2"))
    {
        parseEndOfGame(command, trim);
    }
    else if(isAnalyzing())
    {
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

    while(index < (int)command.length())
    {

        //feature name terminates with an =
        equalsIndex = command.indexOf('=', index);
        feature = command.mid(index, equalsIndex - index);

        //string values are delimited by ", others end with whitespace
        if(command[equalsIndex + 1] == '"')
        {
            endIndex = command.indexOf('"', equalsIndex + 2);
            value = command.mid(equalsIndex + 2, (endIndex - equalsIndex) - 2);
            index = endIndex + 2;
        }
        else
        {
            endIndex = command.indexOf(' ', equalsIndex + 1);
            value = command.mid(equalsIndex + 1, (endIndex - equalsIndex) - 1);
            index = endIndex + 1;
        }

        //if endIndex is -1 then the end of the string has been reached
        if(endIndex == -1)
        {
            index = command.length();
        }

        //process feature/value pair
        if(feature == "setboard")
        {
            m_setboard = (bool)value.toInt();
            send("accepted " + feature);
        }
        else if(feature == "analyze")
        {
            m_analyze = (bool)value.toInt();
            send("accepted " + feature);
        }
        else if(feature == "done")
        {
            send("accepted " + feature);
            featureDone((bool)value.toInt());
        }
        else
        {
            //unknown feature, reject it
            send("rejected " + feature);
        }

        QString name = feature;
        EngineOptionData option;
        option.m_name = name;
        option.m_defVal = value;
        option.m_type = OPT_TYPE_SPIN;

        m_options.append(option);
    }
}

void WBEngine::featureDone(bool done)
{
    // We've received a "done" feature offer from engine,
    // so it supports V2 or better of the xboard protocol

    // No need to wait any longer wondering if we're talking to a V1 engine

    if(done)
    {
        setActive(true);
    }

    // The engine will send done=1, when its ready to go,
    //  and done=0 if it needs more than 2 seconds to start.
}

void WBEngine::featureTimeout()
{
    if(!isActive())
    {
        v1TurnOffPondering();
        setActive(true);
    }
}

void WBEngine::parseBestMove(const QString& message)
{
    QString bestMove = message.section(' ', 1, 1, QString::SectionSkipEmpty);

    if (!bestMove.isEmpty())
    {
        Analysis analysis;
        Move move = m_board.parseMove(bestMove);
        MoveList moves;
        if (move.isLegal())
        {
            moves.append(move);
        }
        analysis.setVariation(moves);
        analysis.setBestMove(true);
        sendAnalysis(analysis);
    }
}

void WBEngine::parseEndOfGame(const QString& command, const QString& message)
{
    Analysis analysis;
    analysis.setEndOfGame(true);
    if (command == "1-0" || message.contains("1-0"))
    {
        analysis.setScore(999);
    }
    else if (command == "0-1" || message.contains("0-1"))
    {
        analysis.setScore(-999);
    }
    else if (command == "1/2-1/2")
    {
       analysis.setScore(0);
    }
    else if (message.isEmpty())
    {
        analysis.setScore(m_board.toMove() == White ? 999 : -999);
    }
    sendAnalysis(analysis);
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
    if(!ok)
    {
        depth.truncate(depth.length() - 1);
        analysis.setDepth(depth.toInt(&ok));
        if(!ok)
        {
            return;
        }
        timeInSeconds = true;
    }

    //Score
    int score = trimmed.section(' ', 1, 1).toInt(&ok);
    if(!ok)
    {
        return;
    }
    if(m_invertBlack && m_board.toMove() == Black)
    {
        score = -score;
    }
    analysis.setScore(score);

    //Time
    int time = trimmed.section(' ', 2, 2).toInt(&ok);
    if(!ok)
    {
        return;
    }
    if(timeInSeconds)
    {
        time = time * 1000;
    }
    else
    {
        time *= 10;
    }
    analysis.setTime(time);

    //Node
    analysis.setNodes(trimmed.section(' ', 3 , 3).toLongLong(&ok));
    if(!ok)
    {
        return;
    }

    //Variation
    Board board = m_board;
    QString sanMove;
    MoveList moves;
    int section = 4;
    while((sanMove = trimmed.section(' ', section, section)) != "")
    {
        if(sanMove.startsWith("("))
        {
            break;
        }
        // SBE -- What the heck is "<HT>" and why does crafty send it?
        if(!sanMove.endsWith(".") && sanMove != "<HT>")
        {
            Move move = board.parseMove(sanMove);
            if(!move.isLegal())
            {
                break;
            }
            board.doMove(move);
            moves.append(move);
        }
        section++;
    }
    analysis.setVariation(moves);

    if(analysis.isValid())
    {
        sendAnalysis(analysis);
    }
}

void WBEngine::v1TurnOffPondering()
{
    send("hard");
    send("easy");
}


