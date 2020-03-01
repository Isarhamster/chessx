/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef LOGSTRAM_H
#define LOGSTRAM_H

#include <QFile>
#include <QTextStream>

class LogStream
{
public:
    LogStream();
    ~LogStream();

    static QTextStream s_logStream;

private:
    void startFileLog();
    void stopFileLog();

    QFile m_logFile;
};


#endif // LOGSTRAM_H
