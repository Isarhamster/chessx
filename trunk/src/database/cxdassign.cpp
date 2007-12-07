/***************************************************************************
                          cxdassign.h  - makes the link between game indices
			                 and internally used indices.
                             -------------------
   begin                : 23 November 2007
   copyright            : (C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 3 as     *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License version 3 for more details.                 *
 *                                                                         *
 ***************************************************************************/

#include "cxdassign.h"
#include "common.h"
#include "cxdcompact.h"

CxdAssign::CxdAssign()
:m_assignCFile(sizeof(qint32)), m_assignCBlock(m_assignCFile,0)
{
  m_isOpen=0;
}

bool CxdAssign::open(SaxHandler& saxhandler)
{
  if(m_isOpen) return 1;
  if(!m_assignCFile.open(saxhandler.m_assignFilename)) return 0;

  // loading data into memory
  m_assign.clear();
  for(int i=0; i<m_assignCBlock.nb_entries(); ++i)
  {
    m_assign.append(m_assignCBlock.read(i));
  }

  m_isOpen=1;
  return 1;
}

void CxdAssign::close()
{
  if(!m_isOpen) return;
  m_assignCFile.close();
  m_isOpen=false;
}

bool CxdAssign::create(SaxHandler& saxhandler)
{
  if(m_isOpen) return 0;
  if(!m_assignCFile.create(saxhandler.m_assignFilename)) return 0;

  m_isOpen=1;
  return 1;
}

qint32 CxdAssign::iid(const qint32& id) const
{
  return m_assign.at(id);
}

int CxdAssign::nb_id() const
{
  return m_assign.size();
}

bool CxdAssign::append(const qint32& iid)
{
  m_assign.append(iid);
  return appendToAssignFile(iid);
}

bool CxdAssign::replace(const qint32& id, const qint32& iid)
{
  Q_ASSERT(0<=id && id<nb_id());

  // update in memory
  m_assign[id]=iid;

  // update on disk
  m_assignCBlock.write(id,iid);
  
  return 1;
}

void CxdAssign::compact(const QList<bool>& ql)
{
  CxdCompact::compactList(m_assign,ql);

  qint32 maxiid(0);
  for(int i=0; i<m_assign.size(); ++i)
  {if(maxiid<m_assign[i]) maxiid=m_assign[i];}

  QList<qint32> sortediid(m_assign);
  qSort(sortediid);

  QVector<qint32> iidmap(maxiid+1);
  for(int i=0; i<sortediid.size(); ++i)
  {
   iidmap[sortediid[i]]=i;
  } 
  // Compact memory
  for(int i=0; i<m_assign.size(); ++i)
  {
    m_assign[i]=iidmap[m_assign[i]];
  }
 
  // Compact file
  m_assignCFile.qf()->resize(m_assign.size()*m_assignCFile.recordsize());
  for(int i=0; i<m_assign.size(); ++i)
  {
    m_assignCBlock.write(i,m_assign[i]);
  }
}

bool CxdAssign::appendToAssignFile(const qint32& iid)
{
  // This is a non-standard way for adding an item to the file managed by
  // m_assignCFile. Normally we should first call the appendEntries function
  // of m_assignCFile. In this special case as m_assignCBlock is the only
  // block working with the file we can avoid this overhead.
  m_assignCBlock.write(m_assignCBlock.nb_entries(),iid);
  return 1;
}
