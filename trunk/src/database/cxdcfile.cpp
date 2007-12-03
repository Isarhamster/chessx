/***************************************************************************
 * 	cxdcfile.cpp - Class for managing simple files with constant record size.
 *
 * 	(C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>
 *
 * 	This file is a part of Chessx (http://chessx.sourceforge.net).
 * 	ChessX is free software; you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License version 3,
 * 	as published by the Free Software Foundation.
 ****************************************************************************/

#include "cxdcfile.h"
#include "cxdcblock.h"
#include "common.h"


CxdCFile::CxdCFile(const int& recordsize)
{
  m_recordsize=recordsize;

  m_defaultentry = new char[m_recordsize];
  memset(m_defaultentry,0,m_recordsize);
  m_isOpen=0;
}

CxdCFile::~CxdCFile()
{
  delete[] m_defaultentry;
  if(m_isOpen) close();
}

bool CxdCFile::open(const QString& filename)
{
  if(m_isOpen) return 1;
  m_qf.setFileName(filename);
  if(!m_qf.exists())
  {return 0;}
  m_qf.open(QIODevice::ReadWrite);
  m_isOpen=1;
  return 1;
}

void CxdCFile::close()
{
  if(!m_isOpen) return;
  m_qf.close();
  for(int i=0; i<m_blocklist.size(); ++i) m_blocklist[i]->close();
  m_isOpen=0;
}

bool CxdCFile::create(const QString& filename)
{
  if(m_isOpen) return 0;
  m_qf.setFileName(filename);
  // Here we could warn if the file already exists.
  m_qf.resize(0);
  m_qf.open(QIODevice::ReadWrite);
  m_isOpen=1;
  return 1;
}

void CxdCFile::seek(const int& id, const int& offset)
{
  m_qf.seek(id*m_recordsize+offset);
}

void CxdCFile::compact(QList<bool>& ql)
{
  Q_ASSERT(ql.size()==nb_entries());
  int newid=0;
  for(int id=0; id<nb_entries(); ++id)
  {
    if(ql[id])
    {
      copyentry(id,newid);
      ++newid;
    }
  }
  // Now newid contains the number of elements after compactification.
  m_qf.resize(newid*m_recordsize);
}

int CxdCFile::nb_entries() const
{
  return m_qf.size()/m_recordsize;
}

void CxdCFile::appendEntries(const int& nb)
{
  m_qf.seek(m_qf.size());
  for(int i=0; i<nb; ++i)
  {
    m_qf.write(m_defaultentry,m_recordsize);
  }
  m_qf.flush();
}

bool CxdCFile::addBlock(CxdCBlock& cb)
{
  if(doesOverlap(cb)) return 0;
  m_blocklist.append(&cb);
  memcpy(m_defaultentry,cb.defaultentry(),cb.blocksize());
  return 1;
}

bool CxdCFile::isInRange(const int& id) const
{
  if(id >=0 && id <nb_entries()) return 1;
  return 0;
}

bool CxdCFile::existsFile() const
{
  return m_qf.exists();
}

QString CxdCFile::fileName() const
{
  return m_qf.fileName();
}

bool CxdCFile::doesOverlap(CxdCBlock& cb) const
{
  for(int i=0; i<m_blocklist.size(); ++i)
  {
    if(cb.lastbyte() >= m_blocklist[i]->offset() &&
       cb.offset() <= m_blocklist[i]->lastbyte())
    {return 1;}
  }
  return 0;
}

int CxdCFile::recordsize() const
{
  return m_recordsize;
}

void CxdCFile::copyentry(const int& sourceId, const int& destId)
{
  Q_ASSERT(isInRange(sourceId) && isInRange(destId));
  seek(sourceId);
  QByteArray qba = m_qf.read(m_recordsize);
  seek(destId);
  m_qf.write(qba);
}

QFile* CxdCFile::qf()
{
  return &m_qf;
}


