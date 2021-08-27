/***************************************************************************
 *   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                 *
 *   Copyright (C) 2006 by Tobias Koenig <tokoe@kde.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef OOO_FORMATPROPERTY_H
#define OOO_FORMATPROPERTY_H

#include <QVector>
#include <QColor>
#include <QTextListFormat>

#define USETODONEXT Q_UNUSED

#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_MM(mm) ((mm)/2.83465058)     ////////  0.352777778
#define POINT_TO_DM(dm) ((dm)/283.465058)
#define POINT_TO_INCH(inch) ((inch)/72.0)
#define POINT_TO_PI(pi) ((pi)/12)
#define POINT_TO_DD(dd) ((dd)/154.08124)
#define POINT_TO_CC(cc) ((cc)/12.840103)

#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)     ///// 28.346456693
#define DM_TO_POINT(dm) ((dm)*283.465058)
#define INCH_TO_POINT(inch) ((inch)*72.0)
#define PI_TO_POINT(pi) ((pi)*12)
#define DD_TO_POINT(dd) ((dd)*154.08124)
#define CC_TO_POINT(cc) ((cc)*12.840103)

class QTextBlockFormat;
class QTextCharFormat;
class QTextFormat;
class QTextTableFormat;

namespace OOO {

class StyleInformation;

class FontFormatProperty
{
  public:
    FontFormatProperty();

    void apply( QTextFormat *format ) const;

    void setFamily( const QString &name );

  private:
    QString m_Family;
};

class ParagraphFormatProperty
{
  public:
    enum WritingMode
    {
      LRTB,
      RLTB,
      TBRL,
      TBLR,
      LR,
      RL,
      TB,
      PAGE
    };

    ParagraphFormatProperty();

    void apply( QTextFormat *format ) const;

    void setPageNumber( int number );
    void setWritingMode( WritingMode mode );
    void setTextAlignment( Qt::Alignment alignment );
    void setBackgroundColor( const QColor &color );
    void setLeftMargin( const qreal margin );

    bool writingModeIsRightToLeft() const;

  private:
    int m_PageNumber;
    WritingMode m_WritingMode;
    Qt::Alignment m_Alignment;
    bool m_HasAlignment;
    QColor m_BackgroundColor;
    bool m_HasLeftMargin;
    qreal m_LeftMargin;
};

class TextFormatProperty
{
  public:
    TextFormatProperty();
    TextFormatProperty( const StyleInformation *information );

    void apply( QTextCharFormat *format ) const;

    void setFontSize( int size );
    void setFontName( const QString &name );
    void setFontWeight( int weight );
    void setFontStyle( int style );
    void setTextPosition( int position );
    void setColor( const QColor &color );
    void setBackgroundColor( const QColor &color );
    void setUnderline(bool underline);

  private:
    const StyleInformation *m_StyleInformation;
    int m_FontSize;
    bool m_HasFontSize;
    int m_FontWeight;
    QString m_FontName;
    int m_FontStyle;
    int m_TextPosition;
    QColor m_Color;
    QColor m_BackgroundColor;
    bool m_underline;
};

class PageFormatProperty
{
  public:
    enum PageUsage
    {
      All,
      Left,
      Right,
      Mirrored
    };

    enum PrintOrientation
    {
      Portrait,
      Landscape
    };

    PageFormatProperty();

    void apply( QTextFormat *format ) const;

    void setPageUsage( PageUsage usage );
    void setBottomMargin( double margin );
    void setLeftMargin( double margin );
    void setTopMargin( double margin );
    void setRightMargin( double margin );
    void setHeight( double height );
    void setWidth( double width );
    void setPrintOrientation( PrintOrientation orientation );

    double width() const;
    double height() const;
    double margin() const;

  private:
    PageUsage m_PageUsage;
    double m_BottomMargin;
    double m_LeftMargin;
    double m_TopMargin;
    double m_RightMargin;
    double m_Height;
    double m_Width;
    PrintOrientation m_PrintOrientation;
};

class ListFormatProperty
{
  public:
    enum Type
    {
      Number,
      Bullet
    };

    ListFormatProperty();

    void apply(QTextListFormat *format) const;

    void addItem( int level, double indent = 0 );
    void setType(QTextListFormat::Style type);

  private:
    QTextListFormat::Style m_Type;
    QVector<double> m_Indents;
    QColor m_BackgroundColor;
};

class TableColumnFormatProperty
{
  public:
    TableColumnFormatProperty();

    void apply( QTextTableFormat *format ) const;

    void setWidth( double width );

  private:
    double m_Width;
    bool isValid;
};

class TableCellFormatProperty
{
  public:
    TableCellFormatProperty();

    void apply( QTextBlockFormat *format ) const;

    void setBackgroundColor( const QColor &color );
    void setPadding( double padding );
    void setAlignment( Qt::Alignment alignment );

  private:
    double m_Padding;
    Qt::Alignment m_Alignment;
    bool m_HasAlignment;
    QColor m_BackgroundColor;

};

class StyleFormatProperty
{
  public:
    StyleFormatProperty();
    StyleFormatProperty( const StyleInformation *information );

    void applyBlock( QTextBlockFormat *format ) const;
    void applyText( QTextCharFormat *format ) const;
    void applyTableColumn( QTextTableFormat *format ) const;
    void applyTableCell( QTextBlockFormat *format ) const;

    void setParentStyleName( const QString &parentStyleName );
    QString parentStyleName() const;

    void setFamily( const QString &family );
    void setDefaultStyle( bool defaultStyle );

    void setMasterPageName( const QString &masterPageName );

    void setParagraphFormat( const ParagraphFormatProperty &format );
    void setTextFormat( const TextFormatProperty &format );
    void setTableColumnFormat( const TableColumnFormatProperty &format );
    void setTableCellFormat( const TableCellFormatProperty &format );

  private:
    QString m_ParentStyleName;
    QString m_Family;
    QString m_MasterPageName;
    ParagraphFormatProperty m_ParagraphFormat;
    TextFormatProperty m_TextFormat;
    TableColumnFormatProperty m_TableColumnFormat;
    TableCellFormatProperty m_TableCellFormat;
    const StyleInformation *m_StyleInformation;
    bool m_DefaultStyle;
};

}

#endif
