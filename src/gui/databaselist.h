/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef DATABASELIST_H
#define DATABASELIST_H

#include "tableview.h"

class QSortFilterProxyModel;
class DatabaseListModel;

class DatabaseList : public TableView
{
    Q_OBJECT
public:
    explicit DatabaseList(QWidget *parent = 0);
    ~DatabaseList();
    void save() const;

    int getLastIndex(const QString& s) const;

public slots:
    void addFileOpen(const QString& s, bool utf8);
    void setFileFavorite(const QString& s, bool bFavorite, int index);
    void setStars(const QString& s, int stars);
    void setFileUtf8(const QString&, bool);
    void setFileClose(const QString& s, int lastIndex);
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
    void requestAppendGames(QString path, QList<int> indexes);
    void requestAppendDatabase(QString dest, QString src);
    void requestMakeBook(QString s);

protected: //Drag'n'Drop Support
    void startToDrag(const QModelIndex&);
    virtual int minRowHeight() const { return 24; }

protected:
    void startDrag(Qt::DropActions supportedActions);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

    void appendGameToDataBase(QModelIndex index, QList<int> gameIndexList);
    void appendDataBaseToDataBase(QPoint pos, QString src);

private slots:
    void itemSelected(const QModelIndex& index);
    void slotContextMenu(const QPoint& pos);
    void dbAddToFavorites2();
    void dbAddToFavorites3();
    void dbKeepFile();
    void dbRemoveFromFavorites();
    void slotShowInFinder();
    void slotMakeBook();
    void slotExtTool1();
    void dbOpen();
    void dbToggleUTF8();
    void dbClose();

private:
    DatabaseListModel* m_model;
    QSortFilterProxyModel* m_filterModel;
    QModelIndex m_cell;
    Qt::KeyboardModifiers m_lastModifier;
};

#endif // DATABASELIST_H
