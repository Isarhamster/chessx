/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <QtCore>

#ifndef SHELLHELPER_H
#define SHELLHELPER_H

class ShellHelper
{
public:
    ShellHelper();

    static void showInFinder(QString path);
};

#endif // SHELLHELPER_H
