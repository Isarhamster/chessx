/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef DATABASELIST_H
#define DATABASELIST_H

#include "tableview.h"
#include "game.h"

class QSortFilterProxyModel;
class DatabaseListModel;

class DatabaseList : public TableView
{
    Q_OBJECT
public:
    explicit DatabaseList(QWidget *parent = 0);
    ~DatabaseList();
    void save() const;


public slots:
    void addFileOpen(const QString& s);
    void setFileFavorite(const QString& s, bool bFavorite);
    void setFileClose(const QString& s);
    void setFileCurrent(const QString& s);
    void update(const QString& s);
    void slotCurrentIndexChanged(const QModelIndex&);

signals:
    void selected(int);
    void requestOpenDatabase(QString s);
    void requestLinkDatabase(QString s);
    void requestAppendGame(QString path, const Game& game);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

    void appendGameToDataBase(QPoint, const Game& g);


private slots:
    void itemSelected(const QModelIndex& index);
    void slotContextMenu(const QPoint& pos);
    void dbAddToFavorites();
    void dbRemoveFromFavorites();
    void slotShowInFinder();
    void rowsChanged(const QModelIndex &,int start,int end);

private:
    DatabaseListModel* m_model;
    QSortFilterProxyModel* m_filterModel;
    QModelIndex m_cell;
    Qt::KeyboardModifiers m_lastModifier;
};

#endif // DATABASELIST_H
