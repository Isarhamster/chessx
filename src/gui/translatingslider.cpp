/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "translatingslider.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

TranslatingSlider::TranslatingSlider(QWidget *parent) :
    QSlider(parent),
    m_offset(0),
    m_multiplier(1),
    m_start2(INT_MAX),
    m_multiplier2(1)
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
    emit translatedValueChanged(translateValue(value));
}

void TranslatingSlider::setMultiplier2(int multiplier2)
{
    m_multiplier2 = multiplier2;
}

void TranslatingSlider::setStart2(int start2)
{
    m_start2 = start2;
}

void TranslatingSlider::setTranslatedValue(int value)
{
    if (value > m_offset + m_start2*m_multiplier)
    {
        value -= m_offset;
        int v = value - m_start2*m_multiplier;
        setValue(m_start2 + v/m_multiplier2);
    }
    else
    {
        setValue((value-m_offset)/m_multiplier);
    }
}

int TranslatingSlider::translatedValue() const
{
    return translateValue(value());
}

int TranslatingSlider::translateValue(int v) const
{
    if (v<=m_start2)
    {
        return (v*m_multiplier + m_offset);
    }
    else
    {
        return m_offset + m_start2*m_multiplier + ((v-m_start2)*m_multiplier2);
    }
}
