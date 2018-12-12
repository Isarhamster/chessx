/***************************************************************************
                          quicksearch.cpp  -  Quick Tag search dialog
                             -------------------
    begin                : Sun 6 May 2007
    copyright            : (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "quicksearch.h"
#include "filter.h"
#include "settings.h"

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMenu>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

QuickSearchDialog::QuickSearchDialog(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    ui.setupUi(this);

    ui.modeCombo->addItem(tr("Find in current filter"), FilterOperator::And);
    ui.modeCombo->addItem(tr("Search whole database"), FilterOperator::NullOperator);
    ui.modeCombo->addItem(tr("Add to current filter"), FilterOperator::Or);
    ui.modeCombo->addItem(tr("Remove from current filter"), FilterOperator::Remove);

    ui.buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Find"));
    connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));

    QMenu *menu = new QMenu();
    QAction *actionFront = new QAction(tr("Match tags from beginning"), menu);
    actionFront->setCheckable(true);
    QAction *actionTail = new QAction(tr("Match to end of tags"), menu);
    actionTail->setCheckable(true);
    menu->addAction(actionFront);
    menu->addAction(actionTail);

    ui.optionBox->setMenu(menu);

    AppSettings->layout(this);
}

QuickSearchDialog::~QuickSearchDialog()
{
}

void QuickSearchDialog::setTag(int tag)
{
    ui.tagCombo->setCurrentIndex(tag);
}

void QuickSearchDialog::addTag(QString tag)
{
    ui.tagCombo->addItem(tag);
}

int QuickSearchDialog::tag() const
{
    return ui.tagCombo->currentIndex();
}

QString QuickSearchDialog::value() const
{
    QString s = ui.valueEdit->text();
    if (!s.isEmpty() && tag()!=0 && tag()!=9 && tag()!=11)
    {
        QMenu* menu = ui.optionBox->menu();
        if (menu->actions().at(0)->isChecked())
        {
            s.prepend("^");
        }
        if (menu->actions().at(1)->isChecked())
        {
            s.append("$");
        }
    }
    return s;
}

int QuickSearchDialog::exec()
{
    ui.valueEdit->setFocus();
    return QDialog::exec();
}

int QuickSearchDialog::mode() const
{
    if(ui.modeCombo->currentIndex() == -1)
    {
        return 0;
    }
    return ui.modeCombo->itemData(ui.modeCombo->currentIndex()).toInt();
}

void QuickSearchDialog::setMode(int index)
{
    ui.modeCombo->setCurrentIndex(index);
}

void QuickSearchDialog::accept()
{
    AppSettings->setLayout(this);
    QDialog::accept();
}

void QuickSearchDialog::reject()
{
    AppSettings->setLayout(this);
    QDialog::reject();
}

void QuickSearchDialog::buttonClicked(QAbstractButton* button)
{
    QDialogButtonBox::StandardButton sb = ui.buttonBox->standardButton(button);
    switch(sb)
    {
    case QDialogButtonBox::Ok:
        accept();
        break;
    case QDialogButtonBox::Cancel:
        reject();
        break;
    default:
        break;
    }
}
