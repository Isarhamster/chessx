/****************************************************************************
*   Copyright (C) 2017 by Jens Nissen jens-chessx@gmx.net                   *
*
* Internal structure definitions of ctg
* Some code taken from Daydreamer.
* The huffman codes and ctg move decoding are
* based on Stephan Vermeire's ctg code for Brutus.
****************************************************************************/


#include "ctgdatabase.h"
#include "ctg.h"
#include "square.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

#define create_square(file,rank) SquareFromRankAndFile(rank,file)
#define mirror_rank(square)         (Square((square) ^ 0x38))
#define mirror_file(square)         (Square((square) ^ 0x07))
#define flip_piece(p)               (flip_piece[p])

// ---------------------------------------------------------
// construction
// ---------------------------------------------------------

CtgDatabase::CtgDatabase() :
    Database(),
    ctg_file(0),
    m_count(0)
{
}

CtgDatabase::~CtgDatabase()
{
    close();
}

// ---------------------------------------------------------
// Database Interface implementation
// ---------------------------------------------------------

bool CtgDatabase::open(const QString &filename, bool)
{
    if(ctg_file)
    {
        return false;
    }
    m_break = false;
    m_filename = filename;
    if(openFile(filename, true))
    {
        m_utf8 = false;

        // Read out upper and lower page limits.
        ctb_file->read((char*)&page_bounds, 12);
        page_bounds.low = ntohl((uint32_t)page_bounds.low);
        page_bounds.high = ntohl((uint32_t)page_bounds.high);
        // Actually, closing ctb here would be ok
        return true;
    }
    return false;
}

bool CtgDatabase::parseFile()
{
    return false;
}

QString CtgDatabase::filename() const
{
    return m_filename;
}

quint64 CtgDatabase::count() const
{
    return 0;
}

quint64 CtgDatabase::positionCount() const
{
    return m_count;
}

void CtgDatabase::loadGameMoves(GameId, Game &)
{

}

int CtgDatabase::findPosition(GameId, const Board &)
{
    return NO_MOVE;
}

bool CtgDatabase::openFile(const QString &filename, bool readOnly)
{
    //opens both required files

    QFileInfo fi(filename);
    QString filename_cto = fi.path() + QDir::separator() + fi.completeBaseName() + ".cto";
    QString filename_ctb = fi.path() + QDir::separator() + fi.completeBaseName() + ".ctb";

    QFile* file = new QFile(filename);
    QFile* file_cto = new QFile(filename_cto);
    QFile* file_ctb = new QFile(filename_ctb);

    if(!readOnly || (file->exists() && file_cto->exists() && file_ctb->exists()))
    {
        QFile::OpenMode mode = readOnly ? QIODevice::ReadOnly : QIODevice::WriteOnly;
        if (file->open(mode) && file_cto->open(mode) && file_ctb->open(mode))
        {
            ctg_file = file;
            cto_file = file_cto;
            ctb_file = file_ctb;
            return true;
        }
    }

    delete file;
    delete file_cto;
    delete file_ctb;
    return false;
}

void CtgDatabase::close()
{
    //close the files, and delete objects
    if(ctg_file)
    {
        ctg_file->close();
        cto_file->close();
        ctb_file->close();
    }
    delete ctg_file;
    delete cto_file;
    delete ctb_file;

    ctg_file = 0;
    cto_file = 0;
    ctb_file = 0;
}

// ---------------------------------------------------------
// CTG implementation - CTG only code
// ---------------------------------------------------------

void CtgDatabase::append_bits_reverse(ctg_signature_t* sig,
        uint8_t bits,
        int bit_position,
        int num_bits) const
{
    uint8_t * sig_byte = &sig->buf[bit_position/8];
    int offset = bit_position % 8;
    for (int i=offset; i<num_bits+offset; ++i, bits>>=1) {
        if (bits & 1) *sig_byte |= 1 << (7-(i%8));
        if (i%8 == 7) *(++sig_byte) = 0;
    }
}

// ---------------------------------------------------------

