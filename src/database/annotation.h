/****************************************************************************
*   Copyright (C) 2019 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ANNOTATION_H
#define ANNOTATION_H

#include <QString>
#include <QRegularExpression>

class Annotation : public QObject
{
public:
    Annotation(QString a=QString());
    virtual QRegularExpression filter() const { return QRegularExpression("\\[%([\\)]*)\\s*([^\\]]*)\\]"); };
    virtual QString asAnnotation() const { return ""; };
    virtual QString text() const { return annotation; }
    virtual bool isEmpty() const { return annotation.isEmpty(); }
    virtual void toggle(const QString& e); //!< Append e if it is not already in the existing annotation, otherwise remove it
    virtual void removeOne(const QRegularExpression& re);
protected:
    QString annotation;
};

class VisualAnnotation : public Annotation
{
public:
    VisualAnnotation(QString a=QString()) : Annotation(a) {};
    virtual QRegularExpression filter() const { return QRegularExpression("\\[%(csl|cal)\\s*([^\\]]*)\\]"); };
};

class ArrowAnnotation : public VisualAnnotation
{
public:
    ArrowAnnotation(QString a=QString()) : VisualAnnotation(a) {};
    virtual QRegularExpression filter() const { return QRegularExpression("\\[%(cal)\\s*([^\\]]*)\\]"); };
    virtual QString asAnnotation() const { return annotation.isEmpty() ? QString() : QString("[%cal %1]").arg(annotation); };
};

class SquareAnnotation : public VisualAnnotation
{
public:
    SquareAnnotation(QString a=QString()) : VisualAnnotation(a) {};
    virtual QRegularExpression filter() const { return QRegularExpression("\\[%(csl)\\s*([^\\]]*)\\]"); };
    virtual QString asAnnotation() const { return annotation.isEmpty() ? QString() : QString("[%csl %1]").arg(annotation); };
};

class TimeAnnotation : public Annotation
{
public:
    TimeAnnotation(QString a=QString()) : Annotation(a) {};
    virtual QRegularExpression filter() const { return QRegularExpression("\\[%(egt|emt|clk)\\s*(\\d?\\d:\\d?\\d:\\d\\d(\\.\\d)?)\\]"); };
};

class ClockAnnotation : public TimeAnnotation
{
public:
    ClockAnnotation(QString a=QString()) : TimeAnnotation(a) {};
    virtual QRegularExpression filter() const { return QRegularExpression("\\[%(clk)\\s*(\\d?\\d:\\d?\\d:\\d\\d(\\.\\d)?)\\]"); };
    virtual QString asAnnotation() const { return annotation.isEmpty() ? QString() : QString("[%clk %1]").arg(annotation); };
};

class ElapsedMoveTimeAnnotation : public TimeAnnotation
{
public:
    ElapsedMoveTimeAnnotation(QString a=QString()) : TimeAnnotation(a) {};
    virtual QRegularExpression filter() const { return QRegularExpression("\\[%(emt)\\s*(\\d?\\d:\\d?\\d:\\d\\d(\\.\\d)?)\\]"); };
    virtual QString asAnnotation() const { return annotation.isEmpty() ? QString() : QString("[%emt %1]").arg(annotation); };
};

class ElapsedGameTimeAnnotation : public TimeAnnotation
{
public:
    ElapsedGameTimeAnnotation(QString a=QString()) : TimeAnnotation(a) {};
    virtual QRegularExpression filter() const { return QRegularExpression("\\[%(egt)\\s*(\\d?\\d:\\d?\\d:\\d\\d(\\.\\d)?)\\]"); };
    virtual QString asAnnotation() const { return annotation.isEmpty() ? QString() : QString("[%egt %1]").arg(annotation); };
};

class EvalAnnotation : public Annotation
{
public:
    EvalAnnotation(QString a=QString()) : Annotation(a) {};
    virtual QRegularExpression filter() const { return QRegularExpression("\\[%(eval)\\s*((-?\\d?\\d(\\.\\d\\d?)?)|#\\d\\d?)\\]"); };
    virtual QString asAnnotation() const { return annotation.isEmpty() ? QString() : QString("[%eval %1]").arg(annotation); };
};

#endif // ANNOTATION_H
