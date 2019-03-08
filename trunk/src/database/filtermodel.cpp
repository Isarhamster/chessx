/***************************************************************************
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "filtermodel.h"

#include "database.h"
#include "filter.h"
#include "settings.h"
#include "tags.h"

#include <QtGui>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

FilterModel::FilterModel(Filter* filter, QObject* parent)
    : QAbstractItemModel(parent), m_filter(filter), m_modelUpdateStarted(0)
{
    setupColumns();
}

FilterModel::~FilterModel()
{
}

int FilterModel::rowCount(const QModelIndex& index) const
{
    if(index.isValid())
    {
        return 0;
    }
    return m_filter ? m_filter->size() : 0;
}

int FilterModel::columnCount(const QModelIndex&) const
{
    return m_columnNames.count();
}

void FilterModel::addColumns(const QStringList& tags)
{
    m_columnNames.clear();
    m_columnTags.clear();

    m_columnNames << tr("Nr")
                  << tr("White")
                  << tr("White Elo")
                  << tr("Black")
                  << tr("Black Elo")
                  << tr("Event")
                  << tr("Site")
                  << tr("Round")
                  << tr("Date")
                  << tr("Result")
                  << tr("ECO")
                  << tr("Moves");

    m_columnTags << "Nr"
                 << TagNameWhite
                 << TagNameWhiteElo
                 << TagNameBlack
                 << TagNameBlackElo
                 << TagNameEvent
                 << TagNameSite
                 << TagNameRound
                 << TagNameDate
                 << TagNameResult
                 << TagNameECO
                 << TagNameLength;

    m_columnNames << tags;
    m_columnTags << tags;
}

QStringList FilterModel::additionalTags()
{
    QString addTags = AppSettings->getValue("/GameList/AdditionalTags").toString();
    QStringList tags = addTags.split(QRegExp("[^a-zA-Z]"), QString::SkipEmptyParts);
    return tags;
}

void FilterModel::setupColumns()
{
    addColumns(additionalTags());
}

void FilterModel::updateColumns()
{
    startUpdate();
    setupColumns();
    endUpdate();
}

void FilterModel::disableUpdate()
{
    ++m_modelUpdateStarted;
}

void FilterModel::enableUpdate()
{
    --m_modelUpdateStarted;
}

void FilterModel::startUpdate()
{
    if (!m_modelUpdateStarted) beginResetModel();
    ++m_modelUpdateStarted;
}

void FilterModel::endUpdate()
{
    --m_modelUpdateStarted;
    if (m_modelUpdateStarted==0)
    {
        endResetModel();
    }
}

void FilterModel::set(GameId game, int value)
{
    filter()->set(game, value);
    QModelIndex start = createIndex(game, 0, (void*) 0);
    QModelIndex end = createIndex(game, columnCount()-1, (void*) 0);
    emit dataChanged(start, end);
}

QVariant FilterModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && index.row() < m_filter->size())
    {
        GameId i = index.row();
        QString medal = m_filter->database()->tagValue(i, "Medal");
        if (VALID_INDEX(i))
        {
            if (role == Qt::DisplayRole)
            {
                if(index.column() == 0)
                {
                    return i + 1;
                }

                QString tag = m_filter->database()->tagValue(i, m_columnTags.at(index.column()));
                if(tag == "?")
                {
                    tag.clear();
                }
                return tag;
            }
            else if(role == Qt::BackgroundRole)
            {
                QColor bg(medal);
                if (bg.isValid())
                {
                    bg.setAlpha(80);
                    return QBrush(bg);
                }
            }
            else if(role == Qt::FontRole)
            {
                if(m_filter->database()->deleted(i))
                {
                    QFont font;
                    font.setStrikeOut(true);
                    return font;
                }
            }
            else if(role == Qt::ForegroundRole)
            {
                if(!m_filter->database()->getValidFlag(i))
                {
                    QVariant v = QColor(Qt::red);
                    return v;
                }
                QVariant v = QColor(Qt::black);
                return v;
            }
        }
        else
        {
            qDebug() << index.row();
        }
    }
    return QVariant();
}

QVariant FilterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        return QString("%1").arg(m_columnNames.at(section));
    }
    else
    {
        return QString("%1").arg(section);
    }
}

Qt::ItemFlags FilterModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if(index.isValid())
    {
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags | Qt::ItemIsSelectable;
    }
    else
    {
        return Qt::ItemIsDropEnabled | defaultFlags | Qt::ItemIsSelectable;
    }
}

QModelIndex FilterModel::index(int row, int column, const QModelIndex& parent) const
{
    if(parent.isValid())
    {
        return QModelIndex();
    }
    return createIndex(row, column, (void*) 0);
}

Filter* FilterModel::filter()
{
    return m_filter;
}

void FilterModel::setFilter(Filter* filter)
{
    startUpdate();
    m_filter = filter;
    endUpdate();
}

void FilterModel::invert()
{
    startUpdate();
    m_filter->invert();
    endUpdate();
}

void FilterModel::setAll(int value)
{
    startUpdate();
    m_filter->setAll(value);
    endUpdate();
}

void FilterModel::executeSearch(Search* search, FilterOperator searchOperator, int preSelect)
{
    Filter* f;
    if (searchOperator==FilterOperator::NullOperator)
    {
        m_filter->cancel();
        f = new Filter(*m_filter);
        f->setAll(preSelect); // ??
    }
    else
    {
        m_filter->wait();
        f = new Filter(*m_filter);
    }

    connect(f, SIGNAL(searchFinished()), SLOT(endSearch()), Qt::QueuedConnection);
    connect(f, SIGNAL(searchProgress(int)), SIGNAL(searchProgress(int)), Qt::QueuedConnection);

    m_filter->lock(f);
    f->executeSearch(search, searchOperator);
}

void FilterModel::endSearch()
{
    startUpdate();
    Filter* f = qobject_cast<Filter*>(sender());
    if (f && f!= m_filter)
    {
        *m_filter = *f;
        m_filter->lock(0);
        delete f;
    }
    endUpdate();
    emit searchFinished();
}


