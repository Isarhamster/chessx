#ifndef TESTADAPTER_H
#define TESTADAPTER_H

#include <QObject>
#include <QString>
#include <QCommandLineParser>

class TestAdapter : public QObject
{
    Q_OBJECT
public:
    explicit TestAdapter(QObject *parent = nullptr);

    bool dispatchTests();
    void convertPgn(const QString& filename, const QString& outfile, QCommandLineParser& parser);

    int getResult() const;

private:
    int result;

};

#endif // TESTADAPTER_H
