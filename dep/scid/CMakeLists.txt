add_library(scid STATIC
  code/src/board_def.h
  code/src/bytebuf.h
  code/src/codec.h
  code/src/codec_native.h
  code/src/codec_scid4.cpp
  code/src/codec_scid4.h
  code/src/common.h
  code/src/containers.h
  code/src/date.h
  code/src/error.h
  code/src/fastgame.h
  code/src/filebuf.h
  code/src/game.cpp
  code/src/game.h
  code/src/hfilter.h
  code/src/index.h
  code/src/indexentry.h
  code/src/matsig.cpp
  code/src/matsig.h
  code/src/movetree.h
  code/src/misc.cpp
  code/src/misc.h
  code/src/namebase.h
  code/src/position.cpp
  code/src/position.h
  code/src/sortcache.cpp
  code/src/sortcache.h
  code/src/stored.cpp
  code/src/stored.h
  code/src/textbuf.cpp
  code/src/textbuf.h
  code/src/tree.h
)

target_include_directories(scid
  PUBLIC
  code/src
)

target_compile_features(scid
  PUBLIC
    cxx_std_17
)

