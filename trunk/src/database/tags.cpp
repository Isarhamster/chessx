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
//    std::cout <<  i  << "\n";

static Q_UINT32 Magic = (Q_UINT32)0x00DDB0DE; // 'magic' number
static Q_UINT32 Version = (Q_UINT32)100; // file format version
static QString Tagfile_suffix = ".ctf";
static char* knownTagNames[]={"Event","Site", "Date", "Round", "Name","Result",
"Title", "Elo", "USCF", "NA", "Type", "EventDate", "EventSponsor", "Section", "Stage", "Board",
"Opening", "Variation", "Subvariation", "ECO", "NIC", "Time", "UTCTime", "UTCDate", 
"TimeControl", "SetUp", "FEN", "Termination","Annotator", "Mode", "PlyCount", 
"Source"
};

Tags::Tags(const QString& fname){
  m_fname = fname;
  m_tags_file.setName(m_fname+Tagfile_suffix);
  clear();
}

void Tags::writeFile(){
// set which QDataset version format to use
  if (Version==(Q_UINT32)100){
    m_tags_ds.setVersion(6);
  }
  else{
    m_tags_ds.setVersion(6);//default
  }
  m_tags_file.open( IO_WriteOnly );
  m_tags_ds.setDevice(&m_tags_file);
  m_tags_ds << Magic;
  m_tags_ds << Version;
  //store basic information for each tag type
  m_tags_ds << (Q_UINT32)(m_allTags.count());
  for(int i=0; i<(int)m_allTags.count(); i++){
    m_tags_ds << (Q_UINT32)count(i);
    if (isCustom(i)){
      m_tags_ds << tagName(i).utf8();//write custom tag name
//used flag
      if (m_customTags_m.find(tagName(i)).data().second==true){
        m_tags_ds << (Q_UINT8)1;
      }
      else{
        m_tags_ds << (Q_UINT8)0;
      }
    }
  }
  //write the tag value data
  QString currentValue;
  QString lastValue;
  QByteArray ba;
  bool first;
  Q_UINT32 id;
  Q_UINT32 freq;
  Q_UINT8 prefixCount;
  for(int i=0; i<(int)m_allTags.count(); i++){
    first=true;
    QMap <QString,QPair<Q_UINT32,Q_UINT32> > map = m_allTags[i].second;
    QMap <QString,QPair<Q_UINT32,Q_UINT32> >::iterator map_itr;
    for(map_itr = map.begin(); map_itr != map.end(); ++map_itr){
      id = map_itr.data().first;
      freq = map_itr.data().second;
      m_tags_ds << id;
      m_tags_ds << freq;
      currentValue = map_itr.key();
      if (first){
        ba = currentValue.utf8();
        m_tags_ds << ba;
        first=false;
      }
      else{//use prefix coding, since values are sorted in the map
        prefixCount=0;
        while(lastValue.at(prefixCount)==currentValue.at(prefixCount)){
          prefixCount++;
        }
        ba = currentValue.mid(prefixCount).utf8();
        m_tags_ds << ba;
        m_tags_ds << prefixCount;
      }
      lastValue = currentValue;
    }
  }
  closeFile();
}