int32_t CtgDatabase::ctg_signature_to_hash(ctg_signature_t* sig) const
{
    static const uint32_t hash_bits[64] = {
        0x3100d2bf, 0x3118e3de, 0x34ab1372, 0x2807a847,
        0x1633f566, 0x2143b359, 0x26d56488, 0x3b9e6f59,
        0x37755656, 0x3089ca7b, 0x18e92d85, 0x0cd0e9d8,
        0x1a9e3b54, 0x3eaa902f, 0x0d9bfaae, 0x2f32b45b,
        0x31ed6102, 0x3d3c8398, 0x146660e3, 0x0f8d4b76,
        0x02c77a5f, 0x146c8799, 0x1c47f51f, 0x249f8f36,
        0x24772043, 0x1fbc1e4d, 0x1e86b3fa, 0x37df36a6,
        0x16ed30e4, 0x02c3148e, 0x216e5929, 0x0636b34e,
        0x317f9f56, 0x15f09d70, 0x131026fb, 0x38c784b1,
        0x29ac3305, 0x2b485dc5, 0x3c049ddc, 0x35a9fbcd,
        0x31d5373b, 0x2b246799, 0x0a2923d3, 0x08a96e9d,
        0x30031a9f, 0x08f525b5, 0x33611c06, 0x2409db98,
        0x0ca4feb2, 0x1000b71e, 0x30566e32, 0x39447d31,
        0x194e3752, 0x08233a95, 0x0f38fe36, 0x29c7cd57,
        0x0f7b3a39, 0x328e8a16, 0x1e7d1388, 0x0fba78f5,
        0x274c7e7c, 0x1e8be65c, 0x2fa0b0bb, 0x1eb6c371
    };

    int32_t hash = 0;
    int16_t tmp = 0;
    for (int i=0; i<sig->buf_len; ++i) {
        int8_t byte = sig->buf[i];
        tmp += ((0x0f - (byte & 0x0f)) << 2) + 1;
        hash += hash_bits[tmp & 0x3f];
        tmp += ((0xf0 - (byte & 0xf0)) >> 2) + 1;
        hash += hash_bits[tmp & 0x3f];
    }
    return hash;
}

// ---------------------------------------------------------

bool CtgDatabase::ctg_get_page_index(int hash, int* page_index) const
{
    uint32_t key = 0;
    for (int mask = 1; key <= (uint32_t)page_bounds.high;
            mask = (mask << 1) + 1)
    {
        key = (hash & mask) + mask;
        if (key >= (uint32_t)page_bounds.low)
        {
            cto_file->seek(16 + key*4);
            int n;
            cto_file->read((char*)&n, 4);
            *page_index = ntohl((uint32_t)n);
            if (*page_index >= 0)
            {
                return true;
            }
            qDebug() << "found invalid entry with key "<< key << endl;
        }
    }
    qDebug() << "didn't find entry for hash " << hash << endl;
    return false;
}

// ---------------------------------------------------------

bool CtgDatabase::ctg_lookup_entry(int page_index,
        ctg_signature_t* sig,
        ctg_entry_t* entry) const
{
    // Pages are a uniform 4096 bytes.
    uint8_t buf[4096];
    ctg_file->seek(4096*(page_index + 1));
    if (!ctg_file->read((char*)buf, 4096)) return false;
    int num_positions = (buf[0]<<8) + buf[1];

    qDebug() << "found positions " << num_positions << endl;

    // Just scan through the list until we find a matching signature.
    int pos = 4;
    for (int i=0; i<num_positions; ++i) {
        int entry_size = buf[pos] % 32;
        bool equal = true;
        if (sig->buf_len != entry_size) equal = false;
        for (int j=0; j<sig->buf_len && equal; ++j) {
            if (buf[pos+j] != sig->buf[j]) equal = false;
        }
        if (!equal) {
            pos += entry_size + buf[pos+entry_size] + 33;
            continue;
        }
        // Found it, fill in the entry and return. Annoyingly, most of the
        // fields are 24 bits long.
        pos += entry_size;
        entry_size = buf[pos];
        for (int j=1; j<entry_size; ++j) entry->moves[j-1] = buf[pos+j];
        entry->num_moves = (entry_size - 1)/2;
        pos += entry_size;
        entry->total = read_24(buf, pos);
        pos += 3;
        entry->losses = read_24(buf, pos);
        pos += 3;
        entry->wins = read_24(buf, pos);
        pos += 3;
        entry->draws = read_24(buf, pos);
        pos += 3;
        entry->unknown1 = read_32(buf, pos);
        pos += 4;
        entry->avg_rating_games = read_24(buf, pos);
        pos += 3;
        entry->avg_rating_score = read_32(buf, pos);
        pos += 4;
        entry->perf_rating_games = read_24(buf, pos);
        pos += 3;
        entry->perf_rating_score = read_32(buf, pos);
        pos += 4;
        entry->recommendation = buf[pos];
        pos += 1;
        entry->unknown2 = buf[pos];
        pos += 1;
        entry->comment = buf[pos];
        return true;
    }
    return false;
}

