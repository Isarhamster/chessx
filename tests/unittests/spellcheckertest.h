/***************************************************************************
                          spellcheckertests.h  -  description
                             -------------------
    begin                : 15/09/2005
    copyright            : (C) 2005, 2006 Heinz R. Hopfgartner
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
/**
   Test the speellchecker class.
*/


#ifndef __SPELLCHECKERTEST_H
#define __SPELLCHECKERTEST_H

#include <QtTest>

class SpellCheckerTest : public QObject
{

    Q_OBJECT

private slots:
    void testBasics();
    void testBasics_data();
    void testImport();
};


#endif
