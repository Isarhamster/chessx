/***************************************************************************
                          tipoftheday.cpp  -  tip of the day window
                             -------------------
    begin                : 27.12.2006
    copyright            : (C) 2006 Heinz R. Hopfgartner 
                           <heinz.hopfgartner@gmx.at>
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "tipoftheday.h"
#include "settings.h"
#include <QTextStream>
#include <QFile>

TipOfDayDialog::TipOfDayDialog(QWidget* parent) : QDialog(parent)
{
  tipDialog.setupUi(this);

  connect(tipDialog.nextTipButton, SIGNAL(clicked()), SLOT(slotNextTip()));
  loadTips();
  tipDialog.tipBrowser->setText(this->readNextTip());
}

TipOfDayDialog::~TipOfDayDialog()
{ 
}

void TipOfDayDialog::accept()
{
  AppSettings->beginGroup("/Tips/");
  AppSettings->writeEntry("showTips", tipDialog.showTipOfDayCheckBox->isChecked());
  AppSettings->endGroup();
  QDialog::accept();
}

void TipOfDayDialog::slotNextTip()
{
 tipDialog.tipBrowser->setText( this->readNextTip());
}

QString TipOfDayDialog::readNextTip()
{
  if(tipsMap.isEmpty())
    return tr("No tip found");
  AppSettings->beginGroup("/Tips/");
  int nextTip = AppSettings->readNumEntry("next", 1);
  if(nextTip == tipsMap.size())
    AppSettings->writeEntry("next", 1);
  else
  AppSettings->writeEntry("next", nextTip + 1);
  AppSettings->endGroup();
  return tipsMap.value(nextTip);
}

int TipOfDayDialog::loadTips()
{
  Q_INIT_RESOURCE(tipoftheday);
  QFile tipsFile(":tips.txt");
  if (!tipsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    return 1;

  QTextStream in(&tipsFile);
  QString line, key, value;
  while (!in.atEnd()) {
    line = in.readLine();
    if(line.left(1) == "#")
      { //skip line
       }
    else {
      key = line.left(3);
      value = line.mid(4, 10000);
      tipsMap.insert(key.toInt(), value);
    }
  }

  tipsFile.close();
  Q_CLEANUP_RESOURCE(tipoftheday);
  return 0;
}
