/***************************************************************************
 *   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                 *
 *   Copyright (C) 2006 by Tobias Koenig <tokoe@kde.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef OOO_STYLEINFORMATION_H
#define OOO_STYLEINFORMATION_H

#include "formatproperty.h"
#include <QMap>

namespace OOO {

class MetaInformation
{
  public:
    typedef QList<MetaInformation> List;

    MetaInformation( const QString &key, const QString &value, const QString &title );

    QString key() const;
    QString value() const;
    QString title() const;

  private:
    QString mKey;
    QString mValue;
    QString mTitle;
};

class StyleInformation
{
  public:
    StyleInformation();
    ~StyleInformation();

    void addFontProperty( const QString &name, const FontFormatProperty &property );
    FontFormatProperty fontProperty( const QString &name ) const;

    void addStyleProperty( const QString &name, const StyleFormatProperty &property );
    StyleFormatProperty styleProperty( const QString &name ) const;

    void addPageProperty( const QString &name, const PageFormatProperty &property );
    PageFormatProperty pageProperty( const QString &name ) const;
    bool pagePropertyExists( const QString &name ) const;

    void addListProperty( const QString &name, const ListFormatProperty &property );
    ListFormatProperty listProperty( const QString &name ) const;

    void addMasterLayout( const QString &name, const QString &layoutName );
    QString masterLayout( const QString &name );
    void setMasterPageName( const QString &name );
    QString masterPageName() const;

    void addMetaInformation( const QString &key, const QString &value, const QString &title );
    MetaInformation::List metaInformation() const;

    void dump() const;

  private:
    QMap<QString, FontFormatProperty> m_FontProperties;
    QMap<QString, StyleFormatProperty> m_StyleProperties;
    QMap<QString, PageFormatProperty> m_PageProperties;
    QMap<QString, ListFormatProperty> m_ListProperties;
    QMap<QString, QString> m_MasterLayouts;
    MetaInformation::List m_MetaInformation;
    QString m_MasterPageName;
};

}

#endif
