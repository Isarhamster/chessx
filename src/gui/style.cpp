/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "settings.h"
#include "style.h"

#include <QApplication>
#include <QFile>
#include <QFont>
#include <QProxyStyle>
#include <QStyleFactory>
#include <QWidget>
#include <QtGui/qstylehints.h>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

#include "style.h"

Style::Style() : Style(styleBase()) {}

Style::Style(QStyle *style) : QProxyStyle(style) {}

QStyle *Style::styleBase()
{
    static QStyle *base = 0;
    if (!base)
    {
        QString motif = AppSettings->getValue("/MainWindow/Theme").toString();
        if (motif == "Orange")
        {
            base = QStyleFactory::create(QStringLiteral("fusion"));
            loadStyle(qApp);
        }
        else
        {
            base = QStyleFactory::create(motif);
            if (!base) base = QStyleFactory::create(QStringLiteral("fusion"));
        }
    }
    return base;
}

QStyle *Style::baseStyle() { return styleBase(); }

void Style::modifyPalette(QPalette& palette)
{
    bool isDark = QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
    if (AppSettings->getValue("/MainWindow/DarkTheme").toBool() || isDark)
    {
        palette.setColor(QPalette::Window,QColor(53,53,53));
        palette.setColor(QPalette::WindowText,Qt::white);
        palette.setColor(QPalette::Disabled,QPalette::WindowText,QColor(127,127,127));
        palette.setColor(QPalette::Base,QColor(42,42,42));
        palette.setColor(QPalette::AlternateBase,QColor(66,66,66));
        palette.setColor(QPalette::ToolTipBase,QColor(20,20,20));
        palette.setColor(QPalette::ToolTipText,Qt::yellow);
        palette.setColor(QPalette::Text,Qt::white);
        palette.setColor(QPalette::Disabled, QPalette::Text,QColor(127,127,127));
        palette.setColor(QPalette::Disabled, QPalette::Light, QColor(0, 0, 0, 0));
        palette.setColor(QPalette::Dark,QColor(35,35,35));
        palette.setColor(QPalette::Shadow,QColor(20,20,20));
        palette.setColor(QPalette::Button,QColor(53,53,53));
        palette.setColor(QPalette::ButtonText,Qt::white);
        palette.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(127,127,127));
        palette.setColor(QPalette::BrightText,Qt::red);
        palette.setColor(QPalette::Link,QColor(42,130,218));
        palette.setColor(QPalette::Highlight,QColor(42,130,218));
        palette.setColor(QPalette::Disabled,QPalette::Highlight,QColor(80,80,80));
        palette.setColor(QPalette::HighlightedText,Qt::white);
        palette.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(127,127,127));
    }
    else
    {
        palette.setColor(QPalette::BrightText,Qt::red);
    }
}

void Style::polish(QPalette & /*palette*/)
{
    // Modify Style at runtime -> may cause issues on MacOS
}

void Style::loadStyle(QApplication *app)
{
    QFile orangeStyle(QStringLiteral(":/styles/orange.css"));
    if (orangeStyle.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // set stylesheet
        QString qsStylesheet = QString::fromLatin1(orangeStyle.readAll());
        app->setStyleSheet(qsStylesheet);
        orangeStyle.close();
    }
}

void Style::polish(QApplication *app)
{
    if (!app) return;
}
