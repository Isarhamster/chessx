#include "duplicatepositionswidget.h"
#include <QPushButton>
#include <QLayout>
#include <QTreeView>
#include "settings.h"

QBrush const darkModeWarningBackground {Qt::darkRed};
// A light red
QBrush const lightModeWarningBackground {QColor{0xff, 0xb6, 0xc1}};

DuplicatePositionItem::DuplicatePositionItem(QString const & datum, DuplicatedPosition::WarningLevel warningLevel,
        DuplicatePositionItem * parent)
    : warning{warningLevel}, m_itemDatum{datum}, m_parentItem{parent}, m_move{CURRENT_MOVE}
{
    if (m_parentItem)
    {
        m_parentItem->appendChild(this);
    }
}

DuplicatePositionItem::DuplicatePositionItem(QString const & datum, MoveId move, DuplicatePositionItem * parent)
    : warning{DuplicatedPosition::WarningLevel::None}, m_itemDatum{datum}, m_parentItem{parent}, m_move{move}
{
    if (m_parentItem)
    {
        m_parentItem->appendChild(this);
    }
}

DuplicatePositionItem::~DuplicatePositionItem()
{
    deleteChildren();
}

MoveId DuplicatePositionItem::move() const
{
    return m_move;
}

void DuplicatePositionItem::appendChild(DuplicatePositionItem * item)
{
    m_childItems.append(item);
}

DuplicatePositionItem * DuplicatePositionItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
    {
        return nullptr;
    }
    return m_childItems.at(row);
}

int DuplicatePositionItem::childCount() const
{
    return m_childItems.count();
}

int DuplicatePositionItem::row() const
{
    if (m_parentItem)
    {
        return m_parentItem->m_childItems.indexOf(const_cast<DuplicatePositionItem*>(this));
    }
    return 0;
}

QString DuplicatePositionItem::datum() const
{
    return m_itemDatum;
}

DuplicatePositionItem * DuplicatePositionItem::parentItem()
{
    return m_parentItem;
}

void DuplicatePositionItem::deleteChildren() noexcept
{
    qDeleteAll(m_childItems);
    m_childItems.clear();
}

DuplicatePositionModel::DuplicatePositionModel(QObject * parent)
    : QAbstractItemModel(parent)
{
    rootItem = new DuplicatePositionItem("Duplicate Positions", ROOT_NODE);
}

DuplicatePositionModel::~DuplicatePositionModel()
{
    delete rootItem;
}

QModelIndex DuplicatePositionModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return {};
    }
    DuplicatePositionItem * parentItem {nullptr};
    if (!parent.isValid())
    {
        parentItem = rootItem;
    }
    else
    {
        parentItem = static_cast<DuplicatePositionItem*>(parent.internalPointer());
    }
    DuplicatePositionItem * childItem {parentItem->child(row)};
    if (!childItem)
    {
        return {};
    }
    return createIndex(row, column, childItem);
}

QModelIndex DuplicatePositionModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return {};
    }
    DuplicatePositionItem * childItem = static_cast<DuplicatePositionItem*>(index.internalPointer());
    DuplicatePositionItem * parentItem = childItem->parentItem();
    if (parentItem == rootItem)
    {
        return {};
    }
    return createIndex(parentItem->row(), 0, parentItem);
}

int DuplicatePositionModel::rowCount(const QModelIndex &parentIndex) const
{
    DuplicatePositionItem * parentItem;
    if (parentIndex.column() > 0)
    {
        return 0;
    }
    if (!parentIndex.isValid())
    {
        parentItem = rootItem;
    }
    else
    {
        parentItem = static_cast<DuplicatePositionItem*>(parentIndex.internalPointer());
    }
    return parentItem->childCount();
}

int DuplicatePositionModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant DuplicatePositionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.column() != 0)
    {
        return {};
    }
    if (role != Qt::DisplayRole && role != Qt::BackgroundRole)
    {
        return {};
    }
    DuplicatePositionItem * item = static_cast<DuplicatePositionItem*>(index.internalPointer());
    if (role == Qt::DisplayRole)
    {
        return item->datum();
    }
    // Return a background setting
    if (item->warning == DuplicatedPosition::WarningLevel::None)
    {
        return {};
    }
    if (AppSettings->getValue("/MainWindow/DarkTheme").toBool())
    {
        return darkModeWarningBackground;
    }
    else
    {
        return lightModeWarningBackground;
    }
}

std::optional<MoveId> DuplicatePositionModel::getLink(const QModelIndex &index) const
{
    if (!index.isValid() || index.column() != 0)
    {
        return {};
    }
    DuplicatePositionItem * item {static_cast<DuplicatePositionItem*>(index.internalPointer())};
    return item->move();
}

Qt::ItemFlags DuplicatePositionModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }
    return QAbstractItemModel::flags(index);
}

QVariant DuplicatePositionModel::headerData(int /*section*/, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    {
        return {};
    }
    return rootItem->datum();
}

void DuplicatePositionModel::setData(std::vector<DuplicatedPosition> const & duplicates, GameX const & rawGame) noexcept
{
    beginResetModel();
    if (!rootItem)
    {
        // A terrible internal error
        return;
    }
    rootItem->deleteChildren();
    GameX game {rawGame};
    QVector<DuplicatePositionItem*> parents;
    for (DuplicatedPosition const & dupe : duplicates)
    {
        // This item repsents one position that has duplicates
        DuplicatePositionItem * duplicatePosition{new DuplicatePositionItem{dupe.fen, dupe.warning, rootItem}};
        for (DuplicateMoveList const & ml : dupe.moveLists)
        {
            game.moveToStart();
            QString list;
            for (unsigned i{0}; i < ml.moves.size(); ++i)
            {
                if (!game.findNextMove(ml.moves[i]))
                {
                    list.append("Could not find moves for line");
                    break;
                }
                list.append(game.moveToSan(GameX::MoveStringFlags::MoveOnly, GameX::NextPreviousMove::PreviousMove));
                if (i+1 < ml.moves.size())
                {
                    list.append(", ");
                }
            }
            new DuplicatePositionItem{list, ml.lastMove, duplicatePosition};
        }
    }
    endResetModel();
}

