/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <QtCore>
#include <QAction>
#include <QMenu>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "actiondialog.h"
#include "kbaction.h"
#include "settings.h"
#include "ui_actiondialog.h"

ActionDialog::ActionDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::ActionDialog)
{
    ui->setupUi(this);

    resetList();

    connect(ui->btReset, SIGNAL(clicked()),
            this, SLOT(resetKeys()));
    connect(ui->actionsTable, SIGNAL(cellActivated(int,int)),
            this, SLOT(saveOldActionText(int, int)));
    connect(ui->actionsTable, SIGNAL(cellChanged(int, int)),
            this, SLOT(validateAction(int, int)));
}

ActionDialog::~ActionDialog()
{
    delete ui;
}

void ActionDialog::resetList()
{
    if (!parent())
        return;
    QList<KbAction*> actions = parent()->findChildren<KbAction*>();

    int row = 0;

    ui->actionsTable->setSortingEnabled(false);
    ui->actionsTable->clearContents();
    foreach (KbAction* action, actions)
    {
        QString text = normalizedText(action);
        if (!text.isEmpty())
        {
            connect(this, SIGNAL(signalResetKey()), action, SLOT(resetKey()), Qt::UniqueConnection);
            if (row >= ui->actionsTable->rowCount())
            {
                ui->actionsTable->insertRow(row);
            }
            QTableWidgetItem* firstCol = new QTableWidgetItem(text);
            firstCol->setIcon(action->icon());
            firstCol->setFlags(firstCol->flags() & ~Qt::ItemIsEditable);
            QTableWidgetItem* secondCol = new QTableWidgetItem(action->shortcut().toString(QKeySequence::PortableText));
            secondCol->setFlags(secondCol->flags() | Qt::ItemIsEditable);
            ui->actionsTable->setItem(row, 0, firstCol);
            ui->actionsTable->setItem(row, 1, secondCol);
            ++row;
        }
    }
    ui->actionsTable->resizeColumnsToContents();
    ui->actionsTable->setSortingEnabled(true);

}

QString ActionDialog::normalizedText(const QAction* action) const
{
    QString text;

    const QMenu* parentMenu = qobject_cast<const QMenu*>(action->parent());

    if (parentMenu && !parentMenu->title().isEmpty())
    {
        text = parentMenu->title() + "/" + action->text();
    }
    else
    {
        text = action->text();
    }

    text.remove('&');
    return text;
}

void ActionDialog::accept()
{
    QList<KbAction*> actions = parent()->findChildren<KbAction*>();
    AppSettings->remove("/Keyboard/");
    AppSettings->beginGroup("/Keyboard/");
    foreach (KbAction* action, actions)
    {
        QString text = normalizedText(action);
        for (int i=0;i<ui->actionsTable->rowCount();++i)
        {
            QTableWidgetItem* pItem = ui->actionsTable->item(i, 0);
            if (pItem->text() == text)
            {
                QString newAccel = ui->actionsTable->item(i,1)->text();
                if (newAccel != action->shortcut().toString(QKeySequence::PortableText))
                {
                    action->setShortcut(QKeySequence(newAccel,QKeySequence::PortableText));
                    action->saveKeyShortcut();
                }
            }
        }
    }
    AppSettings->endGroup();
    QDialog::accept();
}

void ActionDialog::saveOldActionText(int row, int column)
{
    oldAccelText = ui->actionsTable->item(row, column)->text();
}

void ActionDialog::validateAction(int row, int column)
{
    QTableWidgetItem* item = ui->actionsTable->item(row, column);
    QString accelText = QString(item->text());
    if (!accelText.isEmpty())
    {
        for (int i=0;i<ui->actionsTable->rowCount();++i)
        {
            QTableWidgetItem* pItem = ui->actionsTable->item(i, column);
            if (pItem != item)
            {
                QString currentAccel = ui->actionsTable->item(i,column)->text();
                if (currentAccel == accelText)
                {
                    item->setText(oldAccelText);
                    return;
                }
            }
        }
    }
    // todo: test valid accel pattern
    // if invalid, oldAccelText contains previous action text
}

void ActionDialog::resetKeys()
{
    emit signalResetKey();
    disconnect(ui->actionsTable, SIGNAL(cellActivated(int,int)),
            this, SLOT(saveOldActionText(int, int)));
    disconnect(ui->actionsTable, SIGNAL(cellChanged(int, int)),
            this, SLOT(validateAction(int, int)));
    resetList();
    connect(ui->actionsTable, SIGNAL(cellActivated(int,int)),
            this, SLOT(saveOldActionText(int, int)));
    connect(ui->actionsTable, SIGNAL(cellChanged(int, int)),
            this, SLOT(validateAction(int, int)));
}
