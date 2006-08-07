
#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <qstring.h>
#include <qmap.h>
#include "game.h"



class Output
{
   public:
      enum MarkupType {
         MarkupHeaderBlock,
         MarkupNotationBlock,
         MarkupResult,
         MarkupNoFormat,
         MarkupColumnStyleMainline,
         MarkupColumnStyleMove,
         MarkupColumnStyleRow,
         MarkupMainLineMove,
         MarkupMainLine,
         MarkupVariationMove,
         MarkupVariationInline,
         MarkupVariationIndent,
         MarkupNag,
         MarkupAnnotationInline,
         MarkupAnnotationIndent,
         MarkupHeaderLine,
         MarkupHeaderTagName,
         MarkupHeaderTagValue,
         MarkupWhiteTag,
         MarkupBlackTag,
         MarkupEventTag,
         MarkupDateTag,
         MarkupSiteTag,
         MarkupResultTag,
         MarkupRoundTag
      };
      enum OutputType {
         Html,
         Pgn,
         Latex,
         NotationWidget
      };
      enum CommentIndentOption {
         Always,
         OnlyMainline,
         Never
      };
      Output (OutputType output);

      /** Sets the default settings for the specific output format */
      void initialize();

      /** Sets the start and end tag for a certain markup type */
      void setMarkupTag (MarkupType type, const QString& startTag, const QString& endTag);
      void markupTag (MarkupType type , QString& startTag, QString& endTag);

      /** Static list of objects. */
      static QMap<OutputType,QString>& getFormats();

      /* Read and write tag settings to user settings */
      void readConfig();
      void writeConfig();

      /* Reload default tag settings */
      void reset();

      /* Create the output for the given game */
      QString output(Game* game);

      /* User definable settings */
      void setTextWidth(int textWidth);
      int textWidth();
      void setColumnStyle (bool columnStyle);
      bool columnStyle();
      void setVariationIndentSize(int variationIndentSize);
      int variationIndentSize();
      void setVariationIndentLevel(int variationIndentLevel);
      int variationIndentLevel();
      void setCommentIndentOption(CommentIndentOption option);
      CommentIndentOption commentIndentOption();


   private:
      /* User definable settings */
      /** If true, output will only have one move per line in 2 columns */
      bool m_columnStyle;
      /** Maximum number of characters in one line. Markup chars not counted */
      int m_textWidth;
      /** Number of spaces used for each indent level */
      int m_variationIndentSize;
      /** Level up to which to indent
       *  -1 - No indentation
       *  0 - variation start on new line, but not indented
       *  1,2,n  Each level of variation is one level more indented, up to a maximum of n.
       */
      int m_variationIndentLevel;
      /** Currently there are 3 options. Here indented means on a new line at the same level
       * of indentation as the previous line
       * Never: All comments are inline
       * OnlyMainline : Only comments in the mainline are indented
       * Always : All comments are indented
       */
      CommentIndentOption m_commentIndentOption;

      /* Internally used */
      bool m_justBreaked;
      /** Text to be written at the top of the output */
      QString m_header;
      /** Text to be written at the bottom of the output */
      QString m_footer;
      /** The type of output that the object will generate */
      OutputType m_outputType;
      /** Indicator whether or not to write the move number, when it is black to move */
      bool m_dirtyBlack;
      /** Keep track of the current level of variation, for indent purposes */
      int m_currentVariationLevel;
      /** Character/string used for newline */
      QString m_newlineChar;
      /** String used once for each level of indentation */
      QString m_variationIndentStr;
      /** Pointer to the game being exported */
      QString m_whiteColumnSpace;
      QString m_blackColumnSpace;
      Game * m_game;
      /** The export result */
      QString m_output;
      /** Map containing the different types of outputs available, and a description of each */
      static QMap <OutputType,QString> m_outputMap;
      /** Map containing the start markup tag for each markup type */
      QMap <MarkupType,QString> m_startTagMap;
      /** Map containing the end markup tag for each markup type */
      QMap <MarkupType,QString> m_endTagMap;

      /* Writing Methods */
      /** Writes a single move including nag and annotation */
      void writeMove(int variation=0);
      /** Writes a variation, including sub variations */
      void writeVariation();
      /** Writes a newline, and indent the following line */
      void writeNewlineIndent();
      /** Writes a game tag */
      void writeTag(QString tagName);

};

#endif

