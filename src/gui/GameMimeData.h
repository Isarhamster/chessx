/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef GAMEMIMEDATA_H
#define GAMEMIMEDATA_H

#include <QMimeData>
#include <QModelIndex>
#include "game.h"

class GameMimeData : public QMimeData
{
    Q_OBJECT

public:
    QList<GameId> m_indexList;
    QString source;
};

class DbMimeData : public QMimeData
{
    Q_OBJECT
#ifdef Q_OS_DARWIN
public:
    QList<QUrl> urls() const { return m_urls; }
    void setUrls(const QList<QUrl> &urls) { m_urls = urls; }
    bool hasUrls() const { return m_urls.count() > 0; }
private:
    QList<QUrl> m_urls;
#endif
};

#endif // GAMEMIMEDATA_H
