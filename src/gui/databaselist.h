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

    void addRecentFile(const QString& s);

signals:
    void selected(int);
    void requestOpenDatabase(QString s);

public slots:

private slots:
    void itemSelected(const QModelIndex& index);
private:
    DatabaseListModel* m_model;
    QSortFilterProxyModel* m_filterModel;
};

#endif // DATABASELIST_H
