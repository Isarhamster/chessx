#ifndef ANNOTATIONTIMEEDIT_H
#define ANNOTATIONTIMEEDIT_H

#include <QTimeEdit>

class AnnotationTimeEdit : public QTimeEdit
{
    Q_OBJECT
public:
    explicit AnnotationTimeEdit(QWidget *parent = nullptr);

private:
    void stepBy(int steps);

signals:

};

#endif // ANNOTATIONTIMEEDIT_H