// ---------------------------------------------------------

/*
 * Print out the huffman signature |sig|, as both bits and bytes.
 */
void CtgDatabase::dump_signature(ctg_signature_t* sig) const
{
    qDebug() << "Signature length: " << sig->buf_len << endl;

    // Print as chars.
    for (int i=0; i<sig->buf_len; ++i) {
        if (i % 8 == 0) qDebug() << endl;
        qDebug() << sig->buf[i] << " ";
    }
    qDebug() << endl;
}

// ---------------------------------------------------------
// CTG implementation - Code which interfaces with Board
// ---------------------------------------------------------

/**
 * Given source and destination squares for a move, produce the corresponding
 * native format move.
 */
Move CtgDatabase::squares_to_move(const Board& position, Square from, Square to) const
{
    Board b(position);
    Move m = b.prepareMove(from, to); // TODO: Check enumeration values

    // Check the promotion piece and convert
    if (m.isPromotion())
    {
        m.setPromoted(Queen); // CTG does not support underpromotion
    }

    return m;
}

// ---------------------------------------------------------

Move CtgDatabase::byte_to_move(const Board& pos, uint8_t byte) const
{
    const char* piece_code =
        "PNxQPQPxQBKxPBRNxxBKPBxxPxQBxBxxxRBQPxBPQQNxxPBQNQBxNxNQQQBQBxxx"
        "xQQxKQxxxxPQNQxxRxRxBPxxxxxxPxxPxQPQxxBKxRBxxxRQxxBxQxxxxBRRPRQR"
        "QRPxxNRRxxNPKxQQxxQxQxPKRRQPxQxBQxQPxRxxxRxQxRQxQPBxxRxQxBxPQQKx"
        "xBBBRRQPPQBPBRxPxPNNxxxQRQNPxxPKNRxRxQPQRNxPPQQRQQxNRBxNQQQQxQQx";
    const int piece_index[256]= {
        5, 2, 9, 2, 2, 1, 4, 9, 2, 2, 1, 9, 1, 1, 2, 1,
        9, 9, 1, 1, 8, 1, 9, 9, 7, 9, 2, 1, 9, 2, 9, 9,
        9, 2, 2, 2, 8, 9, 1, 3, 1, 1, 2, 9, 9, 6, 1, 1,
        2, 1, 2, 9, 1, 9, 1, 1, 2, 1, 1, 2, 1, 9, 9, 9,
        9, 2, 1, 9, 1, 1, 9, 9, 9, 9, 8, 1, 2, 2, 9, 9,
        1, 9, 1, 9, 2, 3, 9, 9, 9, 9, 9, 9, 7, 9, 9, 5,
        9, 1, 2, 2, 9, 9, 1, 1, 9, 2, 1, 0, 9, 9, 1, 2,
        9, 9, 2, 9, 1, 9, 9, 9, 9, 2, 1, 2, 3, 2, 1, 1,
        1, 1, 6, 9, 9, 1, 1, 1, 9, 9, 1, 1, 1, 9, 2, 1,
        9, 9, 2, 9, 1, 9, 2, 1, 1, 1, 1, 3, 9, 1, 9, 2,
        2, 9, 1, 8, 9, 2, 9, 9, 9, 2, 9, 2, 9, 2, 2, 9,
        2, 6, 1, 9, 9, 2, 9, 1, 9, 2, 9, 5, 2, 2, 1, 9,
        9, 1, 2, 1, 2, 2, 2, 7, 7, 2, 2, 6, 2, 1, 9, 4,
        9, 2, 2, 2, 9, 9, 9, 1, 2, 1, 1, 1, 9, 9, 5, 1,
        2, 1, 9, 2, 9, 1, 4, 1, 1, 1, 9, 4, 1, 1, 2, 1,
        2, 1, 9, 2, 2, 2, 0, 1, 2, 2, 2, 2, 9, 1, 2, 9
    };
    const int forward[256]= {
        1,-1, 9, 0, 1, 1, 1, 9, 0, 6,-1, 9, 1, 3, 0,-1,
        9, 9, 7, 1, 1, 5, 9, 9, 1, 9, 6, 1, 9, 7, 9, 9,
        9, 0, 2, 6, 1, 9, 7, 1, 5, 0,-2, 9, 9, 1, 1, 0,
       -2, 0, 5, 9, 2, 9, 1, 4, 4, 0, 6, 5, 5, 9, 9, 9,
        9, 5, 7, 9,-1, 3, 9, 9, 9, 9, 2, 5, 2, 1, 9, 9,
        6, 9, 0, 9, 1, 1, 9, 9, 9, 9, 9, 9, 1, 9, 9, 2,
        9, 6, 2, 7, 9, 9, 3, 1, 9, 7, 4, 0, 9, 9, 0, 7,
        9, 9, 7, 9, 0, 9, 9, 9, 9, 6, 3, 6, 1, 1, 3, 0,
        6, 1, 1, 9, 9, 2, 0, 5, 9, 9,-2, 1,-1, 9, 2, 0,
        9, 9, 1, 9, 3, 9, 1, 0, 0, 4, 6, 2, 9, 2, 9, 4,
        3, 9, 2, 1, 9, 5, 9, 9, 9, 0, 9, 6, 9, 0, 3, 9,
        4, 2, 6, 9, 9, 0, 9, 5, 9, 3, 9, 1, 0, 2, 0, 9,
        9, 2, 2, 2, 0, 4, 5, 1, 2, 7, 3, 1, 5, 0, 9, 1,
        9, 1, 1, 1, 9, 9, 9, 1, 0, 2,-2, 2, 9, 9, 1, 1,
       -1, 7, 9, 3, 9, 0, 2, 4, 2,-1, 9, 1, 1, 7, 1, 0,
        0, 1, 9, 2, 2, 1, 0, 1, 0, 6, 0, 2, 9, 7, 3, 9
    };
    const int left[256] = {
       -1, 2, 9,-2, 0, 0, 1, 9,-4,-6, 0, 9, 1,-3,-3, 2,
        9, 9,-7, 0,-1,-5, 9, 9, 0, 9, 0, 1, 9,-7, 9, 9,
        9,-7, 2,-6, 1, 9, 7, 1,-5,-6,-1, 9, 9,-1,-1,-1,
        1,-3,-5, 9,-1, 9,-2, 0, 4,-5,-6, 5, 5, 9, 9, 9,
        9,-5, 7, 9,-1,-3, 9, 9, 9, 9, 0, 5,-1, 0, 9, 9,
        0, 9,-6, 9, 1, 0, 9, 9, 9, 9, 9, 9,-1, 9, 9, 0,
        9,-6, 0, 7, 9, 9, 3,-1, 9, 0,-4, 0, 9, 9,-5,-7,
        9, 9, 7, 9,-2, 9, 9, 9, 9, 6, 0, 0,-1, 0, 3,-1,
        6, 0, 1, 9, 9, 1,-7, 0, 9, 9,-1,-1, 1, 9, 2,-7,
        9, 9,-1, 9, 0, 9,-1, 1,-3, 0, 0, 0, 9, 0, 9, 4,
        0, 9,-2, 0, 9, 0, 9, 9, 9,-2, 9, 6, 9,-4,-3, 9,
        0, 0, 6, 9, 9,-5, 9, 0, 9,-3, 9, 0,-5, 0,-1, 9,
        9,-2,-2, 2,-1, 0, 0, 1, 0, 0, 3, 0, 5,-2, 9, 0,
        9, 1,-2, 2, 9, 9, 9, 1,-6, 2, 1, 0, 9, 9, 1, 1,
       -2, 0, 9, 0, 9,-4, 0,-4, 0,-2, 9,-1, 0,-7, 1,-4,
       -7,-1, 9, 1, 0,-1, 0, 2,-1, 0,-3,-2, 9, 0, 3, 9
    };

    // Find the piece. Note: the board may be mirrored/flipped.
    bool flip_board = pos.blackToMove();
    Color white = pos.toMove();
    bool mirror_board = (File(pos.kingSquare(white)) < FILE_E) &&
        (pos.castlingRights() == 0); /* pieces.white.0 is the white king - determine if in left half of board */
    int file_from = -1, file_to = -1, rank_from = -1, rank_to = -1;

    // Handle castling.
    if (byte == 107) {
        file_from = 4;
        file_to = 6;
        rank_from = rank_to = flip_board ? 7 : 0;
        return squares_to_move(pos,
                create_square(file_from, rank_from),
                create_square(file_to, rank_to));
    }
    if (byte == 246) {
        file_from = 4;
        file_to = 2;
        rank_from = rank_to = flip_board ? 7 : 0;
        return squares_to_move(pos,
                create_square(file_from, rank_from),
                create_square(file_to, rank_to));
    }

    // Look up piece type. Note: positions are always white to move.
    Piece pc = Empty;
    char glyph = piece_code[byte];
    switch (glyph) {
        case 'P': pc = WhitePawn; break;
        case 'N': pc = WhiteKnight; break;
        case 'B': pc = WhiteBishop; break;
        case 'R': pc = WhiteRook; break;
        case 'Q': pc = WhiteQueen; break;
        case 'K': pc = WhiteKing; break;
        default: break;
    }

    // Find the piece.
    int nth_piece = piece_index[byte], piece_count = 0;
    bool found = false;
    for (int file=0; file<8 && !found; ++file) {
        for (int rank=0; rank<8 && !found; ++rank) {
            Square sq = create_square(file, rank);
            if (flip_board) sq = mirror_rank(sq);
            if (mirror_board) sq = mirror_file(sq);
            Piece piece = pos.pieceAt(sq);
            if (piece == pc) ++piece_count;
            if (piece_count == nth_piece) {
                file_from = file;
                rank_from = rank;
                found = true;
            }
        }
    }

    // Normalize rank and file values.
    file_to = file_from - left[byte];
    file_to = (file_to + 8) % 8;
    rank_to = rank_from + forward[byte];
    rank_to = (rank_to + 8) % 8;
    if (flip_board) {
        rank_from = 7-rank_from;
        rank_to = 7-rank_to;
    }
    if (mirror_board) {
        file_from = 7-file_from;
        file_to = 7-file_to;
    }
    return squares_to_move(pos,
            create_square(file_from, rank_from),
            create_square(file_to, rank_to));
}

