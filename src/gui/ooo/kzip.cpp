/***************************************************************************
 *   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                 *
 ***************************************************************************/
//
// C++ Implementation: KZip to read zip file from ODT document on QBuffer
// NOTE only read not write!
// Description:
// idea from qt QZipReader & http://code.mythtv.org/ code
// to build append LIBS += -lz 
// Author: Peter Hohl <pehohlva@gmail.com>,    24.10.2013
// http://www.freeroad.ch/
// Copyright: See COPYING file that comes with this distribution

#include "kzip.h"
#include <QDateTime>
#include <QtEndian>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QList>
#include <QSharedData>
#include <zlib.h>

#include <QtCore>
#include <QDomDocument>
#include <QTextDocument>
#include <QTextObject>
#include <QApplication>
#include <QColor>
#include <QCryptographicHash>
#include <QIODevice>
#include <QByteArray>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

//! Local header size (excluding signature, excluding variable length fields)
#define UNZIP_LOCAL_HEADER_SIZE 26
//! Central Directory file entry size (excluding signature, excluding variable length fields)
#define UNZIP_CD_ENTRY_SIZE_NS 42
//! Data descriptor size (excluding signature)
#define UNZIP_DD_SIZE 12
//! End Of Central Directory size (including signature, excluding variable length fields)
#define UNZIP_EOCD_SIZE 22
//! Local header entry encryption header size
#define UNZIP_LOCAL_ENC_HEADER_SIZE 12

// Some offsets inside a CD record (excluding signature)
#define UNZIP_CD_OFF_VERSION_MADE 0
#define UNZIP_CD_OFF_VERSION 2
#define UNZIP_CD_OFF_GPFLAG 4
#define UNZIP_CD_OFF_CMETHOD 6
#define UNZIP_CD_OFF_MODT 8
#define UNZIP_CD_OFF_MODD 10
#define UNZIP_CD_OFF_CRC32 12
#define UNZIP_CD_OFF_CSIZE 16
#define UNZIP_CD_OFF_USIZE 20
#define UNZIP_CD_OFF_NAMELEN 24
#define UNZIP_CD_OFF_XLEN 26
#define UNZIP_CD_OFF_COMMLEN 28
#define UNZIP_CD_OFF_LHOFFSET 38

// Some offsets inside a local header record (excluding signature)
#define UNZIP_LH_OFF_VERSION 0
#define UNZIP_LH_OFF_GPFLAG 2
#define UNZIP_LH_OFF_CMETHOD 4
#define UNZIP_LH_OFF_MODT 6
#define UNZIP_LH_OFF_MODD 8
#define UNZIP_LH_OFF_CRC32 10
#define UNZIP_LH_OFF_CSIZE 14
#define UNZIP_LH_OFF_USIZE 18
#define UNZIP_LH_OFF_NAMELEN 22
#define UNZIP_LH_OFF_XLEN 24

// Some offsets inside a data descriptor record (excluding signature)
#define UNZIP_DD_OFF_CRC32 0
#define UNZIP_DD_OFF_CSIZE 4
#define UNZIP_DD_OFF_USIZE 8

// Some offsets inside a EOCD record
#define UNZIP_EOCD_OFF_ENTRIES 6
#define UNZIP_EOCD_OFF_CDOFF 12
#define UNZIP_EOCD_OFF_COMMLEN 16

/*!
 Max version handled by this API.
 0x14 = 2.0 --> full compatibility only up to this version;
 later versions use unsupported features
 */
#define UNZIP_VERSION 0x14

static inline uint readUInt(const uchar *data) {
    return (data[0]) + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);
}

static inline ushort readUShort(const uchar *data) {
    return (data[0]) + (data[1] << 8);
}

static int inflate(Bytef *dest, ulong *destLen, const Bytef *source, ulong sourceLen) {
    z_stream stream;
    int err;

    stream.next_in = (Bytef*) source;
    stream.avail_in = (uInt) sourceLen;
    if ((uLong) stream.avail_in != sourceLen)
        return Z_BUF_ERROR;

    stream.next_out = dest;
    stream.avail_out = (uInt) * destLen;
    if ((uLong) stream.avail_out != *destLen)
        return Z_BUF_ERROR;

    stream.zalloc = (alloc_func) 0;
    stream.zfree = (free_func) 0;

    err = inflateInit2(&stream, -MAX_WBITS);
    if (err != Z_OK)
        return err;

    err = inflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        inflateEnd(&stream);
        if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
            return Z_DATA_ERROR;
        return err;
    }
    *destLen = stream.total_out;

    err = inflateEnd(&stream);
    return err;
}

#define ZIP_VERSION 20

