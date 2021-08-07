#ifndef TESTADAPTER_H
#define TESTADAPTER_H

#include <QObject>
#include <QString>

class TestAdapter : public QObject
{
    Q_OBJECT
public:
    explicit TestAdapter(QObject *parent = nullptr);

    bool dispatchTests();
    void convertPgn(const QString& filename, const QString& outfile);

    int getResult() const;

private:
    int result;

};

#endif // TESTADAPTER_H
