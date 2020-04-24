/****************************************************************************
*   Copyright (C) 2010 by Michal Rudolf <mrudolf@kdewebdev.org>             *
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "annotation.h"
#include "commentdialog.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

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
        QString format = "[%emt %1]";
        if (ui.egtTime->isChecked()) format = "[%egt %1]";
        else if (ui.clkTime->isChecked()) format = "[%clk %1]";
        QString t = ui.timeEdit->time().toString("H:mm:ss");
        if(!t.isEmpty())
        {
            s = format.arg(t);
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
    text.remove(QRegExp(s_can));
    QRegExp tan(s_tan);
    int pos = tan.indexIn(text);
    if(pos >= 0)
    {
        QString w = tan.cap(1);
        QString t = tan.cap(2);
        text = text.remove(tan);
        ui.timeEdit->setTime(QTime::fromString(t.trimmed(), "H:mm:ss"));
        if (w=="egt")
        {
            ui.egtTime->setChecked(true);
            lastTimeMode = Egt;
        }
        else if (w=="clk")
        {
            ui.clkTime->setChecked(true);
            lastTimeMode = Clk;
        }
        else if (w=="emt")
        {
            ui.emtTime->setChecked(true);
            lastTimeMode = Emt;
        }
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

void CommentDialog::accept()
{
    QString s1 = ui.textEdit->toPlainText();
    if(s1.contains('}'))
    {
        int n = s1.indexOf('}');
        QTextCursor cursor(ui.textEdit->document());
        QTextCharFormat fmt = cursor.charFormat();
        fmt.setBackground(Qt::red);
        cursor.setPosition(n, QTextCursor::MoveAnchor);
        cursor.setPosition(n+1, QTextCursor::KeepAnchor);
        cursor.setCharFormat(fmt);
        return;
    }
    QDialog::accept();
}