// ---------------------------------------------------------

void CtgDatabase::position_to_ctg_signature(const Board& pos, ctg_signature_t* sig) const
{
    // Note: initial byte is reserved for length and flags info
    memset(sig, 0, sizeof(ctg_signature_t));
    int bit_position = 8;
    uint8_t bits = 0, num_bits = 0;

    // The board is flipped if it's black's turn, and mirrored if the king is
    // on the queenside with no castling rights for either side.
    bool flip_board = pos.blackToMove();
    Color white = pos.toMove();
    bool mirror_board = (File(pos.kingSquare(white)) < FILE_E) &&
        (pos.castlingRights() == 0);


    // For each board square, append the huffman bit sequence for its contents.
    for (int file=0; file<8; ++file) {
        for (int rank=0; rank<8; ++rank) {
            Square sq = create_square(file, rank);
            if (flip_board) sq = mirror_rank(sq);
            if (mirror_board) sq = mirror_file(sq);
            Piece piece = pos.pieceAt(sq);
            switch (piece) {
                case Empty: bits = 0x0; num_bits = 1; break;
                case WhitePawn: bits = 0x3; num_bits = 3; break;
                case BlackPawn: bits = 0x7; num_bits = 3; break;
                case WhiteKnight: bits = 0x9; num_bits = 5; break;
                case BlackKnight: bits = 0x19; num_bits = 5; break;
                case WhiteBishop: bits = 0x5; num_bits = 5; break;
                case BlackBishop: bits = 0x15; num_bits = 5; break;
                case WhiteRook: bits = 0xD; num_bits = 5; break;
                case BlackRook: bits = 0x1D; num_bits = 5; break;
                case WhiteQueen: bits = 0x11; num_bits = 6; break;
                case BlackQueen: bits = 0x31; num_bits = 6; break;
                case WhiteKing: bits = 0x1; num_bits = 6; break;
                case BlackKing: bits = 0x21; num_bits = 6; break;
                default: break;
            }
            append_bits_reverse(sig, bits, bit_position, num_bits);
            bit_position += num_bits;
        }
    }

    // Encode castling and en passant rights. These must sit flush at the end
    // of the final byte, so we also have to figure out how much to pad.
    int ep = -1;
    int flag_bit_length = 0;
    if (pos.enPassantSquare() != InvalidSquare)
    {
        ep = File(pos.enPassantSquare());
        if (mirror_board) ep = 7 - ep;
        flag_bit_length = 3;
    }
    int castle = 0;
    if (pos.canCastleShort(white)) castle += 4;
    if (pos.canCastleLong(white)) castle += 8;
    if (pos.canCastleShort(oppositeColor(white))) castle += 1;
    if (pos.canCastleLong(oppositeColor(white))) castle += 2;
    if (castle) flag_bit_length += 4;
    uint8_t flag_bits = castle;
    if (ep != -1) {
        flag_bits <<= 3;
        for (int i=0; i<3; ++i, ep>>=1) if (ep&1) flag_bits |= (1<<(2-i));
    }

    //printf("\nflag bits: %d\n", flag_bits);
    //printf("bit_position: %d\n", bit_position%8);
    //printf("flag_bit_length: %d\n", flag_bit_length);

    // Insert padding so that flags fit at the end of the last byte.
    int pad_bits = 0;
    if (8-(bit_position % 8) < flag_bit_length) {
        //printf("padding byte\n");
        pad_bits = 8 - (bit_position % 8);
        append_bits_reverse(sig, 0, bit_position, pad_bits);
        bit_position += pad_bits;
    }

    pad_bits = 8 - (bit_position % 8) - flag_bit_length;
    if (pad_bits < 0) pad_bits += 8;
    //printf("padding %d bits\n", pad_bits);
    append_bits_reverse(sig, 0, bit_position, pad_bits);
    bit_position += pad_bits;
    append_bits_reverse(sig, flag_bits, bit_position, flag_bit_length);
    bit_position += flag_bit_length;
    sig->buf_len = (bit_position + 7) / 8;

    // Write header byte
    sig->buf[0] = ((uint8_t)(sig->buf_len));
    if (ep != -1) sig->buf[0] |= 1<<5;
    if (castle) sig->buf[0] |= 1<<6;
}

