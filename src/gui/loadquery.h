#ifndef LOADQUERY_H
/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

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

protected slots:
    void buttonClicked(QAbstractButton *button);

private:
    Ui::LoadQuery *ui;
};

#endif // LOADQUERY_H
