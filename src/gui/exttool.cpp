/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "exttool.h"
#include "messagedialog.h"
#include "settings.h"

#include <QFileInfo>
#include <QProcess>
#include <QString>
#include <QStringList>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

ExtTool::ExtTool(QObject *parent) : QObject(parent)
{

}

ExtTool::~ExtTool()
{

}

void ExtTool::extToolReadOutput()
{
    while(m_extToolProcess && m_extToolProcess->canReadLine())
    {
        QString message = m_extToolProcess->readLine().simplified();
        errText.append(message);
        errText.append("\n");
    }
}

void ExtTool::RunExtTool1(QString pathIn)
{
    errText.clear();

    QString extTool1 = AppSettings->value("Tools/Path1").toString();
    QFileInfo fiExtTool(extTool1);
    QFileInfo fiPathIn(pathIn);
    m_extToolProcess = new QProcess(this);
    if(fiExtTool.exists() && fiPathIn.exists() && m_extToolProcess)
    {
        m_extToolProcess->setReadChannel(QProcess::StandardError);
        m_extToolProcess->setWorkingDirectory(fiPathIn.absolutePath());
        connect(m_extToolProcess, SIGNAL(readyReadStandardOutput()), SLOT(extToolReadOutput()));
        QStringList options;

        options << fiExtTool.absoluteFilePath();
        QString parameter = AppSettings->value("Tools/CommandLine1").toString();
        parameter.replace("$(InputPath)",fiPathIn.absoluteFilePath());
        parameter.replace("$(InputFile)",fiPathIn.fileName());
        parameter.replace("$(InputDir)",fiPathIn.absolutePath());
        parameter.replace("$(InputName)",fiPathIn.baseName());

        options << parameter;
        QString command = options.join(" ");
        m_extToolProcess->start(command);
        if (!m_extToolProcess->waitForFinished())
        {
            MessageDialog::warning(fiExtTool.baseName() + ": " + m_extToolProcess->errorString());
        }
        else
        {
            extToolReadOutput();
            if (!errText.isEmpty())
            {
                MessageDialog::warning(fiExtTool.baseName() + ": " +errText);
            }
        }
    }
    else
    {
        MessageDialog::warning(fiExtTool.baseName() + tr(": File not found"));
    }
}
