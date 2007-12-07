/***************************************************************************
 * 	cxdflags.h - This compilation unit contains classes for managing
 * 	             flags in cxd databases.
 *
 * 	(C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>
 *
 * 	This file is a part of Chessx (http://chessx.sourceforge.net).
 * 	ChessX is free software; you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License version 3,
 * 	as published by the Free Software Foundation.
 ****************************************************************************/

#ifndef __CXDCFLAGS_H__
#define __CXDCFLAGS_H__

#include "cxdcfile.h"
#include "cxdcblock.h"
#include "cxdsaxhandler.h"
#include "index.h"

#include <QFile>
#include <QDataStream>

/** @ingroup Database
 *  This is a CxdCBlock class tailored for managing flags in cxd databases.
 *
 *  @todo
*/

class CxdCBlockFlags : public CxdCBlock
{
  public:
    CxdCBlockFlags(CxdCFile& cf);

    bool getDeleteFlag(const int& id);
    void setDeleteFlag(const int& id, const bool& b);

  private:
    static const int deletePos;

    static const char singleOne[8];
    static const char singleZero[8];

    // pos indicates the number of bits from the start of the block (the
    // first bit corresponds to pos=0).
    bool getBit(const int& id, const int& pos);
    void setBit(const int& id, const int& pos, const bool& b);

};


/** @ingroup Database
 *  This class is used to manage flags as the deletion flag in cxd databases.
 *
 *  @todo
*/

class CxdFlags
{
  public:
    static const int recordsize;
    CxdFlags(Index& index);
    ~CxdFlags();

    bool open(const SaxHandler& saxhandler);
    void close();
    bool create(const SaxHandler& saxhandler);

    void compact(const QList<bool>& ql);

    void append();

    int nb_entries() const;

    void setDeleteFlag(const int& id, const bool& b);
    bool toggleDeleteFlag(const int& id);

  private:
    bool m_isOpen;

    Index* m_index;

    CxdCFile m_flagsCFile;
    CxdCBlockFlags m_flagsCBlock;
};



#endif