DuplicatePositionsWidget::DuplicatePositionsWidget(QWidget* parent)
    : QWidget{parent}
    , m_requestFindDuplicates{nullptr}
    , m_expandAll{nullptr}
    , m_treeView{nullptr}
    , m_treeModel{nullptr}
{
    setObjectName("duplicate positions widget");
    m_requestFindDuplicates = new QPushButton{"Find Duplicate Positions"};
    m_requestFindDuplicates->setObjectName("request duplicates");
    connect(m_requestFindDuplicates, &QPushButton::clicked, this, &DuplicatePositionsWidget::findDuplicatesRequested);
    m_expandAll = new QPushButton{"Expand All"};
    m_expandAll->setObjectName("duplicates expand all");
    connect(m_expandAll, &QPushButton::clicked, this, &DuplicatePositionsWidget::toggleExpandAll);
    m_treeView = new QTreeView;
    m_treeView->setObjectName("duplicate positions tree view");
    m_treeModel = new DuplicatePositionModel;
    m_treeView->setHeaderHidden(true);
    m_treeView->setModel(m_treeModel);
    connect(m_treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DuplicatePositionsWidget::selectionChanged);
    connect(m_treeView, &QTreeView::expanded, this, &DuplicatePositionsWidget::treeViewExpanded);
    connect(m_treeView, &QTreeView::collapsed, this, &DuplicatePositionsWidget::treeViewExpanded);
    QGridLayout * layout {new QGridLayout{this}};
    m_requestFindDuplicates->setDefault(true);
    layout->addWidget(m_requestFindDuplicates, /*row*/0, 0);
    layout->addWidget(m_expandAll, 0, 1);
    layout->addWidget(m_treeView, 1, 0, /*rowSpan*/1, /*columnSpan*/2);
}

DuplicatePositionsWidget::~DuplicatePositionsWidget() noexcept
{
    delete m_treeModel;
}

void DuplicatePositionsWidget::findDuplicatesRequested()
{
    if (m_treeModel)
    {
        m_treeModel->setData(game.getDuplicatePositions(), game);
    }
}

void DuplicatePositionsWidget::toggleExpandAll()
{
    if (!m_expandAll || !m_treeView)
    {
        return;
    }
    if (m_expandAll->text().contains("Expand"))
    {
        m_treeView->expandAll();
        m_expandAll->setText("Collapse All");
    }
    else
    {
        m_treeView->collapseAll();
        m_expandAll->setText("Expand All");
    }
}

void DuplicatePositionsWidget::gameChanged(GameX const & game)
{
    // Save a copy of the game
    this->game = game;
    if (!m_treeModel)
    {
        return;
    }
    // Clear the duplicate positions because they haven't been calculated for this game yet
    m_treeModel->setData({}, game);
}

void DuplicatePositionsWidget::selectionChanged(QItemSelection const & selected, QItemSelection const & /*deselected*/)
{
    if (!m_treeModel)
    {
        return;
    }
    QModelIndexList indexes {selected.indexes()};
    for (QModelIndex const & index : indexes)
    {
        std::optional<MoveId> maybeMove {m_treeModel->getLink(index)};
        if (!maybeMove)
        {
            continue;
        }
        MoveId move {*maybeMove};
        if (move < 1)
        {
            // Next move, previous move, etc. don't have any meaning here
            continue;
        }
        linkClicked("move:" + QString::number(move));
        break;
    }
}

// Returns the depth of the given index
// Pass index by copy so we can change it
static unsigned getDepth(QModelIndex index) noexcept
{
    unsigned depth{0};
    while (index.isValid())
    {
        ++depth;
        index = index.parent();
    }
    return depth;
}

// Returns the next index in the model that has the same depth as the given index
static QModelIndex getNextSibling(QModelIndex const & start, QAbstractItemModel const * itemModel)
{
    int currentRow = start.row();
    unsigned const startDepth = getDepth(start);
    QModelIndex currentIndex {start};
    while (true)
    {
        ++currentRow;
        currentIndex = itemModel->index(currentRow, 0);
        if (!currentIndex.isValid())
        {
            // We've gone through all the rows of the model, return an invalid index
            return currentIndex;
        }
        unsigned const currentDepth{getDepth(currentIndex)};
        if (currentDepth != startDepth)
        {
            // This index is perhaps a child index, not a sibling
            continue;
        }
        return currentIndex;
    }
}

// If the tree view has all its items expanded, change the button text to
// "Collapse All".
void DuplicatePositionsWidget::treeViewExpanded()
{
    if (!m_treeModel || !m_treeModel->rowCount() || !m_treeView)
    {
        return;
    }
    QModelIndex currentItem = m_treeModel->index(0,0);
    bool allExpanded = true;
    bool allCollapsed = true;
    while (currentItem.isValid())
    {
        if (m_treeView->isExpanded(currentItem))
        {
            allCollapsed = false;
        }
        else
        {
            allExpanded = false;
        }
        currentItem = getNextSibling(currentItem, m_treeModel);
    }
    if (allExpanded)
    {
        m_expandAll->setText("Collapse All");
    }
    else if(allCollapsed)
    {
        m_expandAll->setText("Expand All");
    }
}
