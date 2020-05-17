###
###    inputengine.tcl
###
###    This file adds input eninge protocol support to scid chess
###    database. It adds menu with subitems to the tools-menu and
###    additionally a button to enable the engine from within the GUI.
###    This module is selfcontained and can just be linked into the Scid
###    database upon built.
###
###    $Id: inputengine.tcl,v 4.3 2011/02/13 18:12:02 arwagner Exp $
###    Last change: <Mon, 2010/11/15 13:00:39 arwagner agamemnon>
###    Author     : Alexander Wagner
###    Language   : TCL
###
#----------------------------------------------------------------------

namespace eval ExtHardware {

  set engine     "dgtdrv2.i686";
  set port       "/dev/ttyUSB0"
  set param      "la"

  # the hardware configured by default:
  #  1 : Novag Citrine
  #  2 : Input Engine
  set hardware   1

  set bindbutton "::novag::connect"
  set showbutton 0

  #----------------------------------------------------------------------
  # Save the hardware options
  #----------------------------------------------------------------------
  proc saveHardwareOptions {} {
     set optionF ""
     if {[catch {open [scidConfigFile ExtHardware] w} optionF]} {
        tk_messageBox -title "Scid: Unable to write file" -type ok -icon warning \
           -message "Unable to write options file: [scidConfigFile InputEngine]\n$optionF"
     } else {
        puts $optionF "# Scid options file"
        puts $optionF "# Version: $::scidVersion, $::scidVersionDate"
        puts $optionF "# This file contains commands in the Tcl language format."
        puts $optionF "# If you edit this file, you must preserve valid Tcl"
        puts $optionF "# format or it will not set your Scid options properly."
        puts $optionF ""

        foreach i { ::ExtHardware::engine     \
                    ::ExtHardware::port       \
                    ::ExtHardware::param      \
                    ::ExtHardware::hardware   \
                    ::ExtHardware::showbutton \
                    ::ExtHardware::bindbutton } {
           puts $optionF "set $i [list [set $i]]"
        }

     }
     close $optionF
     set ::statusBar "External hardware options were saved to: [scidConfigFile correspondence]"

     # Check if the hw connect button exists already. If not, add it.
     if { [winfo exists .main.fbutton.button.exthardware]} { 
        return 
     } else {
        if { $::ExtHardware::showbutton == 1 } {

           ttk::frame .main.fbutton.button.space4 -width 15
           button .main.fbutton.button.exthardware -image tb_eng_disconnected
           .main.fbutton.button.exthardware configure -relief flat -border 1 -highlightthickness 0 \
               -anchor n -takefocus 0
           bind .main.fbutton.button.exthardware <Any-Enter> "+.main.fbutton.button.exthardware configure -relief groove"
           bind .main.fbutton.button.exthardware <Any-Leave> "+.main.fbutton.button.exthardware configure -relief flat; break"
           pack .main.fbutton.button.space4 .main.fbutton.button.exthardware -side left -pady 1 -padx 0 -ipadx 0 -pady 0 -ipady 0
           pack .main.fbutton.button.exthardware -side left -pady 1 -padx 0 -ipadx 0 -pady 0 -ipady 0

           .main.fbutton.button.exthardware configure -command $::ExtHardware::bindbutton
        }
     }

  }

  #----------------------------------------------------------------------
  # Set the hardware connect button image
  #----------------------------------------------------------------------
  proc HWbuttonImg {img} {

    if { $::ExtHardware::showbutton == 1 } {
      .main.fbutton.button.exthardware configure -image $img -relief flat
    }
  }

  #----------------------------------------------------------------------
  # Set the hardware connect button command binding
  #----------------------------------------------------------------------
  proc HWbuttonBind {cmd} {

    if { $::ExtHardware::showbutton == 1 } {
       set ::ExtHardware::bindbutton $cmd
    }
  }

