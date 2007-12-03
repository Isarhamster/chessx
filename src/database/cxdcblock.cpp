/***************************************************************************
 * 	cxdcblock.cpp - Class for managing byte-blocks in CxdCFile class
 *
 * 	(C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>
 *
 * 	This file is a part of Chessx (http://chessx.sourceforge.net).
 * 	ChessX is free software; you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License version 3,
 * 	as published by the Free Software Foundation.
 ****************************************************************************/

#include "cxdcblock.h"
#include "common.h"

CxdCBlock::CxdCBlock(CxdCFile& cf, const int& offset, const int& blocksize)
{
  m_cf=&cf; 
  m_qf=&cf.m_qf;

  m_offset=offset;
  m_blocksize=blocksize;

  m_defaultentry=new char[blocksize];
  for(int i=0; i<blocksize; ++i) m_defaultentry[i]=0;

  cf.addBlock(*this);
}

CxdCBlock::~CxdCBlock()
{}

void CxdCBlock::close()
{}

int CxdCBlock::offset() const
{
  return m_offset;
}

int CxdCBlock::blocksize() const
{
  return m_blocksize;
}

int CxdCBlock::lastbyte() const
{
  return offset()+blocksize()-1;
}

const char* CxdCBlock::defaultentry() const
{
  return m_defaultentry;
}

void CxdCBlock::seek(const int& id)
{
  m_cf->seek(id,m_offset);
}

int CxdCBlock::nb_entries() const
{
  return m_cf->nb_entries();
}

bool CxdCBlock::isInRange(const int& id) const
{
  return m_cf->isInRange(id);
}


// Some explicit template instantiations
template class CxdCBlockT<qint64>;
template class CxdCBlockT<qint32>;
