#include "annotationtimeedit.h"

AnnotationTimeEdit::AnnotationTimeEdit(QWidget *parent) : QTimeEdit(parent)
{

}

void AnnotationTimeEdit::stepBy(int steps)
{
    if (currentSection() == QDateTimeEdit::MSecSection)
    {
        steps *= 100;
        if ((steps >0) && (sectionText(QDateTimeEdit::MSecSection)=="9"))
        {
            steps = 0;
        }
    }
    QTimeEdit::stepBy(steps);
}
