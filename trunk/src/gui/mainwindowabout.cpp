/***************************************************************************
 *   (C) 2010 by Michal Rudolf mrudolf@kdewebdev.org                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "aboutdlg.h"
#include "mainwindow.h"

void MainWindow::slotHelpAbout()
{
    AboutDlg dlg;
    dlg.exec();
}

