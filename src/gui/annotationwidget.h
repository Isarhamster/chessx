#ifndef ANNOTATIONWIDGET_H
#define ANNOTATIONWIDGET_H

#include <QList>
#include <QListWidgetItem>
#include <QWidget>
#include "gamecursor.h"

namespace Ui {
class AnnotationWidget;
}

class AnnotationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnnotationWidget(QWidget *parent = nullptr);
    ~AnnotationWidget();

    QString getComment() const;
    void setComment(const QString &value);

signals:
    void signalNewAnnotation(QString);
    void enterVariation(int index);

public slots:
    void showVariations(QList<MoveId>, QStringList);
    void saveConfig();
    void slotReconfigure();
    void setAnnotationPlaceholder(bool enable);

private slots:
    void on_editComment_textChanged();
    void variationClicked(QModelIndex index);

private:
    Ui::AnnotationWidget *ui;
};

#endif // ANNOTATIONWIDGET_H
