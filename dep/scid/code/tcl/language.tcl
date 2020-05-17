### lang.tcl: Support for multiple-language menus, buttons, etc.
### Part of Scid, which is Copyright 2001-2003 Shane Hudson.

array set langEncoding {}
set languages {}

if {[catch {encoding names}]} {
  set hasEncoding 0
} else {
  set hasEncoding 1
}
################################################################################
#  Translation of pieces
#  Note to adopt also tkscid.cpp to allow for a new langauge as well
#  as add transPieces to langPieces[] in game.cpp
################################################################################
array set transPieces {}

set transPieces(F)   { P P K R Q D R T B F N C }
set untransPieces(F) { P P R K D Q T R F B C N }
set transPieces(S)   { P P K R Q D R T B A N C }
set untransPieces(S) { P P R K D Q T R A B C N }
set transPieces(D)   { P B K K Q D R T B L N S }
set untransPieces(D) { B P K K D Q T R L B S N }
set transPieces(I)   { P P K R Q D R T B A N C }
set untransPieces(I) { P P R K D Q T R A B C N }
set transPieces(N)   { P p K K Q D R T B L N P }
set untransPieces(N) { p P K K D Q T R L B P N }
set transPieces(C)   { P P K K Q D R V B S N J }
set untransPieces(C) { P P K K D Q V R S B J N }
## TODO Put in the right letters for greek
set transPieces(G)   { P B K K Q D R T B L N S }
set untransPieces(G) { B P K K D Q T R L B S N }
set transPieces(H)   { P G K K Q V R B B F N H }
set untransPieces(H) { G P K K V Q B R F B H N }
set transPieces(O)   { P B K K Q D R T B L N S }
set untransPieces(O) { B P K K D Q T R L B S N }
set transPieces(W)   { P B K K Q D R T B L N S }
set untransPieces(W) { B P K K D Q T R L B S N }
set transPieces(K)   { P P K R Q D R T B A N C }
set untransPieces(K) { P P R K D Q T R A B C N }
set transPieces(U)   { P S K K Q D R T B L N R }
set untransPieces(U) { S P K K D Q T R L B R N }

################################################################################
proc trans { msg } {
  if { $::language == "E" || ! $::translatePieces || $msg == "\[end\]"} {
    return $msg
  }
  if { [ catch { set t [string map $::transPieces($::language) $msg ]} ] } {
    return $msg
  }
  return $t
}
################################################################################
proc untrans { msg } {
  if { $::language == "E"  || ! $::translatePieces || $msg == "\[end\]"} {
    return $msg
  }
  if { [ catch { set t [string map $::untransPieces($::language) $msg ]} ] } {
    return $msg
  }
  return $t
}
################################################################################
#
################################################################################
proc addLanguage {letter name underline encodingSystem filename} {
  lappend ::languages $letter
  set ::langName($letter) $name
  set ::langUnderline($letter) $underline
  set ::langEncoding($letter) $encodingSystem
  set ::langSourceFile($letter) $filename
}
################################################################################
# menuText:
#    Assigns the menu name and help message for a menu entry and language.
################################################################################
proc menuText {lang tag label underline {helpMsg ""}} {
  set ::menuLabel($lang,$tag) $label
  set ::menuUnder($lang,$tag) $underline
  if {$helpMsg != ""} {
    set ::helpMessage($lang,$tag) $helpMsg
  }
}
################################################################################
# helpMsg:
#    Assigns the help message for a particular language for a button.
# ################################################################################
proc helpMsg {lang button message} {
  set ::helpMessage($lang,$button) $message
}

