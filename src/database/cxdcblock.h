/***************************************************************************
 * 	cxdcblock.h - Class for managing byte-blocks in CxdCFile class
 *
 * 	(C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>
 *
 * 	This file is a part of Chessx (http://chessx.sourceforge.net).
 * 	ChessX is free software; you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License version 3,
 * 	as published by the Free Software Foundation.
 ****************************************************************************/

#ifndef __CXDCBLOCK_H__
#define __CXDCBLOCK_H__

#include "cxdcfile.h"

/** @ingroup Database
    Abstract class building the interfacing for managing byteblocks
    of a CxdCFile class. Time critical function calls as reading and
    writing will not be called by virtual functions to avoid overhead.

    @todo
*/

class CxdCBlock
{
  public:
    CxdCBlock(CxdCFile& cf, const int& offset, const int& blocksize);
    virtual ~CxdCBlock()=0;

    virtual void close();

    int offset() const;
    int blocksize() const;
/** Returns index of last managed byte.*/
    int lastbyte() const;
    const char* defaultentry() const;

  protected:
    int m_offset;
    int m_blocksize;
    char* m_defaultentry; 
    CxdCFile* m_cf;
    QFile* m_qf;
}; 


/** @ingroup Database
 *  The CxdCBlockT is a template class for a simple block representing one
 *  element of type T.
 */
template <class T> class CxdCBlockT : public CxdCBlock
{
  public:
    CxdCBlockT(CxdCFile& cf, const int& offset);
    T read(const int& id);
    void write(const int& id, const T& t);

  private:
    QDataStream m_qds;

};

template <class T>
CxdCBlockT<T>::CxdCBlockT(CxdCFile& cf, const int& offset)
 :CxdCBlock(cf,offset,sizeof(T))
{
  m_qds.setDevice(m_qf);
}

template <class T>
T CxdCBlockT<T>::read(const int& id)
{
  m_cf->seek(id,m_offset);
  T t;
  m_qds >> t;
  return t; 
}

template <class T>
void CxdCBlockT<T>::write(const int& id, const T& t)
{
  m_cf->seek(id,m_offset);
  m_qds << t; 
  m_qf->flush();
}


#endif
