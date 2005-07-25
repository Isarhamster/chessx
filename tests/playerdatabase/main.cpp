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

static QString db_name = "/home/ejner/chessX/players";
static QString source = "/home/ejner/chessX/ratings.ssp";
static QString picture = "/home/ejner/chessX/polgar.jpg";

void queryNewest(){
//query newest full base as of 25/7/2005 (70207 players)
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

  QString d1 = pdb.dateOfBirth("Aaberg, Anton");
  std::cout << "Aaberg, Anton birthdate= " << d1 << "\n";
  QString s = pdb.country("Aaberg, Anton");
  std::cout << "Aaberg, Anton country= " << s << "\n";
  s = pdb.title("Aaberg, Anton");
  std::cout << "Aaberg, Anton title= " << s << "\n";

  d1 = pdb.dateOfBirth("Aaron, Manuel");
  std::cout << "Aaron, Manuel birthdate= " << d1 << "\n";
  s = pdb.country("Aaron, Manuel");
  std::cout << "Aaron, Manuel country= " << s << "\n";
  s = pdb.title("Aaron, Manuel");
  std::cout << "Aaron, Manuel title= " << s << "\n";

  int elo = pdb.elo("Aaberg, Anton",QDate(2000,5,5));
  std::cout << "Aaberg, Anton elo on 2000/5/5= " << elo << "\n";
  elo = pdb.elo("Aaron, Manuel",QDate(2000,5,5));
  std::cout << "Aaron, Manuel elo on 2000/5/5= " << elo << "\n";

  QString bio = pdb.biography("Aaberg, Anton");
  if (bio.isNull())
    std::cout << "Aaberg, Anton bio= null " << "\n";
  else
    std::cout << "Aaberg, Anton bio= " << bio << "\n";

  bool bl = pdb.exists("BYE");
  if (bl)
    std::cout << "player BYE exists \n";
  else
    std::cout << "player BYE does not exist \n";

  d1 = pdb.dateOfBirth("Azmaiparashvili, Zurab");
  std::cout << "Azmaiparashvili, Zurab birthdate= " << d1 << "\n";
  d1 = pdb.dateOfBirth("Babaev, Rashad");
  std::cout << "Babaev, Rashad birthdate= " << d1 << "\n";
  d1 = pdb.dateOfBirth("Zysk, Robert");
  std::cout << "Zysk, Robert birthdate= " << d1 << "\n";
  d1 = pdb.country("Zysk, Robert");
  std::cout << "Zysk, Robert country= " << d1 << "\n";

  d1 = pdb.dateOfBirth("Rudolf, Michal");
  std::cout << "Rudolf, Michal birthdate= " << d1 << "\n";
  elo = pdb.elo("Rudolf, Michal",QDate(2000,10,5));
  std::cout << "MR elo on 2000/10/5= " << elo << "\n";
  elo = pdb.elo("Rudolf, Michal",QDate(2001,5,5));
  std::cout << "MR elo on 2001/5/5= " << elo << "\n";
  elo = pdb.elo("Rudolf, Michal",QDate(2001,10,5));
  std::cout << "MR elo on 2001/10/5= " << elo << "\n";
  elo = pdb.elo("Rudolf, Michal",QDate(2005,10,5));
  std::cout << "MR elo on 2005/10/5= " << elo << "\n";
  elo = pdb.elo("Rudolf, Michal",QDate(1995,10,5));
  std::cout << "MR elo on 1995/10/5= " << elo << "\n";
  elo = pdb.elo("Rudolf, Michal",QDate(1990,10,5));
  std::cout << "MR elo on 1990/10/5= " << elo << "\n";
  elo = pdb.elo("Rudolf, Michal",QDate(1997,1,1));
  std::cout << "MR elo on 1997/1/1= " << elo << "\n";

  QImage img = pdb.photo("'t Jong, Ewoud");
  if (img.isNull())
    std::cout << "'t Jong, Ewoud photo == null \n";
  else
    std::cout << "'t Jong, Ewoud photo != null \n";

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

  QStringList sl = pdb.findPlayers("G",3);
  for ( QStringList::Iterator it = sl.begin(); it != sl.end(); ++it ) {
    std::cout << *it << "\n";
  }

  QString d1 = pdb.dateOfBirth("Aaberg, Anton");
  std::cout << "Aaberg, Anton birthdate= " << d1 << "\n";
  QString s = pdb.country("Aaberg, Anton");
  std::cout << "Aaberg, Anton country= " << s << "\n";
  s = pdb.title("Aaberg, Anton");
  std::cout << "Aaberg, Anton title= " << s << "\n";

  int elo = pdb.elo("Aaberg, Anton",QDate(2000,5,5));
  std::cout << "Aaberg, Anton elo on 2000/5/5= " << elo << "\n";
  elo = pdb.elo("Aaron, Manuel",QDate(2000,5,5));
  std::cout << "Aaron, Manuel elo on 2000/5/5= " << elo << "\n";

  QString bio = pdb.biography("Aaberg, Anton");
  if (bio.isNull())
    std::cout << "Aaberg, Anton bio= null " << "\n";
  else
    std::cout << "Aaberg, Anton bio= " << bio << "\n";

  std::cout << "Zysk, Robert birthdate= " << d1 << "\n";
  d1 = pdb.country("Zysk, Robert");
  std::cout << "Zysk, Robert country= " << d1 << "\n";

  elo = pdb.elo("Zysk, Robert",QDate(2000,10,5));
  std::cout << "Zysk, Robert elo on 2000/10/5= " << elo << "\n";

  QImage img = pdb.photo("Aaberg, Anton");
  if (img.isNull())
    std::cout << "Aaberg, Anton photo == null \n";
  else
    std::cout << "Aaberg, Anton photo != null \n";

  pdb.close();
  std::cout << "closed " << db_name << "\n";
}


void testPlayerDatabaseConversion(){
// The query section fits with the newest ratings.ssp
// from http://members.aon.at/schachverein.steyr/ratings.ssp.zip

  bool b = DatabaseConversion::playerDatabaseFromScidRatings(source,db_name,picture); // the jpg is attached as photo to the first player in ratings.ssp
  if (!b){
    std::cout << "conversion failed\n";
    return;
  }

// query the converted database
//  queryNewest();
// use this to test the small one
 querySmall();

  std::cout << "end of testPlayerDatabaseConversion\n";
}


int main( ) 
{
  testPlayerDatabaseConversion();
  return 0;
}


