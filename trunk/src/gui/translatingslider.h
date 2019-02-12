/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TRANSLATINGSLIDER_H
#define TRANSLATINGSLIDER_H

#include <QSlider>

/** Slider with three segments of linear translation of visual values and its target value */
class TranslatingSlider : public QSlider
{
    Q_OBJECT
public:
    explicit TranslatingSlider(QWidget *parent = 0);

    int multiplier() const;
    void setMultiplier(int multiplier);

    int offset() const;
    void setOffset(int offset);

    int translatedValue() const;

    void setStart2(int start2);
    void setMultiplier2(int multiplier2);
    void setStart3(int start3);
    void setMultiplier3(int multiplier3);

signals:
    void translatedValueChanged(int);

public slots:
    void setTranslatedValue(int);

protected slots:
    void slotValueChanged(int);

protected:
    int m_offset;
    int m_multiplier;
    int m_start2;
    int m_multiplier2;
    int m_start3;
    int m_multiplier3;
protected:
    int translateValue(int v) const;
};

#endif // TRANSLATINGSLIDER_H
