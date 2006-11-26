/***************************************************************************
                          engine.cpp  -  Chess engine class
                             -------------------
    begin                : 25 July 2005
    copyright            : (C) 2005, 2006 William Hoggarth
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

#include "engine.h"

/*** Engine ***/

Engine::Engine(const QString& name, const QString& command,
								QTextStream* logStream) : QObject()
{
 m_name = name;
 m_command = command;
 m_logStream = logStream;
 m_process = 0;
 m_processStream = 0;
 m_active = false;
 m_analyzing = false;
}

Engine::~Engine()
{
	deactivate();
}

void Engine::setLogStream(QTextStream* logStream)
{
	m_logStream = logStream;
}

void Engine::activate()
{
 if(m_active) {
	 return;
 }

 m_process = new QProcess(this);
 m_processStream = new QTextStream(m_process);
 connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(pollProcess()));
 connect(m_process, SIGNAL(processFinished(int, ExitStatus)), this, SLOT(processExited()));

 m_process->start(m_command);
 protocolStart();
}

void Engine::deactivate()
{
 if(!m_active) {
	 return;
 }
 
 protocolEnd();
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
	if(m_logStream) {
		*m_logStream << "<-- " << message << endl;
		emit logUpdated();
	}
	
	*m_processStream << message << endl;
}

void Engine::setActive(bool active)
{
	if(active && !m_active) {
		m_active = true;
		emit activated();
	} else {
		if(!active && m_active) {
			setAnalyzing(false);
			m_active = false;
			emit deactivated();
		}
	}
}

void Engine::setAnalyzing(bool analyzing)
{
	if(analyzing) {
		m_analyzing = true;
		emit analysisStarted();
	} else {
		if(!analyzing && m_analyzing) {
			m_analyzing = false;
			emit analysisStopped();
		}
	}
}

void Engine::sendAnalysis(const Analysis& analysis)
{
	emit analysisUpdated(analysis);
}

void Engine::pollProcess()
{
	QString message;
	while(m_process->canReadLine()) {
		*m_processStream >> message;
		if(m_logStream) {
			*m_logStream << "--> " << message << endl;
			emit logUpdated();
		}
		processMessage(message);
	}
}
				
void Engine::processExited()
{
	setActive(false);
	delete m_processStream;
	delete m_process;
	m_process = 0;
}
