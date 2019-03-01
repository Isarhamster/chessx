/****************************************************************************
* QTableView with some adjustments like storing header layout               *
* (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>                            *
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "settings.h"
#include "tableview.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QMenu>
#include <QWheelEvent>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

TableView::TableView(QWidget *parent)
    : QTableView(parent),
    m_alignDecoration(Qt::AlignCenter),
    m_posDecoration(QStyleOptionViewItem::Left)
{
    setShowGrid(false);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setWordWrap(false);
    setTextElideMode(Qt::ElideRight);
    verticalHeader()->setDefaultSectionSize(fontMetrics().lineSpacing());
    verticalHeader()->hide();
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->setStretchLastSection(true);
#if QT_VERSION < 0x050000
    horizontalHeader()->setMovable(true);
#else
    horizontalHeader()->setSectionsMovable(true);
#endif
    setTabKeyNavigation(false);
    setContextMenuPolicy(Qt::CustomContextMenu);
    horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(ShowContextMenu(const QPoint&)));
}

TableView::~TableView()
{
}

void TableView::wheelEvent(QWheelEvent* e)
{
    unsigned int button = e->modifiers() & Qt::KeyboardModifierMask;
    if (button == (Qt::ControlModifier|Qt::AltModifier))
    {
        QFont f = font();
        int n = e->delta()>0 ? 1 : -1;
        int fontSize = f.pointSize() + n;
        setFontSize(fontSize);
    }
    QTableView::wheelEvent(e);
}

QStyleOptionViewItem TableView::viewOptions() const
{
    QStyleOptionViewItem option = QTableView::viewOptions();
    option.decorationAlignment = m_alignDecoration;
    option.decorationPosition = m_posDecoration;
    return option;
}

QStyleOptionViewItem::Position TableView::posDecoration() const
{
    return m_posDecoration;
}

void TableView::setPosDecoration(const QStyleOptionViewItem::Position &posDecoration)
{
    m_posDecoration = posDecoration;
}

Qt::Alignment TableView::alignDecoration() const
{
    return m_alignDecoration;
}

void TableView::setAlignDecoration(const Qt::Alignment &alignDecoration)
{
    m_alignDecoration = alignDecoration;
}

void TableView::saveConfig()
{
    if (model())
    {
        AppSettings->setLayout(this);
        AppSettings->beginGroup(objectName());
        int n = model()->columnCount();
        AppSettings->setValue("ColumnCount", n);
        QByteArray visualIndex = horizontalHeader()->saveState();
        AppSettings->setByteArray("VisualIndex", visualIndex);
        AppSettings->endGroup();
    }
}

void TableView::setFontSize(int fontSize)
{
    fontSize = std::max(fontSize, 8);
    QFont f = font();
    f.setPointSize(fontSize);
    setFont(f);

    QFontMetrics fm(f);
    int rowHeight = std::max(minRowHeight(), fm.height()+2);

    QHeaderView *vh = verticalHeader();
#if QT_VERSION < 0x050000
    vh->setResizeMode(QHeaderView::Fixed);
#else
    vh->sectionResizeMode(QHeaderView::Fixed);
#endif
    vh->setDefaultSectionSize(rowHeight);
}

void TableView::slotReconfigure()
{
    bool sortIndicator = horizontalHeader()->isSortIndicatorShown();
    AppSettings->layout(this);
    QString objName = objectName();

    AppSettings->beginGroup(objName);

    int n = model()->columnCount();
    int nRestore = AppSettings->getValue("ColumnCount").toInt();
    if (n==nRestore)
    {
        QByteArray visualIndex = AppSettings->byteArray("VisualIndex");
        horizontalHeader()->restoreState(visualIndex);
    }
    else
    {
        for(int i = 0; i < n; ++i)
        {
            showColumn(i);
        }
        AppSettings->setValue("ColumnCount", n);
        resizeColumnsToContents();
    }
    AppSettings->endGroup();

    int fontSize = AppSettings->getValue("/General/ListFontSize").toInt();
    setFontSize(fontSize);

    horizontalHeader()->setSortIndicatorShown(sortIndicator);
    update();
}

void TableView::ShowContextMenu(const QPoint& pos)
{
    QMenu headerMenu;
    QAction* hide = headerMenu.addAction(tr("Hide Column"));

    QAction* resizeAll = headerMenu.addAction(tr("Resize visible Columns"));
    QAction* showAll = headerMenu.addAction(tr("Show all Columns"));
    headerMenu.addSeparator();
    QAction* copyHTML = headerMenu.addAction(tr("Copy as HTML"));
    QAction* copyImage = headerMenu.addAction(tr("Copy as Image"));

    QAction* selectedItem = headerMenu.exec(mapToGlobal(pos));
    if(selectedItem == hide)
    {
        int column = columnAt(pos.x());
        if(column > 0)
        {
            hideColumn(column);
        }
    }
    else if(selectedItem == showAll)
    {
        for(int i = 0; i < model()->columnCount(); ++i)
        {
            showColumn(i);
        }
    }
    else if (selectedItem == resizeAll)
    {
        resizeColumnsToContents();
    }
    else if (selectedItem == copyHTML)
    {
        QString html = renderToHTML();
        QApplication::clipboard()->setText(html);
    }
    else if (selectedItem == copyImage)
    {
        QImage img = renderToImage();
        QApplication::clipboard()->setImage(img);
    }
}

QString TableView::renderHTMLitem(int row,int column) const
{
    QString result = "<td>";
    QVariant data = model()->data(model()->index(row, column));
    QString s = data.toString();
    result += s;
    result += "</td>";
    return result;
}

QString TableView::renderHTMLrow(int row) const
{
    QString result = "<tr>\n";
    int columns = model()->columnCount();
    for (int i=0; i<columns; ++i)
    {
        result += renderHTMLitem(row,i);
    }
    result += "</tr>\n";
    return result;
}

QString TableView::renderHeaderToHTML() const
{
    QString result = "<tr>\n";
    int columns = model()->columnCount();
    for (int i=0; i<columns; ++i)
    {
        result += "<th>";
        result += horizontalHeader()->model()->headerData(i, Qt::Horizontal).toString();
        result += "</th>";
    }
    result += "</tr>\n";
    return result;
}

QString TableView::renderToHTML() const
{
    QString result = "<table border='1'>\n";
    result += renderHeaderToHTML();
    int rows = model()->rowCount();
    for (int i=0; i<rows; ++i)
    {
        result += renderHTMLrow(i);
    }
    result += "</table>\n";
    return result;
}

QImage TableView::renderToImage() const
{
    QTableView* pTableView = new QTableView;
    pTableView->setModel(model());

    int width = 0;
    int height = 0;
    int columns = model()->columnCount();
    int rows = model()->rowCount();

    pTableView->resizeColumnsToContents();

    for( int i = 0; i < columns; ++i ) {
        width += pTableView->columnWidth(i);
    }

    for( int i = 0; i < rows; ++i ) {
        height += pTableView->rowHeight(i);
    }

    height += horizontalHeader()->height();

    pTableView->setFixedSize(width, height);
    pTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPixmap pixmap(width, height);
    pixmap.fill();

    pTableView->render(&pixmap);
    QImage image = pixmap.toImage();

    delete pTableView;
    return image;
}
