/*
# Copyright (C) 2015 Fulvio Benini

* This file is part of Scid (Shane's Chess Information Database).
*
* Scid is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation.
*
* Scid is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Scid. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DBASEPOOL_H
#define DBASEPOOL_H

#include <vector>
struct scidBaseT;


namespace DBasePool {


/**
 * init() - initialize the pool of databases.
 *
 * Call this function just once at the beginning of the program.
 */
void init();


 /// Reset the ClipBase to empty.
void clearClipBase();


/**
 * closeAll() - close all the databases in the pool.
 *
 * Call this function just once before ending the program.
 * This function close all the managed databases ensuring that
 * resources are released and changes are flushed to disks.
 */
void closeAll();


/**
 * find() - search for a database.
 * @param filename: the filename of the wanted database.
 *                  Database in native Scid format do not use extension ("example").
 *                  Other databases require file extension ("example.pgn").
 *
 * @returns
 * - the handle of the database corresponding to @e filename.
 * - 0 if not found.
 */
int find(const char* filename);


/**
 * getBase() - get a database from the pool.
 * @param baseHandle: the handle of the wanted database.
 *
 * @returns
 * - the handle of the database corresponding to @e filename.
 * - a pointer to the scidBaseT object corresponding to @e baseHandle.
 * - 0 (nullptr) if @e baseHandle is invalid.
 */
scidBaseT* getBase(int baseHandle);


/**
 * getClipBase() - return the handle of the clipbase
 *
 * The clipbase is a special memory database that is always open and valid.
 * @returns
 * - the handle of the database corresponding to @e filename.
 * - the handle of the clipbase.
 */
int getClipBase();


/**
 * getFreeSlot() - search for a free database slot.
 *
 * @returns
 * - the handle of the database corresponding to @e filename.
 * - a pointer to an available scidBaseT object.
 * - 0 (nullptr) if there are no free slots.
 */
scidBaseT* getFreeSlot();


/**
 * getHandles() - get the handles of opened databases.
 *
 * @returns
 * - the handle of the database corresponding to @e filename.
 * - a std::vector containing the handles of opened databases.
 */
std::vector<int> getHandles();


/**
 * switchCurrent() - DEPRECATED.
 * @param dbase: a pointer to the new "current" database.
 *               If 0 the function just return the "current" database handle.
 *
 * Some legacy code assume that exist only one database.
 * The hack to use that code is to make a database the "current" one.
 * @returns
 * - the handle of the database corresponding to @e filename.
 * - the handle of the new "current" database.
 */
int switchCurrent(scidBaseT* dbase = 0);


} // End of namespace DBasePool


#endif
