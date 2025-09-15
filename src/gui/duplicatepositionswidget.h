#ifndef DUPLICATEPOSITIONSWIDGET_H
#define DUPLICATEPOSITIONSWIDGET_H

#include <QWidget>
#include "gamex.h"

class QPushButton;
class QTreeView;

// Represents either a duplicated position or its child node: a
// set of moves that led to a duplicated position
class DuplicatePositionItem
{
public:
    DuplicatePositionItem(QString const &, MoveId = CURRENT_MOVE, DuplicatePositionItem * parent = nullptr);
    DuplicatePositionItem(QString const &, DuplicatedPosition::WarningLevel = DuplicatedPosition::WarningLevel::None,
            DuplicatePositionItem * parent = nullptr);
    ~DuplicatePositionItem();

    void appendChild(DuplicatePositionItem *child);
    DuplicatePositionItem *child(int row);
    int childCount() const;
    QString datum() const;
    MoveId move() const;
    int row() const;
    DuplicatePositionItem *parentItem();
    DuplicatedPosition::WarningLevel const warning;
    void deleteChildren() noexcept;

private:
    QVector<DuplicatePositionItem*> m_childItems;
    QString m_itemDatum;
    DuplicatePositionItem *m_parentItem;
    // Used to link to the list of moves that led to a duplicate position
    MoveId m_move;
};

class DuplicatePositionModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit DuplicatePositionModel(QObject *parent = nullptr);
    ~DuplicatePositionModel();

    // A GameX is necessary to be able to convert to standard algebraic notation.
    void setData(std::vector<DuplicatedPosition> const &, GameX const &)noexcept;

    QVariant data(QModelIndex const &index, int role) const override;
    Qt::ItemFlags flags(QModelIndex const &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, QModelIndex const &parent = {}) const override;
    QModelIndex parent(QModelIndex const &index) const override;
    int rowCount(QModelIndex const &parent = {}) const override;
    int columnCount(QModelIndex const &parent = {}) const override;
    std::optional<MoveId> getLink(QModelIndex const &) const;

private:
    DuplicatePositionItem *rootItem;
};

class DuplicatePositionsWidget final : public QWidget
{
    Q_OBJECT

public:
    DuplicatePositionsWidget(QWidget* parent = nullptr);
    ~DuplicatePositionsWidget() noexcept;

    void gameChanged(GameX const &);

signals:
    void linkClicked(QString const &);

private slots:
    void findDuplicatesRequested();
    void toggleExpandAll();
    void treeViewExpanded();

private:
    QPushButton * m_requestFindDuplicates;
    QPushButton * m_expandAll;
    QTreeView * m_treeView;
	DuplicatePositionModel * m_treeModel;
    GameX game;

    void selectionChanged(QItemSelection const &, QItemSelection const &);
};

#endif // DUPLICATEPOSITIONSWIDGET_H
