### sound.tcl
### Functions for playing sound files to announce moves.
### Part of Scid. Copyright (C) Shane Hudson 2004.
### Copyright (C) 2013 Fulvio Benini
###
### Uses the free Tcl/Tk sound package "Snack", which comes with
### most Tcl distributions. See http://www.speech.kth.se/snack/

### when an other application uses the audio device, no sound can be played. Forces a reset of pending sounds after 5 seconds
### which limits the maximum length of a playable sound

namespace eval ::utils::sound {}

set ::utils::sound::pipe ""
set ::utils::sound::hasSound 0
set ::utils::sound::isPlayingSound 0
set ::utils::sound::soundQueue {}
set ::utils::sound::soundFiles [list \
    King Queen Rook Bishop Knight CastleQ CastleK Back Mate Promote Check \
    a b c d e f g h x 1 2 3 4 5 6 7 8 move alert]

# soundMap
#
#   Maps characters in a move to sounds.
#   Before this map is used, "O-O-O" is converted to "q" and "O-O" to "k"
#   Also note that "U" (undo) is used for taking back a move.
#
array set ::utils::sound::soundMap {
  K King Q Queen R Rook B Bishop N Knight k CastleK q CastleQ
  x x U Back # Mate = Promote  + Check alert alert
  a a b b c c d d e e f f g g h h
  1 1 2 2 3 3 4 4 5 5 6 6 7 7 8 8
} 


# ::utils::sound::Setup
#
#   Called once at startup to load the Snack package and set up sounds.
#
proc ::utils::sound::Setup {} {
  variable hasSound
  variable soundFiles
  variable soundFolder

  set hasSound 1
  if {[catch {package require snack 2.0}]} {
    if {$::windowsOS} {
      catch {
        set ::utils::sound::pipe [open "| scidsnd.exe" "r+"]
        fconfigure $::utils::sound::pipe -blocking 0 -buffering line
        fileevent $::utils::sound::pipe readable {
          gets $::utils::sound::pipe
          ::utils::sound::SoundFinished
        }
      }
    }
    if { $::utils::sound::pipe == "" } { set hasSound 0 }
  } else {
    # Set up sounds. Each sound will be empty until a WAV file for it is found.
    foreach soundFile $soundFiles {
      ::snack::sound sound_$soundFile
    }
    ::utils::sound::ReadFolder
  }
}


# ::utils::sound::ReadFolder
#
#   Reads sound files from the specified directory.
#   Returns the number of Scid sound files found in that directory.
#
proc ::utils::sound::ReadFolder {{newFolder ""}} {
  variable soundFiles
  variable soundFolder
  
  if {$newFolder != ""} { set soundFolder "" }
  
  set count 0
  foreach soundFile $soundFiles {
    set f [file join $soundFolder $soundFile.wav]
    if {[file readable $f]} {
      if { $::utils::sound::pipe == "" } {
        sound_$soundFile configure -file $f
      }
      incr count
    }
  }
  return $count
}



proc ::utils::sound::AnnounceMove {move} {
  variable hasSound
  variable soundMap
  
  if {! $hasSound} { return }
  
  if {[string range $move 0 4] == "O-O-O"} { set move q }
  if {[string range $move 0 2] == "O-O"} { set move k }
  set move [::untrans $move]
  set parts [split $move ""]
  set soundList {}
  foreach part $parts {
    if {[info exists soundMap($part)]} {
      lappend soundList sound_$soundMap($part)
    }
  }
  if {[llength $soundList] > 0} {
    CancelSounds
    foreach s $soundList {
      PlaySound $s
    }
  }
}


proc ::utils::sound::AnnounceNewMove {move} {
  if {$::utils::sound::announceNew} { AnnounceMove $move }
}


proc ::utils::sound::AnnounceForward {move} {
  if {$::utils::sound::announceForward} { AnnounceMove $move }
}


proc ::utils::sound::AnnounceBack {} {
  if {$::utils::sound::announceBack} { AnnounceMove U }
}


proc ::utils::sound::SoundFinished {} {
  after cancel ::utils::sound::CancelSounds
  set ::utils::sound::isPlayingSound 0
  CheckSoundQueue
}


