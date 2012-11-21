/****************************************************************************
*   Copyright (C) 2010 by Michal Rudolf <mrudolf@kdewebdev.org>           *
****************************************************************************/

#include "commentdialog.h"

CommentDialog::CommentDialog(QWidget* parent) :
	 QDialog(parent)
{
	ui.setupUi(this);
    connect(ui.clearTime, SIGNAL(clicked()), this, SLOT(clearTimeSlot()));
}

QString CommentDialog::text() const
{
    QString s;
    if (ui.timeEdit->time() != QTime(0,0,0))
    {
        QString format = ui.egtTime->isChecked() ? "[%egt H:mm:ss]" : "[%clk H:mm:ss]";
        s = ui.timeEdit->time().toString(format);
    }
    s.append(ui.textEdit->toPlainText().trimmed());
    return s;
}

void CommentDialog::setText(QString text)
{
    QRegExp egt("\\[\\%egt\\s*(\\d:\\d\\d:\\d\\d)\\]");
    QRegExp clk("\\[\\%clk\\s*(\\d:\\d\\d:\\d\\d)\\]");
    text = text.remove(egt);
    if (egt.captureCount())
    {
        ui.egtTime->setChecked(true);
        ui.timeEdit->setTime(QTime::fromString(egt.capturedTexts().at(0),"H.mm.ss"));
    }
    else
    {
        text = text.remove(clk);
        if (clk.captureCount())
        {
            ui.clkTime->setChecked(true);
            ui.timeEdit->setTime(QTime::fromString(clk.capturedTexts().at(0),"H.mm.ss"));
        }
    }

	ui.textEdit->setPlainText(text);
}

void CommentDialog::clearTimeSlot()
{
    ui.timeEdit->setTime(QTime(0,0,0));
}
