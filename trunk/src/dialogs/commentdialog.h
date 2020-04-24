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
    enum TimeMode { Egt, Emt, Clk};

    CommentDialog(QWidget *parent = nullptr, Qt::WindowFlags f = nullptr);
    void setText(QString text);
    QString text() const;

protected slots:
    void clearTimeSlot();
    void clearTextSlot();

private:
    void accept();
    Ui::CommentDialog ui;
    static enum TimeMode lastTimeMode;
};

#endif // COMMENTDIALOG_H
