    /****************************************************************************
*   Copyright (C) 2019 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "annotation.h"

Annotation::Annotation(QString a)
{
    QStringList l = a.trimmed().split(',', Qt::SkipEmptyParts );
    l.removeDuplicates();
    annotation = l.join(',');
}

void Annotation::toggle(const QString& e)
{
    QStringList l = annotation.split(',');
    int n = l.indexOf(e);
    if (n>=0)
    {
        l.removeAt(n);
    }
    else
    {
        l << e.trimmed();
    }
    annotation = l.join(',');
}

void Annotation::removeOne(const QRegularExpression& re)
{
    QStringList l = annotation.split(',');
    int n = l.indexOf(re);
    if (n>=0)
    {
        l.removeAt(n);
    }
    annotation = l.join(',');
}



