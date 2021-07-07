/***************************************************************************
 *   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                 *
 *   Copyright (C) 2006 by Tobias Koenig <tokoe@kde.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "styleinformation.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

using namespace OOO;

MetaInformation::MetaInformation( const QString &key, const QString &value, const QString &title )
  : mKey( key ), mValue( value ), mTitle( title )
{
}

QString MetaInformation::key() const
{
  return mKey;
}

QString MetaInformation::value() const
{
  return mValue;
}

QString MetaInformation::title() const
{
  return mTitle;
}

StyleInformation::StyleInformation()
{
}

StyleInformation::~StyleInformation()
{
}

void StyleInformation::addFontProperty( const QString &name, const FontFormatProperty &property )
{
  m_FontProperties.insert( name, property );
}

FontFormatProperty StyleInformation::fontProperty( const QString &name ) const
{
  return m_FontProperties[ name ];
}

void StyleInformation::addStyleProperty( const QString &name, const StyleFormatProperty &property )
{
  m_StyleProperties.insert( name, property );
}

StyleFormatProperty StyleInformation::styleProperty( const QString &name ) const
{
  return m_StyleProperties[ name ];
}

void StyleInformation::addPageProperty( const QString &name, const PageFormatProperty &property )
{
  m_PageProperties.insert( name, property );
}

bool StyleInformation::pagePropertyExists( const QString &name ) const
{
  return m_PageProperties.contains(name);
}

PageFormatProperty StyleInformation::pageProperty( const QString &name ) const
{
  return m_PageProperties[ name ];
}

void StyleInformation::addListProperty( const QString &name, const ListFormatProperty &property )
{
  m_ListProperties[ name ] = property;
}

ListFormatProperty StyleInformation::listProperty( const QString &name ) const
{
  return m_ListProperties[ name ];
}

void StyleInformation::addMasterLayout( const QString &name, const QString &layoutName )
{
  m_MasterLayouts.insert( name, layoutName );
}

QString StyleInformation::masterLayout( const QString &name )
{
  return m_MasterLayouts[ name ];
}

void StyleInformation::setMasterPageName( const QString &name )
{
  m_MasterPageName = name;
}

QString StyleInformation::masterPageName() const
{
  return m_MasterPageName.isEmpty() ? m_MasterLayouts[ "Standard" ] :  m_MasterLayouts[ m_MasterPageName ];
}

void StyleInformation::addMetaInformation( const QString &key, const QString &value, const QString &title )
{
  const MetaInformation meta( key, value, title );
  m_MetaInformation.append( meta );
}

MetaInformation::List StyleInformation::metaInformation() const
{
  return m_MetaInformation;
}

void StyleInformation::dump() const
{
  QMapIterator<QString, StyleFormatProperty> it( m_StyleProperties );
  while ( it.hasNext() ) {
    it.next();
    qDebug( "%s", qPrintable( it.key() ) );
  }
}
