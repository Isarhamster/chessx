#ifndef LOADQUERY_H
#define LOADQUERY_H

#include <QDialog>
#include "ui_loadquery.h"

class LoadQuery : public QDialog
{
    Q_OBJECT

public:
    explicit LoadQuery(QWidget *parent = 0);
    ~LoadQuery();

    bool largeDB() const
    {
        return ui->btLargeDatabase->isChecked();
    }

private:
    Ui::LoadQuery *ui;
};

#endif // LOADQUERY_H
