#include "game.h"
#include <iostream.h>
#include <qcstring.h>

Game::Game(){
}
void Game::addTag(const QString & s){
   if (debug)
     std::cout<<"add tag: "<<s<<"\n";
}
void Game::addInitialComment(const QString & s) {
   if (debug)
     std::cout<<"add initial comment: "<<s<<"\n";
}
void Game::addComment(const QString & s) {
   if (debug){
      QCString qc = s.local8Bit();
      std::cout<<"add comment: "<<qc<<"\n";
   }
}
void Game::addMove(const QString & s) {
   if (debug)
     std::cout<<"add move: "<<s<<"\n";
}
void Game::addResult(const QString & s) {
   if (debug)
     std::cout<<"add result: "<<s<<"\n";
}
void Game::addNag(const QString & s) {
   if (debug)
     std::cout<<"add nag: "<<s<<"\n";
}
bool Game::validateMoveNumber(const QString & s) {
   if (debug)
     std::cout<<"validating move number: "<<s<<"\n";
   return true;
}
void Game::enterVariation() {
   if (debug)
     std::cout<<"entering variation... "<<"\n";
}
void Game::exitVariation() {
   if (debug)
     std::cout<<"exiting variation... "<<"\n";
}
void Game::addAnnotationSymbol(const QString & s){
   if (debug)
     std::cout<<"adding annotation symbol: "<<s<<"\n";
}