// ---------------------------------------------------------

int64_t CtgDatabase::move_weight(const Board& pos,
        Move move,
        uint8_t annotation,
        bool* recommended,
        uint64_t* count) const
{
    // Here, the game is needed

    ((Board&)pos).doMove(move);
    ctg_entry_t entry;
    bool success = ctg_get_entry(pos, &entry);
    ((Board&)pos).undoMove(move);
    if (!success) return 0;

    *recommended = false;
    int64_t half_points = 2*entry.wins + entry.draws;
    int64_t games = entry.wins + entry.draws + entry.losses;
    int64_t weight = (games < 1) ? 0 : (half_points * 10000) / games;
    if (entry.recommendation == 64) weight = 0;
    if (entry.recommendation == 128) *recommended = true;

    *count = entry.wins;

    // Adjust weights based on move annotations. Note that moves can be both
    // marked as recommended and annotated with a '?'. Since moves like this
    // are not marked green in GUI tools, the recommendation is turned off in
    // order to give results consistent with expectations.
    switch (annotation) {
        case 0x01: weight *=  8; break;                         //  !
        case 0x02: weight  =  0; *recommended = false; break;   //  ?
        case 0x03: weight *= 32; break;                         // !!
        case 0x04: weight  =  0; *recommended = false; break;   // ??
        case 0x05: weight /=  2; *recommended = false; break;   // !?
        case 0x06: weight /=  8; *recommended = false; break;   // ?!
        case 0x08: weight = INT32_MAX; break;                   // Only move
        case 0x16: break;                                       // Zugzwang
        default: break;
    }
    return weight;
}

