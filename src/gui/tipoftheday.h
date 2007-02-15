/***************************************************************************
                          tipoftheday.h  -  tip of the day window
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


#ifndef __TIPOFTHEDAY_H__
#define __TIPOFTHEDAY__

#include "ui_tipoftheday.h"
#include <QMap>

/** @ingroup GUI 
The TipOfDayDialog class is a dialog to shows tips. */
class TipOfDayDialog: public QDialog
{
  Q_OBJECT
public:
  TipOfDayDialog(QWidget* parent = 0);
  ~TipOfDayDialog();

public slots:

  void accept();
  void slotNextTip();

private:

  Ui::TipOfDayDialog tipDialog;
  QMap<int, QString> tipsMap;
  
  QString readNextTip();
  /** Loads all tips into memory */
  int loadTips();

};

#endif