  #----------------------------------------------------------------------
  # config:
  #    Opens the configuration dialog to input driver engines binary
  #    and parameters required to fire up the engine
  #----------------------------------------------------------------------
  proc config {} {
    global ::ExtHardware::port ::ExtHardware::engine ::ExtHardware::param ::ExtHardware::hardware

    ::ExtHardware::HWbuttonImg tb_eng_query

    set w .exthardwareConfig
    if { [winfo exists $w]} { return }
    win::createDialog $w

    wm title $w [::tr ExtHWConfigConnection]

    ttk::label $w.lport -text  [::tr ExtHWPort]
    ttk::entry $w.eport -width 50 -textvariable ::ExtHardware::port

    ttk::label $w.lengine -text [::tr ExtHWEngineCmd]
    ttk::entry $w.eengine -width 50 -textvariable ::ExtHardware::engine

    ttk::label $w.lparam -text  [::tr ExtHWEngineParam]
    ttk::entry $w.eparam -width 50 -textvariable ::ExtHardware::param

    ttk::label $w.options -text [::tr ExtHWHardware]
    
    ttk::checkbutton $w.showbutton -text [::tr ExtHWShowButton] -variable ::ExtHardware::showbutton

    #--------------
    # Add a new radio button for subsequent new hardware here:
    ttk::radiobutton $w.novag    -text [::tr ExtHWNovag]  -variable ::ExtHardware::hardware -value 1 -command { \
       set ::ExtHardware::bindbutton "::novag::connect"
       .exthardwareConfig.eengine configure -state disabled
       .exthardwareConfig.eparam  configure -state disabled
    }
    ttk::radiobutton $w.inputeng -text [::tr ExtHWInputEngine]   -variable ::ExtHardware::hardware -value 2 -command { \
       set ::ExtHardware::bindbutton "::inputengine::connectdisconnect"
       .exthardwareConfig.eengine configure -state normal
       .exthardwareConfig.eparam  configure -state normal
    }
    #--------------

    if { $::ExtHardware::hardware == 1 } {
       .exthardwareConfig.eengine configure -state disabled
       .exthardwareConfig.eparam  configure -state disabled
    }

    ttk::frame $w.buttons
    ttk::button $w.bOk -text OK -command { ::ExtHardware::saveHardwareOptions
       ::ExtHardware::HWbuttonBind $::ExtHardware::bindbutton
       destroy .exthardwareConfig
       $::ExtHardware::bindbutton
    }
    ttk::button $w.bCancel -text [::tr Cancel] -command "::ExtHardware::HWbuttonImg tb_eng_disconnected ; destroy $w"
    packdlgbuttons $w.bCancel $w.bOk -in $w.buttons

    grid $w.options    -stick ew    -row 0 -column 0
    grid $w.novag      -stick w     -row 0 -column 1
    grid $w.inputeng   -stick w     -row 1 -column 1

    grid $w.lport      -stick ew    -row 2 -column 0 
    grid $w.eport                   -row 2 -column 1

    grid $w.lengine    -stick ew    -row 3 -column 0
    grid $w.eengine                 -row 3 -column 1

    grid $w.lparam     -stick ew    -row 4 -column 0 
    grid $w.eparam                  -row 4 -column 1

    grid $w.showbutton -stick w     -row 5 -column 1

    grid $w.buttons    -sticky news -row 6 -column 0 -columnspan 2
    bind $w <F1> { helpWindow HardwareConfig}

  }

}
  #- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  # source the options file to overwrite the above setup
  if {[catch {source [scidConfigFile ExtHardware]} ]} {
    #::splash::add "Unable to find the options file: [file tail $optionsFile]"
  } else {

     # Add the button to connect the engine to the button bar
     if { $::ExtHardware::showbutton == 1 } {

        ttk::frame .main.fbutton.button.space4 -width 15
        button .main.fbutton.button.exthardware -image tb_eng_disconnected
        .main.fbutton.button.exthardware configure -relief flat -border 1 -highlightthickness 0 \
            -anchor n -takefocus 0
        bind .main.fbutton.button.exthardware <Any-Enter> "+.main.fbutton.button.exthardware configure -relief groove"
        bind .main.fbutton.button.exthardware <Any-Leave> "+.main.fbutton.button.exthardware configure -relief flat; break"
        pack .main.fbutton.button.space4 .main.fbutton.button.exthardware -side left -pady 1 -padx 0 -ipadx 0 -pady 0 -ipady 0
        pack .main.fbutton.button.exthardware -side left -pady 1 -padx 0 -ipadx 0 -pady 0 -ipady 0

        .main.fbutton.button.exthardware configure -command $::ExtHardware::bindbutton
     }

    ::splash::add "External hardware configuration was found and loaded."
  }