// ---------------------------------------------------------

bool CtgDatabase::ctg_pick_move(const Board& pos, ctg_entry_t* entry, Move* move) const
{
    Move moves[100];
    int64_t weights[100] = { 0 };
    bool recommended[100] = { 0 };
    int64_t total_weight = 0;
    uint64_t count[100] = { 0 };
    bool have_recommendations = false;
    for (int i=0; i<2*entry->num_moves; i += 2) {
        uint8_t byte = entry->moves[i];
        Move m = byte_to_move(pos, byte);
        moves[i/2] = m;
        weights[i/2] = move_weight(pos, m, entry->moves[i+1], &recommended[i/2], &count[i/2]);
        if (recommended[i/2]) have_recommendations = true;
        if (!m.isLegal()) break;
    }

    // Do a prefix sum on the weights to facilitate a random choice. If there are recommended
    // moves, ensure that we don't pick a move that wasn't recommended.
    for (int i=0; i<entry->num_moves; ++i) {
        if (have_recommendations && !recommended[i]) weights[i] = 0;
        total_weight += weights[i];
        weights[i] = total_weight;
    }
    if (total_weight == 0) {
        *move = Move();
        return false;
    }
    uint64_t r = qrand();
    r *= (0xFFFFFFFFFFFFFFFFull/RAND_MAX);
    int64_t choice = ((int64_t)r) % total_weight;
    int64_t i;
    for (i=0; choice >= weights[i]; ++i) {}

    *move = moves[i];
    return true;
}

