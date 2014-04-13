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

    int getLastIndex(const QString& s) const;

public slots:
    void addFileOpen(const QString& s, bool utf8);
    void setFileFavorite(const QString& s, bool bFavorite, int index);
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
    void requestAppendGame(QString path, int index);
    void requestAppendDatabase(QString dest, QString src);

protected: //Drag'n'Drop Support
    void startToDrag(const QModelIndex&);

protected:
    void startDrag(Qt::DropActions supportedActions);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

    void appendGameToDataBase(QModelIndex index, int gameIndex);
    void appendDataBaseToDataBase(QPoint pos, QString src);

private slots:
    void itemSelected(const QModelIndex& index);
    void slotContextMenu(const QPoint& pos);
    void dbAddToFavorites();
    void dbRemoveFromFavorites();
    void slotShowInFinder(QString path);
    void slotShowInFinder();
    void slotMakeBook();
    void slotExtTool1();
    void extToolReadOutput();
    void dbOpen();
    void dbToggleUTF8();
    void dbClose();
    void rowsChanged(const QModelIndex &, int start, int end);

private:
    DatabaseListModel* m_model;
    QSortFilterProxyModel* m_filterModel;
    QModelIndex m_cell;
    Qt::KeyboardModifiers m_lastModifier;
    QString errText;
    QProcess* m_extToolProcess;
};

#endif // DATABASELIST_H
