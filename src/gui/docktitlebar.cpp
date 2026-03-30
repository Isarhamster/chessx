#include "docktitlebar.h"
#include "IDockTitleBarExtension.h"
#include <QStyle>
#include <QGraphicsOpacityEffect>

static QToolButton* makeBtn(QWidget* parent,
                            const QIcon& icon,
                            const QString& tip)
{
    auto *b = new QToolButton(parent);

    int h = parent->style()->pixelMetric(
        QStyle::PM_TitleBarHeight);

    int iconSize = h - 8;

    b->setIcon(icon);
    b->setToolTip(tip);
    b->setAutoRaise(true);

    b->setFixedSize(h - 2, h - 2);
    b->setIconSize(QSize(iconSize, iconSize));

    b->setFocusPolicy(Qt::NoFocus);

    return b;
}

DockTitleBar::DockTitleBar(QDockWidget *dock)
    : QWidget(dock), mDock(dock)
{
    mLayout = new QHBoxLayout(this);
    mLayout->setContentsMargins(6,2,6,2);
    mLayout->setSpacing(2);

    mTitle = new QLabel(dock->windowTitle());
    mLayout->addWidget(mTitle);

    connect(dock, &QDockWidget::windowTitleChanged,
            mTitle, &QLabel::setText);

    mLayout->addStretch();

    addDefaultButtons();

    int h = style()->pixelMetric(QStyle::PM_TitleBarHeight);

    setFixedHeight(h);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    setHoverVisible(false);
}

void DockTitleBar::setHoverVisible(bool v)
{
    for (auto *b : mHoverButtons)
    {
        auto eff = qobject_cast<QGraphicsOpacityEffect*>(
            b->graphicsEffect());

        if (eff)
            eff->setOpacity(v ? 1.0 : 0.0);
    }
}

void DockTitleBar::addExtension(IDockTitleBarExtension *ext)
{
    auto buttons = ext->createButtons(mDock);

    int h = style()->pixelMetric(QStyle::PM_TitleBarHeight);
    int iconSize = h - 8;

    for (auto *b : buttons)
    {
        b->setFixedSize(h - 2, h - 2);
        b->setIconSize(QSize(iconSize, iconSize));
        b->setFocusPolicy(Qt::NoFocus);

        mLayout->insertWidget(mLayout->count()-2, b);
        mHoverButtons << b;
    }
 }

void DockTitleBar::addDefaultButtons()
{
    auto *floatBtn = makeBtn(
        this,
        style()->standardIcon(QStyle::SP_TitleBarNormalButton),
        "Detach");

    connect(floatBtn, &QToolButton::clicked,
            this, [this]{
                mDock->setFloating(!mDock->isFloating());
            });

    auto *closeBtn = makeBtn(
        this,
        style()->standardIcon(QStyle::SP_TitleBarCloseButton),
        "Close");

    connect(closeBtn, &QToolButton::clicked,
            mDock, &QDockWidget::close);

    mLayout->addWidget(floatBtn);
    mLayout->addWidget(closeBtn);
}

void DockTitleBar::enterEvent(QEnterEvent *)
{
    for (auto *b : mHoverButtons)
        b->show();
}

void DockTitleBar::leaveEvent(QEvent *)
{
    for (auto *b : mHoverButtons)
        b->hide();
}

void DockTitleBar::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
}
