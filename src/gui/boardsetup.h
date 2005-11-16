/***************************************************************************
                          boardsetup.h  -  Board setup dialog
                             -------------------
    begin                : Mon 14 Nov 2005
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

#ifndef __BOARDSETUPDIALOG_H__
#define __BOARDSETUPDIALOG_H__

#include "boardsetupbase.h"
#include "board.h"

class QPopupMenu;

/**
   The BoardSetupDialog class represents a dialog for setting up a position
 */

class BoardSetupDialog : public BoardSetupBaseDialog
{
  Q_OBJECT
  public:
    /** Create ChessX Preferences dialog */
    BoardSetupDialog(QWidget* parent = 0);
    /** Destroy the dialog */
    ~BoardSetupDialog();
    /** Set current board */
    void setBoard(const Board& b);
    /** Return current board */
    Board board() const;

  public slots:
    /** Execute dialog */
    int exec();
    /** Clear board */
    void slotClear();
    /** Restore standard position */
    void slotReset();
    /** Choose piece */
    void slotChoosePiece(int piece);
    /** Select square and insert piece */
    void slotSelected(const QPoint& pos, ButtonState b);
  private:
    QPopupMenu* m_popup;
    Piece m_piece;
};


#endif
