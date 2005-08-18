/***************************************************************************
                          preferences  -  Preferences dialog
                             -------------------
    begin                : Thu 18 Aug 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __PREFERENCESDIALOG_H__
#define __PREFERENCESDIALOG_H__

#include "preferencesbase.h"


class PreferencesDialog : public PreferencesDialogBase
{
  Q_OBJECT
public:
  PreferencesDialog(QWidget* parent = 0);
  ~PreferencesDialog();

public slots:
  int exec();
};




#endif

