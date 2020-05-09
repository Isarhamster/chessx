#include "sciddatabase.h"

#include "codec_scid4.h"
#include "date.h"
#include "tags.h"

namespace {

class ProgressImpl: public ProgressBase, public Progress::Impl
{
public:
    ProgressImpl(volatile bool *interrupt): ProgressBase(interrupt)
    {}

    // Progress::Impl implementation
    bool report(size_t done, size_t total, const char* msg) override
    {
        int percents = static_cast<int>(100 * done / total);
        emit progressValueChanged(percents);
        if (msg)
        {
            emit progressMessage(msg);
        }
        return !(*m_interrupt);
    }
};

} // anonymous namespace


static inline NagSet ConvertNags(byte* scid)
{
    NagSet nags;
    for (;*scid != 0; ++scid)
    {
        nags.addNag(static_cast<Nag>(*scid));
    }
    return nags;
}

static void ConvertLine(Game& src, GameX& dst, bool movesOnly = false)
{
    // convert main line moves
    int plyCnt = 0;
    while (!src.AtVarEnd())
    {
        auto varsCnt = src.GetNumVariations();

        if (movesOnly)
        {
            dst.dbAddSanMove(src.GetNextSAN());
            src.MoveForward();
        }
        else
        {
            dst.dbAddSanMove(src.GetNextSAN(), "", ConvertNags(src.GetNextNags()));
            src.MoveForward();
            dst.dbSetAnnotation(src.GetMoveComment());
        }
        plyCnt += 1;

        if (varsCnt > 0)
        {
            src.MoveBackup();
            dst.dbBackward();

            auto move = dst.currentMove();
            for (uint v = 0; v < varsCnt; ++v)
            {
                src.MoveIntoVariation(v);
                if (movesOnly)
                {
                    dst.dbAddSanVariation(move, src.GetNextSAN());
                }
                else
                {
                    dst.dbAddSanVariation(move, src.GetNextSAN(), "", ConvertNags(src.GetNextNags()));
                    dst.dbSetAnnotation(src.GetPreviousMoveComment(), GameX::Position::BeforeMove);
                    dst.dbSetAnnotation(src.GetMoveComment());
                }
                src.MoveForward();
                ConvertLine(src, dst, movesOnly);
                src.MoveExitVariation();
                dst.dbBackward();
            }
            dst.dbForward();
            src.MoveForward();
        }
    }
    dst.dbBackward(plyCnt);
}

static void ConvertGame(Game& src, GameX& dst, bool movesOnly = false)
{
    src.MoveToStart();
    if (src.HasNonStandardStart())
    {
        char fen[128];
        bool isChess360 = false; // .si4 does not support chess360
        auto pos = src.GetCurrentPos();
        pos->PrintFEN(fen, FEN_ALL_FIELDS);
        dst.dbSetStartingBoard(fen, isChess360);
    }

    // add initial comment
    if (!movesOnly)
    {
        dst.dbSetAnnotation(src.GetMoveComment());
    }

    // recursively convert main variation
    ConvertLine(src, dst, movesOnly);
}

class ScidStorage
{
public:
    static std::unique_ptr<ScidStorage> open(QString path, Progress &progress);

    bool readTags(IndexX& dst) const;
    bool readTags(IndexX& dst, gamenumT g) const;
    bool readGame(GameX& dst, gamenumT g, bool movesOnly = false) const;

    size_t gamesCount() const { return m_index->GetNumGames(); }

private:
    ScidStorage(std::unique_ptr<Index> index,
                std::unique_ptr<NameBase> names,
                std::unique_ptr<CodecSCID4> codec)
    : m_index(std::move(index))
    , m_names(std::move(names))
    , m_codec(std::move(codec))
    {}

    std::unique_ptr<Index> m_index;
    std::unique_ptr<NameBase> m_names;
    std::unique_ptr<CodecSCID4> m_codec;
};

std::unique_ptr<ScidStorage> ScidStorage::open(QString path, Progress &progress)
{
    auto index = std::make_unique<Index>();
    auto names = std::make_unique<NameBase>();
    
    auto dbname = path.chopped(4); // remove .si4 extension
    auto dbnameUtf8 = dbname.toUtf8().data();
    auto codec = std::make_unique<CodecSCID4>();
    auto err = codec->dyn_open(FMODE_ReadOnly, dbnameUtf8, progress, index.get(), names.get());
    if (err != OK && err != ERROR_NameDataLoss)
    {
        return nullptr;
    }
    
    std::unique_ptr<ScidStorage> storage(new ScidStorage(std::move(index), std::move(names), std::move(codec)));
    return storage;
}

bool ScidStorage::readTags(IndexX& dst) const
{
    gamenumT n = m_index->GetNumGames();
    for (gamenumT g = 0; g < n; ++g)
    {
        if (!readTags(dst, g))
            return false;
    }
    return true;
}

