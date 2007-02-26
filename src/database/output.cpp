#include "output.h"
#include "qregexp.h"
#include <QFile>
#include <QTextStream>
#include "settings.h"

QMap <Output::OutputType,QString> Output::m_outputMap;

Output::Output(OutputType output, const QString& pathToTemplateFile)
{
   m_outputType = output;
   m_whiteColumnSpace = " ";
   m_blackColumnSpace = " ";
   setTemplateFile(pathToTemplateFile);
   initialize();
}

void Output::initialize()
{
   m_variationIndentStr = "";
   if (m_outputType == Pgn) {
      for (int i = 0;i < m_options.getOptionAsInt("VariationIndentSize");i++) {
         m_variationIndentStr += " ";
      }
      m_newlineChar = "\n";
   } else if (m_outputType == Html) {
      m_newlineChar = "<br>\n";
   } else if (m_outputType == Latex) {
      m_newlineChar = "\n";
   } else if (m_outputType == NotationWidget) {
      m_newlineChar = "<br>\n";
   }
   ReadTemplateFile(m_templateFilename);
   // qDebug ("tempfilename - %s",m_templateFilename.latin1());
}

void Output::ReadTemplateFile(QString path)
{
   QFile file( path);
   QStringList optionDefFields;
   enum ReadingStatus {NONE, ReadingOptionDefs, ReadingOptions, ReadingOutputHeader, ReadingOutputFooter, ReadingMarkupTags};
   ReadingStatus status = NONE; 
   m_header = "";
   m_footer = "";
   //qDebug ("Reading template file %s", path.latin1());
   if ( file.open( QIODevice::ReadOnly ) ) {
      QTextStream stream( &file );
      QString line;
      int i = 0;
      while ( !stream.atEnd() ) {
         line = stream.readLine(); // line of text excluding '\n'
         i++;
         //qDebug ("Line %d",i);
         if ((line.indexOf(QRegExp("^\\s*$")) != -1) || (line.indexOf(QRegExp("^\\s*#")) != -1)) {
            // Skip blank lines and comments (#)
            continue;
         }
         if (line == "[OPTIONSDEFINITIONS]") {
            status = ReadingOptionDefs;
            continue;
         } else if (line == "[OPTIONS]") {
            status = ReadingOptions;
            continue;
         } else if (line == "[OUTPUTHEADER]") {
            status = ReadingOutputHeader;
            continue;
         } else if (line == "[OUTPUTFOOTER]") {
            status = ReadingOutputFooter;
            continue;
         } else if (line == "[MARKUPTAGS]") {
            status = ReadingMarkupTags;
            continue;
         }
         line.replace("\\n","\n");

         switch (status) {
            case ReadingOptionDefs:
               optionDefFields = line.split(",");
               if (!m_options.createOption(optionDefFields[0],optionDefFields[1],
                        optionDefFields[2],optionDefFields[3],optionDefFields[4])) {
                  qWarning ("Could not create option. Ignoring line %d in file %s :\n%s",i,path.toLatin1().constData(),line.toLatin1().constData());
               }

               break;
            case ReadingOptions:
               if (!m_options.setOption(line)) {
                  qWarning ("Invalid option or value in file %s line %d. Ignoring : %s",path.toLatin1().constData(),i,line.toLatin1().constData());
               }
               break;
            case ReadingOutputHeader:
               m_header += line;
               break;
            case ReadingOutputFooter:
               m_footer += line;
               break;
            case ReadingMarkupTags:
               {
               QString name = line.mid(0,line.indexOf('='));
               QStringList tags = (line.mid(line.indexOf('=')+1)).split (',');
               if (name == "MarkupHeaderBlock") {
                  setMarkupTag(MarkupHeaderBlock,tags[0],tags[1]);
               } else if (name == "MarkupNotationBlock") {
                  setMarkupTag(MarkupNotationBlock,tags[0],tags[1]); 
               } else if (name == "MarkupResult") {
                  setMarkupTag(MarkupResult,tags[0],tags[1]);
               } else if (name == "MarkupMainLineMove") {
                  setMarkupTag(MarkupMainLineMove,tags[0],tags[1]);
               } else if (name == "MarkupVariationMove") {
                  setMarkupTag(MarkupVariationMove,tags[0],tags[1]);
               } else if (name == "MarkupMainLine") {
                  setMarkupTag(MarkupMainLine,tags[0],tags[1]);
               } else if (name == "MarkupVariationInline") {
                  setMarkupTag(MarkupVariationInline,tags[0],tags[1]);
               } else if (name == "MarkupVariationIndent") {
                  setMarkupTag(MarkupVariationIndent,tags[0],tags[1]);
               } else if (name == "MarkupNag") {
                  setMarkupTag(MarkupNag,tags[0],tags[1]);
               } else if (name == "MarkupAnnotationInline") {
                  setMarkupTag(MarkupAnnotationInline,tags[0],tags[1]);
               } else if (name == "MarkupAnnotationIndent") {
                  setMarkupTag(MarkupAnnotationIndent,tags[0],tags[1]);
               } else if (name == "MarkupHeaderLine") {
                  setMarkupTag(MarkupHeaderLine,tags[0],tags[1]);
               } else if (name == "MarkupHeaderTagName") {
                  setMarkupTag(MarkupHeaderTagName,tags[0],tags[1]);
               } else if (name == "MarkupHeaderTagValue") {
                  setMarkupTag(MarkupHeaderTagValue,tags[0],tags[1]);
               } else if (name == "MarkupWhiteTag") {
                  setMarkupTag(MarkupWhiteTag,tags[0],tags[1]);
               } else if (name == "MarkupBlackTag") {
                  setMarkupTag(MarkupBlackTag,tags[0],tags[1]);
               } else if (name == "MarkupEventTag") {
                  setMarkupTag(MarkupEventTag,tags[0],tags[1]);
               } else if (name == "MarkupDateTag") {
                  setMarkupTag(MarkupDateTag,tags[0],tags[1]);
               } else if (name == "MarkupSiteTag") {
                  setMarkupTag(MarkupSiteTag,tags[0],tags[1]);
               } else if (name == "MarkupResultTag") {
                  setMarkupTag(MarkupResultTag,tags[0],tags[1]);
               } else if (name == "MarkupRoundTag") {
                  setMarkupTag(MarkupRoundTag,tags[0],tags[1]);
               } else if (name == "MarkupColumnStyleMove") {
                  setMarkupTag(MarkupColumnStyleMove,tags[0],tags[1]);
               } else if (name == "MarkupColumnStyleRow") {
                  setMarkupTag(MarkupColumnStyleRow,tags[0],tags[1]);
               } else if (name == "MarkupColumnStyleMainline") {
                  setMarkupTag(MarkupColumnStyleMainline,tags[0],tags[1]);
               } else {
                  qWarning ("Unkown Markup Tag found in file %s line %d. Ignoring : %s", path.toLatin1().constData(), i,  line.toLatin1().constData());
               }
               }
               break;
            default :
               qWarning ("Unknown Section in Template File %s line %d : %s", path.toLatin1().constData(), i, line.toLatin1().constData());
         }
         //setWriteStatus(line);
         ////setWritingColor();
         //m_pgnText += line ;
      }
      file.close();
   }
}

