/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef RESULT_H
#define RESULT_H

#include <QString>

enum Result { ResultUnknown, WhiteWin, Draw, BlackWin };

Result ResultFromString(QString res);

/** Returns a string representation of a result */
QString resultString(Result result);

#endif // RESULT_H
