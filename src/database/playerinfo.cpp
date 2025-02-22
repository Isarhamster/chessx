/***************************************************************************
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include <algorithm>
#include <QCoreApplication>
#include <QHash>

#include "ecopositions.h"
#include "database.h"
#include "playerinfo.h"
#include "tags.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

static bool sortEcoFrequencyLt(const EcoFrequencyItem& left, const EcoFrequencyItem& right)
{
    if(left.second.count == right.second.count)
    {
        return (left.first < right.first);
    }
    return left.second.count > right.second.count;
}

PlayerInfo::PlayerInfo()
{
    m_database = nullptr;
    reset();
}

PlayerInfo::~PlayerInfo()
{
}

PlayerInfo::PlayerInfo(Database* db, const QString & player)
{
    setDatabase(db);
    setName(player);
    reset();
    update();
}

QString PlayerInfo::name() const
{
    return m_name;
}

void PlayerInfo::setDatabase(Database* db)
{
    m_database = db;
}

void PlayerInfo::setName(const QString& player)
{
    m_name = player;
    update();
}

int PlayerInfo::toResult(const QString& res) const
{
    if(res.startsWith("1/2"))
    {
        return Draw;
    }
    else if(res.startsWith('1'))
    {
        return WhiteWin;
    }
    else if(res.startsWith('0'))
    {
        return BlackWin;
    }
    else
    {
        return ResultUnknown;
    }
}

void PlayerInfo::update()
{
    QHash<QString, EcoFrequencyInfo> openings[2];
    QList<QString> openingsX[2];
    QHash<QString, int> openingsXX[2];
    const IndexX* index = m_database->index();

    // Determine matching tag values
    ValueIndex player = index->getValueIndex(m_name);

    // Clean previous statistics
    reset();

    for(int i = 0; i < (int)m_database->count(); ++i)
    {
        Color c;
        if(index->valueIndexFromTag(TagNameWhite, i) == player)
        {
            c = White;
        }
        else if(index->valueIndexFromTag(TagNameBlack, i) == player)
        {
            c = Black;
        }
        else
        {
            continue;
        }
        int res = toResult(index->tagValue(TagNameResult, i));
        m_result[c][res]++;
        m_count[c]++;
        int elo = index->tagValue(c == White ? TagNameWhiteElo : TagNameBlackElo, i).toInt();
        if(elo)
        {
            m_rating[0] = qMin(elo, m_rating[0]);
            m_rating[1] = qMax(elo, m_rating[1]);
        }
        PartialDate date(index->tagValue(TagNameDate, i));
        if(date.year() > 1000)
        {
            m_date[0] = qMin(date, m_date[0]);
            m_date[1] = qMax(date, m_date[1]);
        }
        QString eco = index->tagValue(TagNameECO, i).left(3);
        if(eco.length() == 3)
        {
            openings[c][eco].count++;
            openings[c][eco].result[res]++;
        }
        QString ecoX = index->tagValue(TagNameECO, i).left(4);
        if(ecoX.length() >= 3)
        {
            QString opening = EcoPositions::findEcoName(ecoX);
            if (!opening.isEmpty())
            {
                openingsX[c].append(ecoX);
                openingsXX[c][opening]++;
            }
        }
    }

    for(int i = 0; i < 2; ++i)
    {
        for(auto it = openings[i].cbegin(); it != openings[i].cend(); ++it)
        {
            m_eco[i].append(EcoFrequencyItem(it.key(), it.value()));
        }
        std::sort(m_eco[i].begin(), m_eco[i].end(), sortEcoFrequencyLt);
    }

    for(int i = 0; i < 2; ++i)
    {
        for (auto it = openingsX[i].cbegin(); it != openingsX[i].cend(); ++it)
        {
            QString opening = EcoPositions::findEcoName(*it);
            m_MapOpeningToECOCodes[i][opening].append(*it);
        }
        auto& counts = m_opening[i];
        for (auto it = openingsXX[i].cbegin(); it != openingsXX[i].cend(); ++it)
        {
            counts.append(OpeningCountItem(it.key(), it.value()));
        }
        // comparison function for sorting m_opening[c]
        auto cmp = [](const OpeningCountItem& lhs, const OpeningCountItem& rhs) {
            // first sort by descending frequency
            if (lhs.second != rhs.second)
                return lhs.second > rhs.second;
            // then sort by ECO name alphabetically
            return lhs.first < rhs.first;
        };
        std::sort(counts.begin(), counts.end(), cmp);
    }

    qSwap(m_result[Black][WhiteWin], m_result[Black][BlackWin]);
}

QString PlayerInfo::formattedScore() const
{
    int total[4];
    for(int i = 0; i < 4; ++i)
    {
        total[i] = m_result[White][i] + m_result[Black][i];
    }
    int count = m_count[White] + m_count[Black];
    QString ref1 = QString("<a href='result-white:%1#").arg(m_name);
    QString ref2 = QString("<a href='result-black:%1#").arg(m_name);
    return QCoreApplication::translate("PlayerInfo", "Total: %1<br>White: %2<br>Black: %3<br>")
           .arg(formattedScore(total, count), formattedScore(m_result[White], m_count[White], ref1, true), formattedScore(m_result[Black], m_count[Black], ref2, false));
}

QString PlayerInfo::formattedScore(const int result[4], int count) const
{
    if(!count)
    {
        return QCoreApplication::translate("PlayerInfo", "<i>no games</i>");
    }
    QString score = "<b>";
    QChar scoresign[4] = {'*', '+', '=', '-'};
    score += QString("%1%2").arg(scoresign[WhiteWin]).arg(result[WhiteWin]);
    for(int i = Draw; i <= BlackWin; ++i)
    {
        score += QString(" &nbsp;%1%2").arg(scoresign[i]).arg(result[i]);
    }
    if(result[ResultUnknown])
    {
        score += QString(" &nbsp;*%1").arg(result[ResultUnknown]);
    }
    if(count - result[ResultUnknown])
        score += QString(" &nbsp;(%1%)").
                 arg((100.0 * result[WhiteWin] + 50.0 * result[Draw]) / (count - result[ResultUnknown]), 1, 'f', 1);
    score += "</b>";
    return score;
}

QString PlayerInfo::formattedScore(const int result[4], int count, QString ref, bool mode) const
{
    if(!count)
    {
        return QCoreApplication::translate("PlayerInfo", "<i>no games</i>");
    }
    QString score = "<b>";
    QChar scoresign[4] = {'*', '+', '=', '-'};
    QString format = "%1%2'>%3%4</a>";

    score += format.arg(ref, (mode ? "1-0":"0-1")).arg(scoresign[WhiteWin]).arg(result[WhiteWin]);
    score += " &nbsp;";
    score += format.arg(ref, "1/2-1/2").arg(scoresign[Draw]).arg(result[Draw]);
    score += " &nbsp;";
    score += format.arg(ref, (!mode ? "1-0":"0-1")).arg(scoresign[BlackWin]).arg(result[BlackWin]);
    if(result[ResultUnknown])
    {
        score += " &nbsp;";
        score += format.arg(ref, "\\*").arg(scoresign[ResultUnknown]).arg(result[ResultUnknown]);
    }
    if(count - result[ResultUnknown])
        score += QString(" &nbsp;(%1%)").
                 arg((100.0 * result[WhiteWin] + 50.0 * result[Draw]) / (count - result[ResultUnknown]), 1, 'f', 1);
    score += "</b>";
    return score;
}

void PlayerInfo::reset()
{
    for(int c = White; c <= Black; ++c)
    {
        for(int r = 0; r < 4; ++r)
        {
            m_result[c][r] = 0;
        }
        m_count[c] = 0;
        m_eco[c].clear();
        m_opening[c].clear();
        m_MapOpeningToECOCodes[c].clear();
    }
    m_rating[0] = 99999;
    m_rating[1] = 0;
    m_date[0] = PDMaxDate;
    m_date[1] = PDMinDate;
}

QString PlayerInfo::formattedGameCount() const
{
    return QCoreApplication::translate("PlayerInfo", "Games in database <i>%1</i>: <b>%2</b><br>")
           .arg(m_database->name()).arg(m_count[White] + m_count[Black]);
}

QString PlayerInfo::formattedRating() const
{
    if(!m_rating[1])
    {
        return QString();
    }
    else if(m_rating[0] == m_rating[1])
    {
        return QCoreApplication::translate("PlayerInfo", "Rating: <b>%1</b><br>").arg(m_rating[0]);
    }
    else
        return QCoreApplication::translate("PlayerInfo", "Rating: <b>%1-%2</b><br>")
               .arg(m_rating[0]).arg(m_rating[1]);
}

QString PlayerInfo::formattedRange() const
{
    if(m_date[0].year() == 9999)	// No date
    {
        return QCoreApplication::translate("PlayerInfo", "Date: <b>????.??.\?\?<b><br>");
    }
    else if(m_date[0].year() < 1000)
    {
        return QString();
    }
    else
    {
        return QCoreApplication::translate("PlayerInfo", "Date: <b>%1</b><br>").arg(m_date[0].range(m_date[1]));
    }
}

QString PlayerInfo::listOfOpenings() const
{
    QStringList openingsList;
    openingsList.append(QString("<a name='ListWhite'></a><p><a href='#ListBlack'>&#8681;</a>&nbsp;<a href='player:%1#%2'>%3:</a></p>").arg(m_name, TagNameWhite, tr("White Openings")));
    openingsList.append(QString("<a name='ListBlack'></a><p><a href='#ListWhite'>&#8679;</a>&nbsp;<a href='player:%1#%2'>%3:</a></p>").arg(m_name, TagNameBlack, tr("Black Openings")));
    openingsList.append("");

    for(int i = 0; i < 2; ++i)
    {
        QStringList l;
        for(OpeningCountList::const_iterator it = m_opening[i].constBegin(); it != m_opening[i].constEnd(); ++it)
        {
            QString opening = (*it).first;
            QString codes = m_MapOpeningToECOCodes[i].value(opening).join("|");
            if (((*it).second)==1) break; // leave out things played only once
            if (((*it).second)*25<m_count[i]) break; // leave out things played rarely
            l.append(QString("<a href='eco-%1:%2'>%3</a> (%4)")
                     .arg((i == 0 ? "white" : "black"), codes, opening)
                     .arg((*it).second));
        }
        openingsList[i] += l.join(", ");
    }

    for(int i = 0; i < 2; ++i)
    {
        openingsList[i].append("<ul>");
        for(EcoFrequency::const_iterator it = m_eco[i].constBegin(); it != m_eco[i].constEnd(); ++it)
        {
            QString score;
            int count = (*it).second.count;
            int resultUnknown = (*it).second.result[ResultUnknown];
            int cResult = (*it).second.result[i == 0 ? WhiteWin : BlackWin];
            int dResult = (*it).second.result[Draw];
            if(count - resultUnknown)
            {
                score = QString(" (%1%)").arg((100.0 * cResult + 50.0 * dResult) / (count - resultUnknown), 1, 'f', 1);
            }

            openingsList[i] += QString("<li><a href='eco-%1:%2'>%3</a>: %4%5")
                               .arg((i == 0 ? "white" : "black"), (*it).first, (*it).first)
                               .arg(count)
                               .arg(score);
        }
    }

    QString s = openingsList.join("</ul>");
    return s;
}
