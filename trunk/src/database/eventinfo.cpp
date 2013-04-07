/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <QCoreApplication>
#include <QHash>

#include "eventinfo.h"
#include "database.h"

static bool sortPlayersLt(const PlayerInfoListItem& a1, const PlayerInfoListItem& a2)
{
    if (a1.second == a2.second)
    {
        return (a1.first < a2.first);
    }
    return a1.second > a2.second;
}

EventInfo::EventInfo()
{
    m_database = 0;
    reset();
}

EventInfo::~EventInfo()
{
}

EventInfo::EventInfo(Database* db, const QString & event)
{
    setDatabase(db);
    setName(event);
    reset();
    update();
}

QString EventInfo::name() const
{
    return m_name;
}

void EventInfo::setDatabase(Database* db)
{
    m_database = db;
}

void EventInfo::setName(const QString& event)
{
    m_name = event;
    update();
}


int EventInfo::toResult(const QString& res) const
{
    if (res.startsWith("1/2")) return Draw;
    else if (res.startsWith('1')) return WhiteWin;
    else if (res.startsWith('0')) return BlackWin;
    else return ResultUnknown;
}

float EventInfo::toPoints(const QString& res) const
{
    if (res.startsWith("1/2")) return 0.5;
    else if (res.startsWith('1')) return 1.0;
    else if (res.startsWith('0')) return 0.0;
    else if (res.startsWith("+-")) return 1.0;
    else if (res.startsWith("-+")) return 0.0;
    else if (res.startsWith("+--")) return 1.0;
    else if (res.startsWith("--+")) return 0.0;
    else return ResultUnknown;
}

void EventInfo::update()
{
    QHash<QString, float> players;

    const Index* index = m_database->index();

    // Determine matching tag values
    ValueIndex event = index->getValueIndex(m_name);

    // Clean previous statistics
    reset();

    for (int i = 0; i < m_database->count(); ++i) {
        if (index->valueIndexFromTag(TagNameEvent, i) != event)
            continue;
        QString result = index->tagValue(TagNameResult, i);
        int res = toResult(result);
        QString whitePlayer = index->tagValue(TagNameWhite, i);
        QString blackPlayer = index->tagValue(TagNameBlack, i);
        // The following works as QHash initializes a default-constructed value to 0
        players[whitePlayer] += toPoints(result);
        players[blackPlayer] += (1.0-toPoints(result));
        m_games[whitePlayer]++;
        m_games[blackPlayer]++;
        m_result[res]++;
        m_count++;
        PartialDate date(index->tagValue(TagNameDate, i));
        if (date.year() > 1000) {
            m_date[0] = qMin(date, m_date[0]);
            m_date[1] = qMax(date, m_date[1]);
        }
    }

    foreach(QString s, players.keys())
    {
        m_players.append(PlayerInfoListItem(s, players.value(s)));
        qSort(m_players.begin(),m_players.end(),sortPlayersLt);
    }
}


QString EventInfo::formattedScore(const int result[4], int count) const
{
    if (!count)
        return QCoreApplication::translate("EventInfo", "<i>no games</i>");
    QString score = "<b>";
    QChar scoresign[4] = {'*', '+', '=', '-'};
    for (int i = WhiteWin; i <= BlackWin; ++i)
        score += QString(" &nbsp;%1%2").arg(scoresign[i]).arg(result[i]);
    if (result[ResultUnknown])
        score += QString(" &nbsp;*%1").arg(result[ResultUnknown]);
    if (count - result[ResultUnknown])
        score += QString(" &nbsp;(%1%)").arg((100.0 * result[WhiteWin] + 50.0 * result[Draw]) / (count - result[ResultUnknown]),
                                                         1, 'f', 1);
    score += "</b>";
    return score;
}


QString EventInfo::formattedScore() const
{
    return QCoreApplication::translate("EventInfo", "Total: %1")
            .arg(formattedScore(m_result, m_count));
}

void EventInfo::reset()
{
    for (int c = White; c <= Black; ++c)
    {
        for (int r = 0; r < 4; ++r)
            m_result[r] = 0;
    }
    m_players.clear();
    m_games.clear();
    m_count = 0;
    m_rating[0] = 99999;
    m_rating[1] = 0;
    m_date[0] = PDMaxDate;
    m_date[1] = PDMinDate;

}

QString EventInfo::formattedGameCount() const
{
    return QCoreApplication::translate("EventInfo", "Games in database %1: %2<br>")
            .arg(m_database->name()).arg(m_count);
}

QString EventInfo::formattedRating() const
{
    if (!m_rating[1])
        return QString();
    else if (m_rating[0] == m_rating[1])
        return QCoreApplication::translate("EventInfo", "Rating: <b>%1</b><br>").arg(m_rating[0]);
    else
        return QCoreApplication::translate("EventInfo", "Rating: <b>%1-%2</b><br>")
                .arg(m_rating[0]).arg(m_rating[1]);
}

QString EventInfo::formattedRange() const
{
    if (m_date[0].year() == 9999)	// No date
        return QCoreApplication::translate("EventInfo","Date: <b>????.??.\?\?<b><br>");
    else if (m_date[0].year() < 1000)
        return QString();
    else
        return QCoreApplication::translate("EventInfo", "Date: <b>%1</b><br>").arg(m_date[0].range(m_date[1]));
}

QString EventInfo::listOfPlayers() const
{
    QString playersList;
    playersList.append(QCoreApplication::translate("EventInfo","<table><tr><th>Participants<th>Score"));

    for (PlayerInfoList::const_iterator it=m_players.begin(); it != m_players.end(); ++it)
    {
        playersList += QString("<tr><td><a href='player:%1'>%2</a><td>%3/%4")
                .arg((*it).first)
                .arg((*it).first)
                .arg((*it).second)
                .arg(m_games[(*it).first]);
    }

    playersList = playersList.append("</table>");
    return playersList;
}
