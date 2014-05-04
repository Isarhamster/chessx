/***************************************************************************
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QCoreApplication>
#include <QHash>

#include "playerinfo.h"
#include "database.h"

static bool sortEcoFrequencyLt(const EcoFrequencyItem& a1, const EcoFrequencyItem& a2)
{
    if(a1.second == a2.second)
    {
        return (a1.first < a2.first);
    }
    return a1.second > a2.second;
}

PlayerInfo::PlayerInfo()
{
    m_database = 0;
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
    QHash<QString, unsigned> openings[2];
    const Index* index = m_database->index();

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
            openings[c][eco]++;
        }
    }

    for(int i = 0; i < 2; ++i)
    {
        foreach(QString s, openings[i].keys())
        {
            m_eco[i].append(EcoFrequencyItem(s, openings[i].value(s)));
            qSort(m_eco[i].begin(), m_eco[i].end(), sortEcoFrequencyLt);
        }
    }

    qSwap(m_result[Black][WhiteWin], m_result[Black][BlackWin]);
}


QString PlayerInfo::formattedScore(const int result[4], int count) const
{
    if(!count)
    {
        return QCoreApplication::translate("PlayerInfo", "<i>no games</i>");
    }
    QString score = "<b>";
    QChar scoresign[4] = {'*', '+', '=', '-'};
    for(int i = WhiteWin; i <= BlackWin; ++i)
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


QString PlayerInfo::formattedScore() const
{
    int total[4];
    for(int i = 0; i < 4; ++i)
    {
        total[i] = m_result[White][i] + m_result[Black][i];
    }
    int count = m_count[White] + m_count[Black];
    return QCoreApplication::translate("PlayerInfo", "Total: %1<br>White: %2<br>Black: %3<br>")
           .arg(formattedScore(total, count))
           .arg(formattedScore(m_result[White], m_count[White]))
           .arg(formattedScore(m_result[Black], m_count[Black]));
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
    openingsList.append(QCoreApplication::translate("PlayerInfo", "<p>White Openings:</p><ul>"));
    openingsList.append(QCoreApplication::translate("PlayerInfo", "</ul><p>Black Openings:</p><ul>"));

    for(int i = 0; i < 2; ++i)
    {
        for(EcoFrequency::const_iterator it = m_eco[i].begin(); it != m_eco[i].end(); ++it)
        {
            openingsList[i] += QString("<li><a href='eco-%1:%2'>%3</a>: %4")
                               .arg(i == 0 ? "white" : "black")
                               .arg((*it).first)
                               .arg((*it).first)
                               .arg((*it).second);
        }
    }
    QString s = openingsList.at(0);
    s = s.append(openingsList.at(1));
    s = s.append("</ul>");
    return s;
}