KZipStream::KZipStream(const QString odtfile) {

    d = new QBuffer();
    ErrorCode ec = ReadFailed;
    uBuffer = (unsigned char*) buffer1;
    //// crcTable = (quint32*) get_crc_table(); /// zlib
    is_open = false;
    if (d->open(QIODevice::ReadWrite)) {
        is_open = LoadFile(odtfile);
        ec = seekToCentralDirectory();
        ////qDebug() << "### seekToCentralDirectory return ...  " << ec;
        if (ec != KZipStream::OkFunky) {
            is_open = false;
        }
        if (cdEntryCount == 0) {
            is_open = false;
        }
    }

    if (!is_open) {
        clear(); // remove buffer 
    } else {
        ec = openArchive();
        if (ec == KZipStream::OkFunky) {
            int i;
            QStringList pieces;
            for (i = 0; i < fileHeaders.size(); ++i) {
                FileHeader metaheader = fileHeaders.at(i);
                const QString zfile = QString::fromLocal8Bit(metaheader.file_name);
                if (zfile.indexOf("/")) {
                    QStringList subdir = zfile.split("/");
                    pieces << subdir;
                }
                QByteArray chunk = fileByte(zfile);
                zip_files << zfile;
                if (chunk.size() > 0) {
                    corefilelist.insert(zfile, chunk);
                }
            }
            zip_files << QString("##items##");
            zip_files << pieces;
            zip_files.removeDuplicates();
        }
    }
}

QByteArray KZipStream::fileByte(const QString &fileName) {

    int compressed_size = 0;
    int uncompressed_size = 0;
    int i;
    bool found = false;
    for (i = 0; i < fileHeaders.size(); ++i) {
        if (QString::fromLocal8Bit(fileHeaders.at(i).file_name) == fileName) {
            found = true;
            break;
        }

    }
    start(); /// seek 0;
    if (!found || !device()) {
        return QByteArray();
    }

    FileHeader metaheader = fileHeaders.at(i);
    compressed_size = readUInt(metaheader.h.compressed_size);
    uncompressed_size = readUInt(metaheader.h.uncompressed_size);
    if (uncompressed_size < 1) {
        return QByteArray();
    }
    int start = readUInt(metaheader.h.offset_local_header);
    device()->seek(start);
    LocalFileHeader lh;
    device()->read((char *) &lh, sizeof (LocalFileHeader));
    uint skip = readUShort(lh.file_name_length) + readUShort(lh.extra_field_length);
    device()->seek(device()->pos() + skip);
    int compression_method = readUShort(lh.compression_method);
    QByteArray compressed = device()->read(compressed_size);
    const QString zfile = QString::fromLocal8Bit(metaheader.file_name);

    if (compression_method == 0) {
        // no compression
        compressed.truncate(uncompressed_size);
        return compressed;
    } else if (compression_method == 8) {
        /// real unzip part file 
        compressed.truncate(compressed_size);
        QByteArray decompress_chunk;
        ulong len = qMax(uncompressed_size, 1);
        int res;
        do {
            decompress_chunk.resize(len);
            res = inflate((uchar*) decompress_chunk.data(), &len, (uchar*) compressed.constData(), compressed_size);
            if (res == Z_OK) {
                if ((int) len != decompress_chunk.size()) {
                    decompress_chunk.resize(len);
                }
                break;
            } else {
                decompress_chunk.clear();
                qWarning("KZip: Z_DATA_ERROR: Input data is corrupted");
            }

        } while (res == Z_BUF_ERROR);

        return decompress_chunk;
    }

    qWarning() << "KZip: Unknown compression method";
    return QByteArray();
}

QIODevice *KZipStream::device() {
    return d;
}

QByteArray KZipStream::stream() {
    return d->data();
}

KZipStream::ErrorCode KZipStream::openArchive() {
    Q_ASSERT(device());

    if (!canread()) {
        qDebug() << "Unable to open device for reading";
        return KZipStream::OpenFailed;
    }

    start(); /// seek 0;

    uchar tmp[4];
    device()->read((char *) tmp, 4);
    if (getULong(tmp, 0) != 0x04034b50) {
        qWarning() << "KZip: not a zip file!";
        return KZipStream::OpenFailed;
    }

    // find EndOfDirectory header
    int i = 0;
    int start_of_directory = -1;
    int num_dir_entries = 0;
    EndOfDirectory eod;
    while (start_of_directory == -1) {
        int pos = device()->size() - sizeof (EndOfDirectory) - i;
        if (pos < 0 || i > 65535) {
            qWarning() << "KZip: EndOfDirectory not found";
            return KZipStream::OpenFailed;
        }
        device()->seek(pos);
        device()->read((char *) &eod, sizeof (EndOfDirectory));
        if (readUInt(eod.signature) == 0x06054b50)
            break;
        ++i;
    }
    start_of_directory = readUInt(eod.dir_start_offset);
    num_dir_entries = readUShort(eod.num_dir_entries);
    if (cdEntryCount != num_dir_entries) {
        return KZipStream::OpenFailed;
    }

    int comment_length = readUShort(eod.comment_length);
    if (comment_length != i) {
        qWarning() << "KZip: failed to parse zip file.";
        return KZipStream::OpenFailed;
    }
    commentario = device()->read(qMin(comment_length, i));

    device()->seek(start_of_directory);
    for (i = 0; i < num_dir_entries; ++i) {
        FileHeader header;
        int read = device()->read((char *) &header.h, sizeof (GentralFileHeader));
        if (read < (int) sizeof (GentralFileHeader)) {
            qWarning() << "KZip: Failed to read complete header, index may be incomplete";
            break;
        }
        if (readUInt(header.h.signature) != 0x02014b50) {
            qWarning() << "KZip: invalid header signature, index may be incomplete";
            break;
        }

        int l = readUShort(header.h.file_name_length);
        header.file_name = device()->read(l);
        if (header.file_name.length() != l) {
            qWarning() << "KZip: Failed to read filename from zip index, index may be incomplete";
            break;
        }
        l = readUShort(header.h.extra_field_length);
        header.extra_field = device()->read(l);
        if (header.extra_field.length() != l) {
            qWarning() << "KZip: Failed to read extra field in zip file, skipping file, index may be incomplete";
            break;
        }
        l = readUShort(header.h.file_comment_length);
        header.file_comment = device()->read(l);
        if (header.file_comment.length() != l) {
            qWarning() << "KZip: Failed to read read file comment, index may be incomplete";
            break;
        }

        fileHeaders.append(header);
    }

    return KZipStream::OkFunky;
}

