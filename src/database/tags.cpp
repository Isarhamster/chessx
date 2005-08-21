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
static int MaxNameLength=999;

bool Tags::create(const QString& fname) {
  m_tags_file.setName(fname+Tagfile_suffix);
  if (m_tags_file.exists())
     return false;
// set QDataset format version to use
  if (Version==(Q_UINT32)100){
    m_tags_ds.setVersion(6);
  }
  else{//default
    m_tags_ds.setVersion(6);
  }
  m_tags_file.open( IO_ReadWrite );
  m_tags_ds.setDevice(&m_tags_file);
  m_tags_ds << Magic;
  m_tags_ds << Version;
  m_tags_file.flush();
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
// set QDataset format version to use
  if (tag_version==(Q_UINT32)100){
    m_tags_ds.setVersion(6);
  }
  else{//unknown version
    m_tags_ds.unsetDevice();
    m_tags_file.close();
    return false;
  }

  Q_UINT8 tagType;
  Q_UINT32 index;
  QString name;
  TagType t;
  while (!m_tags_ds.atEnd()){
    m_tags_ds >> tagType;
    t = (TagType)tagType;
    m_tags_ds >> index;
    m_tags_ds >> name;
    
    switch(t)
    {
      case Name:
        if (name.startsWith("0")){//was removed
          m_name_map.remove(name.mid(1,MaxNameLength));
        }
        else{
          m_name_map.insert(name,(int)index);
          m_name_vector.push_back(name);
        }
        break;
      case Event:
        if (name.startsWith("0")){//was removed
          m_event_map.remove(name.mid(1,MaxNameLength));
        }
        else{
          m_event_map.insert(name,(int)index);
          m_event_vector.push_back(name);
        }
        break;
      case Site:
        if (name.startsWith("0")){//was removed
          m_site_map.remove(name.mid(1,MaxNameLength));
        }
        else{
          m_site_map.insert(name,(int)index);
          m_site_vector.push_back(name);
        }
        break;
      case Round:
        if (name.startsWith("!")){//was removed
          m_round_map.remove(name.mid(1,MaxNameLength));
        }
        else{
          m_round_map.insert(name,(int)index);
          m_round_vector.push_back(name);
        }
        break;
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

QString Tags::name(TagType t, int index){
  switch(t)
  {
    case Name:
      return m_name_vector[index];
      break;
    case Event:
      return m_event_vector[index];
      break;
    case Site:
      return m_site_vector[index];
      break;
    case Round:
      return m_round_vector[index];
      break;
  }
  return "Tag name not found: "+index;
}

int Tags::index(TagType t, const QString& name){
  switch(t)
  {
    case Name:
      if (m_name_map.contains(name))
        return m_name_map.find(name).data();
      break;
    case Event:
      if (m_event_map.contains(name))
        return m_event_map.find(name).data();
      break;
    case Site:
      if (m_site_map.contains(name))
        return m_site_map.find(name).data();
      break;
    case Round:
      if (m_round_map.contains(name))
        return m_round_map.find(name).data();
      break;
    default:
      return -1;
  }
}


QValueVector<bool> Tags::find(TagType t, const QString& pattern, QValueVector<QString>& vector, QMap<QString,int>& map){
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

QValueVector<bool> Tags::find(TagType t, const QRegExp& pattern, QValueVector<QString>& vector, QMap<QString,int>& map){
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


QValueVector<bool> Tags::find(TagType t, const QString& pattern){
  switch(t)
  {
    case Name:
      return find(t, pattern, m_name_vector, m_name_map);
      break;
    case Event:
      return find(t, pattern, m_event_vector, m_event_map);
      break;
    case Site:
      return find(t, pattern, m_site_vector, m_site_map);
      break;
    case Round:
      return find(t, pattern, m_round_vector, m_round_map);
      break;
  }
}

QValueVector<bool> Tags::find(TagType t, const QRegExp& pattern){
  switch(t)
  {
    case Name:
      return find(t, pattern, m_name_vector, m_name_map);
      break;
    case Event:
      return find(t, pattern, m_event_vector, m_event_map);
      break;
    case Site:
      return find(t, pattern, m_site_vector, m_site_map);
      break;
    case Round:
      return find(t, pattern, m_round_vector, m_round_map);
      break;
  }
}

int Tags::addIndex(TagType& tp, QValueVector<QString>& vector, QMap<QString,int>& map, const QString& name){
  if (map.contains(name)) {
    return map.find(name).data() ;
  }
  else{
    int idx = vector.count();
    vector.push_back(name);
    map.insert(name,idx);
    m_tags_ds << (Q_UINT8)tp;
    m_tags_ds << (Q_UINT32)idx;
    m_tags_ds << name;
    return idx;
  }
}

int Tags::add(TagType t, const QString& name){
  //  std::cout << "add called \n";
    switch(t)
    {
      case Name:
        return addIndex(t, m_name_vector, m_name_map, name);
        break;
      case Event:
        return addIndex(t, m_event_vector, m_event_map, name);
        break;
      case Site:
        return addIndex(t, m_site_vector, m_site_map, name);
        break;
      case Round:
        return addIndex(t, m_round_vector, m_round_map, name);
        break;
      default: 
        return -1;
    };

} 

int Tags::count(TagType t){
  switch(t)
  {
    case Name:
      return m_name_vector.count();
      break;
    case Event:
      return m_name_vector.count();
      break;
    case Site:
      return m_name_vector.count();
      break;
    case Round:
      return m_name_vector.count();
      break;
    default: 
      return -1;
  }
}

void Tags::remove(TagType t, int index, QValueVector<QString>& vector, QMap<QString,int>& map, const QString& prefix){
   QString name = vector[index];
   QString newName;
   if (!name.startsWith(prefix)){
     newName = prefix+name;
     map.remove(name);
     m_tags_ds << (Q_UINT8)t;
     m_tags_ds << (Q_UINT32)index;
     m_tags_ds << newName;
     vector[index] = newName;
   }
}

void Tags::remove(TagType t, int index){
  switch(t)
  {
    case Name:
      remove(t, index, m_name_vector, m_name_map, "0");
      break;
    case Event:
      remove(t, index, m_event_vector, m_event_map, "0");
      break;
    case Site:
      remove(t, index, m_site_vector, m_site_map, "0");
      break;
    case Round:
      remove(t, index, m_round_vector, m_round_map, "!");
      break;
  }
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