QMap<Output::OutputType,QString>& Output::getFormats()
{
   m_outputMap.clear();
   m_outputMap[Html] = "Html Output";
   m_outputMap[Pgn] = "Pgn Output";
   m_outputMap[Latex] = "Latex Output";
   m_outputMap[NotationWidget] = "Notation Widget Output";
   return m_outputMap;
}

void Output::writeMove(int variation)
{
   QString mvno = QString::number(m_game->moveId(variation));
   if (m_options.getOptionAsBool("ColumnStyle") && (m_currentVariationLevel == 0)) {
      // *** If the column style option is set and it's a mainline move
      // *** some special markup is required
      if (m_game->board().toMove() == White) {
         // ** If it is white to move, start a new row
         m_output += m_startTagMap[MarkupColumnStyleRow] + m_startTagMap[MarkupColumnStyleMove];
         if (m_currentVariationLevel > 0) {
            m_output += m_startTagMap[MarkupVariationMove].arg(mvno);
         } else {
            m_output += m_startTagMap[MarkupMainLineMove].arg(mvno);
         }
         m_output += QString::number(m_game->moveNumber()) + ".";
      } else if (m_dirtyBlack) {
         // ** If it is black to move and we need the ... then add a extra
         // ** column before adding the column for the move
         m_output += m_startTagMap[MarkupColumnStyleRow] + m_startTagMap[MarkupColumnStyleMove];
         m_output += QString::number(m_game->moveNumber()) + ". ...";
         m_output += m_startTagMap[MarkupColumnStyleMove];
         if (m_currentVariationLevel > 0) {
            m_output += m_startTagMap[MarkupVariationMove].arg(mvno);
         } else {
            m_output += m_startTagMap[MarkupMainLineMove].arg(mvno);
         }
      } else {
         // ** If it is black to move and we don't need the ... 
         m_output += m_startTagMap[MarkupColumnStyleMove];
         if (m_currentVariationLevel > 0) {
            m_output += m_startTagMap[MarkupVariationMove].arg(mvno);
         } else {
            m_output += m_startTagMap[MarkupMainLineMove].arg(mvno);
         }
      }  
   } else {
      // *** Markup for the move
      if (m_currentVariationLevel > 0) {
         m_output += m_startTagMap[MarkupVariationMove].arg(mvno);
      } else {
         m_output += m_startTagMap[MarkupMainLineMove].arg(mvno);
      }
      // *** Write the move number
      if (m_game->board().toMove() == White) {
         m_output += QString::number(m_game->moveNumber()) + ".";
      } else if (m_dirtyBlack) {
         m_output += QString::number(m_game->moveNumber()) + "...";
      }
   }
   m_dirtyBlack = false;

   // *** Write the actual move
   m_output += m_game->moveToSan(Game::MoveOnly,variation);
   // *** End the markup for the move
   if (m_currentVariationLevel > 0) {
      m_output += m_endTagMap[MarkupVariationMove];
   } else {
      m_output += m_endTagMap[MarkupMainLineMove];
   }
   // *** Write the nags if there are any
   if(m_game->nags(variation).count() > 0) {
      m_output += m_startTagMap[MarkupNag] + m_game->nags(variation).toString() + m_endTagMap[MarkupNag];
   }
   // *** Write the annotations if any
   if (m_game->annotation(variation) != QString::null) {
      if (m_options.getOptionAsBool("ColumnStyle") && (m_currentVariationLevel == 0)) {
         m_output += m_endTagMap[MarkupColumnStyleMainline];
      }
      if ((m_options.getOptionAsString("CommentIndent") == "Always") 
            || ((m_options.getOptionAsString("CommentIndent") == "OnlyMainline")
               && (m_currentVariationLevel == 0))) {
         m_output += m_startTagMap[MarkupAnnotationIndent].arg(mvno) +
                     m_game->annotation(variation) +
                     m_endTagMap[MarkupAnnotationIndent];
      } else {
         m_output += " " + m_startTagMap[MarkupAnnotationInline].arg(mvno) +
                     m_game->annotation(variation) +
                     m_endTagMap[MarkupAnnotationInline];
      }
      if (m_options.getOptionAsBool("ColumnStyle") && (m_currentVariationLevel == 0)) {
         m_output += m_startTagMap[MarkupColumnStyleMainline];
      }
      m_dirtyBlack = true;
   }
   m_output += " ";
}
void Output::writeNewlineIndent()
{
   int indentLevel;
   if (m_currentVariationLevel > m_options.getOptionAsInt("VariationIndentLevel")) {
      indentLevel = m_options.getOptionAsInt("VariationIndentLevel");
   } else {
      indentLevel = m_currentVariationLevel;
   }
   if (m_options.getOptionAsInt("VariationIndentLevel") >= 0) {
      m_output += m_newlineChar;
      for (int i = 0;i < indentLevel;++i) {
         m_output += m_variationIndentStr;
      }
   }
}
void Output::writeVariation()
{
	while(!m_game->atEnd()) {		
      // *** Writes move in the current variation
      writeMove();
      if (m_game->variationCount() > 1) {
         for (int i = 1;i < m_game->variationCount();++i) {
            m_currentVariationLevel += 1;
            if (m_options.getOptionAsBool("ColumnStyle") && (m_currentVariationLevel == 1)) {
               m_output += m_endTagMap[MarkupColumnStyleMainline];
            }
            //qDebug ("VariationIndentLevel - %d",m_options.getOptionAsInt("VariationIndentLevel"));
            if (m_currentVariationLevel <= m_options.getOptionAsInt("VariationIndentLevel")) {
               m_output += m_startTagMap[MarkupVariationIndent];
            } else {
               m_output += m_startTagMap[MarkupVariationInline];
            }

            m_output +=  "(";
            m_dirtyBlack = true;
				
            // *** Writes the first move in variation i
            writeMove(i);
				
            // *** Enter variation i, and write the rest of the moves
				m_game->enterVariation(i);
				writeVariation();
				m_game->exitVariation();
				
            // *** End the variation
            m_output.replace ( QRegExp ("\\s+$"), "" ); // We don't want any spaces before the )
            m_output += ")";
            //qDebug ("VariationIndentLevel - %d",m_options.getOptionAsInt("VariationIndentLevel"));
            if (m_currentVariationLevel <= m_options.getOptionAsInt("VariationIndentLevel")) {
               m_output += m_endTagMap[MarkupVariationIndent];
            } else {
               m_output += m_endTagMap[MarkupVariationInline];
            }
            m_currentVariationLevel -= 1;
            if (m_options.getOptionAsBool("ColumnStyle") && (m_currentVariationLevel == 0)) {
               m_output += m_startTagMap[MarkupColumnStyleMainline];
            }
            m_dirtyBlack = true;
			}
		}
		m_game->forward();
	}
}
   
