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

#include <QComboBox>
#include <QLineEdit>

QuickSearchDialog::QuickSearchDialog(QWidget* parent) : QDialog(parent)
{
  ui.setupUi(this);

  connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

QuickSearchDialog::~QuickSearchDialog()
{
}

void QuickSearchDialog::setTag(int tag)
{
  ui.tagCombo->setCurrentIndex(tag);
}

int QuickSearchDialog::tag() const
{
  return ui.tagCombo->currentIndex();
}

QString QuickSearchDialog::value() const
{
  return ui.valueEdit->text();
}

int QuickSearchDialog::exec()
{
  ui.valueEdit->setFocus();
  return QDialog::exec();
}









