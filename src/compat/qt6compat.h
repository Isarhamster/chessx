#ifndef QT6COMPAT_H
#define QT6COMPAT_H

#include <QtGlobal>

#ifdef USE_SOUND
#if QT_VERSION < 0x060000
#include <QSound>
#else
#include <QSoundEffect>
class QSound : public QSoundEffect {
public:
    QSound(const QString& url) : QSoundEffect()
    {
        setSource(QUrl(url));
    }
    void play()
    {
        QSoundEffect::play();
    }
    static void play(const QString& url)
    {
        static QSoundEffect effect;
        effect.setSource(QUrl(url));
        effect.play();
    }
};
#endif
#endif

#if QT_VERSION < 0x060000
#include <QTextCodec>
#define SET_CODEC_UTF8(s) \
    QTextCodec* textCodec = QTextCodec::codecForName("ISO 8859-1"); \
    if(textCodec) s.setCodec(textCodec);
#define SET_CODEC_LATIN1(s) \
    QTextCodec* textCodec = QTextCodec::codecForName("utf8"); \
    if(textCodec) s.setCodec(textCodec);
#else
#include <QStringConverter>
#define SET_CODEC_UTF8(s)   s.setEncoding(QStringConverter::Utf8);
#define SET_CODEC_LATIN1(s) s.setEncoding(QStringConverter::Latin1);
#endif

#endif // QT6COMPAT_H