void Output::writeTag(QString tagName)
{
   MarkupType specialTag = MarkupNoFormat;
   if (tagName == "Event") {
      specialTag = MarkupEventTag;
   } else if (tagName == "Site") {
      specialTag = MarkupSiteTag;
   } else if (tagName == "Round") {
      specialTag = MarkupRoundTag;
   } else if (tagName == "Date") {
      specialTag = MarkupDateTag;
   } else if (tagName == "White") {
      specialTag = MarkupWhiteTag;
   } else if (tagName == "Black") {
      specialTag = MarkupBlackTag;
   } else if (tagName == "Result") {
      specialTag = MarkupResultTag;
   }

   m_output += m_startTagMap[MarkupHeaderLine] +
               m_startTagMap[specialTag] +
               m_startTagMap[MarkupHeaderTagName] +
               tagName + " " +
               m_endTagMap[MarkupHeaderTagName] +
               m_startTagMap[MarkupHeaderTagValue] +
               m_game->tag(tagName) +
               m_endTagMap[MarkupHeaderTagValue] +
               m_endTagMap[specialTag] +
               m_endTagMap[MarkupHeaderLine];
   //writeNewlineIndent(); //fine

}

QString Output::output(Game* game)
{

   m_game = game;
   int id = m_game->currentMoveId();
   m_dirtyBlack = false;
   m_currentVariationLevel = 0;

   m_output = m_header;
   m_output += m_startTagMap[MarkupHeaderBlock];
   writeTag("Event");
   writeTag("Site");
   writeTag("Date");
   writeTag("Round");
   writeTag("White");
   writeTag("Black");
   writeTag("Result");
   m_output += m_endTagMap[MarkupHeaderBlock];
   //writeNewlineIndent(); //fine 
   m_game->moveToStart();
   m_output += m_startTagMap[MarkupNotationBlock];
   m_output += m_startTagMap[MarkupMainLine];
   if (m_options.getOptionAsBool("ColumnStyle")) {
      m_output += m_startTagMap[MarkupColumnStyleMainline];
   }
   writeVariation();
   if (m_options.getOptionAsBool("ColumnStyle")) {
      m_output += m_endTagMap[MarkupColumnStyleMainline];
   }
   m_output += m_endTagMap[MarkupMainLine];
   m_output += m_endTagMap[MarkupNotationBlock];
   m_output += m_startTagMap[MarkupResult] + resultString(game->result()) + m_endTagMap[MarkupResult];
   m_output += m_footer;

   QRegExp var("@(\\w+)@"); 
   while (var.indexIn(m_output) != -1) {
      QStringList cap = var.capturedTexts();
      m_output.replace("@"+cap[1]+"@",m_options.getOptionAsString(cap[1]));
   }

   m_game->moveToId(id);
   return m_output;
}
void Output::setTemplateFile(const QString& filename)
{
   if (filename.isEmpty()) {
      switch (m_outputType) {
         case Html:
            m_templateFilename = DEFAULT_HTML_TEMPLATE;
            break;
         case Latex:
            m_templateFilename = DEFAULT_LATEX_TEMPLATE;
            break;
         case NotationWidget:
            m_templateFilename = DEFAULT_NOTATION_TEMPLATE;
            break;
         case Pgn:
            m_templateFilename = DEFAULT_PGN_TEMPLATE;
            break;
         default :
            qWarning ("Could not decide which template file to use. Maybe strange OutputType");
      }
   } else {
      m_templateFilename = filename;
   }
   
   if (!QFile::exists(m_templateFilename)) {
      QString dataPath;
      if (AppSettings) {
         dataPath = AppSettings->dataPath();
      } else {
         /* This is temporary. AppSettings should always be defined.
          * This enables testing the class in isolation from the main 
          * program
          */
         dataPath = "/home/mroets/data/chessx/qt3/data";
      }
      m_templateFilename = dataPath + "/" + TEMPLATE_DIR + "/" + m_templateFilename;
      if (!QFile::exists(m_templateFilename)) {
         qWarning("Could not set template file");
      }
      
   }

}

