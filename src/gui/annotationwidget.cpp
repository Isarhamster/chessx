#include "annotationwidget.h"
#include "ui_annotationwidget.h"
#include "htmlitemdelegate.h"
#include "settings.h"

AnnotationWidget::AnnotationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnnotationWidget)
{
    ui->setupUi(this);

    connect(ui->listVariations, SIGNAL(clicked(QModelIndex)),
            this, SLOT(variationClicked(QModelIndex)));

    HTMLItemDelegate* htmlItemDelegate = new HTMLItemDelegate(this);
    ui->listVariations->setItemDelegate(htmlItemDelegate);
}

AnnotationWidget::~AnnotationWidget()
{
    delete ui;
}

void AnnotationWidget::variationClicked(QModelIndex index)
{
    emit enterVariation(index.row());
}

QString AnnotationWidget::getComment() const
{
    QString s = ui->editComment->toPlainText();
    if (AppSettings->getValue("HTMLComments").toBool())
    {
        s.replace("\n","<br>");
    }
    return s;
}

void AnnotationWidget::setComment(const QString &value)
{
    QString s = value;
    if (AppSettings->getValue("HTMLComments").toBool())
    {
        s.replace("<br>","\n");
    }
    if (s != ui->editComment->toPlainText())
    {
        blockSignals(true);
        ui->editComment->setPlainText(s);
        blockSignals(false);
    }
}

void AnnotationWidget::showVariations(QList<MoveId> /* listVariations */, QStringList textVariations)
{
    ui->listVariations->clear();
    ui->listVariations->addItems(textVariations);
}

void AnnotationWidget::setAnnotationPlaceholder(bool enable)
{
    if (enable)
    {
        ui->editComment->setPlaceholderText(tr("Enter comments and annotations here."));
    }
    else
    {
        ui->editComment->setPlaceholderText("");
    }
}

void AnnotationWidget::on_editComment_textChanged()
{
    emit signalNewAnnotation(getComment());
}

void AnnotationWidget::saveConfig()
{
    AppSettings->setLayout(this);
    AppSettings->layout(ui->annotationsSplitter);
}

void AnnotationWidget::slotReconfigure()
{
    AppSettings->layout(this);
    AppSettings->setLayout(ui->annotationsSplitter);
}
