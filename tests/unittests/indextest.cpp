
#include "indextest.h"
#include "index.h"

void IndexTest::initTestCase() {}
void IndexTest::init(){}
void IndexTest::cleanup(){}
void IndexTest::cleanupTestCase() {}

void IndexTest::testIndex()
{
   Index index;

   index.setTag("Event", "Ch World (match)",0);
   index.setTag("Site", "Buenos Aires ARG",0);
   index.setTag("Date", "1927.??.??",0);
   index.setTag("Round", "12",0);
   index.setTag("White", "Alekhine, Alexander A",0);
   index.setTag("Black", "Capablanca, Jose Raul",0);
   index.setTag("Result", "1-0",0);
   index.setTag("ECO", "D64",0);
   index.setTag("EventDate", "1927.??.??",0);
   index.setTag("PlyCount", "81",0);
   index.setTag("Source", "Chessbase",0);
   index.setTag("WhiteTeamCountry", "YUG",0);
   index.setTag("BlackTeamCountry", "SUI",0);
   index.setTag("Event", "Ch World (match)",1);
   index.setTag("Site", "Buenos Aires ARG",1);
   index.setTag("Date", "1927.??.??",1);
   index.setTag("Round", "20",1);
   index.setTag("White", "Alekhine, Alexander A",1);
   index.setTag("Black", "Capablanca, Jose Raul",1);
   index.setTag("Result", "1/2-1/2",1);
   index.setTag("ECO", "D67",1);
   index.setTag("EventDate", "1927.??.??",1);
   index.setTag("PlyCount", "86",1);
   index.setTag("Source", "Chessbase",1);
   index.setTag("WhiteTeamCountry", "YUG",1);
   index.setTag("BlackTeamCountry", "SUI",1);

   QCOMPARE ( index.tagValue(TagEvent,0) , QString("Ch World (match)") );
   QCOMPARE ( index.tagValue(TagSite,0) , QString("Buenos Aires ARG") );
   QCOMPARE ( index.tagValue(TagDate,0) , QString("1927.??.??") );
   QCOMPARE ( index.tagValue(TagRound,0) , QString("12") );
   QCOMPARE ( index.tagValue(TagWhite,0) , QString("Alekhine, Alexander A") );
   QCOMPARE ( index.tagValue(TagBlack,0) , QString("Capablanca, Jose Raul") );
   QCOMPARE ( index.tagValue(TagResult,0) , QString("1-0") );
   QCOMPARE ( index.tagValue(TagECO,0) , QString("D64") );
   QCOMPARE ( index.tagValue(TagEventDate,0) , QString("1927.??.??") );
   QCOMPARE ( index.tagValue(TagPlyCount,0) , QString("81") );
   QCOMPARE ( index.tagValue(TagSource,0) , QString("Chessbase") );
   QCOMPARE ( index.tagValue(TagEvent,1) , QString("Ch World (match)") );
   QCOMPARE ( index.tagValue(TagSite,1) , QString("Buenos Aires ARG") );
   QCOMPARE ( index.tagValue(TagDate,1) , QString("1927.??.??") );
   QCOMPARE ( index.tagValue(TagRound,1) , QString("20") );
   QCOMPARE ( index.tagValue(TagWhite,1) , QString("Alekhine, Alexander A") );
   QCOMPARE ( index.tagValue(TagBlack,1) , QString("Capablanca, Jose Raul") );
   QCOMPARE ( index.tagValue(TagResult,1) , QString("1/2-1/2") );
   QCOMPARE ( index.tagValue(TagECO,1) , QString("D67") );
   QCOMPARE ( index.tagValue(TagEventDate,1) , QString("1927.??.??") );
   QCOMPARE ( index.tagValue(TagPlyCount,1) , QString("86") );
   QCOMPARE ( index.tagValue(TagSource,1) , QString("Chessbase") );

}

void IndexTest::testIndexFromDatabase()
{
   PgnDatabase db;
   db.open("data/game1.pgn");

   QCOMPARE ( db.index()->tagValue(TagEvent,0) , QString("Ch World (match)") );
   QCOMPARE ( db.index()->tagValue(TagSite,0) , QString("Buenos Aires ARG") );
   QCOMPARE ( db.index()->tagValue(TagDate,0) , QString("1927.??.??") );
   QCOMPARE ( db.index()->tagValue(TagRound,0) , QString("12") );
   QCOMPARE ( db.index()->tagValue(TagWhite,0) , QString("Alekhine, Alexander A") );
   QCOMPARE ( db.index()->tagValue(TagBlack,0) , QString("Capablanca, Jose Raul") );
   QCOMPARE ( db.index()->tagValue(TagResult,0) , QString("1-0") );
   QCOMPARE ( db.index()->tagValue(TagECO,0) , QString("D64") );
   QCOMPARE ( db.index()->tagValue(TagEventDate,0) , QString("1927.??.??") );
   QCOMPARE ( db.index()->tagValue(TagPlyCount,0) , QString("81") );
   QCOMPARE ( db.index()->tagValue(TagEvent,1) , QString("Ch World (match)") );
   QCOMPARE ( db.index()->tagValue(TagSite,1) , QString("Buenos Aires ARG") );
   QCOMPARE ( db.index()->tagValue(TagDate,1) , QString("1927.??.??") );
   QCOMPARE ( db.index()->tagValue(TagRound,1) , QString("20") );
   QCOMPARE ( db.index()->tagValue(TagWhite,1) , QString("Alekhine, Alexander A") );
   QCOMPARE ( db.index()->tagValue(TagBlack,1) , QString("Capablanca, Jose Raul") );
   QCOMPARE ( db.index()->tagValue(TagResult,1) , QString("1/2-1/2") );
   QCOMPARE ( db.index()->tagValue(TagECO,1) , QString("D67") );
   QCOMPARE ( db.index()->tagValue(TagEventDate,1) , QString("1927.??.??") );
   QCOMPARE ( db.index()->tagValue(TagPlyCount,1) , QString("86") );
}
