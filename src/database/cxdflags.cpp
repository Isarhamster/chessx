/***************************************************************************
 * 	cxdflags.cpp - This compilation unit contains classes for managing
 *	flags in cxd databases.
 *
 * 	(C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>
 *
 * 	This file is a part of Chessx (http://chessx.sourceforge.net).
 * 	ChessX is free software; you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License version 3,
 * 	as published by the Free Software Foundation.
 ****************************************************************************/

#include "cxdflags.h"
#include "common.h"

const int CxdFlags::recordsize=1;

const int CxdCBlockFlags::deletePos=0;
const char CxdCBlockFlags::singleOne[8]={1<<7,1<<6,1<<5,1<<4,1<<3,1<<2,1<<1,1};
// The explicit conversion of char below avoid compiler overflow warnings.
const char CxdCBlockFlags::singleZero[8]={char(~(1<<7)),~(1<<6),~(1<<5),~(1<<4),
                                          ~(1<<3),~(1<<2),~(1<<1),~1};


CxdCBlockFlags::CxdCBlockFlags(CxdCFile& cf)
:CxdCBlock(cf,0,CxdFlags::recordsize)
{}

bool CxdCBlockFlags::getDeleteFlag(const int& id)
{
  Q_ASSERT(isInRange(id));
  return getBit(id,deletePos);
}

void CxdCBlockFlags::setDeleteFlag(const int& id, const bool& b)
{
  Q_ASSERT(isInRange(id));
  setBit(id,deletePos,b);
}

bool CxdCBlockFlags::getBit(const int& id, const int& pos)
{
  m_cf->seek(id,pos/8);
  char c;
  m_qf->getChar(&c);
  return c&singleOne[pos%8];
} 

void CxdCBlockFlags::setBit(const int& id, const int& pos, const bool& b)
{
  m_cf->seek(id,pos/8);
  char c;
  m_qf->getChar(&c);
  m_cf->seek(id,pos/8);
  if(b) m_qf->putChar(c|singleOne[pos%8]);
  else m_qf->putChar(c&singleZero[pos%8]);
}

// CxdFlags class definitions
// --------------------------------------------------------------

CxdFlags::CxdFlags(Index& index)
:m_flagsCFile(recordsize), m_flagsCBlock(m_flagsCFile)
{
  m_index=&index;
  m_isOpen=0;
}

CxdFlags::~CxdFlags()
{
  if(m_isOpen) close();
}

bool CxdFlags::open(const SaxHandler& saxhandler)
{
  if(m_isOpen) return 1;
  if(!m_flagsCFile.open(saxhandler.m_flagsFilename)) return 0;

  // loading data into memory
  for(int i=0; i<nb_entries(); ++i)
  {
    m_index->setDeleteFlag(i,m_flagsCBlock.getDeleteFlag(i));
  }  

  m_isOpen=true;
  return 1; 
}

void CxdFlags::close()
{
  if(!m_isOpen) return;
  m_flagsCFile.close();
  m_isOpen=false;
}

bool CxdFlags::create(const SaxHandler& saxhandler)
{
  if(m_isOpen) return 0;
  if(!m_flagsCFile.create(saxhandler.m_flagsFilename)) return 0;

  m_isOpen=1;
  return 1;
}

void CxdFlags::append()
{
  m_flagsCFile.appendEntries(1);
}


int CxdFlags::nb_entries() const
{
  return m_flagsCFile.nb_entries();
}


void CxdFlags::setDeleteFlag(const int& id, const bool& b)
{
  // update in memory
  m_index->setDeleteFlag(id,b);
  // update on disk
  m_flagsCBlock.setDeleteFlag(id,b);
}

bool CxdFlags::toggleDeleteFlag(const int& id)
{
  bool newflag;
  // update in memory
  newflag=m_index->toggleDeleteFlag(id); 
  // update on disk
  m_flagsCBlock.setDeleteFlag(id,newflag);
  return newflag;
}