// ---------------------------------------------------------

Move CtgDatabase::get_best_book_move(const Board &pos) const
{
    Move move;
    ctg_entry_t entry;
    if (!ctg_get_entry(pos, &entry)) return Move();
    if (!ctg_pick_move(pos, &entry, &move)) return Move();
    return move;
}

// ---------------------------------------------------------

bool CtgDatabase::ctg_get_entry(const Board& pos, ctg_entry_t* entry) const
{
    ctg_signature_t sig;
    position_to_ctg_signature(pos, &sig);
    int page_index, hash = ctg_signature_to_hash(&sig);
    if (!ctg_get_page_index(hash, &page_index)) return false;
    if (!ctg_lookup_entry(page_index, &sig, entry)) return false;
    return true;
}

// ---------------------------------------------------------
// Book parser - public interface
// ---------------------------------------------------------

bool CtgDatabase::findMove(quint64 /*key*/, MoveData& /*m*/)
{
    // todo
    return false;
}

int CtgDatabase::getMoveMapForBoard(const Board &pos, QMap<Move, MoveData>& moveList)
{
    ctg_entry_t entry;
    if (!ctg_get_entry(pos, &entry)) return 0;
    // Position is here, output the moves associated with it
    QVector<Move> moves(100);
    int64_t weights[100] = { 0 };
    bool recommended[100] = { 0 };
    uint64_t count[100] = { 0 };
    int64_t total_weight = 0;
    bool have_recommendations = false;
    for (int i=0; i<2*entry.num_moves; i += 2)
    {
        uint8_t byte = entry.moves[i];
        Move m = byte_to_move(pos, byte);
        moves[i/2] = m;
        weights[i/2] = move_weight(pos, m, entry.moves[i+1], &recommended[i/2], &count[i]);
        if (recommended[i/2]) have_recommendations = true;
        if (!m.isLegal()) break;
    }

    // Do a prefix sum on the weights to facilitate a random choice. If there are recommended
    // moves, ensure that we don't pick a move that wasn't recommended.
    for (int i=0; i<entry.num_moves; ++i)
    {
        if (have_recommendations && !recommended[i]) weights[i] = 0;
        total_weight += weights[i];
        weights[i] = total_weight;
    }
    if (total_weight == 0)
    {
        //return 0;
    }

    for (int i=0; i<entry.num_moves; ++i)
    {
        MoveData md;
        md.count = count[i];
        md.move = moves.at(i);
        md.san = pos.moveToSan(md.move);
        moveList.insert(md.move, md);
    }

    return moveList.count();
}

// ---------------------------------------------------------
// Book building - public interface
// ---------------------------------------------------------

bool CtgDatabase::openForWriting(const QString &filename, int /*maxPly*/, int /*minGame*/, bool /*uniform*/)
{
    if(ctg_file)
    {
        return false;
    }

    // todo

    return openFile(filename, false);
}

void CtgDatabase::book_make(Database& /*db*/, volatile bool& /*breakFlag*/)
{
    // todo
}