array set tr {}
array set translations {}
################################################################################
# translate:
#    Assigns a translation for future reference.
################################################################################
proc translate {lang tag label} {
  regsub {\\n} $label "\n" label
  set ::translations($lang,$tag) $label
  set ::tr($tag) $label
  foreach extra {":" "..."} {
    set newtag "${tag}${extra}"
    set newlabel "${label}${extra}"
    set ::translations($lang,$newtag) $newlabel
    set ::tr($newtag) $newlabel
  }
}
################################################################################
# translateECO:
#    Given a pair list of ECO opening name phrase translations,
#    assigns the translations for future reference.
################################################################################
proc translateECO {lang pairList} {
  foreach {from to} $pairList {
    sc_eco translate $lang $from $to
  }
}
################################################################################
# tr:
#    Given a tag and language, returns the stored text for that tag.
################################################################################
proc tr {tag {lang ""}} {
  global menuLabel tr
  if {$lang == ""} {set lang $::language}
  if {$lang == "X"} {return $tag}
  # First, look for a menu label
  if {[info exists menuLabel($lang,$tag)]} {
    return $menuLabel($lang,$tag)
  }
  if {[info exists menuLabel(E,$tag)]} {
    return $menuLabel(E,$tag)
  }
  # Now look for a regular button/message translation
  if {[info exists tr($tag)]} {
    return $tr($tag)
  }
  # Finally, just give up and return the original tag
  return $tag
}
################################################################################
#
################################################################################
proc setLanguage {} {
  global menuLabel menuUnder helpTitle helpText tips
  set lang $::language
  
  if { $::translatePieces } {
    switch $lang {
      C {sc_info language cz}
      D {sc_info language de}
      F {sc_info language fr}
      G {sc_info language gr}
      H {sc_info language hu}
      I {sc_info language it}
      K {sc_info language ca}
      N {sc_info language ne}
      O {sc_info language no}
      S {sc_info language es}
      U {sc_info language fi}
      W {sc_info language sw}
      default {sc_info language en}
    }
  } else {
    sc_info language en
  }
  
  if {[catch {
    if {[info exists ::langSourceFile($lang)]} {
      source -encoding $::langEncoding($lang) [file nativename [file join "$::scidTclDir" "lang/$::langSourceFile($lang)"]]
      unset ::langSourceFile($lang)
    }
    setLanguage_$lang
  } err ]} {
    tk_messageBox -message "Error loading $lang language: $err"
  }

  # If using Tk, translate all menus:
  if {[winfo exists .menu]} { setLanguageMenus }
  # update notation window
  if {[winfo exists .pgnWin]} { ::pgn::Refresh 1 }
  
  foreach i [array names ::tr] {
    if {[info exists ::translations($lang,$i)]} {
      set ::tr($i) $::translations($lang,$i)
    } elseif {[info exists ::translations(E,$i)]} {
      set ::tr($i) $::translations(E,$i)
    }
  }
}
################################################################################
# Will switch language only for Scid backoffice, no UI
# Used to make callbacks use english by default
################################################################################
proc setLanguageTemp { lang } {
  switch $lang {
    C {sc_info language cz}
    D {sc_info language de}
    F {sc_info language fr}
    G {sc_info language gr}
    H {sc_info language hu}
    I {sc_info language it}
    K {sc_info language ca}
    N {sc_info language ne}
    O {sc_info language no}
    S {sc_info language es}
    U {sc_info language fi}
    W {sc_info language sw}
    default {sc_info language en}
  }
}

addLanguage E English 0 iso8859-1 english.tcl
addLanguage K Català 2 iso8859-1 catalan.tcl
addLanguage C Czech 0 iso8859-2 czech.tcl
addLanguage D Deutsch 0 iso8859-1 deutsch.tcl
addLanguage F Francais 0 utf-8 francais.tcl
addLanguage G Greek 0 utf-8 greek.tcl
addLanguage H Hungarian 0 iso8859-2 hungary.tcl
addLanguage I Italian 0 iso8859-1 italian.tcl
addLanguage N Nederlands 0 iso8859-1 nederlan.tcl
addLanguage O Norsk 1 iso8859-1 norsk.tcl
addLanguage P Polish 0 iso8859-2 polish.tcl
addLanguage B {Brazil Portuguese} 0 iso8859-1 portbr.tcl
addLanguage R Russian 1 utf-8 russian.tcl
addLanguage Y Serbian 2 iso8859-2 serbian.tcl
addLanguage S Español 1 iso8859-1 spanish.tcl
addLanguage U Suomi 1 iso8859-1 suomi.tcl
addLanguage W Swedish 1 iso8859-1 swedish.tcl

setLanguage

### End of file: lang.tcl
