#include <QCoreApplication>
#include <QTemporaryDir>

#include "output.h"
#include "memorydatabase.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    AppSettings = new Settings("pgn-convert.settings");

    if (argc >= 2)
    {
        PgnDatabase db;
        QString filename = argv[1];
        QFileInfo fi(filename);
        QString outfile = fi.baseName() + ".processed.pgn";
        if (db.open(filename, true) && db.parseFile())
        {
            qDebug() << "Processing" << db.count() << "games in file" << filename;
            Output output(Output::Pgn);
            output.output(outfile, db);
            qDebug() << "Done";
        }
        return 0;
    }
    return 1;
}