bool ScidStorage::readTags(IndexX& dst, gamenumT g) const
{
    // must fit date (????.??.??) or ECO code
    char strBuf[16];
    auto nb = m_names.get();
    auto ie = m_index->GetEntry(g);
    auto gameOffset = ie->GetOffset();
    auto gameLength = ie->GetLength();
    auto data = m_codec->getGameData(gameOffset, gameLength);
    if (!data)
        return false;

    // STR tags
    auto event = ie->GetEventName(nb);
    dst.setTag_nolock(TagNameEvent, event, g);
    auto site = ie->GetSiteName(nb);
    dst.setTag_nolock(TagNameSite, site, g);
    auto date = ie->GetDate();
    date_DecodeToString(date, strBuf);
    dst.setTag_nolock(TagNameDate, strBuf, g);
    auto round = ie->GetRoundName(nb);
    dst.setTag_nolock(TagNameRound, round, g);
    auto white = ie->GetWhiteName(nb);
    dst.setTag_nolock(TagNameWhite, white, g);
    auto black = ie->GetBlackName(nb);
    dst.setTag_nolock(TagNameBlack, black, g);
    auto result = ie->GetResult();
    dst.setTag_nolock(TagNameResult, RESULT_LONGSTR[result], g);

    // proceed with other available non-STR tags
    auto plyCnt = ie->GetNumHalfMoves();
    dst.setTag_nolock(TagNameLength, QString("%1").arg(plyCnt), g);
    auto whiteElo = ie->GetWhiteElo();
    if (whiteElo != 0)
    {
        dst.setTag_nolock(TagNameWhiteElo, QString("%1").arg(whiteElo), g);
    }
    auto blackElo = ie->GetBlackElo();
    if (blackElo != 0)
    {
        dst.setTag_nolock(TagNameBlackElo, QString("%1").arg(blackElo), g);
    }
    auto eventDate = ie->GetEventDate();
    if (eventDate != ZERO_DATE)
    {
        date_DecodeToString(eventDate, strBuf);
        dst.setTag_nolock(TagNameEventDate, strBuf, g);
    }
    auto eco = ie->GetEcoCode();
    if (eco != 0)
    {
//            eco_ToBasicString(eco, strBuf);
        eco_ToExtendedString(eco, strBuf);
        dst.setTag_nolock(TagNameECO, strBuf, g);
    }

    // read the rest from game data
    auto bbuf = ByteBuffer(data, gameLength);
    bbuf.decodeTags([&dst, g](const auto& tag, const auto& val) {
        QByteArray qtag(tag.data(), tag.size());
        QByteArray qval(val.data(), val.size());
        dst.setTag_nolock(qtag, qval, g);
    });

    return true;
}

bool ScidStorage::readGame(GameX& dst, gamenumT g, bool movesOnly) const
{
    auto ie = m_index->GetEntry(g);
    auto length = ie->GetLength();
    auto offset = ie->GetOffset();
    auto data = m_codec->getGameData(offset, length);
    if (!data)
        return false;
    auto bbuf = ByteBuffer(data, length);
    Game src;
    if (src.DecodeMovesOnly(bbuf) != OK)
        return false;
    ConvertGame(src, dst, movesOnly);
    return true;
}

ScidDatabase::ScidDatabase()
    : m_filename()
    , m_storage()
{
}

bool ScidDatabase::open(const QString& filename, bool /*utf8*/)
{
    auto progressImpl = new ProgressImpl(&m_break);
    Progress progress(progressImpl);

    connect(progressImpl, SIGNAL(progressValueChanged(int)), this, SIGNAL(progress(int)));
    auto storage = ScidStorage::open(filename, progress);
    if (!storage)
    {
        return false;
    }

    m_filename = filename;
    m_files.clear();
    auto dbname = m_filename.chopped(4);
    m_files << dbname + ".si4" << dbname + ".sn4" << dbname + ".sg4";

    m_storage = std::move(storage);
    return true;
}

bool ScidDatabase::parseFile()
{
    m_storage->readTags(m_index);
    return true;
}

QString ScidDatabase::filename() const
{
    return m_files[0];
}

qint64 ScidDatabase::diskSize() const
{
    qint64 size = 0;
    for (const auto& fname: m_files)
    {
        QFileInfo fi(fname);
        size += fi.size();
    }
    return size;
}

bool ScidDatabase::loadGame(GameId index, GameX& game)
{
    QMutexLocker m(&m_mutex);
    if (!m_storage->readGame(game, index))
        return false;
    loadGameHeaders(index, game);
    return true;
}

void ScidDatabase::loadGameMoves(GameId index, GameX& game)
{
    QMutexLocker m(&m_mutex);
    m_storage->readGame(game, index, true);
}

int ScidDatabase::findPosition(GameId index, const BoardX& position)
{
    GameX g;
    loadGameMoves(index, g);
    return g.findPosition(position);
}

quint64 ScidDatabase::count() const
{
    return m_storage->gamesCount();
}
