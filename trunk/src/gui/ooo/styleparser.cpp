/***************************************************************************
 *   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                 *
 *   Copyright (C) 2006 by Tobias Koenig <tokoe@kde.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "styleparser.h"
#include "document.h"
#include "styleinformation.h"

#include <QDateTime>
#include <QFont>
#include <QDomDocument>
#include <QDomElement>
#include <QXmlSimpleReader>
#include <QDebug>

using namespace OOO;

StyleParser::StyleParser( const Document *document, const QDomDocument &domDocument, StyleInformation *styleInformation )
  : m_pDocument( document ), m_DomDocument( domDocument ),
    m_StyleInformation( styleInformation ), m_MasterPageNameSet( false )
{
}

bool StyleParser::parse()
{
  int foundStyles = 0;
  if ( !parseContentFile() )
    foundStyles++;

  if ( !parseStyleFile() )
    foundStyles++;

  if ( !parseMetaFile() )
    foundStyles++;

  return (foundStyles>0);
}

bool StyleParser::parseContentFile()
{
  const QDomElement documentElement = m_DomDocument.documentElement();
  QDomElement element = documentElement.firstChildElement();
  while ( !element.isNull() ) {
    if ( element.tagName() == QLatin1String( "document-common-attrs" ) ) {
      if ( !parseDocumentCommonAttrs( element ) )
        return false;
    } else if ( element.tagName() == QLatin1String( "font-face-decls" ) ) {
      if ( !parseFontFaceDecls( element ) )
        return false;
    } else if ( element.tagName() == QLatin1String( "styles" ) ) {
      if ( !parseStyles( element ) )
        return false;
    } else if ( element.tagName() == QLatin1String( "automatic-styles" ) ) {
      if ( !parseAutomaticStyles( element ) )
        return false;
    }

    element = element.nextSiblingElement();
  }

  return true;
}

bool StyleParser::parseStyleFile()
{
  QXmlSimpleReader reader;

  QXmlInputSource source;
  source.setData( m_pDocument->styles() );

  QString errorMsg;
  int errorLine, errorCol;

  QDomDocument document;
  if ( !document.setContent( &source, &reader, &errorMsg, &errorLine, &errorCol ) ) {
    qDebug( "%s at (%d,%d)", qPrintable( errorMsg ), errorLine, errorCol );
    return false;
  }

  const QDomElement documentElement = document.documentElement();
  QDomElement element = documentElement.firstChildElement();
  while ( !element.isNull() ) {
    if ( element.tagName() == QLatin1String( "styles" ) ) {
      if ( !parseAutomaticStyles( element ) )
        return false;
    } else if ( element.tagName() == QLatin1String( "automatic-styles" ) ) {
      if ( !parseAutomaticStyles( element ) )
        return false;
    } else if ( element.tagName() == QLatin1String( "master-styles" ) ) {
      if ( !parseMasterStyles( element ) )
        return false;
    }

    element = element.nextSiblingElement();
  }

  return true;
}

bool StyleParser::parseMetaFile()
{
  QXmlSimpleReader reader;

  QXmlInputSource source;
  source.setData( m_pDocument->meta() );

  QString errorMsg;
  int errorLine, errorCol;

  QDomDocument document;
  if ( !document.setContent( &source, &reader, &errorMsg, &errorLine, &errorCol ) ) {
    qDebug( "%s at (%d,%d)", qPrintable( errorMsg ), errorLine, errorCol );
    return false;
  }

  const QDomElement documentElement = document.documentElement();
  QDomElement element = documentElement.firstChildElement();
  while ( !element.isNull() ) {
    if ( element.tagName() == QLatin1String( "meta" ) ) {
      QDomElement child = element.firstChildElement();
      while ( !child.isNull() ) {
        if ( child.tagName() == QLatin1String( "generator" ) ) {
          m_StyleInformation->addMetaInformation( "producer", child.text(), "Producer");
        } else if ( child.tagName() == QLatin1String( "creation-date" ) ) {
          const QDateTime dateTime = QDateTime::fromString( child.text(), Qt::ISODate );
          qDebug() << "### dateTime  ...  " << dateTime << " file" <<  __FILE__ << ":" << __LINE__;
          ////  mStyleInformation->addMetaInformation( "creationDate", KGlobal::locale()->formatDateTime( dateTime, KLocale::LongDate, true ),i18n( "Created" ) );
        } else if ( child.tagName() == QLatin1String( "initial-creator" ) ) {
          m_StyleInformation->addMetaInformation( "creator", child.text(), "Creator");
        } else if ( child.tagName() == QLatin1String( "creator" ) ) {
          m_StyleInformation->addMetaInformation( "author", child.text(), "Author");
        } else if ( child.tagName() == QLatin1String( "date" ) ) {
          const QDateTime dateTime = QDateTime::fromString( child.text(), Qt::ISODate );
          qDebug() << "### dateTime  ...  " << dateTime << " file" <<  __FILE__ << ":" << __LINE__;
          //// mStyleInformation->addMetaInformation( "modificationDate", KGlobal::locale()->formatDateTime( dateTime, KLocale::LongDate, true ), i18n( "Modified" ) );
        }

        child = child.nextSiblingElement();
      }
    }

    element = element.nextSiblingElement();
  }

  return true;
}

bool StyleParser::parseDocumentCommonAttrs( QDomElement& )
{
  return true;
}

bool StyleParser::parseFontFaceDecls( QDomElement &parent )
{
  QDomElement element = parent.firstChildElement();
  while ( !element.isNull() ) {
    if ( element.tagName() == QLatin1String( "font-face" ) ) {
      FontFormatProperty property;
      property.setFamily( element.attribute( "font-family" ) );

      m_StyleInformation->addFontProperty( element.attribute( "name" ), property );
    } else {
      qDebug( "unknown tag %s", qPrintable( element.tagName() ) );
    }

    element = element.nextSiblingElement();
  }

  return true;
}

bool StyleParser::parseStyles( QDomElement& )
{
  return true;
}

bool StyleParser::parseMasterStyles( QDomElement &parent )
{
  QDomElement element = parent.firstChildElement();
  while ( !element.isNull() ) {
    if ( element.tagName() == QLatin1String( "master-page" ) ) {
      m_StyleInformation->addMasterLayout( element.attribute( "name" ), element.attribute( "page-layout-name" ) );
      if ( !m_MasterPageNameSet ) {
        m_StyleInformation->setMasterPageName( element.attribute( "name" ) );
        m_MasterPageNameSet = true;
      }
    } else {
      qDebug( "unknown tag %s", qPrintable( element.tagName() ) );
    }

    element = element.nextSiblingElement();
  }

  return true;
}

bool StyleParser::parseAutomaticStyles( QDomElement &parent )
{
  QDomElement element = parent.firstChildElement();
  while ( !element.isNull() ) {
    if ( element.tagName() == QLatin1String( "style" ) )
    {
      const StyleFormatProperty property = parseStyleProperty( element );
      m_StyleInformation->addStyleProperty( element.attribute( "name" ), property );
    }
    else if ( element.tagName() == QLatin1String( "page-layout" ) )
    {
      QDomElement child = element.firstChildElement();
      while ( !child.isNull() ) {
        if ( child.tagName() == QLatin1String( "page-layout-properties" ) ) {
          const PageFormatProperty property = parsePageProperty( child );
          m_StyleInformation->addPageProperty( element.attribute( "name" ), property );
        }

        child = child.nextSiblingElement();
      }
    }
    else if ( element.tagName() == QLatin1String( "list-style" ) )
    {
      const ListFormatProperty property = parseListProperty( element );
      m_StyleInformation->addListProperty( element.attribute( "name" ), property );
    }
    else if ( element.tagName() == QLatin1String( "default-style" ) )
    {
      StyleFormatProperty property = parseStyleProperty( element );
      property.setDefaultStyle( true );
      m_StyleInformation->addStyleProperty( element.attribute( "family" ), property );
    }
    else
    {
      qDebug( "unknown tag %s", qPrintable( element.tagName() ) );
    }

    element = element.nextSiblingElement();
  }

  return true;
}

StyleFormatProperty StyleParser::parseStyleProperty( QDomElement &parent )
{
  StyleFormatProperty property( m_StyleInformation );

  property.setParentStyleName( parent.attribute( "parent-style-name" ) );
  property.setFamily( parent.attribute( "family" ) );
  if ( parent.hasAttribute( "master-page-name" ) ) {
    property.setMasterPageName( parent.attribute( "master-page-name" ) );
    if ( !m_MasterPageNameSet ) {
      m_StyleInformation->setMasterPageName( parent.attribute( "master-page-name" ) );
      m_MasterPageNameSet = true;
    }
  }

  QDomElement element = parent.firstChildElement();
  while ( !element.isNull() )
  {
    if ( element.tagName() == QLatin1String( "paragraph-properties" ) )
    {
      const ParagraphFormatProperty paragraphProperty = parseParagraphProperty( element );
      property.setParagraphFormat( paragraphProperty );
    }
    else if ( element.tagName() == QLatin1String( "text-properties" ) )
    {
      const TextFormatProperty textProperty = parseTextProperty( element );
      property.setTextFormat( textProperty );
    }
    else if ( element.tagName() == QLatin1String( "table-column-properties" ) )
    {
      const TableColumnFormatProperty tableColumnProperty = parseTableColumnProperty( element );
      property.setTableColumnFormat( tableColumnProperty );
    }
    else if ( element.tagName() == QLatin1String( "table-cell-properties" ) )
    {
      const TableCellFormatProperty tableCellProperty = parseTableCellProperty( element );
      property.setTableCellFormat( tableCellProperty );
    }
    else
    {
      qDebug( "unknown tag %s", qPrintable( element.tagName() ) );
    }

    element = element.nextSiblingElement();
  }

  return property;
}

ParagraphFormatProperty StyleParser::parseParagraphProperty( QDomElement &parent )
{
  ParagraphFormatProperty property;

  property.setPageNumber( parent.attribute( "page-number" ).toInt() );

  static QMap<QString, ParagraphFormatProperty::WritingMode> map;
  if ( map.isEmpty() ) {
    map.insert( "lr-tb", ParagraphFormatProperty::LRTB );
    map.insert( "rl-tb", ParagraphFormatProperty::RLTB );
    map.insert( "tb-rl", ParagraphFormatProperty::TBRL );
    map.insert( "tb-lr", ParagraphFormatProperty::TBLR );
    map.insert( "lr", ParagraphFormatProperty::LR );
    map.insert( "rl", ParagraphFormatProperty::RL );
    map.insert( "tb", ParagraphFormatProperty::TB );
    map.insert( "page", ParagraphFormatProperty::PAGE );
  }
  property.setWritingMode( map[ parent.attribute( "writing-mode" ) ] );

  static QMap<QString, Qt::Alignment> alignMap;
  if ( alignMap.isEmpty() ) {
    alignMap.insert( "center", Qt::AlignCenter );
    alignMap.insert( "left", Qt::AlignLeft );
    alignMap.insert( "right", Qt::AlignRight );
    alignMap.insert( "justify", Qt::AlignJustify );
    if ( property.writingModeIsRightToLeft() ) {
      alignMap.insert( "start", Qt::AlignRight );
      alignMap.insert( "end", Qt::AlignLeft );
    } else {
      // not right to left
      alignMap.insert( "start", Qt::AlignLeft );
      alignMap.insert( "end", Qt::AlignRight );
    }
  }
  if ( parent.hasAttribute( "text-align" ) ) {
    property.setTextAlignment( alignMap[ parent.attribute( "text-align", "left" ) ] );
  }

  const QString marginLeft = parent.attribute( "margin-left" );
  if ( !marginLeft.isEmpty() ) {
    qreal leftMargin = qRound( convertUnit( marginLeft ) );
    property.setLeftMargin( leftMargin );
  }

  const QString colorText = parent.attribute( "background-color" );
  if ( !colorText.isEmpty() && colorText != QLatin1String( "transparent" ) ) {
    property.setBackgroundColor( QColor( colorText ) );
  }

  return property;
}

TextFormatProperty StyleParser::parseTextProperty( QDomElement &parent )
{
  TextFormatProperty property;

  const QString fontSize = parent.attribute( "font-size" );
  if ( !fontSize.isEmpty() )
    property.setFontSize( qRound( convertUnit( fontSize ) ) );

  static QMap<QString, QFont::Weight> weightMap;
  if ( weightMap.isEmpty() )
  {
    weightMap.insert( "normal", QFont::Normal );
    weightMap.insert( "bold", QFont::Bold );
  }

  const QString fontWeight = parent.attribute( "font-weight" );
  if ( !fontWeight.isEmpty() )
  {
    property.setFontWeight( weightMap[ fontWeight ] );
  }

  static QMap<QString, QFont::Style> fontStyleMap;
  if ( fontStyleMap.isEmpty() )
  {
    fontStyleMap.insert( "normal", QFont::StyleNormal );
    fontStyleMap.insert( "italic", QFont::StyleItalic );
    fontStyleMap.insert( "oblique", QFont::StyleOblique );
  }

  const QString fontStyle = parent.attribute( "font-style" );
  if ( !fontStyle.isEmpty() )
  {
    property.setFontStyle( fontStyleMap.value( fontStyle, QFont::StyleNormal ) );
  }

  const QColor color( parent.attribute( "color" ) );
  if ( color.isValid() )
  {
    property.setColor( color );
  }

  const QString colorText = parent.attribute( "background-color" );
  if ( !colorText.isEmpty() && colorText != QLatin1String( "transparent" ) )
  {
    property.setBackgroundColor( QColor( colorText ) );
  }

  const QString underlineType = parent.attribute( "text-underline-style" );
  if ( !underlineType.isEmpty() && underlineType != QLatin1String( "none" ) )
  {
    property.setUnderline( true );
  }

  return property;
}

PageFormatProperty StyleParser::parsePageProperty( QDomElement &parent )
{
  PageFormatProperty property;

  property.setBottomMargin( convertUnit( parent.attribute( "margin-bottom" ) ) );
  property.setLeftMargin( convertUnit( parent.attribute( "margin-left" ) ) );
  property.setTopMargin( convertUnit( parent.attribute( "margin-top" ) ) );
  property.setRightMargin( convertUnit( parent.attribute( "margin-right" ) ) );
  property.setWidth( convertUnit( parent.attribute( "page-width" ) ) );
  property.setHeight( convertUnit( parent.attribute( "page-height" ) ) );

  return property;
}

ListFormatProperty StyleParser::parseListProperty( QDomElement &parent )
{
  ListFormatProperty property;

  static QMap<QString, QTextListFormat::Style> map;
  if ( map.isEmpty() )
  {
    map.insert( "●", QTextListFormat::ListDisc );
    map.insert( "○", QTextListFormat::ListCircle );
    map.insert( "□", QTextListFormat::ListSquare );
    map.insert( "1", QTextListFormat::ListDecimal );
    map.insert( "a", QTextListFormat::ListLowerAlpha );
    map.insert( "A", QTextListFormat::ListUpperAlpha );
    map.insert( "i", QTextListFormat::ListLowerRoman );
    map.insert( "I", QTextListFormat::ListUpperRoman );
  }

  QDomElement element = parent.firstChildElement();
  if ( element.tagName() == QLatin1String( "list-level-style-number" ) )
  {
    property.setType( map[ element.attribute( "num-format" ) ] );
  }
  else if (element.tagName() == QLatin1String( "list-level-style-bullet" ) )
  {
    property.setType( map[ element.attribute( "bullet-char" ) ] );
  }

  while ( !element.isNull() )
  {
    if ( element.tagName() == QLatin1String( "list-level-style-number" ) )
    {
      int level = element.attribute( "level" ).toInt();
      property.addItem( level, 0.0 );
    }
    else if ( element.tagName() == QLatin1String( "list-level-style-bullet" ) )
    {
      int level = element.attribute( "level" ).toInt();
      property.addItem( level, convertUnit( element.attribute( "space-before" ) ) );
    }

    element = element.nextSiblingElement();
  }

  return property;
}

TableColumnFormatProperty StyleParser::parseTableColumnProperty( QDomElement &parent )
{
  TableColumnFormatProperty property;

  const double width = convertUnit( parent.attribute( "column-width" ) );
  property.setWidth( width );

  return property;
}

TableCellFormatProperty StyleParser::parseTableCellProperty( QDomElement &parent )
{
  TableCellFormatProperty property;

  if ( parent.hasAttribute( "background-color" ) )
    property.setBackgroundColor( QColor( parent.attribute( "background-color" ) ) );

  property.setPadding( convertUnit( parent.attribute( "padding" ) ) );

  static QMap<QString, Qt::Alignment> map;
  if ( map.isEmpty() ) {
    map.insert( "top", Qt::AlignTop );
    map.insert( "middle", Qt::AlignVCenter );
    map.insert( "bottom", Qt::AlignBottom );
    map.insert( "left", Qt::AlignLeft );
    map.insert( "right", Qt::AlignRight );
    map.insert( "center", Qt::AlignHCenter );
  }

  if ( parent.hasAttribute( "align" ) && parent.hasAttribute( "vertical-align" ) ) {
    property.setAlignment( map[ parent.attribute( "align" ) ] | map[ parent.attribute( "vertical-align" ) ] );
  } else if ( parent.hasAttribute( "align" ) ) {
    property.setAlignment( map[ parent.attribute( "align" ) ] );
  } else if ( parent.hasAttribute( "vertical-align" ) ) {
    property.setAlignment( map[ parent.attribute( "vertical-align" ) ] );
  }

  return property;
}

double StyleParser::convertUnit( const QString &data )
{
  #define MM_TO_POINT(mm) ((mm)*2.83465058)
  #define CM_TO_POINT(cm) ((cm)*28.3465058)
  #define DM_TO_POINT(dm) ((dm)*283.465058)
  #define INCH_TO_POINT(inch) ((inch)*72.0)
  #define PI_TO_POINT(pi) ((pi)*12)
  #define DD_TO_POINT(dd) ((dd)*154.08124)
  #define CC_TO_POINT(cc) ((cc)*12.840103)

  double points = 0;
  if ( data.endsWith( "pt" ) ) {
    points = data.left( data.length() - 2 ).toDouble();
  } else if ( data.endsWith( "cm" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = CM_TO_POINT( value );
  } else if ( data.endsWith( "mm" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = MM_TO_POINT( value );
  } else if ( data.endsWith( "dm" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = DM_TO_POINT( value );
  } else if ( data.endsWith( "in" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = INCH_TO_POINT( value );
  } else if ( data.endsWith( "inch" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = INCH_TO_POINT( value );
  } else if ( data.endsWith( "pi" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = PI_TO_POINT( value );
  } else if ( data.endsWith( "dd" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = DD_TO_POINT( value );
  } else if ( data.endsWith( "cc" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = CC_TO_POINT( value );
  } else {
    if ( !data.isEmpty() ) {
      qDebug( "unknown unit for '%s'", qPrintable( data ) );
    }
    points = 12;
  }

  return points;
}
