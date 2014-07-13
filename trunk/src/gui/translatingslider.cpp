/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "translatingslider.h"

TranslatingSlider::TranslatingSlider(QWidget *parent) :
    QSlider(parent),
    m_offset(0),
    m_multiplier(1)
{
    connect(this, SIGNAL(valueChanged(int)), SLOT(slotValueChanged(int)));
}

int TranslatingSlider::multiplier() const
{
    return m_multiplier;
}

void TranslatingSlider::setMultiplier(int multiplier)
{
    m_multiplier = multiplier;
}

int TranslatingSlider::offset() const
{
    return m_offset;
}

void TranslatingSlider::setOffset(int offset)
{
    m_offset = offset;
}

void TranslatingSlider::slotValueChanged(int value)
{
    emit translatedValueChanged(value*m_multiplier + m_offset);
}

void TranslatingSlider::setTranslatedValue(int value)
{
    setValue((value-m_offset)/m_multiplier);
}

int TranslatingSlider::translatedValue() const
{
    int v = value();
    return (v*m_multiplier + m_offset);
}
