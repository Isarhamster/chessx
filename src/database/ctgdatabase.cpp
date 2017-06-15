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

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

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
    QString filename_cto = fi.path() + fi.completeBaseName() + ".cto";
    QString filename_ctb = fi.path() + fi.completeBaseName() + ".ctb";

    QFile* file = new QFile(filename);
    QFile* file_cto = new QFile(filename_cto);
    QFile* file_ctb = new QFile(filename_cto);

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
// CTG implementation
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
            cto_file->read((char*)page_index, 4);
            *page_index = ntohl((uint32_t)*page_index);
            if (*page_index >= 0)
            {
                return true;
            }
            qDebug() << "found invalid entry with key "<< key << endl;
        }
        else
        {
            qDebug() << "key out of range "<< key << endl;
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
    // Print as bits.
    qDebug() << "Signature length: " << sig->buf_len << endl;

    // Print as chars.
    for (int i=0; i<sig->buf_len; ++i) {
        if (i % 8 == 0) qDebug() << endl;
        qDebug() << sig->buf[i] << " ";
    }
    qDebug() << endl;
}
// ---------------------------------------------------------
// Book parser - public interface
// ---------------------------------------------------------

bool CtgDatabase::findMove(quint64 /*key*/, MoveData& /*m*/)
{
    // todo
    return false;
}

int CtgDatabase::getMoveMapForBoard(const Board &board, QMap<Move, MoveData>& moves)
{
    return 0;
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
