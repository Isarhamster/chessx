/****************************************************************************
*   Copyright (C) 2010 by Michal Rudolf <mrudolf@kdewebdev.org>           *
****************************************************************************/

#ifndef COMMENTDIALOG_H
#define COMMENTDIALOG_H

#include "ui_commentdialog.h"

class CommentDialog : public QDialog
{
    Q_OBJECT
public:
    CommentDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);
    void setText(QString text);
    QString text() const;

protected slots:
    void clearTimeSlot();
    void clearTextSlot();

private:
    Ui::CommentDialog ui;
    static bool lastTimeWasEgt;
};

#endif // COMMENTDIALOG_H
