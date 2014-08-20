/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef SHELLHELPER_H
#define SHELLHELPER_H

#include <QString>

class ShellHelper
{
public:
    ShellHelper();

    static void showInFinder(QString path);
};

#endif // SHELLHELPER_H