#======================================================================

namespace eval inputengine {
  global ::ExtHardware::port ::ExtHardware::engine ::ExtHardware::param ::ExtHardware::hardware

  set engine     $::ExtHardware::engine
  set port       $::ExtHardware::port
  set param      $::ExtHardware::param

  set InputEngine(pipe)     ""
  set InputEngine(log)      ""
  set InputEngine(logCount) 0
  set InputEngine(init)     0
  set connectimg            tb_eng_ok
  set MovingPieceImg        $::board::letterToPiece(.)80
  set MoveText              "     "

  set NoClockTime           "--:--"
  set StoreClock            0

  set WhiteClock            $::inputengine::NoClockTime
  set BlackClock            $::inputengine::NoClockTime
  set oldWhiteClock         $::inputengine::NoClockTime
  set oldBlackClock         $::inputengine::NoClockTime
  set toMove                "White"

  font create moveFont -family Helvetica -size 56 -weight bold

  #----------------------------------------------------------------------
  # Generate the console window also used for status display
  #----------------------------------------------------------------------
  proc consoleWindow {} {

    set w .inputengineconsole
    if { [winfo exists $w]} { 
       ::inputengine::disconnect
       return
    }

    ::createToplevel $w
    $w configure -background [ttk::style lookup . -background]
    ::setTitle $w [::tr IEConsole]

    ttk::scrollbar $w.ysc     -command { .inputengineconsole.console yview }
    text      $w.console -height 5  -width 80 -wrap word -yscrollcommand "$w.ysc set"

    ttk::label     $w.lmode   -text [::tr IESending]

    ::board::new $w.bd 25
     $w.bd configure -relief solid -borderwidth 1

    ttk::label     $w.engine      -text "$::ExtHardware::engine $::ExtHardware::port $::ExtHardware::param"

    ttk::radiobutton $w.sendboth  -text [::tr Both]  -variable send -value 1 -command { ::inputengine::sendToEngine sendboth  }
    ttk::radiobutton $w.sendwhite -text [::tr White] -variable send -value 2 -command { ::inputengine::sendToEngine sendwhite }
    ttk::radiobutton $w.sendblack -text [::tr Black] -variable send -value 3 -command { ::inputengine::sendToEngine sendblack }

    ttk::button $w.bInfo          -text Info           -command { ::inputengine::sysinfo }

    ###---### rotate does not work yet
    ttk::button $w.bRotate        -text [::tr IERotate]      -command { ::inputengine::rotateboard }

    ttk::button $w.bSync          -text [::tr IESynchronise] -command { ::inputengine::synchronise }
    ttk::button $w.bClose         -text [::tr Close]         -command { ::inputengine::connectdisconnect }

    # Buttons for visual move announcement
    button $w.bPiece -image $inputengine::MovingPieceImg
    button $w.bMove  -font moveFont -text  $inputengine::MoveText
    $w.bPiece configure -relief flat -border 0 -highlightthickness 0 -takefocus 0
    $w.bMove  configure -relief flat -border 0 -highlightthickness 0 -takefocus 0

    # Buttons for clock display
    button $w.wClock -text  $inputengine::WhiteClock
    button $w.bClock -text  $inputengine::BlackClock
    $w.wClock configure -relief flat -border 0 -highlightthickness 0 -takefocus 0
    $w.bClock configure -relief flat -border 0 -highlightthickness 0 -takefocus 0


    # Store the time as comment
    ttk::checkbutton $w.bStoreClock -text "Store Clock" -variable ::inputengine::StoreClock

    grid $w.console    -stick ns    -column 0  -row 0 -columnspan 12
    grid $w.ysc        -stick ns    -column 12 -row 0

    grid $w.engine     -stick ewns   -column 0  -row 1 -columnspan 9

    grid $w.lmode      -stick ew    -column 0  -row 2
    grid $w.sendboth   -stick e     -column 2  -row 2 
    grid $w.sendwhite               -column 4  -row 2 
    grid $w.sendblack  -stick w     -column 6  -row 2 

    grid $w.bInfo      -stick ew    -column 0  -row 3
    ###---### grid $w.bRotate   -stick ew    -column 0  -row 4
    grid $w.bSync      -stick ew    -column 0  -row 5
    grid $w.bStoreClock -stick ew   -column 0  -row 6
    grid $w.bClose     -stick ew    -column 0  -row 11

    grid $w.bPiece     -stick nwes  -column 2  -row 3 -rowspan 9 -columnspan 3
    grid $w.bMove      -stick nwes  -column 5  -row 3 -rowspan 9 -columnspan 3

    grid $w.wClock     -stick nwes  -column 9 -row 11 -columnspan 7
    grid $w.bClock     -stick nwes  -column 9 -row 1  -columnspan 7

    grid $w.bd         -stick nw    -column 9  -row 2 -rowspan 9 -columnspan 7

    bind $w <Destroy> { catch ::inputengine::connectdisconnect }
    bind $w <F1> { helpWindow InputEngine}

    ::createToplevelFinalize $w
  }

