// main() to convert a scid ratings.ssp to our player db format,
// and test the content of the resulting player database.
//
// To use it, change the parameters to
// DatabaseConversion::PlayerDatabaseFromScidRatings()
// from "~ejner/" to point at your actual directory and files.


#include <iostream>
#include "databaseconversion.h"


void testPlayerDatabaseConversion(){
  QString db_name = "/home/ejner/chessX/players";

  int x = DatabaseConversion::PlayerDatabaseFromScidRatings("/home/ejner/chessX/ratings.ssp",db_name,"/home/ejner/chessX/polgar.jpg"); // the jpg is attached as photo to the first player in ratings.ssp
  if (x!=0){
    std::cout << "conversion failed\n";
    return;
  }

// query the converted database

  PlayerDatabase pdb;
  if (pdb.open(db_name)) {
    std::cout << "opened " << db_name << "\n";
  }
  else{
    std::cout << "failure opening " << db_name << "\n";
    return;
  }

  std::cout << "number of players = " << pdb.numberOfPlayers() <<"\n"; 
  QStringList sl = pdb.playerNames();

//  for (int i=0; i<sl.size()&&i<5; i++ ) {
//    std::cout << sl[i] << "\n";
//  }
//  int i=0;
//  for ( QStringList::Iterator it = sl.begin(); it != sl.end(); ++it ) {
//    std::cout << *it << "\n";
    //i++;
//  }

  QString d1 = pdb.dateOfBirth("Aaberg, Anton");
  std::cout << "Aaberg, Anton birthdate= " << d1 << "\n";
  QString s = pdb.country("Aaberg, Anton");
  std::cout << "Aaberg, Anton country= " << s << "\n";
  s = pdb.title("Aaberg, Anton");
  std::cout << "Aaberg, Anton title= " << s << "\n";

  d1 = pdb.dateOfBirth("Zysk, Robert");
  std::cout << "Zysk, Robert birthdate= " << d1 << "\n";
  d1 = pdb.country("Zysk, Robert");
  std::cout << "Zysk, Robert country= " << d1 << "\n";


  pdb.close();
  std::cout << "closed " << db_name << "\n";

  std::cout << "end of testPlayerDatabaseConversion\n";
}


int main( ) 
{
testPlayerDatabaseConversion();
  return 0;
}


