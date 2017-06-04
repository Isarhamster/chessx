/****************************************************************************
* Copyright (C) 2014  Markus Kolb <mkolb@users.sourceforge.net>             *
*                                                                           *
* This program is free software; you can redistribute it and/or             *
* modify it under the terms of the GNU General Public License               *
* as published by the Free Software Foundation; either version 2            *
* of the License, or (at your option) any later version.                    *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with this program; if not, write to the Free Software               *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,                *
* MA 02110-1301, USA.                                                       *
****************************************************************************/

#include "messagedialog.h"
#include "recipientaddressdialog.h"
#include "ui_recipientaddressdialog.h"
#include "settings.h"

#include <QPushButton>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

RecipientAddressDialog::RecipientAddressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecipientAddressDialog)
{
    ui->setupUi(this);
    ui->recipientEMail->setText(AppSettings->value("/Mail/LastRecipient", "").toString());
    restoreLayout();
}

void RecipientAddressDialog::restoreLayout()
{
    AppSettings->layout(this);
}

RecipientAddressDialog::~RecipientAddressDialog()
{
    delete ui;
}

void RecipientAddressDialog::accept()
{
    const QString email = ui->recipientEMail->text();
    const QString mailregex = "([0-9a-zA-Z]([+-_.\\w]*[0-9a-zA-Z])*@([0-9a-zA-Z][-\\w]*[0-9a-zA-Z]\\.)+[a-zA-Z]{2,9})";
    if(email.length() == 0 || (email.indexOf(QRegExp("^" + mailregex + "$")) == -1 && email.indexOf(QRegExp("<" + mailregex + ">")) == -1))
    {
        MessageDialog::warning(tr("This is no valid email address."));
        ui->recipientEMail->setFocus();
        return;
    }
    emailSet = true;
    AppSettings->setValue("/Mail/LastRecipient", email);
    AppSettings->setLayout(this);
    QDialog::accept();
}

void RecipientAddressDialog::reject()
{
    emailSet = false;
    AppSettings->setLayout(this);
    QDialog::reject();
}

bool RecipientAddressDialog::isEmailSet() const
{
    return emailSet;
}

const QString RecipientAddressDialog::getEmail() const
{
    return ui->recipientEMail->text();
}

void RecipientAddressDialog::enableCompleteDatabase(bool enable)
{
    ui->completeDatabase->setEnabled(enable);
}

bool RecipientAddressDialog::completeDatabase() const
{
    return ui->completeDatabase->isChecked();
}
