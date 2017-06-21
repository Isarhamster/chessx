/****************************************************************************
*   Copyright (C) 2017 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef CTGDATABASE_H
#define CTGDATABASE_H

#include "database.h"
#include "movedata.h"
#include <stdint.h>
#include "ctg.h"

struct _results_t;

class CtgDatabase : public Database
{
    Q_OBJECT
public:
    CtgDatabase();
    ~CtgDatabase();

    /**
     * Initialize the ctg-format opening book with the given filename. The
     * filename gives the .ctg file, and there must be corresponding .cto and .ctb
     * files in the same directory. Note that all these methods depend on global
     * state in the form of ctg and cto file pointers.
     */
    virtual bool open(const QString& filename, bool);
    virtual bool openForWriting(const QString& filename, int maxPly, int minGame, bool uniform);
    /** Parse the database */
    virtual bool parseFile();
    /** File-based database name */
    virtual QString filename() const;

    /** Get the number of stored games*/
    virtual quint64 count() const;
    /** Get the number of stored positions*/
    quint64 positionCount() const;

    /** Loads only moves into a game from the given position */
    virtual void loadGameMoves(GameId index, Game& game);
    virtual int findPosition(GameId index, const Board& position);
    quint64 getHashFromBoard(Board b) const;

    /** Open a book data File */
    bool openFile(const QString& filename, bool readOnly=false);
    /** Closes the database */
    void close();
    /** Find Information to a given key */
    bool findMove(quint64 key, MoveData &move);
    /** Get a map of MoveData from a given board position */
    int getMoveMapForBoard(const Board &board, QMap<Move, MoveData>& moves);
    /** Start a search for a new key */
    void reset();
    void book_make(Database& db, volatile bool& breakFlag);

signals:

public slots:

protected: // Methods which are CTG only
    /**
     * Push the given bits on to the end of @p sig. This is a helper function that
     * makes the huffman encoding of positions a little cleaner.
     */
    void append_bits_reverse(ctg_signature_t *sig, uint8_t bits, int bit_position, int num_bits) const;

    /** Convert a position's huffman code to a 4 byte hash. */
    int32_t ctg_signature_to_hash(ctg_signature_t* sig) const;

    /** Find the page index associated with a given position @p hash */
    bool ctg_get_page_index(int hash, int* page_index) const;

    /** Find and copy out a ctg entry, given its page index and signature. */
    bool ctg_lookup_entry(int page_index,
            ctg_signature_t* sig,
            ctg_entry_t* entry) const;

    void dump_signature(ctg_signature_t* sig) const;

protected: // Methods which interface with ChessX
    /** Generate a Move from a pair of squares */
    Move squares_to_move(const Board& position, Square from, Square to) const;
    /**
     * Convert a ctg-format move to native format. The ctg move format seems
     * really bizarre; maybe there's some simpler formulation. The ctg move
     * indicates the piece type, the index of the piece to be moved (counting
     * from A1 to H8 by ranks), and the delta x and delta y of the move.
     * We just look these values up in big tables.
     */
    Move byte_to_move(const Board& pos, uint8_t byte) const;

    /** Compute the ctg-huffman encoding of the given position */
    void position_to_ctg_signature(const Board& pos, ctg_signature_t* sig) const;

    /**
     * Assign a weight to the given move, which indicates its relative
     * probability of being selected.
     * To do this, do the move on the board and look up the
     * resulting position, which determines the actual weight of the
     * move, corrected by some annotations.
     */
    int64_t move_weight(const Board& pos, Move move, MoveData& md) const;

    /** Get the ctg entry associated with the given position. */
    bool ctg_get_entry(const Board& pos, ctg_entry_t* entry) const;

private:
    QString m_filename;
    QIODevice* ctg_file;
    QIODevice* cto_file;
    QIODevice* ctb_file;
    quint64 m_count;

    page_bounds_t page_bounds;
    Piece flip_piece[16];

};

#endif // CTGDATABASE_H
