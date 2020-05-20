# Copyright (C) 2014 Fulvio Benini
#
# This file is part of Scid (Shane's Chess Information Database).
#
# Scid is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation.
#
# Scid is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Scid.  If not, see <http://www.gnu.org/licenses/>.

namespace eval ERROR {
  proc getErrorMsg {} {
    if {! [info exists ::ERROR::msg($::errorCode)] } {
      return "Unknown error: $::errorCode\n\n$::errorInfo"
    }
    regexp {(.*?)while executing.*} $::errorInfo -> extra
    return "$::ERROR::msg($::errorCode)\n\n$extra"
  }

  proc MessageBox { {extra ""} {title "ERROR"} } {
    tk_messageBox -icon warning -type ok -parent . \
      -title "$title" -message "$extra\n[getErrorMsg]"
  }

  set UserCancel         2
  set BadArg             3
  set FileOpen         101
  set FileWrite        102
  set FileRead         103
  set FileSeek         104
  set BadMagic         105
  set FileNotOpen      106
  set FileInUse        107
  set FileMode         108
  set FileVersion      109
  set OldScidVersion   110
  set FileReadOnly     111
  set CompactRemove    121
  set MallocFailed     151
  set CorruptData      152
  set Corrupt          152
  set Full             201
  set NameNotFound     202
  set NotFound         202
  set NameExists       203
  set Exists           203
  set NameBaseEmpty    204
  set Empty            204
  set NoMatchFound     205
  set NameDataLoss     206
  set NameTooLong      207
  set NameLimit        208
  set OffsetLimit      209
  set GameLengthLimit  210
  set NumGamesLimit    211
  set InvalidFEN       301
  set InvalidMove      302
  set PieceCount       303
  set Game             400
  set EndOfMoveList    401
  set StartOfMoveList  402
  set NoVariation      403
  set EmptyVariation   404
  set VariationLimit   405
  set Decode           406
  set GameFull         407
  set PGNTag           501
  set BufferFull       601
  set BufferRead       602
  set CodecUnsupFeat   701

}


#TODO: improve and translate the messages
after idle {
  set ::ERROR::msg($::ERROR::UserCancel) \
    "Operation canceled."
  set ::ERROR::msg($::ERROR::BadArg) \
    "Wrong parameters."
  set ::ERROR::msg($::ERROR::FileOpen) \
    "Error: can not open file."
  set ::ERROR::msg($::ERROR::FileWrite) \
    "Error writing file."
  set ::ERROR::msg($::ERROR::FileRead) \
    "Error: can not read file."
  set ::ERROR::msg($::ERROR::FileSeek) \
    "Error: can not seek in file."
  set ::ERROR::msg($::ERROR::BadMagic) \
    "Error: bad magic (corrupted file?)."
  set ::ERROR::msg($::ERROR::FileNotOpen) \
    "Error: the file is not open."
  set ::ERROR::msg($::ERROR::FileInUse) \
    "Error: the file is already in use."
  set ::ERROR::msg($::ERROR::FileMode) \
    "Error: file mode."
  set ::ERROR::msg($::ERROR::FileVersion) \
    "Database version newer than Scid; please upgrade Scid."
  set ::ERROR::msg($::ERROR::OldScidVersion) \
    "Old format Scid file, now out of date."
  set ::ERROR::msg($::ERROR::FileReadOnly) \
    $::tr(ErrReadOnly)

  set ::ERROR::msg(CompactCreate) \
    "A temporary database from a previous unsuccessfully compact operation already exists.\nPlease remove the files with suffix __COMPACT__ and retry.\n"
  set ::ERROR::msg($::ERROR::CompactRemove) \
    "A compacted database has been successfully created with suffix __COMPACT__.\nHowever Scid could not remove the original database (due to insufficient privileges or because a file is opened in another program).\nPlease rename it manually.\n"
  set ::ERROR::msg($::ERROR::CorruptData) \
    "Error while processing data: corrupted.\n"
  set ::ERROR::msg($::ERROR::Full) \
    "Error: insufficient space"
  set ::ERROR::msg($::ERROR::NameDataLoss) \
    "Some names (player, event, site or round) are missing and have been replaced by \"?\"\nCompact the database to make the changes permanent."
  set ::ERROR::msg($::ERROR::NameTooLong) \
    "The entered values are too long"
	set ::ERROR::msg($::ERROR::NameLimit) \
    "The maximum number of different names allowed by this database type has been reached"
	set ::ERROR::msg($::ERROR::OffsetLimit) \
    "The maximum space for games allowed by this database type has been reached"
	set ::ERROR::msg($::ERROR::GameLengthLimit) \
    "The maximum length for a game allowed by this database type has been reached"
	set ::ERROR::msg($::ERROR::NumGamesLimit) \
    "The maximum number of games allowed by this database type has been reached"

  set ::ERROR::msg($::ERROR::CodecUnsupFeat) \
    "The requested function is not supported by this type of database."
}
