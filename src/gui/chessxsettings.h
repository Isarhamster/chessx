#ifndef CHESSXSETTINGS_H
#define CHESSXSETTINGS_H

#include "settings.h"

class Widget;

class ChessXSettings : public Settings
{
public:
    ChessXSettings();
    ChessXSettings(const QString &fileName);

    virtual bool layout(QWidget* w);
    virtual void setLayout(const QWidget* w);

protected:
    virtual void initWidgetValues(QMap<QString, QVariant>& map) const;
};

#endif // CHESSXSETTINGS_H
