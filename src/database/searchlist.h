#ifndef SEARCHLIST_H
#define SEARCHLIST_H

#include <QObject>
#include <QList>
#include "search.h"

class SearchList : public QObject
{
    Q_OBJECT
public:
    explicit SearchList(QObject *parent = 0);

    void execute();
signals:

public slots:

private:
    QList<Search> searchList;
};

#endif // SEARCHLIST_H
