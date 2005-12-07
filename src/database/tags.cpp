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

#include <tags.h>
//#include <iostream>

static Q_UINT32 Magic = (Q_UINT32)0x00DDB0DE; // 'magic' number
static Q_UINT32 Version = (Q_UINT32)100; // file format version
static QString Tagfile_suffix = ".ctf";
static char* knownTagNames[]={"Event","Site", "Date", "Round", "Name","Result",
"Title", "Elo", "USCF", "NA", "Type", "EventDate", "EventSponsor", "Section", "Stage", "Board",
"Opening", "Variation", "Subvariation", "ECO", "NIC", "Time", "UTCTime", "UTCDate", 
"TimeControl", "SetUp", "FEN", "Termination","Annotator", "Mode", "PlyCount", 
"Source"
};


bool Tags::create(const QString& fname) {
  m_tags_file.setName(fname+Tagfile_suffix);
  if (m_tags_file.exists())
     return false;
// set which QDataset version format to use
  if (Version==(Q_UINT32)100){
    m_tags_ds.setVersion(6);
  }
  else{
    m_tags_ds.setVersion(6);//default
  }
  m_tags_file.open( IO_ReadWrite );
  m_tags_ds.setDevice(&m_tags_file);
  m_tags_ds << Magic;
  m_tags_ds << Version;
  m_tags_file.flush();
  m_allTags.clear();
  for (uint i=0; i<Source; i++){
    QValueVector<QString> first;
    QMap<QString,int> second;
    QPair <QValueVector<QString>, QMap <QString,int> > elm = qMakePair(first,second); 
    m_allTags.insert(i,elm);
  }
  return true;
}


bool Tags::open(const QString& fname) {
  m_tags_file.setName(fname+Tagfile_suffix);
  if (!m_tags_file.open( IO_ReadWrite ))
    return false;
  m_tags_ds.setDevice(&m_tags_file);
  Q_UINT32 tag_magic;
  Q_UINT32 tag_version;
  m_tags_ds >> tag_magic;
  m_tags_ds >> tag_version;
  if (tag_magic!=Magic){
    m_tags_ds.unsetDevice();
    m_tags_file.close();
    return false;
  }
// set which QDataset version format to use
  if (tag_version==(Q_UINT32)100){
    m_tags_ds.setVersion(6);
  }
  else{//unknown version
    m_tags_ds.unsetDevice();
    m_tags_file.close();
    return false;
  }
  m_allTags.clear();
  for (uint i=0; i<Source; i++){
    QValueVector<QString> first;
    QMap<QString,int> second;
    QPair <QValueVector<QString>, QMap <QString,int> > elm = qMakePair(first,second); 
    m_allTags.insert(i,elm);
  }
  Q_UINT8 tagId;
  Q_UINT32 index;
  QString name;
  char* c_name;
  while (!m_tags_ds.atEnd()){
    m_tags_ds >> tagId;
    m_tags_ds >> index;
    m_tags_ds >> c_name;
    name = c_name;//work with QString internally
    if (index>0){// 0 means value was deleted
      index--;
      m_allTags[tagId].second.insert(name,(int)index);
      m_allTags[tagId].first.push_back(name);
    }
    else{
      m_allTags[tagId].second.remove(name);
    }
  }
  return true;
}

bool Tags::removeTagsFile(const QString& fname) {
  m_tags_file.setName(fname+Tagfile_suffix);
  return m_tags_file.remove();
}

void Tags::close() {
  m_tags_ds.unsetDevice();
  m_tags_file.flush();
  m_tags_file.close();
}

QString Tags::value(uint tagId, int index){
  QString s = m_allTags[tagId].first[index];
   if (m_allTags[tagId].second.contains(s)){
     return s;
   }
   else{
     return "";
   }
}

int Tags::index(uint tagId, const QString& value){
   if (m_allTags[tagId].second.contains(value)){
     return m_allTags[tagId].second.find(value).data();
   }
   else{
     return -1;
   }
}

QValueVector<bool> Tags::find(const QString& pattern, QValueVector<QString>& vector, QMap<QString,int>& map){
  QValueVector<bool> result;
  QValueVector<QString>::iterator it;
  for( it = vector.begin(); it != vector.end(); ++it ){
    if (map.contains(*it) && (*it).startsWith(pattern)){
      result.push_back(true);
    }
    else{
      result.push_back(false);
    }
  }
  return result;
}

QValueVector<bool> Tags::find(const QRegExp& pattern, QValueVector<QString>& vector, QMap<QString,int>& map){
  QValueVector<bool> result;
  QValueVector<QString>::iterator it;
  for( it = vector.begin(); it != vector.end(); ++it ){
    if (map.contains(*it) && pattern.search(*it)>=0){
      result.push_back(true);
    }
    else{
      result.push_back(false);
    }
  }
  return result;
}

QValueVector<bool> Tags::find(uint tagId, const QString& pattern){
  return find(pattern, m_allTags[tagId].first, m_allTags[tagId].second);
}

QValueVector<bool> Tags::find(uint tagId, const QRegExp& pattern){
  return find(pattern, m_allTags[tagId].first, m_allTags[tagId].second);
}

int Tags::add(uint tagId, const QString& value){
  //  std::cout << "add called \n";
  if (m_allTags[tagId].second.contains(value)) {
    return m_allTags[tagId].second.find(value).data() ;
  }
  else{
    int idx = m_allTags[tagId].first.count();
    m_allTags[tagId].first.push_back(value);
    m_allTags[tagId].second.insert(value,idx);
    m_tags_ds << (Q_UINT8)tagId;
    m_tags_ds << (Q_UINT32)idx+1;//0 is used for marking values as deleted
    const char* c_name = value;//store as char* - QString serializes too heavily as UTF-16
    m_tags_ds << c_name;
    return idx;
  }
} 

int Tags::count(uint tagId){
  return m_allTags[tagId].first.count();
}

void Tags::remove(uint tagId, int index){
   QString value = m_allTags[tagId].first[index];
   m_allTags[tagId].second.remove(value);
   m_tags_ds << (Q_UINT8)tagId;
   m_tags_ds << (Q_UINT32)0;//0 is used for marking deleted values
   m_tags_ds << value;
}

void Tags::compact(){
//not done yet
/*  QValueVector<QString>::iterator it;
  for( it = m_name_vector.begin(); it != m_name_vector.end(); ++it ){
     if (!m_name_map.contains(*it)){
       m_name_vector.erase(it);
     }...
  }*/
}

void Tags::flushTagsFile(){
  m_tags_file.flush();
}

int Tags::tagFileSize(){
  return m_tags_file.at();
}

QString Tags::tagName(uint tagId){
   return knownTagNames[tagId];
}