  proc updateConsole {line} {
    set t .inputengineconsole.console
    $t insert end "$line\n"
    $t yview moveto 1
  }

  #----------------------------------------------------------------------
  # connectdisconnect()
  #   Connects or disconnects depending on the current status of the
  #   external input engine
  #----------------------------------------------------------------------
  proc connectdisconnect {} {
    global  ::inputengine::InputEngine

    set connection $::inputengine::InputEngine(pipe)

    if {$connection == ""} {
      consoleWindow
      ::inputengine::connect
    } else {
      ::inputengine::disconnect
    }
  }

  #----------------------------------------------------------------------
  # connect():
  #     Fire upt the input engine and connect it to a local pipe.
  #     Also register the eventhandler
  #----------------------------------------------------------------------
  proc connect {} {
    global ::inputengine::InputEngine ::inputengine::engine \
        ::inputengine::port ::inputengine::param

	 set ::inputengine::engine     $::ExtHardware::engine
	 set ::inputengine::port       $::ExtHardware::port
	 set ::inputengine::param      $::ExtHardware::param

    ::ExtHardware::HWbuttonImg tb_eng_connecting

    if {[catch {set InputEngine(pipe) [open "| $engine $port $param" "r+"]} result]} {
      ::ExtHardware::HWbuttonImg tb_eng_error
      tk_messageBox -title "Scid: Input Engine" -icon warning -type ok \
          -message "[::tr IEUnableToStart]\n$engine $port $param"
      ::inputengine::resetEngine
      return
    }

    ::inputengine::Init
  }

  #----------------------------------------------------------------------
  # disconneet()
  #    Disconnect and close the input engine
  #----------------------------------------------------------------------
  proc disconnect {} {
    global ::inputengine::InputEngine
    set pipe $::inputengine::InputEngine(pipe)

    set ::inputengine::connectimg tb_eng_connecting 

    ::inputengine::sendToEngine "stop"
    ::inputengine::sendToEngine "quit"
    set ::inputengine::connectimg tb_eng_disconnected

    if { [winfo exists ::inputengine::.inputengineconsole]} { 
       destroy ::inputengine::.inputengineconsole
    }
  }