void Output::setMarkupTag (MarkupType type, const QString& startTag, const QString& endTag)
{
   m_startTagMap[type] = startTag;
   m_endTagMap[type] = endTag;
}
void Output::markupTag (MarkupType type , QString& startTag, QString& endTag)
{
   startTag = m_startTagMap[type];
   endTag = m_endTagMap[type];
}

bool Output::setOption(const QString& optionName, bool optionValue)
{
   return m_options.setOption(optionName, optionValue);
}
bool Output::setOption(const QString& optionName, int optionValue)
{
   return m_options.setOption(optionName, optionValue);
}
bool Output::setOption(const QString& optionName, const QString& optionValue)
{
   return m_options.setOption(optionName, optionValue);
}

/* Retrieving values */
int Output::getOptionAsInt (const QString& optionName)
{
   return m_options.getOptionAsInt(optionName);
}
QString Output::getOptionAsString (const QString& optionName)
{
   return m_options.getOptionAsString(optionName);
}
bool Output::getOptionAsBool (const QString& optionName)
{
   return m_options.getOptionAsBool(optionName);
}
QString Output::getOptionDescription (const QString& optionName)
{
   return m_options.getOptionDescription(optionName);
}
QStringList Output::getOptionList ()
{
   return m_options.getOptionList();
}
QMap<QString,QString> Output::getOptionListAndDescription ()
{
   return m_options.getOptionListAndDescription();
}

