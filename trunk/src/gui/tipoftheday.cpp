/***************************************************************************
                          tipoftheday.cpp  -  tip of the day window
                             -------------------
    begin                : 27.12.2006
    copyright            : (C) 2006 Heinz R. Hopfgartner <heinz.hopfgartner@gmx.at>
                           (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>
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
#include <QFile>
#include <QTextStream>

TipOfDayDialog::TipOfDayDialog(QWidget* parent) : QDialog(parent)
{
  ui.setupUi(this);

  connect(ui.nextButton, SIGNAL(clicked()), SLOT(slotNextTip()));
  connect(ui.previousButton, SIGNAL(clicked()), SLOT(slotPreviousTip()));
  connect(this, SIGNAL(finished(int)), SLOT(slotSaveConfiguration()));
  loadTips();
}

TipOfDayDialog::~TipOfDayDialog()
{
}

void TipOfDayDialog::slotNextTip()
{
  setCurrentTip(m_current + 1);
  ui.tipBrowser->setText(currentTip());
}

void TipOfDayDialog::slotPreviousTip()
{
  setCurrentTip(m_current - 1);
  ui.tipBrowser->setText(currentTip());
}

bool TipOfDayDialog::loadTips()
{
  Q_INIT_RESOURCE(tipoftheday);
  QFile tipsFile(":tips.txt");
  if (!tipsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  QTextStream tips(&tipsFile);
  QString tip, s;
  while (!tips.atEnd())
  {
    s = tips.readLine().trimmed();
    if (!s.isEmpty())
      tip += s + ' ';
    else
    {
      if (!tip.isEmpty())
        m_tips.append(tip);
      tip = QString();
    }
  }
  if (!tip.isEmpty())
    m_tips.append(tip);
  tipsFile.close();
  Q_CLEANUP_RESOURCE(tipoftheday);
  return true;
}

QString TipOfDayDialog::currentTip() const
{
  if (m_current >= m_tips.count())
    return tr("No tips available!");
  return m_tips[m_current];
}

void TipOfDayDialog::setCurrentTip(int tip)
{
  if (!m_tips.count() || tip >= m_tips.count())
    m_current = 0;
  else if (tip < 0)
    m_current = m_tips.count() - 1;
  else m_current = tip;
}

void TipOfDayDialog::show()
{
  AppSettings->beginGroup("/Tips/");
  ui.startupCheck->setChecked(AppSettings->value("showTips", true).toBool());
  setCurrentTip(AppSettings->value("next", 0).toInt());
  AppSettings->endGroup();
  ui.tipBrowser->setText(currentTip());
  QDialog::show();
}

void TipOfDayDialog::slotSaveConfiguration()
{
  AppSettings->beginGroup("/Tips/");
  AppSettings->setValue("showTips", ui.startupCheck->isChecked());
  setCurrentTip(m_current + 1);
  AppSettings->setValue("next", m_current);
  AppSettings->endGroup();
}
