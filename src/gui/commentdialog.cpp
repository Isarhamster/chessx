/****************************************************************************
*   Copyright (C) 2010 by Michal Rudolf <mrudolf@kdewebdev.org>           *
****************************************************************************/

#include "commentdialog.h"

bool CommentDialog::lastTimeWasEgt = false;

CommentDialog::CommentDialog(QWidget* parent) :
	 QDialog(parent)
{
	ui.setupUi(this);
    connect(ui.clearTime, SIGNAL(clicked()), this, SLOT(clearTimeSlot()));
    connect(ui.clearText, SIGNAL(clicked()), this, SLOT(clearTextSlot()));
}

QString CommentDialog::text() const
{
    QString s;
    if (ui.timeEdit->time() != QTime(0,0,0))
    {
        QString format = ui.egtTime->isChecked() ? "[%egt H:mm:ss]" : "[%clk H:mm:ss]";
        s = ui.timeEdit->time().toString(format);
        if (!s.isEmpty())
        {
            lastTimeWasEgt = ui.egtTime->isChecked();
        }
    }
    QString s1 = ui.textEdit->toPlainText().trimmed();
    if (!s1.isEmpty())
    {
        s.append(" ");
        s.append(s1);
    }
    return s;
}

void CommentDialog::setText(QString text)
{
    QRegExp egt("\\[%egt\\s*(\\d:\\d\\d:\\d\\d)\\]");
    QRegExp clk("\\[%clk\\s*(\\d:\\d\\d:\\d\\d)\\]");
    int pos = egt.indexIn(text);
    if (pos>=0)
    {
        ui.egtTime->setChecked(true);
        QString segt = egt.cap(1);
        text = text.remove(egt);
        ui.timeEdit->setTime(QTime::fromString(segt,"H:mm:ss"));
        lastTimeWasEgt = true;
    }
    else
    {
        int pos = clk.indexIn(text);
        if (pos>=0)
        {
            ui.clkTime->setChecked(true);
            QString sclk = clk.cap(1);
            text = text.remove(clk);
            ui.timeEdit->setTime(QTime::fromString(sclk,"H:mm:ss"));
            lastTimeWasEgt = false;
        }
        else
        {
            if (lastTimeWasEgt)
            {
                // Setup GUI with the last value we had in this dialog
                // That may be a bad guess, as this could come from a different game
                ui.egtTime->setChecked(true);
            }
        }
    }

    ui.textEdit->setPlainText(text);
}

void CommentDialog::clearTimeSlot()
{
    ui.timeEdit->setTime(QTime(0,0,0));
}

void CommentDialog::clearTextSlot()
{
    ui.textEdit->setPlainText("");
}
