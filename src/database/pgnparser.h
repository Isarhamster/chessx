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

#include <qtextstream.h>
#include <qstringlist.h>
#include "game.h"

/**
Usage:

// input from a file:
  PgnParser* p = new PgnParser("~/mygames.pgn");

// or input from a string:
//  QString* s = "some string from clipboard";
//  PgnParser* p = new PgnParser(s);

  Game* g;
  while (!p->isEndOfInput()) {
     g = new Game();
     p->parseGame(g);
  }
  QStringList l = p->parserMessages();
  QStringList::Iterator it = l.begin();
  while ( it != l.end() ) {
        cout << *it << "\n";
        ++it;
  }
  delete p;

*/

class PgnParser
{

public:
  /** Use file(filename) as input for parsing */
  PgnParser(const QString & filename);
  /** Use string pgnText as input for parsing */
  PgnParser(const QString * pgnText);
  ~PgnParser();
  /** Parse next game from the input stream */
  void parseGame(Game & g);
  /** Test if input text stream is exhausted */
  bool isEndOfInput() const;
  /** Get warnings and errors from the parser */
  QStringList parserMessages() const;

private:
  PgnParser();
  PgnParser(const PgnParser&);
  PgnParser& operator=(const PgnParser&);
  QTextStream* m_iStream;
  int m_lineNumber;
  QString m_currentLine;
  inline void readLine() {
     m_currentLine = m_iStream->readLine();
     m_lineNumber++;
  }
  QStringList* m_parserMessages;
  int m_commentStartLine;
  QString m_comment;
  QString m_str;
  int m_ix;
  int m_pos;
  bool m_gameOver;
  bool m_inComment;
  bool m_parseError;
  bool m_tagsBegun;
  bool m_movesBegun;
  QString m_errorText;
  void parseInitialComment(Game & g);
  void parseTags(Game & g);
  void parseMoves(Game & g);
  void parseLine(Game & game);
  void readComment(Game & game);
  void handleMoveNumber(Game & game, const QString & s);
  void handleString(Game & game, const QString & s);
  void windForwardToNextGame();
  bool isResult(const QString & s) const;
  bool isAnnotationSymbol(const QChar & c) const;
  void addWarning(const QString & msg);
  void addError(const QString & msg);
};
