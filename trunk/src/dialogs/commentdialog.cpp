/****************************************************************************
*   Copyright (C) 2010 by Michal Rudolf <mrudolf@kdewebdev.org>             *
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "commentdialog.h"

CommentDialog::TimeMode CommentDialog::lastTimeMode = CommentDialog::Egt;

CommentDialog::CommentDialog(QWidget* parent, Qt::WindowFlags f) :
    QDialog(parent, f)
{
    ui.setupUi(this);
    connect(ui.clearTime, SIGNAL(clicked()), this, SLOT(clearTimeSlot()));
    connect(ui.clearText, SIGNAL(clicked()), this, SLOT(clearTextSlot()));
}

QString CommentDialog::text() const
{
    QString s;
    if(ui.timeEdit->time() != QTime(0, 0, 0))
    {
        QString s2 = "[%emt %1]";
        if (ui.egtTime->isChecked()) s2 = "[%egt %1]";
        else if (ui.clkTime->isChecked()) s2 = "[%clk %1]";
        QString t = ui.timeEdit->time().toString("H:mm:ss");
        if(!t.isEmpty())
        {
            s = s2.arg(t);
            if (ui.egtTime->isChecked()) lastTimeMode = Egt;
            else if (ui.clkTime->isChecked()) lastTimeMode = Clk;
            else if (ui.emtTime->isChecked()) lastTimeMode = Emt;
        }
    }
    QString s1 = ui.textEdit->toPlainText().trimmed();
    if(!s.isEmpty() && !s1.isEmpty())
    {
        s.append(" ");
    }
    s.append(s1);
    return s;
}

void CommentDialog::setText(QString text)
{
    QRegExp egt("\\[%egt\\s*(\\d:\\d\\d:\\d\\d)\\]");
    QRegExp clk("\\[%clk\\s*(\\d:\\d\\d:\\d\\d)\\]");
    QRegExp emt("\\[%emt\\s*(\\d:\\d\\d:\\d\\d)\\]");
    int pos = egt.indexIn(text);
    if(pos >= 0)
    {
        ui.egtTime->setChecked(true);
        QString segt = egt.cap(1);
        text = text.remove(egt);
        ui.timeEdit->setTime(QTime::fromString(segt.trimmed(), "H:mm:ss"));
        lastTimeMode = Egt;
    }
    else
    {
        pos = clk.indexIn(text);
        if(pos >= 0)
        {
            ui.clkTime->setChecked(true);
            QString sclk = clk.cap(1);
            text = text.remove(clk);
            ui.timeEdit->setTime(QTime::fromString(sclk.trimmed(), "H:mm:ss"));
            lastTimeMode = Clk;
        }
        else
        {
            pos = emt.indexIn(text);
            if(pos >= 0)
            {
                ui.emtTime->setChecked(true);
                QString semt = emt.cap(1);
                text = text.remove(emt);
                ui.timeEdit->setTime(QTime::fromString(semt.trimmed(), "H:mm:ss"));
                lastTimeMode = Emt;
            }
            else
            {
                switch(lastTimeMode)
                {
                case Egt: ui.egtTime->setChecked(true); break;
                case Clk: ui.clkTime->setChecked(true); break;
                case Emt: ui.emtTime->setChecked(true); break;
                }
            }
        }
    }

    ui.textEdit->setPlainText(text);
}

void CommentDialog::clearTimeSlot()
{
    ui.timeEdit->setTime(QTime(0, 0, 0));
}

void CommentDialog::clearTextSlot()
{
    ui.textEdit->setPlainText("");
}
