/***************************************************************************
	MessageDialog - wrapper for QMessageBox
	(C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>

	This file is a part of free ChessX (http://chessx.sourceforge.net

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
  ***************************************************************************/
#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

/** @ingroup GUI
	The MessageDialog class is a simple wrapper for QMessageBox, to make
	it easier to display system messages.
 */

#include <QObject>

class MessageDialog : public QObject
{
    Q_OBJECT
public:
    enum {Yes, No, Cancel};
    /** Standard information dialog. */
    static void error(const QString& text, const QString& title = tr("Error"));
    /** Standard warning dialog. */
    static void warning(const QString& text, const QString& title = tr("Warning"));
    /** Standard information dialog. */
    static void information(const QString& text, const QString& title = tr("Information"));
    /** Standard Yes-No dialog. Escape is disabled, 'No' is default. */
    static bool yesNo(const QString& text, const QString& title = tr("Question"),
                      const QString& yes = tr("&Yes"), const QString& no = tr("&No"));
    /** Standard Ok-Cancel dialog. Escape cancels, 'Ok' is default. */
    static bool okCancel(const QString& text, const QString& title = tr("Question"),
                         const QString& ok = tr("&OK"), const QString& cancel = tr("&Cancel"));
    static int yesNoCancel(const QString& text, const QString& title = tr("Question"),
                           const QString& yes = tr("&Yes"), const QString& no = tr("&No"),
                           const QString& cancel = tr("&Cancel"));
};

#endif
