/***************************************************************************
 *   (C) 2009 by Michal Rudolf mrudolf@kdewebdev.org                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "mainwindow.h"
#include "version.h"

void MainWindow::slotHelpAbout()
{
	QString fastbits = Board::fastbitsOption ? tr("Compiled with 'fastbits' option<br>") : "";
	QMessageBox dialog(tr(""), tr("<h1>ChessX</h1>"
						"<p>Free chess database available under GPLv2.<br>Version %1<br>%2"
				      "Copyright 2005-2009 ChessX developers"
				      "<p>Current developer and maintainer: Michal Rudolf <a href=\"mailto:mrudolf"
				      "@kdewebdev.org\">&lt;mrudolf"
				      "@kdewebdev.org&gt;</a>"
				      "<p>Additional coding: Marius Roets, Sean Estabrooks, Rico Zenklusen, "
				      "Ejner Borgbjerg, Heinz Hopfgartner, William Hoggarth."
				      "<p>Homepage: <a href=\"http://chessx.sf.net\">http://chessx.sf.net</a><br>"
				      "Mailing list: <a href=\"mailto:chessx-users@lists.sourceforge.net\">"
						"chessx-users@lists.sourceforge.net").arg(ChessXVersion)
				.arg(fastbits),
			   QMessageBox::NoIcon, QMessageBox::Ok, Qt::NoButton, Qt::NoButton, this);
	dialog.exec();
}

