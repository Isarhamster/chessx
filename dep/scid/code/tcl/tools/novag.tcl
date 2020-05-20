###
### novag.tcl: part of Scid.
### Copyright (C) 2007  Pascal Georges
###
######################################################################
### add NOVAG board support

namespace eval novag {
  set fd ""
  set connected 0
  set waitBetweenMessages 0
  
  ##########################################################
  proc connect {} {
    global ::novag::fd
    
    set serial $::ExtHardware::port
    
    set w .novag
    ::createToplevel $w
    ::setTitle $w "Novag Citrine"
    pack [ttk::panedwindow $w.f -orient vertical] -expand 1 -fill both
    
    ttk::frame $w.f.top
    ttk::frame $w.f.bottom
    $w.f add $w.f.top -weight 1
    $w.f add $w.f.bottom -weight 1
    
    ttk::scrollbar $w.f.top.ysc -command { .novag.f.top.output yview }
    text $w.f.top.output -width 20 -height 20  -wrap word  -yscrollcommand ".novag.f.top.ysc set"
    pack $w.f.top.ysc -side left -fill y -side right
    pack .novag.f.top.output -side left -fill both -expand 1 -side right
    
    entry $w.f.bottom.input -width 20
    ttk::button $w.f.bottom.send -text Send -command {
      ::novag::send [.novag.f.bottom.input get]
      .novag.f.bottom.input delete 0 end
    }
    bind $w.f.bottom.input <Return> " $w.f.bottom.send invoke "
    bind $w <Destroy> { catch ::novag::disconnect }
    bind $w <F1> { helpWindow Novag}
    ::createToplevelFinalize $w
    
    ttk::checkbutton $w.f.bottom.cbref -text [tr "NovagReferee" ] -variable ::novag::referee -offvalue "OFF" -onvalue "ON" -command { ::novag::send  "U $::novag::referee" }
    
    pack $w.f.top.output -fill both -expand 1
    pack $w.f.bottom.input $w.f.bottom.send  $w.f.bottom.cbref -side left
    update
    
    # Set button to "connection in progress"
    ::ExtHardware::HWbuttonImg tb_eng_connecting
    
    if {[catch { set fd [open $serial r+ ] } err]} {
        tk_messageBox -type ok -icon error -parent . -title "Novag Citrine" -message "Connection error for $serial \n $err"
        destroy $w
        return
    }
    
    # 57600 bauds, no parity, 8 bits, 1 stop
    fconfigure $fd -mode 57600,n,8,1 -blocking 0 -buffering line
    fileevent $fd readable ::novag::recv
    # human / human mode
    # get info
    ::novag::send "I"
    wait 200
    # new game
    ::novag::send "N"
    wait 200
    ::novag::send "X ON"
    wait 200
    ::novag::send "U ON"
    set ::novag::connected 1
    
    # Set button to "connected, ready to use"
    ::ExtHardware::HWbuttonImg tb_eng_ok
  }
  ##########################################################
  proc disconnect {} {
    global ::novag::fd
    close $fd
    set ::novag::connected 0
  }
  
  ##########################################################
  proc addMove {san} {
    # if promotion add "/"
    if {[string length $san] == 5} {
      set san "[string range $san 0 3]/[string range $san 4 end]"
    }
    
    ::novag::send "M $san"
    if { [ string first "x" [sc_game info previousMove] ] != -1 } {
      wait 3000
    } else {
      wait 200
    }
    # ::novag::send "T"
    ::novag::send "M $san"
  }
  ##########################################################
  proc send {msg} {
    global ::novag::fd
    puts $fd "$msg\n\r"
  }
  ##########################################################
  proc recv {} {
    global ::novag::fd
    
    set output .novag.f.top.output
    
    set l [gets $fd]
    if { $l == "" } { return }
    $output insert end "$l\n"
    $output yview moveto 1
    
    if {[string match -nocase "New Game*" $l]} {
      sc_game new
      updateBoard -pgn
      ::novag::send "U $::novag::referee"
      return
    }
    
    if {[lindex $l 0] == "M"} {
      
      if {[sc_pos side] == "white" && [string index [lindex $l 1] end ] == ","} {  return }
      if {[sc_pos side] == "black" && [string index [lindex $l 1] end ] != ","} {  return }
      
      set m [lindex $l 2]
      set m [regsub -- "-" $m ""]
      set m [regsub -- "/" $m ""]
      if { [ catch { sc_move addSan $m } err ] } {
        puts $err
      } else {
        if {[winfo exists .fics]} {
          if { $::fics::playing == 1} {
            ::fics::writechan [ sc_game info previousMoveNT ]
          }
        }
      }
      updateBoard -pgn
      return
    }
    
    if {[lindex $l 0] == "T"} {
      sc_move back
      updateBoard -pgn
      return
    }
    
  }
  ##########################################################
  proc wait {ms} {
    after $ms {set ::novag::waitBetweenMessages 1}
    vwait ::novag::waitBetweenMessages
  }
}

###
### End of file: novag.tcl
###