KZipStream::ErrorCode KZipStream::seekToCentralDirectory() {
    Q_ASSERT(device());

    qint64 length = device()->size();
    qint64 offset = length - UNZIP_EOCD_SIZE;
    if (length < UNZIP_EOCD_SIZE) {
        return KZipStream::InvalidArchive;
    }
    if (!device()->seek(offset)) {
        return KZipStream::SeekFailed;
    }
    if (device()->read(buffer1, UNZIP_EOCD_SIZE) != UNZIP_EOCD_SIZE) {
        return KZipStream::ReadFailed;
    }
    bool eocdFound = (buffer1[0] == 'P' && buffer1[1] == 'K' && buffer1[2] == 0x05 && buffer1[3] == 0x06);
    if (eocdFound) {
        // Zip file has no comment (the only variable length field in the EOCD record)
        eocdOffset = offset;
    } else {
        return KZipStream::HandleCommentHere;
        /* 
        qint64 read;
        char* p = 0;
        offset -= UNZIP_EOCD_SIZE;
        if (offset <= 0) {
            return KZipStream::InvalidArchive;
        }
        if (!device()->seek(offset)) {
            return KZipStream::SeekFailed;
        }
        int cursor =-1;
        while ((read = device()->read(buffer1, UNZIP_EOCD_SIZE)) >= 0) {
            cursor++;
            qDebug() << "### cursor:" << cursor << "|";
        }
         * */
    }

    if (!eocdFound) {
        return KZipStream::InvalidArchive;
    }
    // Parse EOCD to locate CD offset
    offset = getULong((const unsigned char*) buffer1, UNZIP_EOCD_OFF_CDOFF + 4);
    cdOffset = offset;
    cdEntryCount = getUShort((const unsigned char*) buffer1, UNZIP_EOCD_OFF_ENTRIES + 4);
    quint16 commentLength = getUShort((const unsigned char*) buffer1, UNZIP_EOCD_OFF_COMMLEN + 4);
    if (commentLength != 0) {
        return KZipStream::HandleCommentHere;
    }
    if (!device()->seek(cdOffset)) {
        return KZipStream::SeekFailed;
    }
    return KZipStream::OkFunky;
}

bool KZipStream::LoadFile(const QString file) {
    if (clear()) {
        QFile f(file);
        if (f.exists()) {
            if (f.open(QFile::ReadOnly)) {
                d->write(f.readAll());
                f.close();
                ///delete f;
                start();
                return true;
            }
        }
    }
    return false;
}

/*!
 \internal Reads an quint16 (2 bytes) from a byte array starting at given offset.
 */
quint16 KZipStream::getUShort(const unsigned char* data, quint32 offset) const {
    return (quint16) data[offset] | (((quint16) data[offset + 1]) << 8);
}

void KZipStream::start() {
    d->seek(0);
}

/*!
 \internal Reads an quint64 (8 bytes) from a byte array starting at given offset.
 */
quint64 KZipStream::getULLong(const unsigned char* data, quint32 offset) const {
    quint64 res = (quint64) data[offset];
    res |= (((quint64) data[offset + 1]) << 8);
    res |= (((quint64) data[offset + 2]) << 16);
    res |= (((quint64) data[offset + 3]) << 24);
    res |= (((quint64) data[offset + 1]) << 32);
    res |= (((quint64) data[offset + 2]) << 40);
    res |= (((quint64) data[offset + 3]) << 48);
    res |= (((quint64) data[offset + 3]) << 56);

    return res;
}

/*!
 \internal Reads an quint32 (4 bytes) from a byte array starting at given offset.
 */
quint32 KZipStream::getULong(const unsigned char* data, quint32 offset) const {
    quint32 res = (quint32) data[offset];
    res |= (((quint32) data[offset + 1]) << 8);
    res |= (((quint32) data[offset + 2]) << 16);
    res |= (((quint32) data[offset + 3]) << 24);

    return res;
}

bool KZipStream::clear() {
    d->write(QByteArray());
    start();
    return d->bytesAvailable() == 0 ? true : false;
}

/*!
    Desctructor
 */
KZipStream::~KZipStream() {
    clear();
    delete d;
}
