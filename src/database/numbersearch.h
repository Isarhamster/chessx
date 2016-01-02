/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef NUMBERSEARCH_H
#define NUMBERSEARCH_H

#include "search.h"

/** @ingroup Search
The NumberSearch class is used for game number search.  */

class NumberSearch : public Search
{
    Q_OBJECT

public:
    /** Simple constructor */
    NumberSearch(Database* database, GameId start, GameId end);
    /** Simple constructor. Supports "N1" or "N1-N2" format */
    NumberSearch(Database* database, const QString& value);
    /** Set range to be selected */
    void setRange(GameId start, GameId end);
    /** Set range to be selected. Supports "N1" or "N1-N2" format. */
    void setRange(const QString& value);
    /** Return true if the game at index matches the search */
    virtual int matches(GameId index);
private:
    GameId m_start, m_end;
};


#endif // NUMBERSEARCH_H