proc ::utils::sound::CancelSounds {} {
  if {! $::utils::sound::hasSound} { return }

  if { $::utils::sound::pipe != "" } {
    puts $::utils::sound::pipe "stop"
  } else {
    snack::audio stop
  }
  set ::utils::sound::soundQueue {}
  set ::utils::sound::isPlayingSound 0
}

################################################################################
#
################################################################################
proc ::utils::sound::PlaySound {sound} {
  if {! $::utils::sound::hasSound} { return }
  lappend ::utils::sound::soundQueue $sound
  after idle ::utils::sound::CheckSoundQueue
}

# ::utils::sound::CheckSoundQueue
#
#   Starts playing the next available sound, if there is one waiting
#   and no sound is currently playing. Called whenever a sound is
#   added to the queue or a sound has finished playing.
#
proc ::utils::sound::CheckSoundQueue {} {
  variable soundQueue
  variable isPlayingSound
  if {$isPlayingSound} { return }
  if {[llength $soundQueue] == 0} { return }
  
  set next [lindex $soundQueue 0]
  set soundQueue [lrange $soundQueue 1 end]
  set isPlayingSound 1
  if { $::utils::sound::pipe != "" } {
    set next [string range $next 6 end]
    set f [file join $::utils::sound::soundFolder $next.wav]
    puts $::utils::sound::pipe "[file nativename $f]"
  } else {
    catch { $next play -blocking 0 -command ::utils::sound::SoundFinished }
    after 5000 ::utils::sound::CancelSounds
  }
}


# ::utils::sound::OptionsDialog
#
#   Dialog window for configuring move sounds.
#
proc ::utils::sound::OptionsDialog { w } {
     if { ! $::utils::sound::hasSound} {
        ttk::label $w.status -text [tr SoundsSoundDisabled]
        pack $w.status -side bottom
    }
    ttk::checkbutton $w.n -variable ::utils::sound::announceNew -text [tr SoundsAnnounceNew]
    ttk::checkbutton $w.f -variable ::utils::sound::announceForward -text [tr SoundsAnnounceForward]
    ttk::checkbutton $w.b -variable ::utils::sound::announceBack -text [tr SoundsAnnounceBack]
    pack $w.n $w.f $w.b -side top -anchor w -padx "0 5"
}

proc ::utils::sound::GetDialogChooseFolder { widget } {
    set newFolder [tk_chooseDirectory \
                       -initialdir $::utils::sound::soundFolder \
                       -title "Scid: $::tr(SoundsFolder)" -parent [winfo toplevel $widget] ]
    # If the user selected a different folder to look in, read it
    # and tell the user how many sound files were found there.
    if {$newFolder != "" && $newFolder != $::utils::sound::soundFolder } {
        if { [::utils::sound::OptionsDialogChooseFolder $newFolder] } {
            $widget delete 0 end
            $widget insert end $newFolder
        }
    }
}

proc ::utils::sound::OptionsDialogChooseFolder { newFolder } {
    set ::utils::sound::soundFolder [file nativename $newFolder]
    set numSoundFiles [::utils::sound::ReadFolder]
    tk_messageBox -title "Scid: Sound Files" -type ok -icon info -parent .resDialog \
        -message "Found $numSoundFiles of [llength $::utils::sound::soundFiles] sound files in $::utils::sound::soundFolder"
    return $numSoundFiles
}

proc ::utils::sound::OptionsDialogOK {} {
  variable soundFolder
  
  # Destroy the Sounds options dialog
  set w .soundOptions
  catch {grab release $w}
  destroy $w
  
  set isNewSoundFolder 0
  if {$soundFolder != $::utils::sound::soundFolder_temp} {
    set isNewSoundFolder 1
  }
  
  # Update the user-settable sound variables:
  foreach v {soundFolder announceNew announceForward announceBack} {
    set ::utils::sound::$v [set ::utils::sound::${v}_temp]
  }
  
  # If the user selected a different folder to look in, read it
  # and tell the user how many sound files were found there.
  
  if {$isNewSoundFolder  &&  $soundFolder != ""} {
    set numSoundFiles [::utils::sound::ReadFolder]
    tk_messageBox -title "Scid: Sound Files" -type ok -icon info \
        -message "Found $numSoundFiles of [llength $::utils::sound::soundFiles] sound files in $::utils::sound::soundFolder"
  }
}

# Read the sound files at startup:
::utils::sound::Setup
