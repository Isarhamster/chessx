find_package(ZLIB REQUIRED)

add_library(quazip STATIC
  JlCompress.cpp
  JlCompress.h
  crypt.h
  ioapi.h
  qioapi.cpp
  quaadler32.cpp
  quaadler32.h
  quachecksum32.h
  quacrc32.cpp
  quacrc32.h
  quagzipfile.cpp
  quagzipfile.h
  quaziodevice.cpp
  quaziodevice.h
  quazip.cpp
  quazip.h
  quazip_global.h
  quazipdir.cpp
  quazipdir.h
  quazipfile.cpp
  quazipfile.h
  quazipfileinfo.h
  quazipnewinfo.cpp
  quazipnewinfo.h
  unzip.cpp
  unzip.h
  zip.cpp
  zip.h
)

target_compile_definitions(quazip
  PUBLIC
    QUAZIP_STATIC
)

target_include_directories(quazip
  PUBLIC
    .
)

target_link_libraries(quazip
  PRIVATE
    qt_config
  PUBLIC
    ZLIB::ZLIB
    Qt::Core
)