  #----------------------------------------------------------------------
  # logEngine
  #    Simple log routine, ie. writing to stdout
  #----------------------------------------------------------------------
  proc logEngine {msg} {
      updateConsole "$msg"
  }

  #----------------------------------------------------------------------
  # sendToEngine()
  #    Send a string to the engine and log it by means of logEngine
  #----------------------------------------------------------------------
  proc sendToEngine {msg} {
    global ::inputengine::InputEngine
    set pipe $::inputengine::InputEngine(pipe)

    ::inputengine::logEngine "> $msg"
    puts $pipe $msg
    flush $pipe
  }

  #----------------------------------------------------------------------
  # init()
  #    Initialises the engine and internal data
  #----------------------------------------------------------------------
  proc Init {} {
    global ::inputengine::InputEngine
    set pipe $::inputengine::InputEngine(pipe)

    # Configure the pipe and initiate the engine
    set pipe $::inputengine::InputEngine(pipe)
    fconfigure $pipe -buffering full -blocking 0
    # register the eventhandler
    fileevent  $pipe readable "::inputengine::readFromEngine"

    ::inputengine::newgame
  }

  #----------------------------------------------------------------------
  # resetEngine()
  #    Resets the engines global variables
  #----------------------------------------------------------------------
  proc resetEngine {} {
    global ::inputengine::InputEngine

    ::ExtHardware::HWbuttonImg tb_eng_disconnected
    destroy .inputengineconsole
    set ::inputengine::InputEngine(pipe)     ""
    set ::inputengine::InputEngine(log)      ""
    set ::inputengine::InputEngine(logCount) 0
    set ::inputengine::InputEngine(init)     0
  }


  #----------------------------------------------------------------------
  # sysinfo()
  #    Initialises the engine and internal data
  #----------------------------------------------------------------------
  proc sysinfo {} {
    global ::inputengine::InputEngine
    set pipe $::inputengine::InputEngine(pipe)

    # call system information
    ::inputengine::sendToEngine "sysinfo"
  }

  #----------------------------------------------------------------------
  # rotateboard()
  #    Rotates the board, ie. exchanges a1 and h8
  #----------------------------------------------------------------------
  proc rotateboard {} {
    global ::inputengine::InputEngine
    set pipe $::inputengine::InputEngine(pipe)

    # rotate the graphical boards
    ::board::flip .main.board
    ::board::flip .inputengineconsole.bd

    ::inputengine::newgame
    # rotate the board for the input engine
    ::inputengine::sendToEngine "rotateboard"
    ::inputengine::synchronise
  }

  #----------------------------------------------------------------------
  # newgame()
  #    Handle NewGame event from board
  #----------------------------------------------------------------------
  proc newgame {} {

    # Ask the user to save the current game
    ::game::Clear
    sc_game tags set -event "InputEngine Input"
    sc_game tags set -date [::utils::date::today]
  }

  #----------------------------------------------------------------------
  # endgame()
  #    Handle game ending (end game event + result)
  #----------------------------------------------------------------------
  proc endgame {result} {

    set filternum [sc_filter first]

    logEngine "  info End Game $filternum: $result"

    sc_game tags set -result $result
    gameAdd
  }

  #----------------------------------------------------------------------
  # synchronise()
  #    read board position and set scid's representation accordingly
  #----------------------------------------------------------------------
  proc synchronise {} {
    global ::inputengine::InputEngine

    logEngine "  info Sync called"
    set InputEngine(init) 0

    ::inputengine::sendToEngine "getposition"
    ::inputengine::sendToEngine "getclock"
  }

  proc strreverse {str} {
     set res {}
     set i [string length $str]
     while {$i > 0} {append res [string index $str [incr i -1]]}
     set res
  }

