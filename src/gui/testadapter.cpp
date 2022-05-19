#include "testadapter.h"
#include <QCoreApplication>
#include <QTemporaryDir>

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

    if (!inputFile.isEmpty())
    {
        if (outputFile.isEmpty())
        {
            QFileInfo fi(inputFile);
            outputFile = fi.baseName() + ".processed.pgn";
        }
        convertPgn(inputFile, outputFile);
        result = 0;
    }

    bool x = parser.isSet(exitOption);
    return x;
}

void TestAdapter::convertPgn(const QString& filename, const QString& outfile)
{
    PgnDatabase db;
    if (db.open(filename, true) && db.parseFile())
    {
        Output output(Output::Pgn);
        output.output(outfile, db);
    }
}

int TestAdapter::getResult() const
{
    return result;
}

