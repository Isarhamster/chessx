#ifndef CHESSXSETTINGS_H
#define CHESSXSETTINGS_H

#include "settings.h"
#include <QStringList>
#include <QLocale>

#ifdef USE_SPEECH
class QTextToSpeech;
#endif

class Widget;

class ChessXSettings : public Settings
{
public:
    ChessXSettings();
    ChessXSettings(const QString &fileName);

    virtual bool layout(QWidget* w);
    virtual void setLayout(const QWidget* w);

    static QLocale locale(); // Get the locale based upon current settings
#ifdef USE_SPEECH
    static QStringList availableVoices(QString lang); // Get the list of voices based upon selected locale
    static void configureSpeech(QTextToSpeech* speech);
#endif

protected:
    virtual void initWidgetValues(QMap<QString, QVariant>& map) const;
};

#endif // CHESSXSETTINGS_H
