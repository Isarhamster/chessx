/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef DATABASELIST_H
#define DATABASELIST_H

#include "gameid.h"
#include "tableview.h"

class QSortFilterProxyModel;
class DatabaseListModel;

class DatabaseList : public TableView
{
    Q_OBJECT
public:
    explicit DatabaseList(QWidget *parent = nullptr);
    ~DatabaseList();
    void save() const;

    GameId getLastIndex(const QString& s) const;
    int stars(const QString& s) const;

    void limitStars(int limit);
public slots:
    void addFileOpen(const QString& s, bool utf8);
    void setFileFavorite(const QString& s, bool bFavorite, int index);
    void setStars(const QString& s, int stars);
    void setFileUtf8(const QString&, bool);
    void setFileClose(const QString& s, GameId lastIndex);
    void setFileCurrent(const QString& s);
    void update(const QString& s);
    void slotCurrentIndexChanged(const QModelIndex&);
    void slotDoubleClicked(const QModelIndex&);

signals:
    void selected(int);
    void raiseRequest();
    void requestOpenDatabase(QString s, bool utf8);
    void requestCloseDatabase(QString s);
    void requestLinkDatabase(QString s);
    void requestAppendGames(QString path, QList<GameId> indexes, QString source);
    void requestAppendDatabase(QString dest, QString src);
    void requestMakeBook(QString s);

protected: //Drag'n'Drop Support
    void startToDrag();
    virtual int minRowHeight() const { return 24; }

protected:
    void startDrag(Qt::DropActions supportedActions);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

    void appendGameToDataBase(QModelIndex index, QList<GameId> gameIndexList, QString source);
    void appendDataBaseToDataBase(QPoint pos, QString src);

private slots:
    void itemSelected(const QModelIndex& index);
    void slotContextMenu(const QPoint& pos);
    void dbAddToFavorites2();
    void dbAddToFavorites3();
    void dbAddToOpenAtStartup();
    void dbSetActiveAtStartup();
    void dbKeepFile();
    void dbRemoveFromFavorites();
    void slotShowInFinder();
    void slotMakeBook();
    void slotExtTool1();
    void dbOpen();
    void dbToggleUTF8();
    void dbClose();
    void dbSetStarsForSelection(int stars);

private:
    DatabaseListModel* m_model;
    QSortFilterProxyModel* m_filterModel;
    QModelIndex m_cell;
    Qt::KeyboardModifiers m_lastModifier;
};

#endif // DATABASELIST_H
