/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef POLYGLOTWRITER_H
#define POLYGLOTWRITER_H

#include <QThread>
#include "database.h"
#include "polyglotdatabase.h"

class PolyglotWriter : public QThread
{
    Q_OBJECT
public:
    explicit PolyglotWriter(QObject *parent = 0);
    ~PolyglotWriter();
    void writeBookForDatabase(Database* src, const QString &out, int maxPly, int minGame, bool uniform, int result, int filterResult);

signals:
    void bookBuildFinished(QString);
    void bookBuildError(QString);
    void progress(int);

public slots:
    void cancel();

    // QThread interface
protected:
    virtual void run();

    QPointer<Database> m_source;
    QPointer<PolyglotDatabase> m_destination;
    QString m_out;

    bool m_break;
};

#endif // POLYGLOTWRITER_H
