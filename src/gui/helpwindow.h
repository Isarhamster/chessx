/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 01/10/2005
    copyright            : (C) 2005 Heinz R. Hopfgartner 
                           <heinz.hopfgartner@gmx.at>
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   The code is based on Trolltech Qt 3 - Help browser example            *
 ***************************************************************************/
/****************************************************************************
 ** $Id$
 **
 ** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
 **
 ** This file is part of an example program for Qt.  This example
 ** program may be used, distributed and modified without limitation.
 **
 *****************************************************************************/

#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <qmainwindow.h>

class QComboBox;
class QPopupMenu;
class QTextBrowser;
class QString;

/** The help system for ChessX. 
    Displays help pages in html format */

class HelpWindow : public QMainWindow
{
  Q_OBJECT
    public:
  HelpWindow();
  ~HelpWindow();

 protected:
  /** Close event is reimplemented to hide the window when the frame is closed */
  void closeEvent( QCloseEvent* );

  private slots:
  void setBackwardAvailable( bool );
  void setForwardAvailable( bool );

  void sourceChanged( const QString& );
  void about();
  void aboutQt();
  void openFile();
  void print();
  void pathSelected( const QString & );

 private:

  void configure();

  QTextBrowser* m_browser;
  QComboBox *m_pathCombo;
  int m_backwardId, m_forwardId;
  QString m_home;

};

#endif
