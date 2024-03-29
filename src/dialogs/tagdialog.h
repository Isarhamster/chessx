/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TAGDIALOG_H
#define TAGDIALOG_H

#include <QDialog>
#include "gamex.h"
#include "gameid.h"

class IndexX;

namespace Ui {
class TagDialog;
}

class TagDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagDialog(QWidget *parent = nullptr);
    ~TagDialog();

    bool editTags(IndexX* index, GameX &game, GameId id);

protected slots:
    void accept();
    void reject();
    void slotAddTag();

private slots:
    void saveOldTagText(int row, int column);
    void validateTag(int row, int column);

private:
    QString oldTagText;

private:
    Ui::TagDialog *ui;
};

#endif // TAGDIALOG_H
