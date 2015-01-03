/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef EXTTOOL_H
#define EXTTOOL_H

#include <QObject>
#include <QString>

class QProcess;

class ExtTool : public QObject
{
    Q_OBJECT
public:
    explicit ExtTool(QObject *parent = 0);
    ~ExtTool();

signals:

public slots:
    void RunExtTool1(QString pathIn);

private slots:
    void extToolReadOutput();

private:
    QString errText;
    QProcess* m_extToolProcess;
};

#endif // EXTTOOL_H