  #----------------------------------------------------------------------
  # readFromEngine()
  #     Event Handler for commands and moves sent from the input
  #     engine, implements input engine protocol
  #----------------------------------------------------------------------
  proc readFromEngine {} {
    global ::inputengine::InputEngine ::inputengine::connectimg
    set pipe $::inputengine::InputEngine(pipe)

    set line     [string trim [gets $pipe] ]

    # Close the pipe in case the engine was stopped
    if [eof $pipe] {
      catch {close $pipe}
      ::inputengine::resetEngine
      return
    }

    switch -regexp -- $line \
        "^move *" {
          set m [string range $line 5 end]

          set s1 [string range $m 0 1]
          set s2 [string range $m 2 end]
          if {$s1 == "0-"} {
            # castling must not be rewritten
            set m "$s1$s2"
          } else {
            set m "$s1-$s2"
          }

          logEngine "$line"

          if {[catch {sc_move addSan $m}]} {
             ::utils::sound::PlaySound "sound_alert"
             logEngine "  info Illegal move detected!"
             logEngine "  info Ignoring: $m"
             .inputengineconsole.bPiece configure -background red
             .inputengineconsole.bMove  configure -background red -text $m
          } else {

            .inputengineconsole.bPiece configure -background green
            .inputengineconsole.bMove  configure -background green -text $m

             updateBoard -animate
             updateBoard -pgn
             ::inputengine::sendToEngine "getposition"
             ::inputengine::sendToEngine "getclock"
          }
        } \
        "info *" {
          logEngine "< $line"
          set event [string range $line 5 end]
          switch -regexp -- $event \
          "string ERROR *" {
            set err [string range $event 7 end]
            tk_messageBox -title "Scid: Input Engine" \
            -icon warning -type ok -message "Engine $err"
            catch {close $pipe}
            ::ExtHardware::HWbuttonImg tb_eng_error
            return
          } \
          "string Chessboard found and initialised*" {
            # switch to xboard mode and disable move
            # announcements by the driver engine
            ::inputengine::sendToEngine "xboard"
            ::inputengine::sendToEngine "announce"
          } \
          "Engine mode    : xboard*" {
            ::inputengine::sendToEngine "getposition"
            ::ExtHardware::HWbuttonImg $inputengine::connectimg
          } \
          "string FEN *" {
            set InputEngine(init) 0
            # The first FEN string is always sent as
            # info string FEN ...
            # as this is compatible with both UCI and xboard.
            # At this stage the engine is not set to xboard mode
            # yet, so this signals a new program startup
            # accordingly.
          } \
          "FEN *" {
            set fenstr [string range $event 4 end]
            set fenstr [string trim $fenstr]
            if { $::inputengine::InputEngine(init) == 0 }  {
              # Initialise scid's representation with the FEN
              # delivered by the external board.
              catch {sc_game startBoard $fenstr}
              updateBoard -pgn
              set InputEngine(init) 1
            } else {
              # Compare the internal representation to the
              # board each time a FEN string arrives from the
              # driver.
              #
              # Do not check the whole FEN, as the external
              # board can not know anything about e.p. or O-O
              # capabilities. Strip them off and compare just
              # the piece settings.
              set space [string first " " $fenstr]
              set fen [string range $fenstr 0 $space]

              set space [string first " " [sc_pos fen]]
              set int [string range [sc_pos fen] 0 $space]

              if {$fen != $int} {
                ::utils::sound::PlaySound "sound_alert"
                logEngine "  info Wrong Position! $int (scid) != $fen (external)"
              } else {
                logEngine "  info Board and internal position match."
              }
              # Generate a board position out of the FEN
              # RNBQKBNRPPPP.PPP............P................n..pppppppprnbqkb.r w
              # Something is in reverse here:
              ###---### set extpos $fen
              ###---### regsub -all {8} $extpos "........" extpos
              ###---### regsub -all {7} $extpos "......." extpos
              ###---### regsub -all {6} $extpos "......" extpos
              ###---### regsub -all {5} $extpos "....." extpos
              ###---### regsub -all {4} $extpos "...." extpos
              ###---### regsub -all {3} $extpos "..." extpos
              ###---### regsub -all {2} $extpos ".." extpos
              ###---### regsub -all {1} $extpos "." extpos
              ###---### regsub -all {/} $extpos "" extpos
              ###---### puts stderr [sc_pos board]
              ###---### puts stderr [strreverse "$extpos"]
              ###---### set extpos "$extpos w"
              ###---### ::board::update .inputengineconsole.bd "$extpos w"
            }
          } \
          {moving piece: [A-Z] *} {
            .inputengineconsole.bPiece configure -image $::board::letterToPiece([string range $event 14 end])80
          }\
          {moving piece: [a-z] *} {
            .inputengineconsole.bPiece configure -image $::board::letterToPiece([string range $event 14 end])80
          }\
          "!new game!" {
            ::inputengine::newgame
            .inputengineconsole.bPiece configure -background blue
            .inputengineconsole.bMove  configure -background blue -text "OK"
            .inputengineconsole.bPiece configure -image $::board::letterToPiece(K)80
          } \
          "!move now!" {
            logEngine "< info $event"
          } \
          "!end game 1-0!" {
            logEngine "< info $event"
            ::inputengine::endgame "1-0"
            .inputengineconsole.bPiece configure -background white
            .inputengineconsole.bMove  configure -background white -text "1-0"
            .inputengineconsole.bPiece configure -image $::board::letterToPiece(K)80
          } \
          "!end game 0-1!" {
            logEngine "< info $event"
            ::inputengine::endgame "0-1"
            .inputengineconsole.bPiece configure -background gray
            .inputengineconsole.bMove  configure -background gray -text "0-1"
            .inputengineconsole.bPiece configure -image $::board::letterToPiece(k)80
          } \
          "!end game 1/2-1/2!" {
            logEngine "< info $event"
            ::inputengine::endgame "1/2-1/2"
            .inputengineconsole.bPiece configure -background black
            .inputengineconsole.bMove  configure -background white -text "1/2-1/2"
            .inputengineconsole.bPiece configure -image $::board::letterToPiece(.)80
          } \
          "!enter setup mode!" {
            .inputengineconsole.bPiece configure -background yellow
            .inputengineconsole.bMove  configure -background yellow -text "Setup"
            .inputengineconsole.bPiece configure -image $::board::letterToPiece(K)80
            logEngine "< info $event"
          } \
          "!end setup mode!" {
            logEngine "< info $event"
            ::inputengine::synchronise
            .inputengineconsole.bPiece configure -background yellow
            .inputengineconsole.bMove  configure -background yellow -text "OK"
            .inputengineconsole.bPiece configure -image $::board::letterToPiece(q)80
          } \
          "!white to move!" {
            set ::inputengine::toMove "White"
            .inputengineconsole.wClock configure -background white
            .inputengineconsole.bClock configure -background gray -foreground black

            if {$::inputengine::StoreClock == 1} {
               if { ($::inputengine::oldWhiteClock != $::inputengine::NoClockTime) && \
                    ($::inputengine::WhiteClock    != $::inputengine::NoClockTime) } {
                  set wHrs [expr $::inputengine::WhiteClock / 60 / 60]
                  set wMin [expr ($::inputengine::WhiteClock - $wHrs*60*60) / 60 ]
                  set wSec [expr ($::inputengine::WhiteClock - $wHrs*60*60 - $wMin * 60) ]
                  set timediff [expr $::inputengine::oldWhiteClock - $::inputengine::WhiteClock]
                  set ::inputengine::oldWhiteClock $::inputengine::WhiteClock
                  sc_pos setComment "\[%ct $wHrs:$wMin:$wSec\] \[%emt $timediff\]"
               }
            }
          } \
          "!black to move!" {
            set ::inputengine::toMove "Black"
            .inputengineconsole.wClock configure -background gray
            .inputengineconsole.bClock configure -background black -foreground white

            if {$::inputengine::StoreClock == 1} {
               if { ($::inputengine::oldBlackClock != $::inputengine::NoClockTime) && \
                    ($::inputengine::BlackClock    != $::inputengine::NoClockTime) } {
                  set bHrs [expr $::inputengine::BlackClock / 60 / 60]
                  set bMin [expr ($::inputengine::BlackClock - $bHrs*60*60) / 60 ]
                  set bSec [expr ($::inputengine::BlackClock - $bHrs*60*60 - $bMin * 60) ]
                  set timediff [expr $::inputengine::oldBlackClock - $::inputengine::BlackClock]
                  set ::inputengine::oldBlackClock $::inputengine::BlackClock
                  sc_pos setComment "\[%ct $bHrs:$bMin:$bSec\] \[%emt $timediff\]"
               }
            }
          } \
          "No Clock detected" {
             set ::inputengine::WhiteClock $::inputengine::NoClockTime
             set ::inputengine::BlackClock $::inputengine::NoClockTime
             .inputengineconsole.wClock configure -text $::inputengine::WhiteClock
             .inputengineconsole.bClock configure -text $::inputengine::BlackClock
          } \
          "Time White:" {
            if { ($::inputengine::oldWhiteClock == $::inputengine::NoClockTime) } {
               set ::inputengine::oldWhiteClock $::inputengine::WhiteClock
            }
            # Get the time in seconds
            regsub -all {[A-Za-z:# ]} $event "" ::inputengine::WhiteClock

            # calculate a sensible format
            set wHrs [expr $::inputengine::WhiteClock / 60 / 60]
            set wMin [expr ($::inputengine::WhiteClock - $wHrs*60*60) / 60 ]
            set wSec [expr ($::inputengine::WhiteClock - $wHrs*60*60 - $wMin * 60) ]

            if {$wHrs > 0} {
               .inputengineconsole.wClock configure -text "$wHrs:$wMin:$wSec (EXT)"
            } else {
               .inputengineconsole.wClock configure -text "$wMin:$wSec (EXT)"
            }

            ###---### Is this enough to set game clocks for all possible occurrences?
            catch { ::gameclock::setSec 1 [expr -1*$::inputengine::WhiteClock] }
          } \
          "Time Black:" {
            if { ($::inputengine::oldBlackClock == $::inputengine::NoClockTime) } {
               set ::inputengine::oldBlackClock $::inputengine::BlackClock
            }
            regsub -all {[A-Za-z:# ]} $event "" ::inputengine::BlackClock

            set bHrs [expr $::inputengine::BlackClock / 60 / 60]
            set bMin [expr ($::inputengine::BlackClock - $bHrs*60*60) / 60 ]
            set bSec [expr ($::inputengine::BlackClock - $bHrs*60*60 - $bMin * 60) ]

            if {$bHrs > 0} {
               .inputengineconsole.bClock configure -text "$bHrs:$bMin:$bSec (EXT)"
            } else {
               .inputengineconsole.bClock configure -text "$bMin:$bSec (EXT)"
            }

            ###---### Is this enough to set game clocks for all possible occurrences?
            catch { ::gameclock::setSec 2 [expr -1*$::inputengine::BlackClock] }
          } \
          "Wrong move performed:" {
             # This event can only be used if there is a possibility to
             # send the last move to the input engine for it to cross
             # check. This however is not easy in Scid, therefore
             # compare FEN.
             #
             # ::utils::sound::PlaySound "sound_alert"
             # logEngine "< info $event"
          } \
          "DGT Projects - This DGT board" {
            set ::inputengine::connectimg tb_eng_dgt
            set txt [string range $event 7 end]
            ## ::utils::tooltip::Set .button.exthardware "$::inputengine::port:\n$txt"
          } \
        } \
        default  {
          logEngine "< $line"
        }
        # Should better show current wooden board position? Return value of 
        # sc_pos board is
        # RNBQKBNRPPPP.PPP............P................n..pppppppprnbqkb.r w
        ::board::update .inputengineconsole.bd [sc_pos board]
  }



}


###
### End of file: inputengine.tcl
###
