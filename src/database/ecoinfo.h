#ifndef ECOINFO_H
#define ECOINFO_H

#include "common.h"

#include <QtCore>
#include <QString>
#include <QList>
#include <QPair>

class Database;
typedef QPair<QString, float> PlayerInfoListItem;
typedef QList<PlayerInfoListItem> PlayerInfoList;

class EcoInfo
{
    Q_DECLARE_TR_FUNCTIONS(EcoInfo)

public:
    EcoInfo();
public:
    /** Set database and player. Automatically updates information. */
    EcoInfo(Database* db, const QString& eco);
    /** Standard destructor. */
    ~EcoInfo();
    /** Changes database. Does not automatically update statistics. */
    void setDatabase(Database* db);
    /** Changes player. Does not automatically update statistics. */
    void setCode(const QString& eco);
    /** @return current player name. */
    QString name() const;
    /** Recalculates all statistics. */
    void update();
    /** @return string with formatted score statistics. */
    QString formattedScore() const;
    /** @return string with formatted rating. */
    QString formattedRating() const;
    /** @return string with formatted game count. */
    QString formattedGameCount() const;
    /** @return string with list of players in the event */
    QString listOfPlayers() const;

private:
    /** Clears all statistics. */
    void reset();
    /** Add statistics for game @p */
    void addGameStats(int index);

    QString m_code;
    Database* m_database;

    /** Format score statistics for single color. */
    QString formattedScore(const int results[4], int count) const;
    int toResult(const QString& res) const;
    float toPoints(const QString& res) const;

    int m_result[4];
    int m_count;
    int m_rating[2];
    PlayerInfoList m_playersWhite;
    PlayerInfoList m_playersBlack;
    QHash<QString, int> m_gamesWhite;
    QHash<QString, int> m_gamesBlack;
};

#endif // ECOINFO_H

