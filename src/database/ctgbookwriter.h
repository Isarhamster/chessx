/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef CTGBOOKWRITER_H
#define CTGBOOKWRITER_H

#include <QThread>
#include "database.h"
#include "ctgdatabase.h"

class CtgBookWriter : public QThread
{
    Q_OBJECT
public:
    explicit CtgBookWriter(QObject *parent = 0);
    ~CtgBookWriter();
    void writeBookForDatabase(Database* src, const QString &out, int maxPly, int minGame, bool uniform);

signals:
    void bookBuildFinished(QString);
    void bookBuildError(QString);

public slots:
    void cancel();

    // QThread interface
protected:
    virtual void run();

    QPointer<Database> m_source;
    QPointer<CtgDatabase> m_destination;
    QString m_out;

    bool m_break;
};

#endif // CTGBOOKWRITER_H
