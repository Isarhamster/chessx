/***************************************************************************
                          filter.cpp  -  holds search results in memory
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

#include <filter.h>

Filter::Filter(const int size, const Filter::WhichGames whichGames){
   m_currentIndex = 0;
   if (whichGames==AllGames){
//Qt3 does not have QByteArray(size,ch)
     m_byteArray = new QByteArray(size);
     for (int i=0; i<size; i++){
       (*m_byteArray)[i]=1;
     }
     m_count = size;
     m_firstIndex = 0;
     m_lastIndex = size-1;
   }
   else{
     m_byteArray = new QByteArray(size);
     for (int i=0; i<size; i++){
       (*m_byteArray)[i]=0;
     }
     m_count = 0;
     m_firstIndex = -1;
     m_lastIndex = -1;
   }
}

Filter::Filter(const Filter& filter){
   m_byteArray = new QByteArray(filter.size());
   m_count = filter.count();
   m_firstIndex = filter.firstIndex();
   m_lastIndex = filter.lastIndex();
   m_currentIndex = filter.currentIndex();
   for (int i=0; i<filter.size(); i++){
     (*m_byteArray)[i]=filter.moveIndex(i);
   }
}

bool Filter::set(const int game, const int moveIndex){
   if (game<0 || game>m_byteArray->count() || moveIndex<1){
     return false;
   }
   else{
     if (m_byteArray->at(game) == 0){
       m_count++;
       if (m_count==1){
         m_firstIndex = game;
         m_lastIndex = game;
       }
       else{
         m_firstIndex = MIN(m_firstIndex,game);
         m_lastIndex = MAX(m_lastIndex,game);
       }
       m_currentIndex = game;
     }
     (*m_byteArray)[game]=moveIndex;
     return true;
   }
}

bool Filter::remove(const int game){
   if (game<0 || game>m_byteArray->count()){
     return false;
   }
   else{
     if (m_byteArray->at(game) > 0){
       m_count--;
       (*m_byteArray)[game]=0;
       if (game==m_firstIndex){
         for(int i=game;i<m_byteArray->count();i++){
           if (m_byteArray->at(i) > 0){
             m_firstIndex = i;
             break;
           }
         }
       }
       if (game==m_lastIndex){
         for(int i=game;i>=0;i--){
           if (m_byteArray->at(i) > 0){
             m_lastIndex = i;
             break;
           }
         }
       }
     }
     return true;
   }
}

int Filter::count() const{
   return m_count;
}
int Filter::size() const{
   return m_byteArray->count();
}

bool Filter::contains(const int game) const{
   return (m_byteArray->at(game) > 0);
}
int Filter::moveIndex(const int game) const{
   return m_byteArray->at(game);
}

int Filter::currentIndex() const{
   return m_currentIndex;
}
bool Filter::setCurrentIndex(const int index) {
   if (index<0||index>=m_byteArray->count()||m_byteArray->at(index)==0){
     return false;
   }
   else{
     m_currentIndex = index;
     return true;
   }
}
int Filter::lastIndex() const{
   return m_lastIndex;
}
int Filter::firstIndex() const{
   return m_firstIndex;
}

void Filter::toFirst(){
   m_currentIndex = m_firstIndex;
}

void Filter::toLast(){
   m_currentIndex = m_lastIndex;
}

int Filter::previousGame(const int index, const int offset) const{
   if (index<=m_firstIndex||m_count==0||index>m_byteArray->count()||offset<0){//there is no previous game
     return -1;
   }
   if (offset==0){
     return index;
   }
   int result=-1;
   int count=0;
   for (int i=index-1; i>=0; i--){
     if (m_byteArray->at(i) > 0){
       count++;
       if (count==offset){
         result=i;
         break;
       }
     }
   }
   return result;
}
int Filter::nextGame(const int index, const int offset) const{
   if (index>=m_lastIndex||m_count==0||index<-1||offset<0){//there is no next game
     return -1;
   }
   if (offset==0){
     return index;
   }
   int result=-1;
   int count=0;
   for (int i=index+1; i<m_byteArray->count(); i++){
     if (m_byteArray->at(i) > 0){
       count++;
       if (count==offset){
         result=i;
         break;
       }
     }
   }
   return result;
}
void Filter::reverse(){
   m_count = m_byteArray->count() - m_count;
   m_firstIndex = -1;
   m_lastIndex = -1;
   m_currentIndex = -1;
   for (int i=0; i<m_byteArray->count(); i++){
     if (m_byteArray->at(i) == 0){
       (*m_byteArray)[i]=1;
       if (m_firstIndex==-1){
         m_firstIndex = i;
         m_lastIndex = i;
         m_currentIndex = i;
       }
       else{
         m_lastIndex = i;
       }
     }
     else{
       (*m_byteArray)[i]=0;
     }
   }
}
void Filter::intersect(const Filter& filter){
   m_firstIndex = -1;
   m_lastIndex = -1;
   m_currentIndex = -1;
   for (int i=0; i<m_byteArray->count(); i++){
     if (m_byteArray->at(i) > 0){
       if (filter.contains(i)){
         if (m_firstIndex==-1){
           m_firstIndex = i;
           m_lastIndex = i;
           m_currentIndex = i;
         }
         else{
           m_lastIndex = i;
         }
       }
       else{
         m_count--;
         (*m_byteArray)[i]=0;
       }
     }
   }
}
void Filter::add(const Filter& filter){
   m_firstIndex = MIN(m_firstIndex,filter.firstIndex());
   m_lastIndex = MAX(m_lastIndex,filter.lastIndex());
   for (int i=0; i<m_byteArray->count(); i++){
     if (m_byteArray->at(i) == 0){
       if (filter.contains(i)){
         m_count++;
         (*m_byteArray)[i]=filter.moveIndex(i);
       }
     }
   }
}
void Filter::remove(const Filter& filter){
   m_firstIndex = -1;
   m_lastIndex = -1;
   m_currentIndex = -1;
   for (int i=0; i<m_byteArray->count(); i++){
     if (m_byteArray->at(i) > 0){
       if (filter.contains(i)){
         m_count--;
         (*m_byteArray)[i]=0;
       }
       else{
         if (m_firstIndex==-1){
           m_firstIndex = i;
           m_lastIndex = i;
           m_currentIndex = i;
         }
         else{
           m_lastIndex = i;
         }
       }
     }
   }
}
