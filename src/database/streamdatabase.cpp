/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "streamdatabase.h"
#include "tags.h"
#include "index.h"

using namespace chessx;

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

bool StreamDatabase::parseFile()
{
    // Does not do anything
    return true;
}

bool StreamDatabase::loadNextGame(GameX& game)
{
    //indexing game positions in the file, game contents are ignored
    int oldFp = -3;

    while(!m_file->atEnd())
    {
        IndexBaseType fp = skipJunk();
        if(fp == oldFp)
        {
            skipLine();
            fp = skipJunk();
        }
        oldFp = fp;
        if(fp != -1)
        {
            if(!m_currentLine.isEmpty())
            {
                int index = m_index.add();
                m_count = 1+index;
                parseTagsIntoIndex(); // This will parse the tags into memory
                game.clear();
                game.setResult(ResultUnknown);
                loadGameHeaders(index, game);
                QString fen = m_index.tagValue(TagNameFEN, index);
                QString variant = m_index.tagValue(TagNameVariant, index).toLower();
                bool chess960 = (variant.startsWith("fischer", Qt::CaseInsensitive) || variant.endsWith("960"));
                if(fen != "?")
                {
                    game.dbSetStartingBoard(fen, chess960);
                }
                bool ok = parseMoves(&game);
                m_index.setValidFlag(index, ok);
                QString valLength = QString::number((game.plyCount() + 1) / 2);
                game.setTag(TagNameLength, valLength);
                setMissingTagsToIndex(game, index);
                return true;
            }
        }
    }
    return false;
}
