/***************************************************************************
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __HISTORYLIST_H__
#define __HISTORYLIST_H__

#include <QStringList>

/** @ingroup Core
  The HistoryList class provides a list of @p N recent string items that
  can be stored/restored.
*/

class HistoryList
{
private:
    QStringList m_data;
    int m_size;
    bool m_unique;
public:
    HistoryList(int historySize = 10);
    ~HistoryList();
    /** Restore the list from application settings. */
    void restore();
    /** Save the list with application settings. */
    void save() const;
    /** @return current number of items. */
    int count() const;
    /** @return current size. */
    int size() const;
    /** Sets maximum size. */
    void setSize(int newSize);
    /** Adds new item. If there are too many items, last ones will be dropped. */
    void append(const QString& item);
    /** Removes the first occurence of given item. */
    void remove(const QString& item);
    /** Removes all the first entries that are not existing files */
    void removeMissingFiles();
    /** Removes all items. */
    void clear();
    /** @return item at @p index - 0 is the newest one. */
    QString item(int index) const;
    /** @return item at @p index */
    QString operator[](int index) const;
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
private:
};

#endif
