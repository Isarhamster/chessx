
#include "doctest.h"
#include "resourcepath.h"

#include "tags.h"
#include "index.h"
#include "pgndatabase.h"

#include "settings.h"

TEST_CASE("testing Index class")
{
    Index index;

    index.setTag("Event", "Ch World (match)", 0);
    index.setTag("Site", "Buenos Aires ARG", 0);
    index.setTag("Date", "1927.??.??", 0);
    index.setTag("Round", "12", 0);
    index.setTag("White", "Alekhine, Alexander A", 0);
    index.setTag("Black", "Capablanca, Jose Raul", 0);
    index.setTag("Result", "1-0", 0);
    index.setTag("ECO", "D64", 0);
    index.setTag("EventDate", "1927.??.??", 0);
    index.setTag("PlyCount", "81", 0);
    index.setTag("Source", "Chessbase", 0);
    index.setTag("WhiteTeamCountry", "YUG", 0);
    index.setTag("BlackTeamCountry", "SUI", 0);
    index.setTag("Event", "Ch World (match)", 1);
    index.setTag("Site", "Buenos Aires ARG", 1);
    index.setTag("Date", "1927.??.??", 1);
    index.setTag("Round", "20", 1);
    index.setTag("White", "Alekhine, Alexander A", 1);
    index.setTag("Black", "Capablanca, Jose Raul", 1);
    index.setTag("Result", "1/2-1/2", 1);
    index.setTag("ECO", "D67", 1);
    index.setTag("EventDate", "1927.??.??", 1);
    index.setTag("PlyCount", "86", 1);
    index.setTag("Source", "Chessbase", 1);
    index.setTag("WhiteTeamCountry", "YUG", 1);
    index.setTag("BlackTeamCountry", "SUI", 1);

    CHECK_EQ(index.tagValue(TagNameEvent, 0) , QString("Ch World (match)"));
    CHECK_EQ(index.tagValue(TagNameSite, 0) , QString("Buenos Aires ARG"));
    CHECK_EQ(index.tagValue(TagNameDate, 0) , QString("1927.??.??"));
    CHECK_EQ(index.tagValue(TagNameRound, 0) , QString("12"));
    CHECK_EQ(index.tagValue(TagNameWhite, 0) , QString("Alekhine, Alexander A"));
    CHECK_EQ(index.tagValue(TagNameBlack, 0) , QString("Capablanca, Jose Raul"));
    CHECK_EQ(index.tagValue(TagNameResult, 0) , QString("1-0"));
    CHECK_EQ(index.tagValue(TagNameECO, 0) , QString("D64"));
    CHECK_EQ(index.tagValue(TagNameEventDate, 0) , QString("1927.??.??"));
    CHECK_EQ(index.tagValue(TagNamePlyCount, 0) , QString("81"));
    CHECK_EQ(index.tagValue(TagNameSource, 0) , QString("Chessbase"));
    CHECK_EQ(index.tagValue(TagNameEvent, 1) , QString("Ch World (match)"));
    CHECK_EQ(index.tagValue(TagNameSite, 1) , QString("Buenos Aires ARG"));
    CHECK_EQ(index.tagValue(TagNameDate, 1) , QString("1927.??.??"));
    CHECK_EQ(index.tagValue(TagNameRound, 1) , QString("20"));
    CHECK_EQ(index.tagValue(TagNameWhite, 1) , QString("Alekhine, Alexander A"));
    CHECK_EQ(index.tagValue(TagNameBlack, 1) , QString("Capablanca, Jose Raul"));
    CHECK_EQ(index.tagValue(TagNameResult, 1) , QString("1/2-1/2"));
    CHECK_EQ(index.tagValue(TagNameECO, 1) , QString("D67"));
    CHECK_EQ(index.tagValue(TagNameEventDate, 1) , QString("1927.??.??"));
    CHECK_EQ(index.tagValue(TagNamePlyCount, 1) , QString("86"));
    CHECK_EQ(index.tagValue(TagNameSource, 1) , QString("Chessbase"));

}

TEST_CASE("testing Index read from PGN database")
{
    // required by PgnDatabase::open() to check if indexing is enabled
    // TODO: remove
    AppSettings = new Settings;

    PgnDatabase db { false };
    db.open(RESOURCE_PATH "game1.pgn", false);
    db.parseFile();

    CHECK_EQ(db.index()->tagValue(TagNameEvent, 0) , QString("Ch World (match)"));
    CHECK_EQ(db.index()->tagValue(TagNameSite, 0) , QString("Buenos Aires ARG"));
    CHECK_EQ(db.index()->tagValue(TagNameDate, 0) , QString("1927.??.??"));
    CHECK_EQ(db.index()->tagValue(TagNameRound, 0) , QString("12"));
    CHECK_EQ(db.index()->tagValue(TagNameWhite, 0) , QString("Alekhine, Alexander A"));
    CHECK_EQ(db.index()->tagValue(TagNameBlack, 0) , QString("Capablanca, Jose Raul"));
    CHECK_EQ(db.index()->tagValue(TagNameResult, 0) , QString("1-0"));
    CHECK_EQ(db.index()->tagValue(TagNameECO, 0) , QString("D64"));
    CHECK_EQ(db.index()->tagValue(TagNameEventDate, 0) , QString("1927.??.??"));
    CHECK_EQ(db.index()->tagValue(TagNamePlyCount, 0) , QString("81"));
    CHECK_EQ(db.index()->tagValue(TagNameEvent, 1) , QString("Ch World (match)"));
    CHECK_EQ(db.index()->tagValue(TagNameSite, 1) , QString("Buenos Aires ARG"));
    CHECK_EQ(db.index()->tagValue(TagNameDate, 1) , QString("1927.??.??"));
    CHECK_EQ(db.index()->tagValue(TagNameRound, 1) , QString("20"));
    CHECK_EQ(db.index()->tagValue(TagNameWhite, 1) , QString("Alekhine, Alexander A"));
    CHECK_EQ(db.index()->tagValue(TagNameBlack, 1) , QString("Capablanca, Jose Raul"));
    CHECK_EQ(db.index()->tagValue(TagNameResult, 1) , QString("1/2-1/2"));
    CHECK_EQ(db.index()->tagValue(TagNameECO, 1) , QString("D67"));
    CHECK_EQ(db.index()->tagValue(TagNameEventDate, 1) , QString("1927.??.??"));
    CHECK_EQ(db.index()->tagValue(TagNamePlyCount, 1) , QString("86"));

    AppSettings = nullptr;
}
