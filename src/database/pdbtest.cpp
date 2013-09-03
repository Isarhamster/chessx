// main() to convert a scid ratings.ssp to our player db format,
// and test the content of the resulting player database.
//
// To use it, change the db_name, source and picture directory to fit
// your filenames. The picture filenames should be = player name + extension
// fe. "Larsen, Bent.gif"  (any format that QImage(file) can handle will work).
//
// Code to query the small test database - see querySmall().
// Code to query the newest full database - see queryNewest().

#include <qapplication.h>
#include <qtextstream.h>
#include "databaseconversion.h"

static QTextStream out(stdout, QIODevice::WriteOnly);

static QString db_name = "players";
static QString source = "ratings.ssp";
static QString picture_dir = "playerphotos";

void queryNewest(){
//query newest full base as of 25/7/2005 (70207 players)
// available from http://members.aon.at/schachverein.steyr/ratings.ssp.zip
  PlayerDatabase pdb;
  if (pdb.open(db_name)) {
    out << "opened " << db_name << "\n";
  }
  else{
    out << "failure opening " << db_name << "\n";
    return;
  }

  out << "number of players = " << pdb.count() <<"\n";

  QStringList sl = pdb.findPlayers("Hu",3);
  for ( QStringList::Iterator it = sl.begin(); it != sl.end(); ++it ) {
    out << *it << "\n";
  }

  pdb.setCurrent("Aaberg, Anton");
  out << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  QString s = pdb.country();
  out << pdb.current() << " country= " << s << "\n";
  s = pdb.title();
  out << pdb.current() << " title= " << s << "\n";

  pdb.setCurrent("Aaron, Manuel");
  out << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  out << pdb.current() << " country= " << pdb.country() << "\n";
  out << pdb.current() << " title= " << pdb.title() << "\n";

  pdb.setCurrent("Aaberg, Anton");
  out << pdb.current() << " elo on 2000/5/5= " << pdb.elo(PartialDate(2000,5,5)) << "\n";

  pdb.setCurrent("Aaron, Manuel");
  out << pdb.current() << " elo on 2000/5/5= " << pdb.elo(PartialDate(2000,5,5)) << "\n";

  pdb.setCurrent("Aaberg, Anton");
  QString bio = pdb.biography();
  if (bio.isNull())
    out << pdb.current() << " bio= null " << "\n";
  else
    out << pdb.current() << " bio= " << bio << "\n";

  bool bl = pdb.exists("BYE");
  if (bl)
    out << "player BYE exists \n";
  else
    out << "player BYE does not exist \n";

  pdb.setCurrent("Azmaiparashvili, Zurab");
  out << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";

  pdb.setCurrent("Babaev, Rashad");
  out << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";

  pdb.setCurrent("Zysk, Robert");
  out << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  out << pdb.current() << " country= " << pdb.country() << "\n";
  out << pdb.current() << " estimated overall elo= " << pdb.estimatedElo() << "\n";
  out << pdb.current() << " elo on 2000/10/5= " << pdb.elo(PartialDate(2000,10,5)) << "\n";
  out << pdb.current() << " estimated elo on 2000/10/5= " << pdb.estimatedElo(PartialDate(2000,10,5)) << "\n";
  out << pdb.current() << " estimated elo on 2000/10/5= " << pdb.estimatedEloNoCache(PartialDate(2000,10,5)) << "\n";

  pdb.setCurrent("Rudolf, Michal");
  out << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  out << pdb.current() << " estimated overall elo= " << pdb.estimatedElo() << "\n";
  out << pdb.current() << " elo on 2000/10/5= " << pdb.elo(PartialDate(2000,10,5)) << "\n";
  out << pdb.current() << " elo on 2001/5/5= " << pdb.elo(PartialDate(2001,5,5)) << "\n";
  out << pdb.current() << " elo on 2001/10/5= " << pdb.elo(PartialDate(2001,10,5)) << "\n";
  out << pdb.current() << " elo on 2005/10/5= " << pdb.elo(PartialDate(2005,10,5)) << "\n";
  out << pdb.current() << " elo on 1995/10/5= " << pdb.elo(PartialDate(1995,10,5)) << "\n";
  out << pdb.current() << " elo on 1990/10/5= " << pdb.elo(PartialDate(1990,10,5)) << "\n";
  out << pdb.current() << " estimated elo on 1990/10/5= " << pdb.estimatedElo(PartialDate(1990,10,5)) << "\n";
  out << pdb.current() << " elo on 1997/1/1= " << pdb.elo(PartialDate(1997,1,1)) << "\n";
  out << pdb.current() << " estimated elo on 1997/1/1= " << pdb.estimatedElo(PartialDate(1997,1,1)) << "\n";

  pdb.setCurrent("'t Jong, Ewoud");
  QImage img = pdb.photo();
  if (img.isNull())
    out << pdb.current() << " photo == null \n";
  else
    out << pdb.current() << " photo != null \n";

// tests for partial birth/death dates
  pdb.setCurrent("Abbott, Hedley");
  out << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  out << pdb.current() << " deathdate= " << pdb.dateOfDeath().asString() << "\n";
  pdb.setCurrent("Baay, JG");
//  out << pdb.current() << " birthdate= " << pdb.dateOfBirth() << "\n";
  out << pdb.current() << " deathdate= " << pdb.dateOfDeath().asString() << "\n";
  pdb.setCurrent("Barcza, Gideon");
  out << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  out << pdb.current() << " deathdate= " << pdb.dateOfDeath().asString() << "\n";
  pdb.setCurrent("Barden, Leonard");
  out << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";


  pdb.close();
  out << "closed " << db_name << "\n";
}

