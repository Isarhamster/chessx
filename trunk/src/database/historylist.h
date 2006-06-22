/***************************************************************************
                          historylist.cpp  -  List of n recent QStrings used
                             -------------------
    begin                : 18 Jun 2006
    copyright            : (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __HISTORYLIST_H__
#define __HISTORYLIST_H__

#include <qt34/qvaluevector.h>
#include <qstringlist.h>

/** The HistoryList class provides a list of @p N recent string items that
can be stored/restored. */

class HistoryList
{
  private:
    QValueVector<QString> m_data;
    int m_count;
    bool m_unique;
  public:
    HistoryList(int historySize = 10);
    ~HistoryList();
    /** Restore the list from application settings. */
    void restore(const QString& group, const QString& key);
    /** Save the list with application settings. */
    void save(const QString& group, const QString& key) const;
    /** @return current number of items. */
    int count() const;
    /** @return current size. */
    int size() const;
    /** Sets current size. */
    void setSize(int newSize);
    /** Adds new item. If there are too many items, last one will be dropped. */
    void append(const QString& item);
    /** Removes item at @p index. */
    void remove(int index);
    /** Removes all items. */
    void clear();
    /** @return item at @p index . 0 is the newest one. */
    QString item(int index) const;
    /** @return item at @p index . */
    QString operator[](int index) const;
    /** @return index of given item or -1. */
    int find(const QString& s) const;
    /** @return @p true if history contains given item. */
    bool contains(const QString& s) const;
    /** @return all items. */
    QStringList items() const;
    /** Sets all items. */
    void setItems(const QStringList& list);
    /** @return if older repeated value is automatically removed */
    bool isUnique() const;
    /** Sets whether older repeated value is automatically removed. Does not
        affect current list content. */
    void setUnique(bool value);
};

#endif
