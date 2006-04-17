/***************************************************************************
                          Savedialog  -  dialog for saving game
                             -------------------
    begin                : Mon 17 Apr 2006
    copyright            : (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef __SAVEDIALOG_H__
#define __SAVEDIALOG_H__

#include "savedialogbase.h"

class Game;

class SaveDialog : public SaveDialogBase
{
  Q_OBJECT
  public:
    SaveDialog(QWidget* parent = 0);
    ~SaveDialog();
    virtual int exec(Game* game);
};


#endif