bool Tags::readFile() {
  m_tags_file.setName(m_fname+Tagfile_suffix);
  if (!m_tags_file.open( IO_ReadOnly ))
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
  clear();
//read the file
  Q_UINT32 tagCount;
  m_tags_ds >> tagCount;
  Q_UINT32 valueCount;
  Q_UINT8 prefixCount;
  QString currentValue;
  QString lastValue;
  QByteArray ba;
  bool first;
  Q_UINT32 id;
  Q_UINT32 freq;
  Q_UINT8 used;
  QValueVector<Q_UINT32> valueCounts;
  for(uint i=0; i<tagCount; i++){
    m_tags_ds >> valueCount;
    valueCounts.push_back(valueCount);
    if (isCustom(i)){
      m_tags_ds >> ba;//read custom tag name
      m_tags_ds >> used;
      currentValue = QString::fromUtf8(ba);
      bool b = (used!=0);
      QPair<uint,bool> p(i,b);
      m_customTags_m.insert(currentValue,p);
      m_customTags_v.push_back(currentValue);
//reserve an entry in values collections m_allTags
      QValueVector<QString> first;
      QMap<QString,QPair<Q_UINT32,Q_UINT32> > second;
      QPair <QValueVector<QString>, QMap <QString,QPair<Q_UINT32,Q_UINT32> > > elm = qMakePair(first,second); 
      m_allTags.insert(i,elm);
    }
  }
  for(uint i=0; i<tagCount; i++){
    m_allTags[i].first.resize(valueCounts[i]);
    first=true;
    for(uint j=0; j<valueCounts[i]; j++){
      m_tags_ds >> id;
      m_tags_ds >> freq;
      m_tags_ds >> ba;
      if (first){
        currentValue = QString::fromUtf8(ba);
        first = false;
      }
      else{
        m_tags_ds >> prefixCount;
        currentValue = lastValue.left(prefixCount).append(QString::fromUtf8(ba));
      }
      lastValue = currentValue;
      m_allTags[i].first[id]=currentValue;
      QPair<Q_UINT32,Q_UINT32> p(id,freq);
      m_allTags[i].second.insert(currentValue,p);
    }
  }
  closeFile();
  return true;
}

bool Tags::removeFile() {
  return m_tags_file.remove();
}

QString Tags::value(const uint tagId, const int valueId) const{
  return m_allTags[tagId].first[valueId];
}

int Tags::valueId(uint const tagId, const QString& value) const{
   if (m_allTags[tagId].second.contains(value)){
     return m_allTags[tagId].second.find(value).data().first;
   }
   else{
     return -1;
   }
}

uint Tags::valueFrequency(const uint tagId, const int valueId) const{
   return m_allTags[tagId].second.find(value(tagId,valueId)).data().second;
}

QValueVector<bool> Tags::find(const QString& pattern, QValueVector<QString>& vector, QMap<QString,QPair<Q_UINT32,Q_UINT32> >& map){
  QValueVector<bool> result;
  QValueVector<QString>::iterator it;
  for(it = vector.begin(); it != vector.end(); ++it){
    if (map.contains(*it) && (*it).startsWith(pattern)){
      result.push_back(true);
    }
    else{
      result.push_back(false);
    }
  }
  return result;
}

QValueVector<bool> Tags::find(const QRegExp& pattern, QValueVector<QString>& vector, QMap<QString,QPair<Q_UINT32,Q_UINT32> >& map){
  QValueVector<bool> result;
  QValueVector<QString>::iterator it;
  for(it = vector.begin(); it != vector.end(); ++it){
    if (map.contains(*it) && pattern.search(*it)>=0){
      result.push_back(true);
    }
    else{
      result.push_back(false);
    }
  }
  return result;
}

QValueVector<bool> Tags::find(const uint tagId, const QString& pattern){
  return find(pattern, m_allTags[tagId].first, m_allTags[tagId].second);
}

QValueVector<bool> Tags::find(const uint tagId, const QRegExp& pattern){
  return find(pattern, m_allTags[tagId].first, m_allTags[tagId].second);
}

int Tags::add(const uint tagId, const QString& value){
  if (m_allTags[tagId].second.contains(value)) {
    (m_allTags[tagId].second.find(value).data().second)++;
    return m_allTags[tagId].second.find(value).data().first;
  }
  else{
    Q_UINT32 idx = m_allTags[tagId].first.count();
    m_allTags[tagId].first.push_back(value);
    QPair<Q_UINT32,Q_UINT32> p(idx,1);
    m_allTags[tagId].second.insert(value,p);
    return idx;
  }
} 

int Tags::count(const uint tagId) const{
  return m_allTags[tagId].first.count();
}

void Tags::remove(const uint tagId, const int valueId){
   QString value = m_allTags[tagId].first[valueId];
   if (m_allTags[tagId].second.find(value).data().second > 0){
     (m_allTags[tagId].second.find(value).data().second)--;
   }
}

void Tags::compact(){
//remove values with frequency==0
// the index file has to be rewritten too.
}

