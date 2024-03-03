#include "testadapter.h"
#include <QCoreApplication>
#include <QTemporaryDir>

#include "ecothread.h"
#include "output.h"
#include "memorydatabase.h"
#include "settings.h"

TestAdapter::TestAdapter(QObject *parent) : QObject(parent)
{

}

bool TestAdapter::dispatchTests()
{
    result = 1;

    QCommandLineParser parser;
    parser.setApplicationDescription("Test interface");
    parser.addHelpOption();
    parser.addVersionOption();

    // Refactoring options
    QCommandLineOption pruneOption(QStringList() << "pn" << "prune-null-moves",
            QCoreApplication::translate("TestAdapter", "Prune null moves."));
    parser.addOption(pruneOption);
    QCommandLineOption rvOption(QStringList() << "rv" << "remove-variations",
            QCoreApplication::translate("TestAdapter", "Remove all variations."));
    parser.addOption(rvOption);
    QCommandLineOption rcOption(QStringList() << "rc" << "remove-comments",
            QCoreApplication::translate("TestAdapter", "Remove all comments."));
    parser.addOption(rcOption);
    QCommandLineOption rtOption(QStringList() << "rt" << "remove-time",
            QCoreApplication::translate("TestAdapter", "Remove all time annotations."));
    parser.addOption(rtOption);
    QCommandLineOption ecOption(QStringList() << "ec" << "eco-classify-always",
            QCoreApplication::translate("TestAdapter", "Always classify games."));
    parser.addOption(ecOption);
    QCommandLineOption ekOption(QStringList() << "ek" << "eco-keep-codes",
            QCoreApplication::translate("TestAdapter", "Keep existing eco codes. Must be combined with ec."));
    parser.addOption(ekOption);
    QCommandLineOption enOption(QStringList() << "en" << "eco-no-classify",
            QCoreApplication::translate("TestAdapter", "Never classify games."));
    parser.addOption(enOption);

    // Do not start the GUI after processing the files
    QCommandLineOption exitOption(QStringList() << "x" << "exit",
            QCoreApplication::translate("TestAdapter", "Exit after processing."));
    parser.addOption(exitOption);

    // I/O File Names
    QCommandLineOption inputOption(QStringList() << "i" << "input",
                QCoreApplication::translate("TestAdapter", "Input Database."),
                QCoreApplication::translate("TestAdapter", "Input"));
    parser.addOption(inputOption);
    QCommandLineOption outputOption(QStringList() << "o" << "output",
                QCoreApplication::translate("TestAdapter", "Output Database."),
                QCoreApplication::translate("TestAdapter", "Output"));
    parser.addOption(outputOption);

    // Process the actual command line arguments given by the user
    parser.process(*qApp);

    const QStringList args = parser.positionalArguments();
        // source is args.at(0), destination is args.at(1)

    QString inputFile = parser.value(inputOption);
    QString outputFile = parser.value(outputOption);

    bool ec = parser.isSet("ec");
    bool ek = parser.isSet("ek");
    bool en = parser.isSet("en");
    if (en)
    {
        AppSettings->setValue("/General/automaticECO", false);
    }
    else if (ec)
    {
        AppSettings->setValue("/General/automaticECO", true);
        AppSettings->setValue("/General/preserveECO", ek);
    }

    if (!inputFile.isEmpty())
    {
        if (outputFile.isEmpty())
        {
            QFileInfo fi(inputFile);
            outputFile = fi.baseName() + ".processed.pgn";
        }
        convertPgn(inputFile, outputFile, parser);
        result = 0;
    }

    bool x = parser.isSet(exitOption);
    return x;
}

void TestAdapter::convertPgn(const QString& filename, const QString& outfile, QCommandLineParser& parser)
{
    MemoryDatabase db;
    EcoThread eco;
    eco.loadAsync();

    bool pn = parser.isSet("pn");
    bool rv = parser.isSet("rv");
    bool rc = parser.isSet("rc");
    bool rt = parser.isSet("rt");

    if (db.open(filename, true) && ((Database*)(&db))->parseFile())
    {
        for (GameId i=0, sz = static_cast<GameId>(db.count()); i < sz; ++i)
        {
            GameX g;
            if(db.loadGame(i, g))
            {
                if (rv) g.removeVariationsDb();
                if (rc) g.removeCommentsDb();
                if (rt) g.removeTimeCommentsDb();
                if (pn) g.removeNullLinesDb();
                db.replace(i, g);
            }
        }

        Output output(Output::Pgn);
        output.outputLatin1(outfile, db);
    }
}

int TestAdapter::getResult() const
{
    return result;
}

