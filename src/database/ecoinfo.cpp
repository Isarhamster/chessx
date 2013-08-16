/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <QCoreApplication>
#include <QHash>

#include "ecoinfo.h"
#include "database.h"

static bool sortPlayersLt(const PlayerInfoListItem& a1, const PlayerInfoListItem& a2)
{
    if (a1.second == a2.second)
    {
        return (a1.first < a2.first);
    }
    return a1.second > a2.second;
}

EcoInfo::EcoInfo()
{
    m_database = 0;
    reset();
}

EcoInfo::~EcoInfo()
{
}

EcoInfo::EcoInfo(Database* db, const QString & eco)
{
    setDatabase(db);
    setCode(eco);
    reset();
    update();
}

QString EcoInfo::name() const
{
    return m_code;
}

void EcoInfo::setDatabase(Database* db)
{
    m_database = db;
}

void EcoInfo::setCode(const QString& eco)
{
    m_code = eco;
    update();
}

int EcoInfo::toResult(const QString& res) const
{
    if (res.startsWith("1/2")) return Draw;
    else if (res.startsWith('1')) return WhiteWin;
    else if (res.startsWith('0')) return BlackWin;
    else return ResultUnknown;
}

float EcoInfo::toPoints(const QString& res) const
{
    if (res.startsWith("1/2")) return 0.5;
    else if (res.startsWith('1')) return 1.0;
    else if (res.startsWith('0')) return 0.0;
    else return -1.0;
}

void EcoInfo::update()
{
    QHash<QString, float> playersWhite;
    QHash<QString, float> playersBlack;

    const Index* index = m_database->index();

    // Determine matching tag values
    ValueIndex eco = index->getValueIndex(m_code);

    // Clean previous statistics
    reset();

    for (int i = 0; i < m_database->count(); ++i) {
        if (index->valueIndexFromTag(TagNameECO, i) != eco)
            continue;
        QString result = index->tagValue(TagNameResult, i);
        int res = toResult(result);
        QString whitePlayer = index->tagValue(TagNameWhite, i);
        QString blackPlayer = index->tagValue(TagNameBlack, i);
        // The following works as QHash initializes a default-constructed value to 0
        float fres = toPoints(result);
        if (fres >= 0)
        {
            playersWhite[whitePlayer] += fres;
            playersBlack[blackPlayer] += (1.0-fres);
            m_gamesWhite[whitePlayer]++;
            m_gamesBlack[blackPlayer]++;
        }
        else
        {
            // This looks silly, but the []operator has a side effect!
            if (playersWhite[whitePlayer] == 0) playersWhite[whitePlayer] = 0;
            if (playersBlack[blackPlayer] == 0) playersBlack[blackPlayer] = 0;
        }

        m_result[res]++;
        m_count++;
    }

    foreach(QString s, playersWhite.keys())
    {
        m_playersWhite.append(PlayerInfoListItem(s, playersWhite.value(s)));
    }
    qSort(m_playersWhite.begin(),m_playersWhite.end(),sortPlayersLt);

    foreach(QString s, playersBlack.keys())
    {
        m_playersBlack.append(PlayerInfoListItem(s, playersBlack.value(s)));
    }
    qSort(m_playersBlack.begin(),m_playersBlack.end(),sortPlayersLt);
}


QString EcoInfo::formattedScore(const int result[4], int count) const
{
    if (!count)
        return QCoreApplication::translate("EcoInfo", "<i>no games</i>");
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


QString EcoInfo::formattedScore() const
{
    return tr("Total: %1")
            .arg(formattedScore(m_result, m_count));
}

void EcoInfo::reset()
{
    for (int c = White; c <= Black; ++c)
    {
        for (int r = 0; r < 4; ++r)
            m_result[r] = 0;
    }
    m_playersWhite.clear();
    m_playersBlack.clear();
    m_gamesWhite.clear();
    m_gamesBlack.clear();
    m_count = 0;
    m_rating[0] = 99999;
    m_rating[1] = 0;
}

QString EcoInfo::formattedGameCount() const
{
    return QCoreApplication::translate("EcoInfo", "Games in database %1: %2")
            .arg(m_database->name()).arg(m_count);
}

QString EcoInfo::formattedRating() const
{
    if (!m_rating[1])
        return QString();
    else if (m_rating[0] == m_rating[1])
        return QCoreApplication::translate("EcoInfo", "Rating: <b>%1</b>").arg(m_rating[0]);
    else
        return QCoreApplication::translate("EcoInfo", "Rating: <b>%1-%2</b>")
                .arg(m_rating[0]).arg(m_rating[1]);
}

QString EcoInfo::listOfPlayers() const
{
    QString playersList;

// Does not work due to bugs in Qt5
//    if (m_playersWhite.count() > 10)
//    {
//        playersList.append(tr("<a href=\"#ListBlack\">Go to Black Players Score</a>"));
//    }

    playersList.append(tr("<table><tr><th>White Player</th><th>Score</th></tr>"));

    for (PlayerInfoList::const_iterator it=m_playersWhite.begin(); it != m_playersWhite.end(); ++it)
    {
        playersList += QString("<tr><td><a href=\"player-white:%1\">%2</a></td><td>%3/%4</td></tr>")
                .arg((*it).first)
                .arg((*it).first)
                .arg((*it).second)
                .arg(m_gamesWhite[(*it).first]);
    }

    playersList = playersList.append("</table>");

    playersList.append(tr("<a name='ListBlack'></a><table><tr><th>Black Player</th><th>Score</th></tr>"));

    for (PlayerInfoList::const_iterator it=m_playersBlack.begin(); it != m_playersBlack.end(); ++it)
    {
        playersList += QString("<tr><td><a href=\"player-black:%1\">%2</a></td><td><td>%3/%4</td></tr>")
                .arg((*it).first)
                .arg((*it).first)
                .arg((*it).second)
                .arg(m_gamesBlack[(*it).first]);
    }

    playersList = playersList.append("</table>");

    return playersList;
}
