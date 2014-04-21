/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "dlgsavebook.h"
#include "messagedialog.h"
#include "settings.h"
#include "ui_dlgsavebook.h"

#include <QStringList>
#include <QFileInfo>

DlgSaveBook::DlgSaveBook(QString path, QWidget *parent) :
  QDialog(parent),
  m_InputPath(path),
  m_process(0),
  ui(new Ui::DlgSaveBook)
{
  QFileInfo fi(m_InputPath);
  QString in = fi.absoluteFilePath();
  m_OutputPath = fi.absoluteDir().absolutePath()+"/"+fi.baseName()+".bin";

  ui->setupUi(this);

  ui->inputFile->setText(fi.fileName());
  ui->outputPath->setText(m_OutputPath);
}

DlgSaveBook::~DlgSaveBook()
{
  delete ui;
}

void DlgSaveBook::accept()
{
    QString exe = "./polyglot";

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    bool hasInternalPolyglot = !AppSettings->getValue("Tools/ExtPolyglot").toBool();
#else
    bool hasInternalPolyglot = false;
#endif

    if (hasInternalPolyglot)
    {
#ifdef Q_OS_WIN
        exe.append(".exe");
#endif
    }
    else
    {
        exe = QFileInfo(AppSettings->getValue("Tools/PathPolyglot").toString()).absoluteFilePath();
    }
    exe.remove('\"');

    QFileInfo fi(m_InputPath);
    m_process = new QProcess(this);
    if(fi.exists() && m_process)
    {
        m_polyglotText.clear();
        m_process->setWorkingDirectory(AppSettings->winboardPath());
        m_process->setReadChannel(QProcess::StandardError);
        connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(readOutput()));
        QStringList options;
        options << "\"" + exe + "\"";
        options << "make-book";
        options << "-min-game";
        options << ui->minGame->cleanText();
        options << "-max-ply";
        options << ui->maxPly->cleanText();
        options << "-pgn";
        options << "\"" + m_InputPath + "\"";
        options << "-bin";
        options << "\"" + m_OutputPath + "\"";
        QString command = options.join(' ');
        m_process->start(command);
        if (!m_process->waitForFinished())
        {
            MessageDialog::warning(m_process->errorString(),tr("Polyglot Error"));
        }
        else
        {
            if (QFileInfo(m_OutputPath).exists())
            {
                emit bookWritten(m_OutputPath);
            }
            else
            {
                readOutput();
                if (m_polyglotText.isEmpty())
                {
                    m_polyglotText = tr("Unknown Error");
                }
                MessageDialog::warning(m_polyglotText,tr("Polyglot Error"));
            }
        }
    }
    else
    {
        MessageDialog::warning(tr("File not found"),tr("Polyglot Error"));
    }
    QDialog::accept();
}

void DlgSaveBook::readOutput()
{
    while(m_process && m_process->canReadLine())
    {
        QString message = m_process->readLine().simplified();
        m_polyglotText.append(message);
        m_polyglotText.append("\n");
    }
}
