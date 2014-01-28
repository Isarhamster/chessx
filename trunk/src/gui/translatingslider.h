/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TRANSLATINGSLIDER_H
#define TRANSLATINGSLIDER_H

#include <QSlider>

class TranslatingSlider : public QSlider
{
    Q_OBJECT
public:
    explicit TranslatingSlider(QWidget *parent = 0);

    int multiplier() const;
    void setMultiplier(int multiplier);

    int offset() const;
    void setOffset(int offset);

signals:
    void translatedValueChanged(int);

public slots:
    void setTranslatedValue(int);

protected slots:
    void slotValueChanged(int);

protected:
    int m_offset;
    int m_multiplier;
};

#endif // TRANSLATINGSLIDER_H
