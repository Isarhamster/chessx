/***************************************************************************
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2008-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "settings.h"
#include "engine.h"
#include "wbengine.h"
#include "uciengine.h"

#define DEBUG_ENGINE
//#undef DEBUG_ENGINE

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

/*** Engine ***/

bool Engine::s_allowEngineOutput = true;

Engine::Engine(const QString& name,
               const QString& command,
               bool bTestMode,
               const QString& directory,
               bool log)
{
    m_name = name;
    m_command = command;
    m_bTestMode = bTestMode;
    QTextStream* logStream = 0;
    if (log)
    {
        m_logFile.setFileName(AppSettings->logPath()+name+".log");
        if (m_logFile.open(QIODevice::WriteOnly))
        {
            logStream = new QTextStream(&m_logFile);
        }
    }
    m_logStream = logStream;
    m_process = 0;
    m_active = false;
    m_analyzing = false;
    m_directory = directory;
}

Engine* Engine::newEngine(int index)
{
    return newEngine(index, false);
}

Engine* Engine::newEngine(EngineList& engineList, int index, bool bTestMode)
{
    Engine *engine = 0;

    QString name      = engineList[index].name;
    QString command   = engineList[index].command;
    QString options   = engineList[index].options;
    QString directory = engineList[index].directory;
    bool logging      = engineList[index].logging;
    EngineData::EngineProtocol protocol = engineList[index].protocol;

    if(command.contains(' '))
    {
        command = QString("\"%1\"").arg(command);
    }
    QString exe = QString("%1 %2").arg(command).arg(options);

    if(protocol == EngineData::WinBoard)
    {
        engine = new WBEngine(name, exe, bTestMode, directory, logging);
    }
    else
    {
        engine = new UCIEngine(name, exe, bTestMode, directory, logging);
    }

    engine->m_mapOptionValues = engineList[index].m_optionValues;
    return engine;
}

Engine* Engine::newEngine(int index, bool bTestMode)
{
    Engine *engine = 0;

    AppSettings->beginGroup("/Engines/");
    QString key(QString::number(index));
    QString name = AppSettings->value(key + "/Name").toString();
    QString command = AppSettings->value(key + "/Command").toString();
    QString options = AppSettings->value(key + "/Options").toString();
    QString directory = AppSettings->value(key + "/Directory").toString();
    QString protocol = AppSettings->value(key + "/Protocol").toString();
    bool log = AppSettings->value(key + "/Logging").toBool();

    if(command.contains(' '))
    {
        command = QString("\"%1\"").arg(command);
    }
    QString exe = QString("%1 %2").arg(command).arg(options);

    if(protocol == "WinBoard")
    {
        engine = new WBEngine(name, exe, bTestMode, directory, log);
    }
    else
    {
        engine = new UCIEngine(name, exe, bTestMode, directory, log);
    }

    AppSettings->getMap(key + "/OptionValues", engine->m_mapOptionValues);
    AppSettings->endGroup();

    return engine;
}

Engine::~Engine()
{
    if(m_process)
    {
        m_process = 0;
    }
    if (m_logStream)
    {
        delete m_logStream;
    }
}

void Engine::activate()
{
    if(m_process)
    {
        return;
    }

    m_process = new QProcess(this);
    if(m_process)
    {
        m_process->setReadChannel(QProcess::StandardOutput);
        if(!m_directory.isEmpty())
        {
            m_process->setWorkingDirectory(m_directory);
        }
        connect(m_process, SIGNAL(started()), SLOT(protocolStart()));
        connect(m_process, SIGNAL(error(QProcess::ProcessError)), SLOT(processError(QProcess::ProcessError)));
        connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(pollProcess()));
        connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(processExited()));
        m_process->start(m_command);
    }
}

void Engine::deactivate()
{
    if(m_active)
    {
        protocolEnd();
        if(m_process)
        {
            m_process->waitForFinished(200);
        }
    }
}

bool Engine::isActive()
{
    return m_active;
}

bool Engine::isAnalyzing()
{
    return m_analyzing;
}

void Engine::send(const QString& message)
{
    if (s_allowEngineOutput && m_logStream)
    {
        *m_logStream << "<-- " << message << endl;
    }

    QString out(message);
    out.append('\n');
    if(m_process && !message.isEmpty())
    {
        m_process->write(out.toLatin1());
    }
}

void Engine::setActive(bool active)
{
    if(active && !m_active)
    {
        m_active = true;
        emit activated();
    }
    else
    {
        if(!active && m_active)
        {
            setAnalyzing(false);
            m_active = false;
            emit deactivated();
        }
    }
}

void Engine::setAnalyzing(bool analyzing)
{
    if(analyzing)
    {
        m_analyzing = true;
        emit analysisStarted();
    }
    else
    {
        if(!analyzing && m_analyzing)
        {
            m_analyzing = false;
            emit analysisStopped();
        }
    }
}

void Engine::sendAnalysis(const Analysis& analysis)
{
    if (s_allowEngineOutput)
    {
        emit analysisUpdated(analysis);
    }
}

void Engine::setMpv(int mpv)
{
    m_mpv = mpv;
}

void Engine::setMoveTime(const EngineParameter& mt)
{
    m_moveTime = mt;
}

void Engine::pollProcess()
{
    QString message;

    while(m_process && m_process->canReadLine())
    {
        message = m_process->readLine().simplified();
        if (s_allowEngineOutput && m_logStream)
        {
            *m_logStream << "--> " << message << endl;
        }
        processMessage(message);
    }
}

void Engine::processError(QProcess::ProcessError errMsg)
{
    setActive(false);
    m_process = 0;
    emit error(errMsg);
}

bool Engine::hasOption(QString name) const
{
    bool found = false;
    foreach (EngineOptionData eod, m_options)
    {
        if (eod.m_name == name)
        {
            found = true;
            break;
        }
    }
    return found;
}

bool Engine::getOption(QString name, EngineOptionData& result)
{
    bool found = false;
    foreach (EngineOptionData eod, m_options)
    {
        if (eod.m_name == name)
        {
            found = true;
            result = eod;
            break;
        }
    }
    return found;
}

void Engine::logError(QString errMsg)
{
    if (s_allowEngineOutput && m_logStream)
    {
        *m_logStream << "### " << errMsg << endl;
    }
    qDebug() << errMsg;
}

void Engine::setAllowEngineOutput(bool allow)
{
    s_allowEngineOutput = allow;
}

void Engine::processExited()
{
    setActive(false);
    m_process = 0;
    emit deactivated();
}
