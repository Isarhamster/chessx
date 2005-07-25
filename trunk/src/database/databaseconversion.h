/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 28/05/2005
    copyright            : (C) 2005 Ejner Borgbjerg 
                           <ejner@users.sourceforge.net>
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
DatabaseConversion contains utility functions to convert from various formats
into ChessX format.
*/

#ifndef __DATABASECONVERSION_H__
#define __DATABASECONVERSION_H__

#include "playerdatabase.h"

class DatabaseConversion{

public:
/**
convert Scid ratings.ssp to player database
outFileName is filename without extension
inFileName is full path for ratings.ssp
*/
static bool playerDatabaseFromScidRatings(const QString& inFileName, const QString& outFileName, const QString& photoFile);

};

#endif
