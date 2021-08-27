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

#ifndef KZIP_H
#define	KZIP_H

#include <QString>
#include <QMap>
#include <QByteArray>
#include <QStringList>

class QBuffer;
class QIODevice;

// zLib authors suggest using larger buffers (128K or 256K) for (de)compression (especially for inflate())
// we use a 256K buffer here - if you want to use this code on a pre-iceage mainframe please change it ;)
#define UNZIP_READ_BUFFER (256*1024)

struct LocalFileHeader {
    uchar signature[4]; //  0x04034b50
    uchar version_needed[2];
    uchar general_purpose_bits[2];
    uchar compression_method[2];
    uchar last_mod_file[4];
    uchar crc_32[4];
    uchar compressed_size[4];
    uchar uncompressed_size[4];
    uchar file_name_length[2];
    uchar extra_field_length[2];
};

struct DataDescriptor {
    uchar crc_32[4];
    uchar compressed_size[4];
    uchar uncompressed_size[4];
};

struct GentralFileHeader {
    uchar signature[4]; // 0x02014b50
    uchar version_made[2];
    uchar version_needed[2];
    uchar general_purpose_bits[2];
    uchar compression_method[2];
    uchar last_mod_file[4];
    uchar crc_32[4];
    uchar compressed_size[4];
    uchar uncompressed_size[4];
    uchar file_name_length[2];
    uchar extra_field_length[2];
    uchar file_comment_length[2];
    uchar disk_start[2];
    uchar internal_file_attributes[2];
    uchar external_file_attributes[4];
    uchar offset_local_header[4];
    LocalFileHeader toLocalHeader() const;
};

struct EndOfDirectory {
    uchar signature[4]; // 0x06054b50
    uchar this_disk[2];
    uchar start_of_directory_disk[2];
    uchar num_dir_entries_this_disk[2];
    uchar num_dir_entries[2];
    uchar directory_size[4];
    uchar dir_start_offset[4];
    uchar comment_length[2];
};

struct FileHeader {
    GentralFileHeader h;  /// h.compressed_size
    QByteArray file_name;
    QByteArray extra_field;
    QByteArray file_comment;
};

class KZipStream {
public:
    KZipStream(const QString odtfile);
    ~KZipStream();
    QByteArray fileByte(const QString &fileName);

    enum ErrorCode {
        OkFunky = 1000,
        ZlibInit,
        ZlibError,
        OpenFailed,
        PartiallyCorrupted,
        Corrupted,
        WrongPassword,
        NoOpenArchive,
        FileNotFound,
        ReadFailed,
        WriteFailed,
        SeekFailed,
        HandleCommentHere,
        CreateDirFailed,
        InvalidDevice,
        InvalidArchive,
        HeaderConsistencyError,
        Skip, SkipAll // internal use only
    };

    QIODevice *device();

    QByteArray stream();

    bool canread() {
        return is_open;
    }
    QStringList filelist() const  {
        return zip_files;
    }
    QMap<QString,QByteArray> listData() {
        return corefilelist;
    }
protected:
    char buffer1[UNZIP_READ_BUFFER];
    char buffer2[UNZIP_READ_BUFFER];
    QList<FileHeader> fileHeaders;
    QStringList zip_files;
    QByteArray commentario;
    uint start_of_directory;

    unsigned char* uBuffer;
    //// const quint32* crcTable;
    quint32 cdOffset;
    // End of Central Directory (EOCD) offset
    quint32 eocdOffset;
    // Number of entries in the Central Directory (as to the EOCD record)
    quint16 cdEntryCount;

private:
    QMap<QString,QByteArray> corefilelist; /// filename && chunk QByteArray inside
    ErrorCode seekToCentralDirectory();
    ErrorCode openArchive();
    quint32 getULong(const unsigned char* data, quint32 offset) const;
    inline quint64 getULLong(const unsigned char* data, quint32 offset) const;
    inline quint16 getUShort(const unsigned char* data, quint32 offset) const;

    void start();
    bool clear(); /// remove buffer 
    bool LoadFile(const QString file);
    QBuffer *d;
    bool is_open;
};






#endif	/* KZIP_H */

