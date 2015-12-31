#include <QFile>

#include "quagzipfile.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

class QuaGzipFilePrivate
{
    friend class QuaGzipFile;
    QString fileName;
    gzFile gzd;
    inline QuaGzipFilePrivate(): gzd(NULL) {}
    inline QuaGzipFilePrivate(const QString &fileName):
        fileName(fileName), gzd(NULL) {}
    template<typename FileId> bool open(FileId id,
                                        QIODevice::OpenMode mode, QString &error);
    gzFile open(int fd, const char *modeString);
    gzFile open(const QString &name, const char *modeString);
};

gzFile QuaGzipFilePrivate::open(const QString &name, const char *modeString)
{
    return gzopen(QFile::encodeName(name).constData(), modeString);
}

gzFile QuaGzipFilePrivate::open(int fd, const char *modeString)
{
    return gzdopen(fd, modeString);
}

template<typename FileId>
bool QuaGzipFilePrivate::open(FileId id, QIODevice::OpenMode mode,
                              QString &error)
{
    char modeString[2];
    modeString[0] = modeString[1] = '\0';
    if((mode & QIODevice::ReadOnly) != 0
            && (mode & QIODevice::WriteOnly) != 0)
    {
        error = "Opening zip for both reading and writing is not supported";
        return false;
    }
    else if((mode & QIODevice::ReadOnly) != 0)
    {
        modeString[0] = 'r';
    }
    else if((mode & QIODevice::WriteOnly) != 0)
    {
        modeString[0] = 'w';
    }
    else
    {
        error = "Opening a file requires a mode";
        return false;
    }
    gzd = open(id, modeString);
    if(gzd == NULL)
    {
        error = "Could not open file";
        return false;
    }
    return true;
}

QuaGzipFile::QuaGzipFile():
    d(new QuaGzipFilePrivate())
{
}

QuaGzipFile::QuaGzipFile(QObject *parent):
    QIODevice(parent),
    d(new QuaGzipFilePrivate())
{
}

QuaGzipFile::QuaGzipFile(const QString &fileName, QObject *parent):
    QIODevice(parent),
    d(new QuaGzipFilePrivate(fileName))
{
}

QuaGzipFile::~QuaGzipFile()
{
    if(isOpen())
    {
        close();
    }
    delete d;
}

void QuaGzipFile::setFileName(const QString& fileName)
{
    d->fileName = fileName;
}

QString QuaGzipFile::getFileName() const
{
    return d->fileName;
}

bool QuaGzipFile::isSequential() const
{
    return true;
}

bool QuaGzipFile::open(QIODevice::OpenMode mode)
{
    QString error;
    if(!d->open(d->fileName, mode, error))
    {
        setErrorString(error);
        return false;
    }
    return QIODevice::open(mode);
}

bool QuaGzipFile::open(int fd, QIODevice::OpenMode mode)
{
    QString error;
    if(!d->open(fd, mode, error))
    {
        setErrorString(error);
        return false;
    }
    return QIODevice::open(mode);
}

bool QuaGzipFile::flush()
{
    return gzflush(d->gzd, Z_SYNC_FLUSH) == Z_OK;
}

void QuaGzipFile::close()
{
    QIODevice::close();
    gzclose(d->gzd);
}

qint64 QuaGzipFile::readData(char *data, qint64 maxSize)
{
    return gzread(d->gzd, (voidp)data, (unsigned)maxSize);
}

qint64 QuaGzipFile::writeData(const char *data, qint64 maxSize)
{
    if(maxSize == 0)
    {
        return 0;
    }
    int written = gzwrite(d->gzd, (voidp)data, (unsigned)maxSize);
    if(written == 0)
    {
        return -1;
    }
    else
    {
        return written;
    }
}
