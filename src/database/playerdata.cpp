/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 28/05/2005
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
#include <playerdata.h>

PlayerData::PlayerData(){
 m_estimatedElo=0;
 m_peakElo=0;
 m_firstEloListIndex=0;
 m_lastEloListIndex=0;
}

PartialDate PlayerData::dateOfBirth() const{
  return m_dateOfBirth;
}
void PlayerData::setDateOfBirth(const PartialDate d){
  m_dateOfBirth = d;
}
PartialDate PlayerData::dateOfDeath() const{
  return m_dateOfDeath;
}
void PlayerData::setDateOfDeath(const PartialDate d){
  m_dateOfDeath = d;
}

QString PlayerData::country() const{
  return m_country;
}
void PlayerData::setCountry(const QString s){
  m_country = s;
}

QString PlayerData::title() const{
  return m_title;
}
void PlayerData::setTitle(const QString s){
  m_title = s;
}

int PlayerData::elo(const int eloListIndex) const{
   if (m_elo.contains(eloListIndex)){
     QMapConstIterator<int,int> it = m_elo.find(eloListIndex);
     return it.data();
   }
   else{
     return 0;
   }
}


int PlayerData::firstEloListIndex(){
  return m_firstEloListIndex;
}
void PlayerData::setFirstEloListIndex(const int ix){
  m_firstEloListIndex=ix;
}
int PlayerData::lastEloListIndex(){
  return m_lastEloListIndex;
}
void PlayerData::setLastEloListIndex(const int ix){
  m_lastEloListIndex=ix;
}

void PlayerData::setElo(const int eloListIndex, const int elo){
   m_elo.insert(eloListIndex,elo);
   if (eloListIndex<m_firstEloListIndex || m_firstEloListIndex==0)
     m_firstEloListIndex=eloListIndex;
   if (eloListIndex>m_lastEloListIndex || m_lastEloListIndex==0)
     m_lastEloListIndex=eloListIndex;
   m_estimatedEloCache.clear();
}

int PlayerData::peakElo() const{
  return m_peakElo;
}
void PlayerData::setPeakElo(const int elo){
  m_peakElo = elo;
}

int PlayerData::estimatedElo() const{
  return m_estimatedElo;
}
void PlayerData::setEstimatedElo(const int elo){
  m_estimatedElo = elo;
  m_estimatedEloCache.clear();
}


int PlayerData::estimatedElo(const int eloListIndex){
   QMapConstIterator<int,int> it;
   if (m_elo.contains(eloListIndex)){
     it = m_elo.find(eloListIndex);
     return it.data();
   }
   else{
     if (m_estimatedEloCache.contains(eloListIndex)){//use cached result
       it = m_estimatedEloCache.find(eloListIndex);
       return it.data();
     }
     else{//search in previous elo lists
       int result=m_estimatedElo;//default to overall estimate
       for ( int i=eloListIndex-1; i>0; --i ) {
         if (m_elo.contains(i)){
           it = m_elo.find(i);
           result = it.data();
           break;
         }
       }
       m_estimatedEloCache.insert(eloListIndex,result);//cache result
       return result;
     }
   }
}

int PlayerData::estimatedEloNoCache(const int eloListIndex) const{
   QMapConstIterator<int,int> it;
   if (m_elo.contains(eloListIndex)){
     it = m_elo.find(eloListIndex);
     return it.data();
   }
   else{
     if (m_estimatedEloCache.contains(eloListIndex)){//use cached result
       it = m_estimatedEloCache.find(eloListIndex);
       return it.data();
     }
     else{//search in previous elo lists
       int result=m_estimatedElo;//default to overall estimate
       for ( int i=eloListIndex-1; i>0; --i ) {
         if (m_elo.contains(i)){
           it = m_elo.find(i);
           result = it.data();
           break;
         }
       }
       return result;
     }
   }
}

QImage PlayerData::photo() const{
  return m_photo;
}
void PlayerData::setPhoto(const QImage img){
  m_photo = img;
}

QString PlayerData::biography() const{
  return m_biography;
}
void PlayerData::setBiography(const QString str){
  m_biography = str;
}
void PlayerData::appendToBiography(const QString str){
  m_biography += str;
}

QValueList<Q_INT32> PlayerData::eloListData() const{
   QValueList<Q_INT32> list;
   QMapConstIterator<int,int> it;
   int nextListIx = 1;

   for ( it = m_elo.begin(); it != m_elo.end(); ++it ) {
     int listIx = it.key();
     if (listIx>nextListIx){
       list.push_back((Q_INT32)-9999);
       list.push_back((Q_INT32)(listIx-nextListIx));//number of lists without the player
     }
     list.push_back((Q_INT32)it.data());//elo
     nextListIx = listIx+1;
   }

   return list;
}

void PlayerData::eloFromListData(QValueList<Q_INT32> eloListData){
    int listIx=1;
    Q_INT32 elem;
    for(uint i=0; i< (uint)eloListData.size();){
      elem = eloListData[i++];
      if (elem==-9999){
        elem = eloListData[i++];
        listIx += elem;
      }
      else{
        m_elo.insert(listIx,elem);
        listIx++;
      }
    }
}
