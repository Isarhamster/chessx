// main() to convert a scid ratings.ssp to our player db format,
// and test the content of the resulting player database.
//
// To use it, change the db_name, source and picture to fit 
// your filenames.
//
// Code to query the small test database - see querySmall().
// Code to query the newest full database - see queryNewest().


#include <iostream>
#include "databaseconversion.h"

static QString db_name = "/home/ejner/chessX/smalldb/players";
static QString source = "/home/ejner/chessX/smalldb/ratings.ssp";
static QString picture = "/home/ejner/chessX/polgar.jpg";

void queryNewest(){
//query newest full base as of 25/7/2005 (70207 players)
// available from http://members.aon.at/schachverein.steyr/ratings.ssp.zip
  PlayerDatabase pdb;
  if (pdb.open(db_name)) {
    std::cout << "opened " << db_name << "\n";
  }
  else{
    std::cout << "failure opening " << db_name << "\n";
    return;
  }

  std::cout << "number of players = " << pdb.count() <<"\n"; 
//  QStringList sl = pdb.playerNames();
//  for ( QStringList::Iterator it = sl.begin(); it != sl.end(); ++it ) {
//    std::cout << *it << "\n";
//  }

  QStringList sl = pdb.findPlayers("Hu",3);
  for ( QStringList::Iterator it = sl.begin(); it != sl.end(); ++it ) {
    std::cout << *it << "\n";
  }

  pdb.setCurrent("Aaberg, Anton");
  std::cout << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  QString s = pdb.country();
  std::cout << pdb.current() << " country= " << s << "\n";
  s = pdb.title();
  std::cout << pdb.current() << " title= " << s << "\n";

  pdb.setCurrent("Aaron, Manuel");
  std::cout << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  std::cout << pdb.current() << " country= " << pdb.country() << "\n";
  std::cout << pdb.current() << " title= " << pdb.title() << "\n";

  pdb.setCurrent("Aaberg, Anton");
  std::cout << pdb.current() << " elo on 2000/5/5= " << pdb.elo(QDate(2000,5,5)) << "\n";

  pdb.setCurrent("Aaron, Manuel");
  std::cout << pdb.current() << " elo on 2000/5/5= " << pdb.elo(QDate(2000,5,5)) << "\n";

  pdb.setCurrent("Aaberg, Anton");
  QString bio = pdb.biography();
  if (bio.isNull())
    std::cout << pdb.current() << " bio= null " << "\n";
  else
    std::cout << pdb.current() << " bio= " << bio << "\n";

  bool bl = pdb.exists("BYE");
  if (bl)
    std::cout << "player BYE exists \n";
  else
    std::cout << "player BYE does not exist \n";

  pdb.setCurrent("Azmaiparashvili, Zurab");
  std::cout << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";

  pdb.setCurrent("Babaev, Rashad");
  std::cout << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";

  pdb.setCurrent("Zysk, Robert");
  std::cout << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  std::cout << pdb.current() << " country= " << pdb.country() << "\n";
  std::cout << pdb.current() << " estimated overall elo= " << pdb.estimatedElo() << "\n";
  std::cout << pdb.current() << " elo on 2000/10/5= " << pdb.elo(QDate(2000,10,5)) << "\n";
  std::cout << pdb.current() << " estimated elo on 2000/10/5= " << pdb.estimatedElo(QDate(2000,10,5)) << "\n";
  std::cout << pdb.current() << " estimated elo on 2000/10/5= " << pdb.estimatedEloNoCache(QDate(2000,10,5)) << "\n";

  pdb.setCurrent("Rudolf, Michal");
  std::cout << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  std::cout << pdb.current() << " estimated overall elo= " << pdb.estimatedElo() << "\n";
  std::cout << pdb.current() << " elo on 2000/10/5= " << pdb.elo(QDate(2000,10,5)) << "\n";
  std::cout << pdb.current() << " elo on 2001/5/5= " << pdb.elo(QDate(2001,5,5)) << "\n";
  std::cout << pdb.current() << " elo on 2001/10/5= " << pdb.elo(QDate(2001,10,5)) << "\n";
  std::cout << pdb.current() << " elo on 2005/10/5= " << pdb.elo(QDate(2005,10,5)) << "\n";
  std::cout << pdb.current() << " elo on 1995/10/5= " << pdb.elo(QDate(1995,10,5)) << "\n";
  std::cout << pdb.current() << " elo on 1990/10/5= " << pdb.elo(QDate(1990,10,5)) << "\n";
  std::cout << pdb.current() << " estimated elo on 1990/10/5= " << pdb.estimatedElo(QDate(1990,10,5)) << "\n";
  std::cout << pdb.current() << " elo on 1997/1/1= " << pdb.elo(QDate(1997,1,1)) << "\n";
  std::cout << pdb.current() << " estimated elo on 1997/1/1= " << pdb.estimatedElo(QDate(1997,1,1)) << "\n";

  pdb.setCurrent("'t Jong, Ewoud");
  QImage img = pdb.photo();
  if (img.isNull())
    std::cout << pdb.current() << " photo == null \n";
  else
    std::cout << pdb.current() << " photo != null \n";

// tests for partial birth/death dates
  pdb.setCurrent("Abbott, Hedley");
  std::cout << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  std::cout << pdb.current() << " deathdate= " << pdb.dateOfDeath().asString() << "\n";
  pdb.setCurrent("Baay, JG");
//  std::cout << pdb.current() << " birthdate= " << pdb.dateOfBirth() << "\n";
  std::cout << pdb.current() << " deathdate= " << pdb.dateOfDeath().asString() << "\n";
  pdb.setCurrent("Barcza, Gideon");
  std::cout << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  std::cout << pdb.current() << " deathdate= " << pdb.dateOfDeath().asString() << "\n";
  pdb.setCurrent("Barden, Leonard");
  std::cout << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";


  pdb.close();
  std::cout << "closed " << db_name << "\n";
}

