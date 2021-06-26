#ifndef CHESSXSETTINGS_H
#define CHESSXSETTINGS_H

#include "settings.h"

class Widget;

class ChessXSettings : public Settings
{
public:
    ChessXSettings();
    ChessXSettings(const QString &fileName);

    /** Restore widget's layout based on its name. Optionally show window if it is visible.
    @return @p true if the state was restored. */
    bool layout(QWidget* w);
    /** Write widget's layout with its name. */
    void setLayout(const QWidget* w);
};

#endif // CHESSXSETTINGS_H
