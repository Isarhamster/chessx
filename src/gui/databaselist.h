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

public slots:
    void addRecentFile(const QString& s);
    void addFileOpen(const QString& s);
    void setFileModified(const QString& s, bool modified);
    void setFileFavorite(const QString& s);
    void setFileClose(const QString& s);
    void setFileCurrent(const QString& s);

signals:
    void selected(int);
    void requestOpenDatabase(QString s);

private slots:
    void itemSelected(const QModelIndex& index);
private:
    DatabaseListModel* m_model;
    QSortFilterProxyModel* m_filterModel;
};

#endif // DATABASELIST_H