void querySmall(){
//query the small test database 
  PlayerDatabase pdb;
  if (pdb.open(db_name)) {
    std::cout << "opened " << db_name << "\n";
  }
  else{
    std::cout << "failure opening " << db_name << "\n";
    return;
  }

  std::cout << "number of players = " << pdb.count() <<"\n"; 
//  QStringList sl = pdb.playerNames();
//  for ( QStringList::Iterator it = sl.begin(); it != sl.end(); ++it ) {
//    std::cout << *it << "\n";
//  }

  std::cout << "max. 3 players starting with G: \n";
  QStringList sl = pdb.findPlayers("G",3);
  for ( QStringList::Iterator it = sl.begin(); it != sl.end(); ++it ) {
    std::cout << *it << "\n";
  }
  std::cout << "all players starting with G: \n";
  sl = pdb.findPlayers("G");
  for ( QStringList::Iterator it = sl.begin(); it != sl.end(); ++it ) {
    std::cout << *it << "\n";
  }

  pdb.setCurrent("Aaberg, Anton");
  std::cout << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  std::cout << pdb.current() << " country= " << pdb.country() << "\n";
  std::cout << pdb.current() << " title= " << pdb.title() << "\n";
  for (int i=1; i<80; i++){
  std::cout << pdb.current() << " elo on list " << i << "pr. " << pdb.eloListToDate(i).toString() << "= " << pdb.elo(i) << "\n";
  }
  std::cout << pdb.current() << " elo on 1991/1/1= " << pdb.elo(QDate(1991,1,1)) << "\n";
  std::cout << pdb.current() << " elo on 2000/5/5= " << pdb.elo(QDate(2000,5,5)) << "\n";
  std::cout << pdb.current() << " estimated elo on 2000/5/5= " << pdb.estimatedElo(QDate(2000,5,5)) << "\n";
  std::cout << pdb.current() << " estimated elo on 1975/5/5= " << pdb.estimatedElo(QDate(1975,5,5)) << "\n";
  std::cout << pdb.current() << " estimated elo on 1993/5/5= " << pdb.estimatedElo(QDate(1993,5,5)) << "\n";
  std::cout << pdb.current() << " overall estimated elo= " << pdb.estimatedElo() << "\n";

  QString bio = pdb.biography();
  if (bio.isNull())
    std::cout << pdb.current() << " bio= null " << "\n";
  else
    std::cout << pdb.current() << " bio= " << bio << "\n";

  pdb.setCurrent("Zysk, Robert");
  std::cout << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  std::cout << pdb.current() << " country= " << pdb.country() << "\n";
  std::cout << pdb.current() << " estimated overall elo= " << pdb.estimatedElo() << "\n";
  std::cout << pdb.current() << " elo on 2000/10/5= " << pdb.elo(QDate(2000,10,5)) << "\n";
  std::cout << pdb.current() << " estimated elo on 2000/10/5= " << pdb.estimatedElo(QDate(2000,10,5)) << "\n";

  pdb.setCurrent("Aaberg, Anton");
  QImage img = pdb.photo();
  if (img.isNull())
    std::cout << pdb.current() << " photo == null \n";
  else
    std::cout << pdb.current() << " photo != null \n";

  pdb.close();
  std::cout << "closed " << db_name << "\n";
}


void testPlayerDatabaseConversion(){
  bool b = DatabaseConversion::playerDatabaseFromScidRatings(source,db_name,picture); // the jpg is attached as photo to the first player in ratings.ssp
  if (!b){
    std::cout << "conversion failed\n";
    return;
  }

// query the converted database
  //queryNewest();
// use this to test the small one
querySmall();

  std::cout << "end of testPlayerDatabaseConversion\n";
}


int main( ) 
{
  testPlayerDatabaseConversion();
  return 0;
}


