/***************************************************************************
                          Savedialog  -  dialog for saving game
                             -------------------
    begin                : Mon 17 Apr 2006
    copyright            : (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "database.h"
#include "game.h"
#include "messagedialog.h"
#include "partialdate.h"
#include "savedialog.h"
#include "settings.h"

#include <QLineEdit>
#include <QCompleter>

SaveDialog::SaveDialog(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    ui.setupUi(this);
    restoreLayout();
    QButtonGroup* group = new QButtonGroup(this);
    group->addButton(ui.result1Button);
    group->addButton(ui.result5Button);
    group->addButton(ui.result0Button);
    group->addButton(ui.resultNoneButton);
    connect(ui.buttonDiscardChanges, SIGNAL(clicked()), SLOT(discardChanges()));
}

void SaveDialog::restoreLayout()
{
    AppSettings->layout(this);
}

SaveDialog::~SaveDialog()
{}


QString formatTagValue(const QString& s)
{
    return (s.trimmed().isEmpty()) ? "?" : s;
}

QString formatTagDate(const QString& s)
{
    return (s.trimmed().isEmpty()) ?
           PDInvalidDate.asString() :
           PartialDate().fromString(s).asString();
}

int SaveDialog::save(Database* database, Game& game)
{
    QString gameTitle;
    QString baseName = database->name();
    if(baseName.isEmpty())
    {
        baseName = tr("Clipboard");
    }

    if(game.tag(TagNameWhite).isEmpty() && game.tag(TagNameBlack).isEmpty())
    {
        gameTitle = "Save game to ";
    }
    else
    {
        QString name1 = game.tag(TagNameWhite);
        if(name1.isEmpty())
        {
            name1 = "?";
        }
        QString name2 = game.tag(TagNameBlack);
        if(name2.isEmpty())
        {
            name2 = "?";
        }

        gameTitle = QString("Save game '%1 vs. %2' to ").arg(name1).arg(name2);
    }
    gameTitle.append(baseName);

    setWindowTitle(gameTitle);

    ui.whiteEdit->setText(game.tag(TagNameWhite));
    ui.whiteEloEdit->setText(game.tag(TagNameWhiteElo));
    ui.blackEdit->setText(game.tag(TagNameBlack));
    ui.blackEloEdit->setText(game.tag(TagNameBlackElo));
    ui.eventEdit->setText(game.tag(TagNameEvent));
    ui.siteEdit->setText(game.tag(TagNameSite));
    ui.roundEdit->setText(game.tag(TagNameRound));
    ui.dateEdit->setText(game.tag(TagNameDate));
    ui.eventDateEdit->setText(game.tag(TagNameEventDate));
    ui.whiteTeamEdit->setText(game.tag(TagNameWhiteTeam));
    ui.blackTeamEdit->setText(game.tag(TagNameBlackTeam));
    QList<QAbstractButton*> buttons = ui.result1Button->group()->buttons();
    for(int i = 0; i < buttons.count(); ++i)
        if(buttons[i]->text() == game.tag(TagNameResult))
        {
            buttons[i]->setChecked(true);
        }
    // Completion
    setLineEdit(ui.whiteEdit, database, TagNameWhite);
    setLineEdit(ui.blackEdit, database, TagNameBlack);
    setLineEdit(ui.siteEdit,  database, TagNameSite);
    setLineEdit(ui.eventEdit, database, TagNameEvent);
    int result = QDialog::exec();
    if(result == Accepted)
    {
        game.setTag(TagNameWhite, formatTagValue(ui.whiteEdit->text()));
        game.setTag(TagNameBlack, formatTagValue(ui.blackEdit->text()));
        game.setTag(TagNameEvent, formatTagValue(ui.eventEdit->text()));
        game.setTag(TagNameSite, formatTagValue(ui.siteEdit->text()));
        game.setTag(TagNameRound, formatTagValue(ui.roundEdit->text()));
        game.setTag(TagNameDate, formatTagDate(ui.dateEdit->text()));
        game.setTag(TagNameEventDate, formatTagDate(ui.eventDateEdit->text()));
        game.setTag(TagNameResult, ui.result1Button->group()->checkedButton()->text());
        // Optional tag
        if(ui.whiteEloEdit->text().toInt() || game.tag(TagNameWhiteElo).toInt())
        {
            game.setTag(TagNameWhiteElo, ui.whiteEloEdit->text());
        }
        if(ui.blackEloEdit->text().toInt() || game.tag(TagNameBlackElo).toInt())
        {
            game.setTag(TagNameBlackElo, ui.blackEloEdit->text());
        }

        QString t = ui.timeControl->text();
        QString format = "H:mm:ss";
        if(!t.isEmpty())
        {
            game.setTag(TagNameTimeControl, t);
        }

        QTime tt = ui.whiteStartTime->time();
        if(tt.secsTo(QTime(0, 0)) > 0)
        {
            t = tt.toString(format);
            game.setTag(TagNameWhiteClock, t);
        }

        tt = ui.blackStartTime->time();
        if(tt.secsTo(QTime(0, 0)) > 0)
        {
            t = tt.toString(format);
            game.setTag(TagNameBlackClock, t);
        }

        t = ui.whiteTeamEdit->text();
        if(!t.isEmpty())
        {
            game.setTag(TagNameWhiteTeam, t);
        }

        t = ui.blackTeamEdit->text();
        if(!t.isEmpty())
        {
            game.setTag(TagNameBlackTeam, t);
        }
    }
    return result;
}

void SaveDialog::setLineEdit(QLineEdit* edit, Database* database, const QString &tagName)
{
    QStringList words;
    if((tagName == TagNameWhite) || (tagName == TagNameBlack))
    {
        words = database->index()->playerNames();
    }
    else
    {
        words = database->index()->tagValues(tagName);
    }

    QCompleter* completer = new QCompleter(words, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    edit->setCompleter(completer);
}

void SaveDialog::accept()
{
    AppSettings->setLayout(this);
    if((PartialDate().fromString(ui.dateEdit->text()).isValid()) &&
            (PartialDate().fromString(ui.eventDateEdit->text()).isValid()))
    {
        //  dates are formatted properly
        QDialog::accept();
    }
    else
    {
        MessageDialog::error(tr("Dates are not properly formatted!", "Invalid Data"));
    }
}

void SaveDialog::discardChanges()
{
    AppSettings->setLayout(this);
    done(Discard);
}

void SaveDialog::reject()
{
    AppSettings->setLayout(this);
    QDialog::reject();
}