bool Tags::isKnown(const uint tagId) const{
  return tagId<=maxKnownTagIndex();
}
bool Tags::isKnownMandatory(const uint tagId) const{
  return tagId<=Result;
}
bool Tags::isKnownOptional(const uint tagId) const{
  return tagId<=maxKnownTagIndex() && tagId>Result;
}
bool Tags::isCustom(const uint tagId) const{
  return tagId>maxKnownTagIndex();
}

int Tags::defineTag(const QString& tagName){
  if (m_customTags_m.contains(tagName)){
    return -1;
  }
  else{
    Q_UINT16 newTagId = m_customTags_v.count()+maxKnownTagIndex()+1;
    QPair<uint,bool> p(newTagId,true);
    m_customTags_m.insert(tagName,p);
    m_customTags_v.push_back(tagName);
//reserve an entry in values collections m_allTags
    QValueVector<QString> first;
    QMap<QString,QPair<Q_UINT32,Q_UINT32> > second;
    QPair <QValueVector<QString>, QMap <QString,QPair<Q_UINT32,Q_UINT32> > > elm = qMakePair(first,second); 
    m_allTags.insert(newTagId,elm);
    return newTagId;
  }
}

bool Tags::unDefineTag(const QString& tagName){
  if (m_customTags_m.contains(tagName)){
    m_customTags_m.find(tagName).data().second = false;
    return true;
  }
  else{
    return false;
  }
}

QStringList Tags::tagNames(const TagNameType type) {
  QStringList qsl;
  if (type==All || type==Known || type==KnownOptional || type==KnownMandatory){
    for (uint i=0; i<=maxKnownTagIndex(); i++){
      if (type==All || type==Known || (type==KnownOptional && isKnownOptional(i)) || (type==KnownMandatory && isKnownMandatory(i))){
        qsl.append(knownTagNames[i]);
      }
    }
  }
  if (type==All || type==Custom){
    QMap<QString,QPair<uint,bool> >::iterator it;
    for( it = m_customTags_m.begin(); it != m_customTags_m.end(); ++it ){
      if(it.data().second){//is used
        qsl.append(it.key());
      }
    }
  }
  return qsl;
}

int Tags::tagId(const QString& tagName) const{
  if (m_customTags_m.contains(tagName)){
    if(m_customTags_m.find(tagName).data().second){//is used
      return m_customTags_m.find(tagName).data().first;
    }
    else{
      return -1;
    }
  }
  else{
    int i = knownTagIndex(tagName);
    if (i>=0){
      return i;
    }
    else{
      return -1;
    }
  }
}

QString Tags::tagName(const uint tagId) const{
  if (tagId>=minKnownTagIndex() && tagId<=maxKnownTagIndex()){
    return knownTagNames[tagId];
  }
  else{
    int ix = tagId-maxKnownTagIndex()-1;
    if ((int)m_customTags_v.count()>ix){
      QString tagName = m_customTags_v[ix];
      if (m_customTags_m.contains(tagName)){
        return tagName;
      }
      else{
        return "";
      }
    }
    else{
      return "";
    }
  }
}


uint Tags::maxKnownTagIndex(){
  return Source;
}
uint Tags::minKnownTagIndex(){
  return Event;
}

int Tags::knownTagIndex(const QString& tagName){
  for (uint i=0; i<=maxKnownTagIndex(); i++){
    if (knownTagNames[i]==tagName){
      return i;
    }
  }
  return -1;
}

void Tags::clear(){
  m_allTags.clear();
  for (uint i=0; i<=maxKnownTagIndex(); i++){
    QValueVector<QString> first;
    QMap<QString,QPair<Q_UINT32,Q_UINT32> > second;
    QPair <QValueVector<QString>, QMap <QString,QPair<Q_UINT32,Q_UINT32> > > elm = qMakePair(first,second); 
    m_allTags.insert(i,elm);
  }
  m_customTags_m.clear();
  m_customTags_v.clear();
}

void Tags::closeFile() {
  m_tags_ds.unsetDevice();
  m_tags_file.flush();
  m_tags_file.close();
}
