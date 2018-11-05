/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <QtCore>
#include <QAction>
#include <QMenu>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "actiondialog.h"
#include "kbaction.h"
#include "settings.h"
#include "ui_actiondialog.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

ActionDialog::ActionDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::ActionDialog)
{
    ui->setupUi(this);

    AppSettings->layout(this);

    resetList();

    connect(ui->buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()),
            this, SLOT(resetKeys()));
    connect(ui->actionsTable, SIGNAL(cellClicked(int,int)),
            this, SLOT(saveOldActionText(int,int)));
    connect(ui->actionsTable, SIGNAL(cellChanged(int,int)),
            this, SLOT(validateAction(int,int)));
    connect(ui->keySequenceEdit, SIGNAL(editingFinished()),
            this, SLOT(keySequenceFinished()));
    connect(ui->clearShortcut, SIGNAL(clicked()),
            this, SLOT(clearShortcutText()));
}

ActionDialog::~ActionDialog()
{
    delete ui;
}

void ActionDialog::keySequenceFinished()
{
    int row = ui->actionsTable->currentRow();
    QString newKey = ui->keySequenceEdit->keySequence().toString(QKeySequence::PortableText);
    ui->actionsTable->item(row, 1)->setText(newKey);
}

void ActionDialog::clearShortcutText()
{
    ui->keySequenceEdit->clear();
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

void ActionDialog::reject()
{
    AppSettings->setLayout(this);
    QDialog::reject();
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
    AppSettings->setLayout(this);
    QDialog::accept();
}

void ActionDialog::saveOldActionText(int row, int)
{
    oldAccelText = ui->actionsTable->item(row, 1)->text();
    ui->keySequenceEdit->setKeySequence(QKeySequence(oldAccelText, QKeySequence::PortableText));
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
                    // Duplicate item, restore old text
                    item->setText(oldAccelText);
                    return;
                }
            }
        }
    }
}

void ActionDialog::resetKeys()
{
    emit signalResetKey();
    disconnect(ui->actionsTable, SIGNAL(cellClicked(int,int)),
            this, SLOT(saveOldActionText(int, int)));
    disconnect(ui->actionsTable, SIGNAL(cellChanged(int, int)),
            this, SLOT(validateAction(int, int)));
    resetList();
    connect(ui->actionsTable, SIGNAL(cellClicked(int,int)),
            this, SLOT(saveOldActionText(int, int)));
    connect(ui->actionsTable, SIGNAL(cellChanged(int, int)),
            this, SLOT(validateAction(int, int)));
}
