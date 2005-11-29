/***************************************************************************
                          main.cpp  -  testing of filter class
                             -------------------
    begin                : 27/11/2005
    copyright            : (C) 2005 Ejner Borgbjerg 
                           <ejner@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qapplication.h>
#include "filter.h"
#include <qtextstream.h>

/* main() for testing the Filter class
*/

static QTextStream out(stdout, IO_WriteOnly);


int main( int argc, char ** argv ) 
{
  out<<"testing creation and basic manipulations"<<"\n";

  Filter f1(10,false);
  out<<"f1.count()="<<f1.count()<<"\n";
  if (!f1.set(0,5))
     out<<"error!\n";
  if (!f1.set(1,8))
     out<<"error!\n";
  if (!f1.set(2,1))
     out<<"error1!\n";
  out<<"f1.count()="<<f1.count()<<"\n";
  out<<"f1.firstIndex="<<f1.firstIndex()<<"\n";
  out<<"f1.lastIndex="<<f1.lastIndex()<<"\n";
  if (!f1.remove(0))
     out<<"error2!\n";
  if (!f1.remove(1))
     out<<"error3!\n";
  out<<"f1.count()="<<f1.count()<<"\n";
  out<<"f1.size()="<<f1.size()<<"\n";
  out<<"f1.firstIndex="<<f1.firstIndex()<<"\n";
  out<<"f1.lastIndex="<<f1.lastIndex()<<"\n";

  Filter f2(f1);
  out<<"f2.count()="<<f2.count()<<"\n";
  out<<"f2.size()="<<f2.size()<<"\n";
  out<<"f2.firstIndex="<<f2.firstIndex()<<"\n";
  out<<"f2.lastIndex="<<f2.lastIndex()<<"\n";
  if (!f2.set(9,1))
     out<<"error4!\n";
  if (!f2.set(8,2))
     out<<"error!\n";
  if (!f2.set(7,9))
     out<<"error5!\n";
  out<<"f2.count()="<<f2.count()<<"\n";
  if (!f2.remove(9))
     out<<"error6!\n";
  out<<"f2.count()="<<f2.count()<<"\n";
  out<<"f2.size()="<<f2.size()<<"\n";
  out<<"f2.firstIndex="<<f2.firstIndex()<<"\n";
  out<<"f2.lastIndex="<<f2.lastIndex()<<"\n";


  for(int i=0;i<f2.size();i++){
    if (f2.contains(i)){
      out<<"f2 contains "<<i<<" at moveIndex: "<<f2.moveIndex(i)<<"\n";
    }
  }

  f2.toFirst();
  out<<"f2.currentIndex="<<f2.currentIndex()<<"\n";
  f2.toLast();
  out<<"f2.currentIndex="<<f2.currentIndex()<<"\n";

  int i=-2;
  int j=0;//offset
  out<<"testing nextGame()"<<"\n";
  while(j<10 && i!=f2.lastIndex()){
    i=f2.nextGame(f2.firstIndex(),j);
    out<<"next game was ="<<i<<"\n";
    j++;
  }

  i=-2;
  j=0;//offset
  out<<"testing previousGame()"<<"\n";
  while(j<10 && i!=f2.firstIndex()){
    i=f2.previousGame(f2.lastIndex(),j);
    out<<"previous game was ="<<i<<"\n";
    j++;
  }


  out<<"testing set manipulations"<<"\n";
  f2.reverse();
  for(int i=0;i<f2.size();i++){
    if (f2.contains(i)){
      out<<"f2 contains "<<i<<" at moveIndex: "<<f2.moveIndex(i)<<"\n";
    }
  }

  for(int i=0;i<f1.size();i++){
    if (f1.contains(i)){
      out<<"f1 contains "<<i<<" at moveIndex: "<<f1.moveIndex(i)<<"\n";
    }
  }

  f1.intersect(f2);
  out<<"f1.count()= "<<f1.count()<<"\n";
  for(int i=0;i<f1.size();i++){
    if (f1.contains(i)){
      out<<"f1 contains "<<i<<" at moveIndex: "<<f1.moveIndex(i)<<"\n";
    }
  }

  f1.add(f2);
  out<<"f1.count()= "<<f1.count()<<"\n";
  for(int i=0;i<f1.size();i++){
    if (f1.contains(i)){
      out<<"f1 contains "<<i<<" at moveIndex: "<<f1.moveIndex(i)<<"\n";
    }
  }

  f1.remove(f2);
  out<<"f1.count()= "<<f1.count()<<"\n";
  for(int i=0;i<f1.size();i++){
    if (f1.contains(i)){
      out<<"f1 contains "<<i<<" at moveIndex: "<<f1.moveIndex(i)<<"\n";
    }
  }

  return 0;
}
