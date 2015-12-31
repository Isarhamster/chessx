/***************************************************************************
 *   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                 *
 *   Copyright (C) 2006 by Tobias Koenig <tokoe@kde.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "formatproperty.h"

#include <QtGui/QTextFormat>

#include "styleinformation.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

using namespace OOO;

FontFormatProperty::FontFormatProperty()
    : m_Family( "Nimbus Sans L" )
{
}

void FontFormatProperty::apply( QTextFormat *format ) const
{
    format->setProperty( QTextFormat::FontFamily, m_Family );
}


void FontFormatProperty::setFamily( const QString &name )
{
    m_Family = name;
}

ParagraphFormatProperty::ParagraphFormatProperty()
    : m_PageNumber( 0 ), m_WritingMode( LRTB ), m_Alignment( Qt::AlignLeft ),
      m_HasAlignment( false ), m_BackgroundColor(Qt::transparent),
      m_HasLeftMargin( false ), m_LeftMargin(0.0)
{
}

void ParagraphFormatProperty::apply( QTextFormat *format ) const
{
    if ( m_WritingMode == LRTB || m_WritingMode == TBLR || m_WritingMode == LR || m_WritingMode == TB )
        format->setLayoutDirection( Qt::LeftToRight );
    else
        format->setLayoutDirection( Qt::RightToLeft );

    if ( m_HasAlignment )
    {
        static_cast<QTextBlockFormat*>( format )->setAlignment( m_Alignment );
    }

    format->setProperty( QTextFormat::FrameWidth, 595 );

    if ( m_HasLeftMargin )
    {
        static_cast<QTextBlockFormat*>( format )->setLeftMargin( m_LeftMargin );
    }

    if ( m_BackgroundColor.isValid() )
    {
        format->setBackground( m_BackgroundColor );
    }
}

void ParagraphFormatProperty::setPageNumber( int number )
{
    m_PageNumber = number;
}

void ParagraphFormatProperty::setWritingMode( WritingMode mode )
{
    m_WritingMode = mode;
}

bool ParagraphFormatProperty::writingModeIsRightToLeft() const
{
    return ( ( m_WritingMode == RLTB ) || ( m_WritingMode == TBRL ) || ( m_WritingMode == RL ) );
}

void ParagraphFormatProperty::setTextAlignment( Qt::Alignment alignment )
{
    m_HasAlignment = true;
    m_Alignment = alignment;
}

void ParagraphFormatProperty::setBackgroundColor( const QColor &color )
{
    m_BackgroundColor = color;
    if (!m_BackgroundColor.isValid())  m_BackgroundColor = Qt::transparent;
}

void ParagraphFormatProperty::setLeftMargin( const qreal margin )
{
    m_HasLeftMargin = true;
    m_LeftMargin = margin;
}

TextFormatProperty::TextFormatProperty()
    : m_StyleInformation( 0 ), m_HasFontSize( false ),
      m_FontWeight( -1 ), m_FontStyle( -1 ), m_TextPosition( 0 ),
      m_Color(Qt::black), m_BackgroundColor(Qt::transparent), m_underline(false)
{
}

TextFormatProperty::TextFormatProperty( const StyleInformation *information )
    : m_StyleInformation( information ), m_HasFontSize( false ),
      m_FontWeight( -1 ), m_FontStyle( -1 ), m_TextPosition( 0 ),
      m_Color(Qt::black), m_BackgroundColor(Qt::transparent), m_underline(false)
{
}

void TextFormatProperty::apply( QTextCharFormat *format ) const
{
    if ( !m_FontName.isEmpty() ) {
        if ( m_StyleInformation ) {
            const FontFormatProperty property = m_StyleInformation->fontProperty( m_FontName );
            property.apply( format );
        }
    }

    if ( m_FontWeight != -1 ) {
        QFont font = format->font();
        font.setWeight( m_FontWeight );
        format->setFont( font );
    }

    if ( m_HasFontSize ) {
        QFont font = format->font();
        font.setPointSize( m_FontSize );
        format->setFont( font );
    }

    if ( m_FontStyle != -1 ) {
        QFont font = format->font();
        font.setStyle( (QFont::Style)m_FontStyle );
        format->setFont( font );
    }

    if ( m_Color.isValid() )
        format->setForeground( m_Color );

    if ( m_BackgroundColor.isValid() )
        format->setBackground( m_BackgroundColor );

    if ( m_underline )
        format->setUnderlineStyle(QTextCharFormat::SingleUnderline);

    // TODO: get FontFormatProperty and apply it
    // TODO: how to set the base line?!?
}

void TextFormatProperty::setFontSize( int size )
{
    m_HasFontSize = true;
    m_FontSize = size;
}

void TextFormatProperty::setFontName( const QString &name )
{
    m_FontName = name;
}

void TextFormatProperty::setFontWeight( int weight )
{
    m_FontWeight = weight;
}

void TextFormatProperty::setFontStyle( int style )
{
    m_FontStyle = style;
}

void TextFormatProperty::setTextPosition( int position )
{
    m_TextPosition = position;
}

void TextFormatProperty::setColor( const QColor &color )
{
    m_Color = color;
}

void TextFormatProperty::setBackgroundColor( const QColor &color )
{
    m_BackgroundColor = color;
    if (!m_BackgroundColor.isValid())  m_BackgroundColor = Qt::transparent;
}

void TextFormatProperty::setUnderline( bool underline )
{
    m_underline = underline;
}

StyleFormatProperty::StyleFormatProperty()
    : m_StyleInformation( 0 ), m_DefaultStyle( false )
{
}

StyleFormatProperty::StyleFormatProperty( const StyleInformation *information )
    : m_StyleInformation( information ), m_DefaultStyle( false )
{
}

void StyleFormatProperty::applyBlock( QTextBlockFormat *format ) const
{
    if ( !m_DefaultStyle && !m_Family.isEmpty() && m_StyleInformation ) {
        const StyleFormatProperty property = m_StyleInformation->styleProperty( m_Family );
        property.applyBlock( format );
    }

    if ( !m_ParentStyleName.isEmpty() && m_StyleInformation ) {
        const StyleFormatProperty property = m_StyleInformation->styleProperty( m_ParentStyleName );
        property.applyBlock( format );
    }

    m_ParagraphFormat.apply( format );
}

void StyleFormatProperty::applyText( QTextCharFormat *format ) const
{
    if ( !m_DefaultStyle && !m_Family.isEmpty() && m_StyleInformation ) {
        const StyleFormatProperty property = m_StyleInformation->styleProperty( m_Family );
        property.applyText( format );
    }

    if ( !m_ParentStyleName.isEmpty() && m_StyleInformation ) {
        const StyleFormatProperty property = m_StyleInformation->styleProperty( m_ParentStyleName );
        property.applyText( format );
    }

    m_TextFormat.apply( format );
}

void StyleFormatProperty::applyTableColumn( QTextTableFormat *format ) const
{
    if ( !m_DefaultStyle && !m_Family.isEmpty() && m_StyleInformation ) {
        const StyleFormatProperty property = m_StyleInformation->styleProperty( m_Family );
        property.applyTableColumn( format );
    }

    if ( !m_ParentStyleName.isEmpty() && m_StyleInformation ) {
        const StyleFormatProperty property = m_StyleInformation->styleProperty( m_ParentStyleName );
        property.applyTableColumn( format );
    }

    m_TableColumnFormat.apply( format );
}

void StyleFormatProperty::applyTableCell( QTextBlockFormat *format ) const
{
    if ( !m_DefaultStyle && !m_Family.isEmpty() && m_StyleInformation ) {
        const StyleFormatProperty property = m_StyleInformation->styleProperty( m_Family );
        property.applyTableCell( format );
    }

    if ( !m_ParentStyleName.isEmpty() && m_StyleInformation ) {
        const StyleFormatProperty property = m_StyleInformation->styleProperty( m_ParentStyleName );
        property.applyTableCell( format );
    }

    m_TableCellFormat.apply( format );
}

void StyleFormatProperty::setParentStyleName( const QString &parentStyleName )
{
    m_ParentStyleName = parentStyleName;
}

QString StyleFormatProperty::parentStyleName() const
{
    return m_ParentStyleName;
}

void StyleFormatProperty::setFamily( const QString &family )
{
    m_Family = family;
}

void StyleFormatProperty::setDefaultStyle( bool defaultStyle )
{
    m_DefaultStyle = defaultStyle;
}

void StyleFormatProperty::setMasterPageName( const QString &masterPageName )
{
    m_MasterPageName = masterPageName;
}

void StyleFormatProperty::setParagraphFormat( const ParagraphFormatProperty &format )
{
    m_ParagraphFormat = format;
}

void StyleFormatProperty::setTextFormat( const TextFormatProperty &format )
{
    m_TextFormat = format;
}

void StyleFormatProperty::setTableColumnFormat( const TableColumnFormatProperty &format )
{
    m_TableColumnFormat = format;
}

void StyleFormatProperty::setTableCellFormat( const TableCellFormatProperty &format )
{
    m_TableCellFormat = format;
}

PageFormatProperty::PageFormatProperty()
    : m_PageUsage(All), m_BottomMargin(0.0), m_LeftMargin(0.0), m_TopMargin(0.0),
      m_RightMargin(0.0), m_Height(0.0), m_Width(0.0), m_PrintOrientation(Portrait)
{
}

void PageFormatProperty::apply( QTextFormat *format ) const
{
    format->setProperty( QTextFormat::BlockBottomMargin, m_BottomMargin );
    format->setProperty( QTextFormat::BlockLeftMargin, m_LeftMargin );
    format->setProperty( QTextFormat::BlockTopMargin, m_TopMargin );
    format->setProperty( QTextFormat::BlockRightMargin, m_RightMargin );
    format->setProperty( QTextFormat::FrameWidth, m_Width );
    format->setProperty( QTextFormat::FrameHeight, m_Height );
}

void PageFormatProperty::setPageUsage( PageUsage usage )
{
    m_PageUsage = usage;
}

void PageFormatProperty::setBottomMargin( double margin )
{
    m_BottomMargin = margin;
}

void PageFormatProperty::setLeftMargin( double margin )
{
    m_LeftMargin = margin;
}

void PageFormatProperty::setTopMargin( double margin )
{
    m_TopMargin = margin;
}

void PageFormatProperty::setRightMargin( double margin )
{
    m_RightMargin = margin;
}

void PageFormatProperty::setHeight( double height )
{
    m_Height = height;
}

void PageFormatProperty::setWidth( double width )
{
    m_Width = width;
}

void PageFormatProperty::setPrintOrientation( PrintOrientation orientation )
{
    m_PrintOrientation = orientation;
}

double PageFormatProperty::width() const
{
    return m_Width;
}

double PageFormatProperty::height() const
{
    return m_Height;
}

double PageFormatProperty::margin() const
{
    return m_LeftMargin;
}

ListFormatProperty::ListFormatProperty()
    : m_Type( QTextListFormat::ListDisc ), m_BackgroundColor(Qt::transparent)
{
    m_Indents.resize( 10 );
}

void ListFormatProperty::setType( QTextListFormat::Style type )
{
    m_Type = type;
}

void ListFormatProperty::apply( QTextListFormat *format ) const
{
    format->setStyle( m_Type );
    format->setBackground(m_BackgroundColor);
}

void ListFormatProperty::addItem( int level, double indent )
{
    if ( level < 0 || level >= 10 )
        return;

    m_Indents[ level ] = indent;
}

TableColumnFormatProperty::TableColumnFormatProperty()
    : m_Width( 0 ), isValid( false )
{
}

void TableColumnFormatProperty::apply( QTextTableFormat *format ) const
{
    if ( ! isValid ) {
        return;
    }
    QVector<QTextLength> lengths = format->columnWidthConstraints();
    lengths.append( QTextLength( QTextLength::FixedLength, m_Width ) );

    format->setColumnWidthConstraints( lengths );
}

void TableColumnFormatProperty::setWidth( double width )
{
    m_Width = width;
    isValid = true;
}

TableCellFormatProperty::TableCellFormatProperty()
    : m_Padding( 0 ), m_HasAlignment( false ), m_BackgroundColor(Qt::transparent)
{
}

void TableCellFormatProperty::apply( QTextBlockFormat *format ) const
{
    if ( m_BackgroundColor.isValid() )
        format->setBackground( m_BackgroundColor );

    if ( m_HasAlignment )
        format->setAlignment( m_Alignment );
}

void TableCellFormatProperty::setBackgroundColor( const QColor &color )
{
    m_BackgroundColor = color;
    if (!m_BackgroundColor.isValid())  m_BackgroundColor = Qt::transparent;
}

void TableCellFormatProperty::setPadding( double padding )
{
    m_Padding = padding;
}

void TableCellFormatProperty::setAlignment( Qt::Alignment alignment )
{
    m_Alignment = alignment;
    m_HasAlignment = true;
}
