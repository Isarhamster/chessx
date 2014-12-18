/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "database.h"
#include "game.h"
#include "settings.h"
#include "tagdialog.h"
#include "ui_tagdialog.h"

TagDialog::TagDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagDialog)
{
    ui->setupUi(this);

    AppSettings->layout(this);

    ui->labelInfo->setVisible(false);
}

TagDialog::~TagDialog()
{
    delete ui;
}

void TagDialog::reject()
{
    AppSettings->setLayout(this);
    QDialog::reject();
}

void TagDialog::accept()
{
    AppSettings->setLayout(this);
    QDialog::accept();
}

void TagDialog::saveOldTagText(int row, int column)
{
    oldTagText = ui->tagTable->item(row, column)->text();
}

void TagDialog::slotAddTag()
{
    int row = ui->tagTable->rowCount();
    ui->tagTable->insertRow(row);

    QTableWidgetItem* firstCol = new QTableWidgetItem(tr("Edit tag..."));
    firstCol->setFlags(firstCol->flags() | Qt::ItemIsEditable);
    firstCol->setCheckState(Qt::Checked);

    QTableWidgetItem* secondCol = new QTableWidgetItem(tr("Enter value..."));
    secondCol->setFlags(secondCol->flags() | Qt::ItemIsEditable);
    ui->tagTable->setItem(row, 0, firstCol);
    ui->tagTable->setItem(row, 1, secondCol);
}

void TagDialog::validateTag(int /*row*/, int column)
{
    if (column == 0)
    {
        int n=0;
        for (int i=0;i<ui->tagTable->rowCount();++i)
        {
            QString tag = ui->tagTable->item(i, 0)->text();
            if (!isStandardTag(tag))
            {
                bool removeTag = ui->tagTable->item(i, 0)->checkState() == Qt::Unchecked;
                if (removeTag)
                {
                    ++n;
                }
            }
        }
        if (n>0)
        {
            QString s = tr("Deleting %1 tags").arg(n);
            ui->labelInfo->setText(s);
            ui->labelInfo->setVisible(true);
        }
        else
        {
            ui->labelInfo->setVisible(false);
        }
    }
    ui->tagTable->resizeColumnsToContents();
}

bool TagDialog::editTags(Index* index, Game& game, GameId id)
{
    const QMap<QString, QString>& tags = game.tags();
    int row = 0;
    foreach(QString key, tags.keys())
    {
        if (row >= ui->tagTable->rowCount())
        {
            ui->tagTable->insertRow(row);
        }
        QString text = game.tag(key);
        QTableWidgetItem* firstCol = new QTableWidgetItem(key);
        firstCol->setFlags(firstCol->flags() & ~Qt::ItemIsEditable);
        if (!isStandardTag(key))
        {
            firstCol->setCheckState(Qt::Checked);
        }
        QTableWidgetItem* secondCol = new QTableWidgetItem(text);
        secondCol->setFlags(secondCol->flags() | Qt::ItemIsEditable);
        ui->tagTable->setItem(row, 0, firstCol);
        ui->tagTable->setItem(row, 1, secondCol);
        ++row;
    }

    ui->tagTable->setSortingEnabled(true);
    ui->tagTable->resizeColumnsToContents();

    connect(ui->tagTable, SIGNAL(cellActivated(int,int)),
            this, SLOT(saveOldTagText(int, int)));
    connect(ui->tagTable, SIGNAL(cellChanged(int, int)),
            this, SLOT(validateTag(int, int)));

    int result = exec();

    if (result == QDialog::Accepted)
    {
        for (int i=0;i<ui->tagTable->rowCount();++i)
        {
            QString tag = ui->tagTable->item(i, 0)->text();
            bool removeTag = ui->tagTable->item(i, 0)->checkState() == Qt::Unchecked;
            if (removeTag)
            {
                if (isStandardTag(tag))
                {
                    removeTag = false;
                }
            }
            if (removeTag)
            {
                QString tag = ui->tagTable->item(i, 0)->text();
                game.removeTag(tag);
                index->removeTag(tag,id);
            }
            else if (!tag.endsWith("..."))
            {
                QString value = ui->tagTable->item(i, 1)->text();
                game.setTag(tag,value);
                index->setTag(tag,value,id);
            }
        }
        return true;
    }
    return false;
}
