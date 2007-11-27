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

CxdAssign::CxdAssign()
{
  m_assignDataStream.setDevice(&m_assignFile);
  m_isOpen=0;
}

bool CxdAssign::open(SaxHandler& saxhandler)
{
  if(m_isOpen) return 1;
  m_assignFile.setFileName(saxhandler.m_assignFilename);
  if(!m_assignFile.exists()) return 0;
  m_assignFile.open(QIODevice::ReadWrite);

  // loading data into memory
  m_assign.clear();
  m_assignFile.peek(0);
  qint32 i;
  while(!m_assignFile.atEnd())
  {
    m_assignDataStream >> i;
    m_assign.append(i);
  }

  m_isOpen=1;
  return 1;
}

void CxdAssign::close()
{
  if(!m_isOpen) return;
  m_assignFile.close();
  m_isOpen=false;
}

bool CxdAssign::create(SaxHandler& saxhandler)
{
  if(m_isOpen) return 0;

  m_assignFile.setFileName(saxhandler.m_assignFilename);
  // Here we could warn if the file already exists.
  m_assignFile.resize(0);
  m_assignFile.open(QIODevice::ReadWrite);
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
  m_assignFile.seek(id*4);
  m_assignDataStream << iid;
  m_assignFile.flush();
  
  return 1;
}

bool CxdAssign::appendToAssignFile(const qint32& iid)
{
  m_assignFile.seek(m_assignFile.size());
  
  m_assignDataStream << iid;
  m_assignFile.flush();
  return 1;
}
