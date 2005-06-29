/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 28/05/2005
    copyright            : (C) 2005 Ejner Borgbjerg 
                           <ejner@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "pgnparser.h"
#include <qfile.h>
//#include <iostream.h>


PgnParser::PgnParser(const QString & filename) {
  QFile* f;
  f = new QFile(filename);
//TODO: handle failure to open
  f->open( IO_ReadOnly );
  m_iStream = new QTextStream(f);
  m_parserMessages = new QStringList();
}

PgnParser::PgnParser(const QString * pgnText) {
   m_iStream = new QTextIStream(pgnText);
   m_parserMessages = new QStringList();
}

PgnParser::~PgnParser()  {
  delete m_iStream;
  delete m_parserMessages;
}


void PgnParser::parseGame(Game & g){
   m_parseError = false;
   parseInitialComment(g);
   parseTags(g);
   parseMoves(g);
}

void PgnParser::parseInitialComment(Game & game){
    m_tagsBegun = false;
    while (!m_tagsBegun && !isEndOfInput()){
      readLine();
      m_tagsBegun = m_currentLine.startsWith(QString("["));
      if (!m_tagsBegun) {
         game.addInitialComment(m_currentLine);
      }
    }
}

void PgnParser::parseTags(Game & game){
    m_movesBegun = false;
    while (!m_movesBegun && !isEndOfInput()){
       m_movesBegun = !m_currentLine.startsWith(QString("["));
       if (!m_movesBegun) {
          game.addTag(m_currentLine);
       }
       readLine();
    }
    if (!m_movesBegun){
        addWarning("End of input in PGN header tags section");
    }
}

void PgnParser::parseMoves(Game & game){
    m_gameOver = false;
    m_inComment = false;
    while (!m_gameOver && !m_parseError && !isEndOfInput()){
       if (m_inComment) {
          readComment(game);
       }
       else {
          parseLine(game);
       }
    }

    if (isEndOfInput()) {
       if (m_inComment) {
          addError(QString("Open comment at end of input started on line %2").arg(m_commentStartLine));
       }
       if (!m_gameOver) {
          addWarning(QString("End of input reached in a game"));
       }
    }

    if (m_parseError) {
       windForwardToNextGame();
    }
}

void PgnParser::parseLine(Game & game){
// assert(!m_inComment)
// split(s,x,true) returns null strings when s matches several times
// this is needed to update m_pos.
    QStringList list = QStringList::split(" ",m_currentLine,true);
    m_pos = 0;
    for (QStringList::Iterator it = list.begin(); it != list.end() && !m_inComment; ++it) {
       m_str = *it;
       handleString(game,m_str);
       m_pos += m_str.length()+1;
    };
    if (!m_inComment){
       readLine();
    }
}

void PgnParser::handleString(Game & game, const QString & s){
// assert(!m_inComment)
    if (s.isNull())
       return;

    if (isResult(s) ) {
       game.addResult(s);
       m_gameOver = true;
    }
    else if (s.startsWith("$")){
       game.addNag(s);
    }
    else if (s.startsWith("(")){
       game.enterVariation();
    }
    else if (s.startsWith(")")){
       game.exitVariation();
    }
    else if (s.startsWith("{")){ 
       m_comment = "";
       m_commentStartLine = m_lineNumber;
       m_inComment = true; // changes state: inside comment
       m_currentLine = m_currentLine.right(m_currentLine.length()-m_pos-1);
    }
    else if (s.contains("}")) {
       addWarning("close-brace character '}' seen outside a comment");
    }
    else if (s.contains("...")) {
       handleMoveNumber(game,"...");
    }
    else if (s.contains(".")) {
       handleMoveNumber(game,".");
    }
    else if (s.startsWith("[")) {
       m_parseError = true;
       addError("PGN header tag seen inside game");
    }
    else if (isAnnotationSymbol(s.at(0))){
       game.addAnnotationSymbol(s);
    }
    else if (s=="D" || s=="N"){
       game.addAnnotationSymbol(s);
    }
    else {
       game.addMove(s);
    }
}

void PgnParser::handleMoveNumber(Game & game, const QString & s){
    QStringList m_sl = QStringList::split(s, m_str);
    if (!game.validateMoveNumber(m_sl[0])) {
       m_errorText = "parse error in move number: " + m_sl[0];
       m_parseError = true;
    }
    if (m_sl.count()>1) {
       game.addMove(m_sl[1]);
    }
}

void PgnParser::readComment(Game & game){
// assert: m_inComment == true
   m_ix = m_currentLine.find('}'); 
   if (m_ix >= 0) {
      m_comment.append(m_currentLine.left(m_ix));
      m_inComment = false;//state changes: outside comment
      game.addComment(m_comment);
      m_currentLine = m_currentLine.right(m_currentLine.length()-m_ix-1);
   }
   else {
      m_comment.append(m_currentLine);
      readLine();
   }
}

bool PgnParser::isEndOfInput() const {
   return m_iStream->atEnd();
}

void PgnParser::windForwardToNextGame() {
   m_gameOver = false;
   while (!m_gameOver && !isEndOfInput()){
      readLine();
      if (isResult(m_currentLine)) {
         m_gameOver = true;
      }
   }
}

bool PgnParser::isResult(const QString & s) const {
   return ( s == "1-0" || s == "0-1" || s == "1/2-1/2" || s == "*" );
}

bool PgnParser::isAnnotationSymbol(const QChar & c) const {
// note: -- can be used for letting a side not move; is that a move
// or an annotation symbol ?
   return ( c=='!' || c=='?' || c=='+' || c=='-' || c=='=' );
}


QStringList PgnParser::parserMessages() const {
   return *m_parserMessages;
}

void PgnParser::addWarning(const QString & msg) {
   QString s = QString("%1 : Warning: ").arg(m_lineNumber);
   s.append(msg);
   m_parserMessages->append(s);
}

void PgnParser::addError(const QString & msg) {
   QString s = QString("%1 : Error: ").arg(m_lineNumber);
   s.append(msg);
   m_parserMessages->append(s);
}