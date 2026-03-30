#ifndef IDOCKTITLEBAREXTENSION_H
#define IDOCKTITLEBAREXTENSION_H

#pragma once

#include <QObject>
#include <QList>

class QToolButton;
class QDockWidget;

class IDockTitleBarExtension
{
public:
    virtual ~IDockTitleBarExtension() = default;

    // Create buttons for this dock
    virtual QList<QToolButton*> createButtons(QDockWidget *dock) = 0;
};

#endif // IDOCKTITLEBAREXTENSION_H
