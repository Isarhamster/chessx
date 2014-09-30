/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TAGDIALOG_H
#define TAGDIALOG_H

#include <QDialog>
#include "game.h"

namespace Ui {
class TagDialog;
}

class Index;

class TagDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagDialog(QWidget *parent = 0);
    ~TagDialog();

    bool editTags(Index* index, Game &game, GameId id);

protected slots:
    void accept();
    void reject();

private slots:
    void saveOldTagText(int row, int column);
    void validateTag(int row, int column);

private:
    QString oldTagText;

private:
    Ui::TagDialog *ui;
};

#endif // TAGDIALOG_H