void querySmall(){
//query the small test database
  PlayerDatabase pdb;
  if (pdb.open(db_name)) {
    out << "opened " << db_name << "\n";
  }
  else{
    out << "failure opening " << db_name << "\n";
    return;
  }

  out << "number of players = " << pdb.count() <<"\n";

  out << "max. 3 players starting with G: \n";
  QStringList sl = pdb.findPlayers("G",3);
  for ( QStringList::Iterator it = sl.begin(); it != sl.end(); ++it ) {
    out << *it << "\n";
  }
  out << "all players starting with G: \n";
  sl = pdb.findPlayers("G");
  for ( QStringList::Iterator it = sl.begin(); it != sl.end(); ++it ) {
    out << *it << "\n";
  }

  pdb.setCurrent("Aaberg, Anton");
  out << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  out << pdb.current() << " country= " << pdb.country() << "\n";
  out << pdb.current() << " title= " << pdb.title() << "\n";
  int start = pdb.firstEloListIndex();
  int end = pdb.lastEloListIndex();
  for (int i=start; i<=end; i++){
  out << pdb.current() << " elo on list " << i << "pr. " << pdb.eloListToDate(i).asString() << "= " << pdb.elo(i) << "\n";
  }
  for (int i=start; i<=end; i++){
  out << pdb.current() << " elo on list - or estimate " << i << "pr. " << pdb.eloListToDate(i).asString() << "= " << pdb.estimatedElo(pdb.eloListToDate(i)) << "\n";
  }
  out << pdb.current() << " elo on 1991/1/1= " << pdb.elo(PartialDate(1991,1,1)) << "\n";
  out << pdb.current() << " elo on 2000/5/5= " << pdb.elo(PartialDate(2000,5,5)) << "\n";
  out << pdb.current() << " estimated elo on 2000/5/5= " << pdb.estimatedElo(PartialDate(2000,5,5)) << "\n";
  out << pdb.current() << " estimated elo on 1975/5/5= " << pdb.estimatedElo(PartialDate(1975,5,5)) << "\n";
  out << pdb.current() << " estimated elo on 1993/5/5= " << pdb.estimatedElo(PartialDate(1993,5,5)) << "\n";
  out << pdb.current() << " overall estimated elo= " << pdb.estimatedElo() << "\n";

  QString bio = pdb.biography();
  if (bio.isNull())
    out << pdb.current() << " bio= null " << "\n";
  else
    out << pdb.current() << " bio= " << bio << "\n";

  pdb.setCurrent("Zysk, Robert");
  out << pdb.current() << " birthdate= " << pdb.dateOfBirth().asString() << "\n";
  out << pdb.current() << " country= " << pdb.country() << "\n";
  out << pdb.current() << " estimated overall elo= " << pdb.estimatedElo() << "\n";
  out << pdb.current() << " elo on 2000/10/5= " << pdb.elo(PartialDate(2000,10,5)) << "\n";
  out << pdb.current() << " estimated elo on 2000/10/5= " << pdb.estimatedElo(PartialDate(2000,10,5)) << "\n";
  if (pdb.hasPhoto())
    out << pdb.current() << " has photo \n";
  else
    out << pdb.current() << " has no photo \n";
  if (pdb.hasBiography())
    out << pdb.current() << " has biography \n";
  else
    out << pdb.current() << " has no biography \n";


  pdb.setCurrent("Aaberg, Anton");
  if (pdb.hasPhoto())
    out << pdb.current() << " has photo \n";
  else
    out << pdb.current() << " has no photo \n";
  QImage img = pdb.photo();
  if (img.isNull())
    out << pdb.current() << " has no photo \n";
  else
    out << pdb.current() << " has photo \n";

  if (pdb.hasBiography())
    out << pdb.current() << " has bio \n";
  else
    out << pdb.current() << " has no bio\n";
  bio = pdb.biography();
  if (bio.isNull())
    out << pdb.current() << " has no bio \n";
  else
    out << pdb.current() << " has bio \n";

  pdb.setCurrent("Adams, Michael");
  if (pdb.hasPhoto())
    out<< pdb.current() << " has a photo\n";
  pdb.setCurrent("Polgar, Judit");
  if (pdb.hasPhoto())
    out<< pdb.current() << " has a photo\n";
  pdb.setCurrent("Anderssen, Adolf");
  if (pdb.hasPhoto())
    out<< pdb.current() << " has a photo\n";
// checking overall estimated elo for a historic player
  out << pdb.current() << " estimated elo= " << pdb.estimatedElo() << "\n";
  pdb.setCurrent("Botvinnik, Mikhail URS");
  if (pdb.hasPhoto())
    out<< pdb.current() << " has a photo\n";


  pdb.close();
  out << "closed " << db_name << "\n";
}


void testPlayerDatabaseConversion(){
  bool b = DatabaseConversion::playerDatabaseFromScidRatings(source,db_name,picture_dir); // the jpg is attached as photo to the first player in ratings.ssp
  if (!b){
    out << "conversion failed\n";
    return;
  }

// query the converted database
  queryNewest();
// use this to test the small one
// querySmall();

  out << "end of testPlayerDatabaseConversion\n";
}




