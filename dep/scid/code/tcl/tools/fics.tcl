
###
### fics.tcl: part of Scid.
### Copyright (C) 2007  Pascal Georges
###

namespace eval fics {
  set server "freechess.org"
  set sockchan 0
  set seeklist {}
  set observedGame -1
  set playing 0
  set rated 0
  set waitForRating ""
  set waitForMoves ""
  set silence 1
  set sought 0
  set soughtlist {}
  set width 300
  set height 300
  set off 20
  set graphon 0
  set timeseal_pid 0
  font create font_offers -family courier -size 12 -weight bold
  set history {}
  set history_pos 0
  set offers_minelo 1000
  set offers_maxelo 2500
  set offers_mintime 0
  set offers_maxtime 60
  variable logged 0
  variable isGuestLogin 0
  array set profileVars {}
  array set findopponent {}

  set showabortreq 1
  set showadjournreq 1
  set showdrawreq 1
  set showtakebackreq 1

  set premoveSq1 -1
  set premoveSq2 -1

  ################################################################################
  #
  ################################################################################
  proc config {} {
    variable logged
    global ::fics::sockChan
    set w ".ficsConfig"

    if {[winfo exists $w]} {
      focus $w
      return
    }

    if {[winfo exists .fics]} {
      focus .fics
      return
    }

    set logged 0
    set ::fics::showPass 0

    win::createDialog $w
    ::setTitle $w [::tr "ConfigureFics"]
    ttk::labelframe $w.f -text "Login"
    ttk::labelframe $w.conf -text $::tr(CCDlgCGeneraloptions)
    ttk::frame $w.fbuttons

    ttk::label $w.f.lLogin -text [::tr "CCDlgLoginName"]
    ttk::entry $w.f.login -width 20 -textvariable ::fics::login
    ttk::label $w.f.lPwd -text [::tr "CCDlgPassword"]
    ttk::entry $w.f.passwd -width 20 -textvariable ::fics::password -show *
    ttk::checkbutton $w.f.showPass -text [::tr "CCDlgShowPassword"] -variable ::fics::showPass -command {
      if {$::fics::showPass} {
        .ficsConfig.f.passwd configure -show {}
      } else {
        .ficsConfig.f.passwd configure -show *
      }
    }

    ttk::button $w.fbuttons.connect -text [::tr "FICSConnect"] -state disabled -command {
      ::fics::connect [.ficsConfig.f.login get] [.ficsConfig.f.passwd get]
      destroy .ficsConfig
    }
    ttk::button $w.fbuttons.guest -text [::tr FICSGuest] -state disabled -command {
      ::fics::connect "guest" ""
      destroy .ficsConfig
    }
    ttk::button $w.fbuttons.cancel -text [::tr "Cancel"] -command { destroy .ficsConfig }

    set row 0
    grid $w.f.lLogin -column 0 -row $row -sticky w -padx "0 5"
    grid $w.f.login -column 1 -row $row -sticky w
    incr row
    grid $w.f.lPwd -column 0 -row $row -sticky w -padx "0 5"
    grid $w.f.passwd -column 1 -row $row -sticky w
    incr row
    grid $w.f.showPass -column 1 -row $row -sticky w
    incr row
    pack $w.f -side top -anchor w -fill x
    pack $w.conf -side top -anchor w -pady "10 0"

    # use default user variables
    ttk::checkbutton $w.conf.cbvars -text [::tr "FICSdefaultuservars"] -variable ::fics::usedefaultvars
    grid $w.conf.cbvars -column 0 -row $row -sticky w -columnspan 2
    incr row

    # enable premove
    ttk::checkbutton $w.conf.premove -text [::tr "FICSpremove"] -variable ::fics::premoveEnabled
    grid $w.conf.premove -column 0 -row $row -sticky w -columnspan 2
    incr row

    # Time seal configuration
    ttk::checkbutton $w.conf.cbts -text "Time seal" -variable ::fics::use_timeseal -onvalue 1 -offvalue 0
    grid $w.conf.cbts -column 0 -row $row -sticky w
    incr row
    ttk::entry $w.conf.eExec -textvariable ::fics::timeseal_exec
    ttk::button $w.conf.bExec -text "..." -command { set ::fics::timeseal_exec [tk_getOpenFile] }
    grid $w.conf.eExec -column 0 -row $row -columnspan 2 -sticky we -padx "20 10"
    grid $w.conf.bExec -column 2 -row $row -sticky w
    incr row
    ttk::label $w.conf.lFICS_ip -text [::tr "FICSServerAddress" ]
    ttk::entry $w.conf.ipserver -width 16 -textvariable ::fics::server_ip -state readonly
    ttk::button $w.conf.bRefresh -text [::tr "FICSRefresh" ] -command ::fics::getIP
    ttk::label $w.conf.lFICS_port -text [::tr "FICSServerPort"]
    ttk::entry $w.conf.portserver -width 6 -textvariable ::fics::port_fics
    ttk::label $w.conf.ltsport -text [::tr "FICSTimesealPort"]
    ttk::entry $w.conf.portts -width 6 -textvariable ::fics::port_timeseal

    grid $w.conf.lFICS_ip -column 0 -row $row -sticky w -padx "0 5"
    grid $w.conf.ipserver -column 1 -row $row -sticky w -padx "0 10"
    grid $w.conf.bRefresh -column 2 -row $row -sticky w
    incr row
    grid $w.conf.lFICS_port -column 0 -row $row -sticky w -padx "0 5"
    grid $w.conf.portserver -column 1 -row $row -sticky w
    incr row
    grid $w.conf.ltsport -column 0 -row $row -sticky w -padx "0 5"
    grid $w.conf.portts -column 1 -row $row -sticky w
    incr row

    pack $w.fbuttons -side top -anchor e
    packdlgbuttons $w.fbuttons.cancel $w.fbuttons.connect $w.fbuttons.guest

    bind $w <Escape> "$w.fbuttons.cancel invoke"
    bind $w <F1> { helpWindow FICSLogin}

    # Get IP address of server (as Timeseal needs IP address)
    if { $::fics::server_ip == "0.0.0.0" } {
      getIP
    }

    $w.fbuttons.connect configure -state normal
    $w.fbuttons.guest configure -state normal

  }
  ################################################################################
  #
  ################################################################################
  proc getIP {} {
    set b .ficsConfig.conf.bRefresh
    $b configure -state disabled
    update
    # First handle the case of a network down
    if { [catch {set sockChan [socket -async $::fics::server $::fics::port_fics]} err]} {
      tk_messageBox -icon error -type ok -title "Unable to contact $::fics::server" -message $err -parent .ficsConfig.f
      return
    }

    # Then the case of a proxy
    set timeOut 5
    set i 0
    while { $i <= $timeOut } {
      after 1000

      if { [catch {set peer [ fconfigure $sockChan -peername ]} err]} {
        if {$i == $timeOut} {
          tk_messageBox -icon error -type ok -title "Unable to contact $::fics::server" -message $err -parent .ficsConfig.f
          return
        }
      } else  {
        break
      }
      incr i
    }

    set ::fics::server_ip [lindex $peer 0]
    ::close $sockChan
    $b configure -state normal
  }
  ################################################################################
  #
  ################################################################################
  proc setProfileVars { login } {
    global  ::fics::profileVars
    if { ! [info exists profileVars(initTime_$login)] } {
      return
    }
    set ::fics::findopponent(initTime) $profileVars(initTime_$login)
    set ::fics::findopponent(incTime) $profileVars(incTime_$login)
    set ::fics::findopponent(rated) $profileVars(rated_$login)
    set ::fics::findopponent(color) $profileVars(color_$login)
    set ::fics::findopponent(limitrating) $profileVars(limitrating_$login)
    set ::fics::findopponent(rating1) $profileVars(rating1_$login)
    set ::fics::findopponent(rating2) $profileVars(rating2_$login)
    set ::fics::findopponent(manual) $profileVars(manual_$login)
    set ::fics::findopponent(formula) $profileVars(formula_$login)
  }
  ################################################################################
  #
  ################################################################################
  proc syncProfileVars { login } {
    global  ::fics::profileVars
    variable isGuestLogin

    if {$isGuestLogin} {
      set login "guest"
    }
    set profileVars(initTime_$login) $::fics::findopponent(initTime)
    set profileVars(incTime_$login) $::fics::findopponent(incTime)
    set profileVars(rated_$login) $::fics::findopponent(rated)
    set profileVars(color_$login) $::fics::findopponent(color)
    set profileVars(limitrating_$login) $::fics::findopponent(limitrating)
    set profileVars(rating1_$login) $::fics::findopponent(rating1)
    set profileVars(rating2_$login) $::fics::findopponent(rating2)
    set profileVars(manual_$login) $::fics::findopponent(manual)
    set profileVars(formula_$login) $::fics::findopponent(formula)
  }
  ################################################################################
  #
  ################################################################################
  proc storeTime { } {
      set side 1
      if { [sc_pos side] == "white" } {set side 2 }
      ::gameclock::storeTimeComment $side
  }
  ################################################################################
  #
  ################################################################################
  proc connect { login passwd } {
    global ::fics::sockchan ::fics::seeklist ::fics::width ::fics::height ::fics::off
    variable isGuestLogin

    if {$login != ""} {
      set ::fics::reallogin $login
      # do not reset the password if we log in as guest.
      # This allows to reset it if we have another UID.
      if {$login != "guest"} {
         set ::fics::password $passwd
      }
    } else {
      return
    }

    set isGuestLogin [string match -nocase "guest" $login]

    setProfileVars $login

    # check timeseal configuration
    if {$::fics::use_timeseal} {
      if {![ file executable $::fics::timeseal_exec ]} {
        tk_messageBox -title "Error" -icon error -type ok -message "Timeseal exec error : $::fics::timeseal_exec"
        return
      }
    }

    set w .fics
    ::createToplevel $w
    ::setTitle $w "Free Internet Chess Server $::fics::reallogin"
    grid [ttk::panedwindow $w.f -orient vertical] -sticky news
    grid rowconfigure $w 0 -weight 1
    grid columnconfigure $w 0 -weight 1

    ttk::notebook $w.f.top
    ttk::frame $w.f.top.fconsole
    ttk::frame $w.f.top.fconsole.f1
    ttk::frame $w.f.top.fconsole.f2

    ttk::frame $w.f.top.foffers
    $w.f.top add $w.f.top.fconsole -sticky nsew -text [::tr "FICSConsole"]
    $w.f.top add $w.f.top.foffers -sticky nsew -text [::tr "FICSOffers"]

    grid $w.f.top.fconsole.f1 -sticky news
    grid $w.f.top.fconsole.f2 -sticky news
    grid rowconfigure $w.f.top.fconsole 0 -weight 1
    grid columnconfigure $w.f.top.fconsole 0 -weight 1
    ttk::frame $w.f.bottom

    $w.f add $w.f.top -weight 1
    $w.f add $w.f.bottom -weight 0

    ttk::frame $w.f.bottom.left
    ttk::frame $w.f.bottom.right
    grid $w.f.bottom.left $w.f.bottom.right -sticky news

    # graph
    canvas $w.f.top.foffers.c -background white -width $width -height $height -relief solid
    grid $w.f.top.foffers.c
    bind $w.f.top.foffers <Configure> { ::fics::configureCanvas}

    ttk::scrollbar $w.f.top.fconsole.f1.ysc -command { .fics.f.top.fconsole.f1.console yview }
    text $w.f.top.fconsole.f1.console -bg $::fics::consolebg -fg $::fics::consolefg -height $::fics::consoleheight -width $::fics::consolewidth  \
         -font font_Fixed -wrap word -yscrollcommand "$w.f.top.fconsole.f1.ysc set"
    grid $w.f.top.fconsole.f1.console $w.f.top.fconsole.f1.ysc -sticky news
    grid rowconfigure $w.f.top.fconsole.f1 0 -weight 1
    grid columnconfigure $w.f.top.fconsole.f1 0 -weight 1

    #define colors for console
    $w.f.top.fconsole.f1.console tag configure seeking     -foreground $::fics::colseeking
    $w.f.top.fconsole.f1.console tag configure game        -foreground $::fics::colgame
    $w.f.top.fconsole.f1.console tag configure gameresult  -foreground $::fics::colgameresult
    $w.f.top.fconsole.f1.console tag configure ficspercent -foreground $::fics::colficspercent
    $w.f.top.fconsole.f1.console tag configure ficshelpnext -foreground $::fics::colficshelpnext -underline 1

    ttk::entry $w.f.top.fconsole.f2.cmd -width 32
    ttk::button $w.f.top.fconsole.f2.send -text [::tr "FICSSend"] -command ::fics::cmd
    bind $w.f.top.fconsole.f2.cmd <Return> { ::fics::cmd }
    bind $w.f.top.fconsole.f2.cmd <Up> { ::fics::cmdHistory up ; break }
    bind $w.f.top.fconsole.f2.cmd <Down> { ::fics::cmdHistory down ; break }
    bind $w.f.top.fconsole.f2.cmd <Left> " [bind TEntry <Left>] ; break "
    bind $w.f.top.fconsole.f2.cmd <Right> " [bind TEntry <Right>] ; break "
    grid $w.f.top.fconsole.f2.cmd $w.f.top.fconsole.f2.send -sticky news
    grid columnconfigure $w.f.top.fconsole.f2 0 -weight 1

    # clock 1 is white
    ::gameclock::new $w.f.bottom.left 1 100 0
    ::gameclock::new $w.f.bottom.left 2 100 0

    set row 0
    ttk::checkbutton $w.f.bottom.right.silence -image FICSsilence -variable ::fics::silence -onvalue 0 -offvalue 1 -command {
      ::fics::writechan "set gin $::fics::silence" "echo"
      ::fics::writechan "set seek $::fics::silence" "echo"
      ::fics::writechan "set silence $::fics::silence" "echo"
      ::fics::writechan "set chanoff [expr ! $::fics::silence ]" "echo"
    }
    ::utils::tooltip::Set $w.f.bottom.right.silence "[::tr FICSSilence]\n(set gin 0\nset seek 0\nset silence 0\nset chanoff 1)"
    set ::fics::silence 1

    set ::fics::graphon 0

    ttk::button $w.f.bottom.right.findopp -image FICSsearch  -command { ::fics::findOpponent }
    ::utils::tooltip::Set $w.f.bottom.right.findopp [::tr "FICSFindOpponent"]
    grid $w.f.bottom.right.findopp -column 0 -row $row -sticky ew -pady 2
    ttk::button $w.f.bottom.right.relay -image FICSrelayedgames -compound image -command { ::fics::writechan "tell relay listgames"}
    ::utils::tooltip::Set $w.f.bottom.right.relay "[::tr FICSRelayedGames]\n(tell relay listgames)"
    grid $w.f.bottom.right.relay -column 1 -row $row -sticky ew -pady 2
    ttk::button $w.f.bottom.right.games -image FICSusers -compound image -command { ::fics::writechan "games /bsu"}
    ::utils::tooltip::Set $w.f.bottom.right.games "[::tr FICSGames]\n(games /bsu)"
    grid $w.f.bottom.right.games -column 2 -row $row -sticky ew -pady 2
    ttk::button $w.f.bottom.right.uno -image FICSunobserve -compound image -command { ::fics::writechan "unobserve"}
    ::utils::tooltip::Set $w.f.bottom.right.uno "[::tr FICSUnobserve]\n(unobserve)"
    grid $w.f.bottom.right.uno -column 3 -row $row -sticky ew -pady 2
    ttk::button $w.f.bottom.right.profile -image FICSprofile -compound image -command { ::fics::writechan "finger" ; ::fics::writechan "history" }
    ::utils::tooltip::Set $w.f.bottom.right.profile "[::tr FICSProfile]\n(finger, history)"
    grid $w.f.bottom.right.profile -column 4 -row $row -sticky ew -pady 2

    incr row

    ttk::button $w.f.bottom.right.draw -image FICSdraw -command { ::fics::writechan "draw"}
    ::utils::tooltip::Set $w.f.bottom.right.draw "[::tr CCClaimDraw]\n(draw)"
    ttk::button $w.f.bottom.right.resign -image FICSresign -command { ::fics::writechan "resign"}
    ::utils::tooltip::Set $w.f.bottom.right.resign "[::tr CCResign]\n(resign)"
    grid $w.f.bottom.right.draw -column 0 -row $row -sticky ew -pady 2
    grid $w.f.bottom.right.resign -column 1 -row $row -sticky ew -pady 2
    ttk::button $w.f.bottom.right.abort -image FICSabort -command { ::fics::writechan "abort" }
    ::utils::tooltip::Set $w.f.bottom.right.abort "[::tr Abort]\n(abort)"
    grid $w.f.bottom.right.abort -column 2 -row $row -sticky ew -pady 2
    grid $w.f.bottom.right.silence -column 4 -row $row -sticky w
    incr row

    ttk::button $w.f.bottom.right.takeback -image FICStakeback1 -command { ::fics::writechan "takeback"}
    ::utils::tooltip::Set $w.f.bottom.right.takeback "[::tr FICSTakeback]\n(takeback)"
    ttk::button $w.f.bottom.right.takeback2 -image FICStakeback2 -command { ::fics::writechan "takeback 2"}
    ::utils::tooltip::Set $w.f.bottom.right.takeback2 "[::tr FICSTakeback2]\n(takeback 2)"

    grid $w.f.bottom.right.takeback -column 0 -row $row -sticky ew -pady 2
    grid $w.f.bottom.right.takeback2 -column 1 -row $row -sticky ew -pady 2
    incr row

    ttk::button $w.f.bottom.right.cancel -image FICSexit -command { ::fics::close }
    ::utils::tooltip::Set $w.f.bottom.right.cancel [::tr "Close"]
    grid $w.f.bottom.right.cancel -column 0 -columnspan 3 -row $row -sticky ew  -pady 2

    bind $w.f.top <<NotebookTabChanged>> { ::fics::tabchanged ; break }
    bind $w <Destroy> { catch ::fics::close }

    bind $w <F1> { helpWindow FICS}
    bind $w.f.top.fconsole.f1.console <FocusIn> "focus $w.f.top.fconsole.f2.cmd"
    bind $w.f.top.fconsole.f1.console <Configure> { .fics.f.top.fconsole.f1.console yview moveto 1 }
    bind $w.f.top.fconsole.f1.console <ButtonPress-1> { ::fics::consoleClick %x %y %W }
    ::createToplevelFinalize $w


    # all widgets must be visible
    update
    set x [winfo reqwidth $w]
    set y [winfo reqheight $w]
    wm minsize $w $x $y

    ::gameclock::setColor 1 white
    ::gameclock::setColor 2 black

    updateConsole "Connecting $login"

    # start timeseal proxy
    if {$::fics::use_timeseal} {
      updateConsole "Starting TimeSeal"
      if { [catch { set timeseal_pid [exec $::fics::timeseal_exec $::fics::server_ip $::fics::port_fics -p $::fics::port_timeseal &]} ] } {
        set ::fics::use_timeseal 0
        set port $::fics::port_fics
      } else {
        #wait for proxy to be ready !?
        after 500
        set server "localhost"
        set port $::fics::port_timeseal
      }
    } else {
      set server $::fics::server
      set port $::fics::port_fics
    }

    updateConsole "Socket opening"

    if { [catch { set sockchan [socket $server $port] } ] } {
      tk_messageBox -title "Error" -icon error -type ok -message "Network error\nCan't connect to $server $port" -parent .fics
      return
    }

    updateConsole "Channel configuration"

    fconfigure $sockchan -blocking 0 -buffering line -translation auto ;#-encoding iso8859-1 -translation crlf
    fileevent $sockchan readable ::fics::readchan
    setState disabled
  }
  ################################################################################
  #
  ################################################################################
  proc cmd {} {
    set l [.fics.f.top.fconsole.f2.cmd get]
    .fics.f.top.fconsole.f2.cmd delete 0 end
    if {$l == "quit"} {
      ::fics::close
      return
    }
    # do nothing if the command is void
    if {[string trim $l] == ""} { return }
    writechan $l "echo"
    lappend ::fics::history $l
    set ::fics::history_pos [llength $::fics::history]
  }
  ################################################################################
  #
  ################################################################################
  proc cmdHistory { action } {
    set t .fics.f.top.fconsole.f2.cmd

    if {$action == "up" && $::fics::history_pos > 0} {
      incr ::fics::history_pos -1
      $t delete 0 end
      $t insert end [lindex $::fics::history $::fics::history_pos]
    }
    if {$action == "down" && $::fics::history_pos < [expr [llength $::fics::history] -1] } {
      incr ::fics::history_pos
      $t delete 0 end
      $t insert end [lindex $::fics::history $::fics::history_pos]
    }
  }
  ################################################################################
  #
  ################################################################################
  proc findOpponent {} {
    set w .ficsfindopp
    if {[winfo exists $w]} {
      focus $w
      return
    }
    win::createDialog $w
    wm title $w [::tr "FICSFindOpponent"]

    ttk::frame $w.f
    pack $w.f -side top -anchor w -fill x
    ttk::label $w.f.linit -text [::tr "FICSInitialTime"]
    ttk::spinbox $w.f.sbTime1 -background white -width 3 -textvariable ::fics::findopponent(initTime) -from 0 -to 120 -increment 1 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    ttk::label $w.f.linc -text [::tr "FICSIncrement"]
    ttk::spinbox $w.f.sbTime2 -background white -width 3 -textvariable ::fics::findopponent(incTime) -from 0 -to 120 -increment 1 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    grid $w.f.linit -column 0 -row 0 -sticky w
    grid $w.f.sbTime1 -column 1 -row 0 -sticky w -pady "0 2"
    grid $w.f.linc -column 0 -row 1 -sticky w
    grid $w.f.sbTime2 -column 1 -row 1 -sticky w

    ttk::checkbutton $w.f.cbrated -text [::tr "FICSRatedGame"] -onvalue "rated" -offvalue "unrated" -variable ::fics::findopponent(rated)
    grid $w.f.cbrated -column 0 -row 2 -columnspan 2 -sticky ew

    ttk::labelframe $w.f.color -text [::tr "FICSColour"]
    grid $w.f.color -column 0 -row 3 -columnspan 2 -sticky ew
    ttk::radiobutton $w.f.rb1 -text [::tr "FICSAutoColour"] -value "" -variable ::fics::findopponent(color)
    ttk::radiobutton $w.f.rb2 -text [::tr "White"] -value "white" -variable ::fics::findopponent(color)
    ttk::radiobutton $w.f.rb3 -text [::tr "Black"] -value "black" -variable ::fics::findopponent(color)
    pack $w.f.rb1 $w.f.rb2 $w.f.rb3 -side top -anchor w -in $w.f.color

    ttk::checkbutton $w.f.cblimitrating -text [::tr "RatingRange"] -variable ::fics::findopponent(limitrating)
    ttk::spinbox $w.f.sbrating1 -background white -width 4 -textvariable ::fics::findopponent(rating1) -from 1000 -to 3000 -increment 50 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    ttk::spinbox $w.f.sbrating2 -background white -width 4 -textvariable ::fics::findopponent(rating2) -from 1000 -to 3000 -increment 50 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    grid $w.f.cblimitrating -column 0 -row 5 -columnspan 2 -sticky ew
    grid $w.f.sbrating1 -column 0 -row 6 -sticky w
    grid $w.f.sbrating2 -column 1 -row 6 -sticky w

    ttk::checkbutton $w.f.cbmanual -text [::tr "FICSManualConfirm"] -onvalue "manual" -offvalue "auto" -variable ::fics::findopponent(manual)
    grid $w.f.cbmanual -column 0 -row 7 -columnspan 2 -sticky ew
    ttk::checkbutton $w.f.cbformula -text [::tr "FICSFilterFormula"] -onvalue "formula" -offvalue "" -variable ::fics::findopponent(formula)
    grid $w.f.cbformula -column 0 -row 8 -columnspan 2 -sticky ew

    ttk::button $w.seek -text [::tr "FICSIssueSeek"] -command {
      ::fics::syncProfileVars $::fics::login

      set range ""
      if {$::fics::findopponent(limitrating) } {
        set range "$::fics::findopponent(rating1)-$::fics::findopponent(rating2)"
      }
      set cmd "seek $::fics::findopponent(initTime) $::fics::findopponent(incTime) $::fics::findopponent(rated) \
          $::fics::findopponent(color) $::fics::findopponent(manual) $::fics::findopponent(formula) $range"
      ::fics::writechan $cmd
      destroy .ficsfindopp
    }
    ttk::button $w.cancel -text [::tr "Cancel"] -command "destroy $w"
    bind $w <F1> { helpWindow FICSfindOpp}

    packdlgbuttons $w.cancel $w.seek
  }
  ################################################################################
  #
  ################################################################################
  proc readchan {} {
    variable logged

    if {[eof $::fics::sockchan]} {
      fileevent $::fics::sockchan readable {}
      tk_messageBox -title "FICS" -icon error -type ok -message "Network error reading channel"
      ::fics::close "error"
      return
    }

    # switch from read to gets in case a read is done at the middle of a line
    if {! $logged} {
      set line [read $::fics::sockchan]
      foreach l [split $line "\n"] {
        readparse $l
      }
    } else  {
      set line [gets $::fics::sockchan]
      set line [string map {"\a" ""} $line]
      readparse $line
    }

    ::fics::makePremove
  }

  ################################################################################
  # Appends an array to soughtlist if the parameter is correct
  # returns 0 if the line is not parsed and so it is still pending for use
  ################################################################################
  proc parseSoughtLine { l } {
    global ::fics::offers_minelo ::fics::offers_maxelo ::fics::offers_mintime ::fics::offers_maxtime

    # it seems that the first offer starts with a prompt
    if {[string match "fics% *" $l]} {
      set l [string range $l 6 end]
    }

    if { [ catch { if {[llength $l] < 8} { return 0} } ] } { return 0}
    array set ga {}

    set offset 0
    set ga(game) [lindex $l 0]
    if { ! [string is integer $ga(game)] } { return 0}
    set tmp [lindex $l 1]
    if { [scan $tmp "%d" ga(elo)] != 1} { set ga(elo) $offers_minelo }
    if { $ga(elo) < $offers_minelo } { set ga(elo) $offers_minelo }
    set ga(name) [lindex $l 2]

    set tmp [lindex $l 3]
    if { [scan $tmp "%d" ga(time_init)] != 1} { set ga(time_init) $offers_maxtime}
    set tmp [lindex $l 4]
    if { [scan $tmp "%d" ga(time_inc)] != 1} { set ga(time_inc) 0 }

    set ga(rated) [lindex $l 5]
    if {$ga(rated) != "rated" && $ga(rated) != "unrated"} { return 0 }

    set ga(type) [lindex $l 6]
    if { $ga(type) != "untimed" && $ga(type) != "blitz" && $ga(type) != "standard" && $ga(type) != "lightning" } {
      return 0
    }
    set ga(color) ""
    if { [lindex $l 7] == "\[white\]" || [lindex $l 7] == "\[black\]" } {
      set ga(color) [lindex $l 7]
      set offset 1
    }
    set ga(rating_range) [lindex $l [expr 7 + $offset]]
    if { [ catch { set ga(start) [lindex $l [expr 8 + $offset]] } ] } {
      set ga(start) ""
    }

    lappend ::fics::soughtlist [array get ga]
    return 1
  }
  ################################################################################
  #
  ################################################################################
  proc readparse {line} {
    variable logged
    variable isGuestLogin

    if {$line == "" || $line == "fics% "} {return}

    if { $::fics::sought } {
      if {[string match "* ad* displayed." $line]} {
        set ::fics::sought 0
        catch { displayOffers }
        return
      }
      # lappend ::fics::soughtlist $line
      if { [ parseSoughtLine $line ] } {
        return
      }
    }

    if {[string match "login: " $line]} {
      writechan $::fics::reallogin
      if { $isGuestLogin} {
        set logged 1
      }
      return
    }
    if {[string match "password: " $line]} {
      writechan $::fics::password
      set logged 1
      return
    }
    if {[string match "<sc>*" $line]} {
      set ::fics::seeklist {}
      return
    }
    if {[string match "<s>*" $line]} {
      parseSeek $line
      return
    }
    if {[string match "<sr>*" $line]} {
      removeSeek $line
      return
    }

    if {[string match "<12>*" $line]} {
      parseStyle12 $line
      return
    }

    # puts "readparse->$line"
    updateConsole $line
    if {[string match "Creating: *" $line]} {
      # hide offers graph
      .fics.f.top select 0
      ::utils::sound::PlaySound sound_move
      # Create a game in an opened base
      if {![sc_base inUse]} {
        sc_base switch $::clipbase_db
      }
      sc_game new
      set idx1 [string first "(" $line]
      set white [string trim [string range $line 10 [expr $idx1 -1]] ]
      set idx2 [string first ")" $line]
      set whiteElo [string trim [string range $line [expr $idx1 +1] [expr $idx2 -1]] ]

      set idx1 [expr $idx2 +1]
      set idx2 [string first "(" $line $idx1]
      set black [string trim [string range $line $idx1 [expr $idx2 -1]] ]

      set idx1 [expr $idx2 +1]
      set idx2 [string first ")" $line $idx1]
      set blackElo [string trim [string range $line $idx1 [expr $idx2 -1]] ]

      if { $whiteElo == "++++"} { set whiteElo 0 }
      if { $blackElo == "++++"} { set blackElo 0 }

      sc_game tags set -white $white
      sc_game tags set -whiteElo $whiteElo
      sc_game tags set -black $black
      sc_game tags set -blackElo $blackElo
      sc_game tags set -date "[::utils::date::today year].[::utils::date::today month].[::utils::date::today day]"
      sc_game tags set -site "FICS freechess.org"
      sc_game tags set -event "FICS played [lrange $line 5 6] game"
      sc_game tags set -extra [list "Timecontrol \"[lindex $line 7]+[lindex $line 8]\""]

      if { [::board::isFlipped .main.board] } {
        if { [ string match -nocase $white $::fics::reallogin ] } { ::board::flip .main.board }
      } else {
        if { [ string match -nocase $black $::fics::reallogin ] } { ::board::flip .main.board }
      }
      ::notify::GameChanged
      set ::fics::rated [string equal [lindex $line 5] "rated"]
      # display the win / draw / loss score
      if { $::fics::rated } { ::fics::writechan "assess" "noecho" }
      # it's a new game so show again abort, draw, etc requests
      set ::fics::showabortreq 1
      set ::fics::showadjournreq 1
      set ::fics::showdrawreq 1
      set ::fics::showtakebackreq 1
      return
    }

    if {[string match "\{Game *" $line]} {
      set num [lindex [lindex $line 0] 1]
      set res [lindex $line end]
      set comment [lrange [lindex $line 0] 2 end]
      set n [string first {)} $comment]
	if {$n > -1} {
	  set comment [string range $comment $n+2 end]
	}
      sc_pos setComment "[sc_pos getComment]$comment"
      if {$num == $::fics::observedGame} {
        if {[string match "1/2*" $res]} {
          tk_messageBox -title [::tr "Result"] -icon info -type ok -message "Draw\n$comment"
        } else {
          tk_messageBox -title [::tr "Result"] -icon info -type ok -message "$res\n$comment"
        }
        sc_game tags set -result $res
        set ::fics::playing 0
        set ::fics::observedGame -1
        ::gameclock::stop 1
        ::gameclock::stop 2
        updateBoard -pgn
      }
      return
    }

    if { [string match "You are now observing game*" $line] } {
      scan $line "You are now observing game %d." ::fics::observedGame
    }

    # Start session
    if {[string match "*Starting FICS session*" $line]} {

      # mandatory init commands
      writechan "set interface Scid/$::scidVersion ([tk windowingsystem]; $::tcl_platform(os) $::tcl_platform(machine); rv:$::scidVersionDate) Tcl/Tk [info patchlevel]"
      writechan "iset seekremove 1"
      writechan "iset seekinfo 1"
      writechan "style 12"
      writechan "iset nowrap 1"
      writechan "iset nohighlight 1"

      # user init commands
      if { $::fics::usedefaultvars } {
        writechan "set seek 1" ; # be informed of "seek" ads when they are made
        writechan "set silence 1" ; #  turn off shouts, cshouts and channel tells while you play
        writechan "set chanoff 0" ; # stop hearing tells to channels
        writechan "set echo 1" ; # shouts and most other communications will be echoed to you
        writechan "set cshout 0" ; # do not hear cshouts
      }
      setState normal
      return
    }

    if { $::fics::waitForRating == "wait" } {
      if {[catch {set val [lindex $line 0]}]} {
        return
      } else  {
        if {[lindex $line 0] == "Standard"} {
          set ::fics::waitForRating [lindex $line 1]
          return
        }
      }
    }

    if { $::fics::waitForMoves != "" } {
      set m1 ""
      set m2 ""
      set t2 ""
      set t4 ""
      set line [string trim $line]

      # Because some free text may be in the form (".)
      if {[catch {llength $line} err]} {
        puts "Exception $err llength $line"
        return
      }

      if {[llength $line ] == 5 && [scan $line "%d. %s (%d:%d) %s (%d:%d)" t1 m1 t2 t3 m2 t4 t5] != 7} {
        return
      }
      if {[llength $line ] == 3 && [scan $line "%d. %s (%d:%d)" t1 m1 t2 t3] != 4} {
        return
      }
      catch { sc_move addSan $m1 }
      if {$t2 != ""} {
	  storeEmtComment 0 $t2 $t3
      }
      if {$m2 != ""} {
        catch { sc_move addSan $m2 }
      }
      if {$t4 != ""} {
	  storeEmtComment 0 $t4 $t5
      }

      if {[sc_pos fen] == $::fics::waitForMoves } {
        set ::fics::waitForMoves ""
      }
    }

    if {[string match "Challenge:*" $line]} {
      set ans [tk_dialog .challenge [::tr "FICSChallenge"] $line "" 0 [::tr "FICSAccept"] [::tr "FICSDecline"]]
      if {$ans == 0} {
        writechan "accept"
      } else {
        writechan "decline"
      }
    }

    # abort request
    # for the abort, etc requests, added the "cancel" option so that during this game
    # the message box won't open again for the canceled type of request
    # to avoid "denial of play" attack by the opponent constantly issuing such a request
    # (because  tk_messageBox  "waits for the user to select one of the buttons")
    if {[string match "* would like to abort the game;*" $line] && $::fics::showabortreq} {
      set ans [tk_messageBox -title [::tr "Abort"] -icon question -type yesnocancel -message "$line\nDo you accept ?" ]
      switch -- $ans {
        yes {writechan "accept"}
        no  {writechan "decline"}
        cancel {set ::fics::showabortreq 0}
      }
    }

    # takeback
    if {[string match "* would like to take back *" $line] && $::fics::showtakebackreq} {
      set ans [tk_messageBox -title "Abort" -icon question -type yesnocancel -message "$line\nDo you accept ?" ]
      switch -- $ans {
        yes {writechan "accept"}
        no  {writechan "decline"}
        cancel {set ::fics::showtakebackreq 0}
      }
    }

    # draw
    if {[string match "*offers you a draw*" $line] && $::fics::showdrawreq} {
      set ans [tk_messageBox -title "Abort" -icon question -type yesnocancel -message "$line\nDo you accept ?" ]
      switch -- $ans {
        yes {writechan "accept"}
        no  {writechan "decline"}
        cancel {set ::fics::showdrawreq 0}
      }
    }

    # adjourn
    if {[string match "*would like to adjourn the game*" $line] && $::fics::showadjournreq} {
      set ans [tk_messageBox -title "Abort" -icon question -type yesnocancel -message "$line\nDo you accept ?" ]
      switch -- $ans {
        yes {writechan "accept"}
        no  {writechan "decline"}
        cancel {set ::fics::showadjournreq 0}
      }
    }

    # guest logging
    if {[string match "Logging you in as*" $line]} {
      set line [string map {"\"" "" ";" ""} $line ]
      set ::fics::reallogin [lindex $line 4]
      ::setTitle .fics "Free Internet Chess Server $::fics::reallogin"
    }
    if {[string match "Press return to enter the server as*" $line]} {
      writechan "\n"
    }

  }
  ################################################################################
  #  Set the state of user interface related to connection state
  ################################################################################
  proc setState { state } {
    set w .fics

    foreach elt [winfo children $w.f.bottom.right] {
      if { $elt != "$w.f.bottom.right.cancel" } {
        $elt configure -state $state
      }
    }

    foreach elt [list $w.f.top.fconsole.f2.send $w.f.top.fconsole.f2.cmd ] {
      $elt configure -state $state
    }

    if {$state == "normal" } {
      $w.f.top add $w.f.top.foffers
    } else  {
      $w.f.top hide $w.f.top.foffers
    }
  }
  ################################################################################
  #
  ################################################################################
  proc updateConsole {line} {
    set t .fics.f.top.fconsole.f1.console

    if { [string match "* seeking *" $line ] } {
      $t insert end "$line\n" seeking
    } elseif { [string match "\{Game *\}" $line ] } {
      $t insert end "$line\n" game
    } elseif { [string match "\{Game *\} *" $line ] } {
      $t insert end "$line\n" gameresult
    } elseif { [string match "fics% *" $line ] } {
      $t insert end "$line\n" ficspercent
    } elseif  { $line == "Type \[next\] to see next page."  } {
      $t insert end "Click or type \[next\] to see next page.\n" ficshelpnext
    } else  {
      $t insert end "$line\n"
    }

    set pos [ lindex [ .fics.f.top.fconsole.f1.ysc get ] 1 ]
    if {$pos == 1.0} {
      $t yview moveto 1
    }

  }
  ################################################################################
  #
  ################################################################################
  proc removeSeek {line} {
    global ::fics::seeklist
    foreach l $line {

      if { $l == "<sr>" } {continue}

      # remove seek from seeklist
      for {set i 0} {$i < [llength $seeklist]} {incr i} {
        array set a [lindex $seeklist $i]
        if {$a(index) == $l} {
          set seeklist [lreplace $seeklist $i $i]
          break
        }
      }

      # remove seek from graph
      if { $::fics::graphon } {
        for {set idx 0} { $idx < [llength $::fics::soughtlist]} { incr idx } {
          array set g [lindex $::fics::soughtlist $idx]
          set num $g(game)
          if { $num == $l } {
            .fics.f.top.foffers.c delete game_$idx
            break
          }
        }
      }

    }
  }
  ################################################################################
  #
  ################################################################################
  proc parseStyle12 {line} {
    set color [lindex $line 9]
    set gameNumber [lindex $line 16]
    set white [lindex $line 17]
    set black [lindex $line 18]
    set relation [lindex $line 19]
    set initialTime [lindex $line 20]
    set increment [lindex $line 21]
    set whiteMaterial [lindex $line 22]
    set blackMaterial [lindex $line 23]
    set whiteRemainingTime  [lindex $line 24]
    set blackRemainingTime  [lindex $line 25]
    set moveNumber [lindex $line 26]
    set verbose_move [lindex $line 27]
    set moveTime [lindex $line 28]
    set moveSan [lindex $line 29]

    set ::fics::playing $relation
    set ::fics::observedGame $gameNumber

    ::gameclock::setSec 1 [ expr 0 - $whiteRemainingTime ]
    ::gameclock::setSec 2 [ expr 0 - $blackRemainingTime ]
    if {$color == "W"} {
      ::gameclock::start 1
      ::gameclock::stop 2
    } else {
      ::gameclock::start 2
      ::gameclock::stop 1
    }

    set fen ""
    for {set i 1} {$i <=8} { incr i} {
      set l [lindex $line $i]
      set count 0

      for { set col 0 } { $col < 8 } { incr col } {
        set c [string index $l $col]
        if { $c == "-"} {
          incr count
        } else {
          if {$count != 0} {
            set fen "$fen$count"
            set count 0
          }
          set fen "$fen$c"
        }
      }

      if {$count != 0} { set fen "$fen$count" }
      if {$i != 8} { set fen "$fen/" }
    }

    set fen "$fen [string tolower $color]"
    set f [lindex $line 10]

    # en passant
    if { $f == "-1" || $verbose_move == "none"} {
      set enpassant "-"
    } else {
      set enpassant "-"
      set conv "abcdefgh"
      set fl [string index $conv $f]
      if {$color == "W"} {
        if { [ string index [lindex $line 4] [expr $f - 1]] == "P" || [ string index [lindex $line 4] [expr $f + 1]] == "P" } {
          set enpassant "${fl}6"
        }
      } else {
        if { [ string index [lindex $line 5] [expr $f - 1]] == "p" || [ string index [lindex $line 5] [expr $f + 1]] == "p" } {
          set enpassant "${fl}3"
        }
      }
    }

    set castle ""
    if {[lindex $line 11] == "1"} {set castle "${castle}K"}
    if {[lindex $line 12] == "1"} {set castle "${castle}Q"}
    if {[lindex $line 13] == "1"} {set castle "${castle}k"}
    if {[lindex $line 14] == "1"} {set castle "${castle}q"}
    if {$castle == ""} {set castle "-"}

    set fen "$fen $castle $enpassant [lindex $line 15] $moveNumber"

    # try to play the move and check if fen corresponds. If not this means the position needs to be set up.
    if {$moveSan != "none" && $::fics::playing != -1} {
      # first check side's coherency
      if { ([sc_pos side] == "white" && $color == "B") || ([sc_pos side] == "black" && $color == "W") } {
        # puts "sc_move addSan $moveSan"
        ::utils::sound::PlaySound sound_move
        ::utils::sound::AnnounceNewMove $moveSan
        if { [catch { sc_move addSan $moveSan } err ] } {
          puts "error $err"
        } else {
	    if {  $::fics::playing == 1 } {
		::fics::storeTime
	    } else {
		set t1 ""; set t2 ""
		if { [scan $moveTime "(%d:%d)" t1 t2] == 2} {
		    storeEmtComment 0 $t1 $t2
		}
	    }
          if { $::novag::connected } {
            set m $verbose_move
            if { [string index $m 1] == "/" } { set m [string range $m 2 end] }
            set m [string map { "-" "" "=" "" } $m]
            ::novag::addMove $m
          }
          updateBoard -pgn -animate
        }
      }
    }

    if {$fen != [sc_pos fen]} {
      # Create a game in an opened base
      if {![sc_base inUse]} {
        sc_base switch $::clipbase_db
      }
      sc_game new

      set ::fics::waitForRating "wait"
      writechan "finger $white /s"
      vwaitTimed ::fics::waitForRating 2000 "nowarn"
      if {$::fics::waitForRating == "wait"} { set ::fics::waitForRating "0" }
      sc_game tags set -white $white
      sc_game tags set -whiteElo $::fics::waitForRating

      set ::fics::waitForRating "wait"
      writechan "finger $black /s"
      vwaitTimed ::fics::waitForRating 2000 "nowarn"
      if {$::fics::waitForRating == "wait"} { set ::fics::waitForRating "0" }
      sc_game tags set -black $black
      sc_game tags set -blackElo $::fics::waitForRating

      set ::fics::waitForRating ""

      sc_game tags set -site "FICS freechess.org"
      sc_game tags set -event "FICS observed game"
      sc_game tags set -extra [list "Timecontrol \"$initialTime+$increment\""]
      sc_game tags set -date "[::utils::date::today year].[::utils::date::today month].[::utils::date::today day]"

      # try to get first moves of game
      writechan "moves $gameNumber"
      set ::fics::waitForMoves $fen
      vwaitTimed ::fics::waitForMoves 2000 "nowarn"
      set ::fics::waitForMoves ""

      # Did not manage to reconstruct the game, just set its position
      if {$fen != [sc_pos fen]} {
        sc_game startBoard $fen
      }
      ::notify::GameChanged
    }
  }
  ################################################################################
  #
  ################################################################################
  proc parseSeek {line} {
    array set seekelt {}
    set seekelt(index) [lindex $line 1]
    foreach m [split $line] {
      if {[string match "w=*" $m]} { set seekelt(name_from) [string range $m 2 end] ; continue }
      if {[string match "ti=*" $m]} { set seekelt(titles) [string range $m 3 end] ; continue }
      if {[string match "rt=*" $m]} { set seekelt(rating) [string range $m 3 end] ; continue }
      if {[string match "t=*" $m]} { set seekelt(time) [string range $m 2 end] ; continue }
      if {[string match "i=*" $m]} { set seekelt(increment) [string range $m 2 end] ; continue }
      if {[string match "r=*" $m]} { set seekelt(rated) [string range $m 2 end] ; continue }
      if {[string match "tp=*" $m]} { set seekelt(type) [string range $m 3 end] ; continue }
      if {[string match "c=*" $m]} { set seekelt(color) [string range $m 2 end] ; continue }
      if {[string match "rr=*" $m]} { set seekelt(rating_range) [string range $m 3 end] ; continue }
      if {[string match "a=*" $m]} { set seekelt(automatic) [string range $m 2 end] ; continue }
      if {[string match "f=*" $m]} { set seekelt(formula_checked) [string range $m 2 end] ; continue }
    }
    lappend ::fics::seeklist [array get seekelt]
  }
  ################################################################################
  #
  ################################################################################
  proc updateOffers { } {
    set ::fics::sought 1
    set ::fics::soughtlist {}
    writechan "sought"
    vwaitTimed ::fics::sought 5000 "nowarn"
    after 3000 ::fics::updateOffers
  }
  ################################################################################
  #
  ################################################################################
  proc configureCanvas {} {
    set w .fics.f.top.foffers
    set ::fics::height [winfo height $w]
    set ::fics::width [winfo width $w]
    $w.c configure -width $::fics::width -height $::fics::height
    displayOffers
  }
  ################################################################################
  #
  ################################################################################
  proc displayOffers { } {
    global ::fics::width ::fics::height ::fics::off \
        ::fics::offers_minelo ::fics::offers_maxelo ::fics::offers_mintime ::fics::offers_maxtime
    after cancel ::fics::updateOffers

    set w .fics.f.top.foffers
    set size 5
    set idx 0

    #first erase the canvas
    foreach id [ $w.c find all] { $w.c delete $id }

    # Draw horizontal lines
    set y_unit [expr $height / 32.0]
    for {set i 0} {$i < 32} {incr i} {
      set y [expr $height - $i * $y_unit]
      $w.c create line 0 $y $width $y -fill "light gray"
    }

    # Draw horizontal tics and labels
    set x1_tick [expr $width - $off]
    set x_text [expr $width - 2]
    foreach elo [list 5 10 15 20 25 30] {
      set y [expr $height - $elo * $y_unit]
      $w.c create line $x1_tick $y $width $y -fill black
      $w.c create text $x_text $y -fill black -anchor se -text [expr $elo * 100]
    }

    # Draw vertical lines, tics and labels
    set x_unit [expr ($width - 3 * $off) / 60.0]
    set y2_tick [expr $height - $off]
    foreach t [list 2 5 10 15 30 60] {
      set x [expr $t * $x_unit + $off]
      $w.c create line $x $height $x 0 -fill "light gray"
      $w.c create line $x $height $x $y2_tick -fill black
      $w.c create text [expr $x + 2] $height -fill black -anchor sw -text "${t}m"
    }

    foreach g $::fics::soughtlist {
      array set l $g
      set fillcolor green
      # if the time is too large, put it in red
      set tt [expr $l(time_init) + $l(time_inc) * 2 / 3 ]
      if { $tt > $offers_maxtime } {
        set tt $offers_maxtime
        set fillcolor red
      }
      # if a computer, put it in blue
      if { [string match "*(C)" $l(name)] } {
        set fillcolor blue
      }
      # if player without ELO, in gray
      if { [string match "Guest*" $l(name)] } {
        set fillcolor gray
      }

      set x [expr $tt * $x_unit + $off]
      set y [expr $height - ($l(elo) / 100.0) * $y_unit]

      if { $l(rated) == "rated" } {
        set object "oval"
      } else {
        set object "rectangle"
      }
      $w.c create $object [expr $x - $size ] [expr $y - $size ] [expr $x + $size ] [expr $y + $size ] -tag game_$idx -fill $fillcolor

      $w.c bind game_$idx <Enter> "::fics::setOfferStatus $idx %x %y"
      $w.c bind game_$idx <Leave> "::fics::setOfferStatus -1 %x %y"
      $w.c bind game_$idx <ButtonPress> "::fics::getOffersGame $idx"
      incr idx
    }

  }
  ################################################################################
  # Play the selected game
  ################################################################################
  proc getOffersGame { idx } {
    array set ga [lindex $::fics::soughtlist $idx]
    catch { writechan "play $ga(game)" }
  }
  ################################################################################
  #
  ################################################################################
  proc setOfferStatus { idx x y } {
    global ::fics::height ::fics::width ::fics::off

    set w .fics.f.top.foffers
    if { $idx != -1 } {
      set gl [lindex $::fics::soughtlist $idx]
      if { $gl == "" } { return }
      array set l [lindex $::fics::soughtlist $idx]
      set m "$l(game) $l(name)($l(elo))\n$l(time_init)/$l(time_inc) $l(rated)\n$l(color) $l(start)"

      if {$y < [expr $height / 2]} {
        set anchor "n"
      } else {
        set anchor "s"
      }

      if {$x < [expr $width / 2]} {
        append anchor "w"
      } else {
        append anchor "e"
      }

      $w.c create text [expr $x + $off] $y -tags status -text $m -font font_offers -anchor $anchor
      $w.c raise game_$idx
    } else {
      $w.c delete status
    }
  }
  ################################################################################
  #
  ################################################################################
  proc play {index} {
    writechan "play $index"
    # set ::fics::playing 1
    set ::fics::observedGame $index
  }
  ################################################################################
  #
  ################################################################################
  proc writechan {line {echo "noecho"}} {
    after cancel ::fics::stayConnected
    if {[eof $::fics::sockchan]} {
      tk_messageBox -title "FICS" -icon error -type ok -message "Network error writing channel"
      ::fics::close "error"
      return
    }
    puts $::fics::sockchan $line
    if {$echo != "noecho"} {
      updateConsole "->>$line"
    }
    after 2700000 ::fics::stayConnected
  }
  ################################################################################
  # FICS seems to close connexion after 1 hr idle. So send a dummy command
  # every 45 minutes
  ################################################################################
  proc stayConnected {} {
    catch {
      writechan "date" "noecho"
      after 2700000 ::fics::stayConnected
    }
  }
  ################################################################################
  #  returns 1 if premove is set
  ################################################################################
  proc setPremove {sq1 sq2} {
      if { $::fics::premoveEnabled && $::fics::playing == -1 && $sq2 != -1 } {
          set ::fics::premoveSq1 $sq1
          set ::fics::premoveSq2 $sq2
          ::board::mark::DrawArrow .main.board.bd $sq2 $sq1 $::highlightLastMoveColor
          return 1
      }
      return 0
  }
  ################################################################################
  #  execute FICS premove if possible
  ################################################################################
  proc makePremove {} {
    if { $::fics::premoveEnabled && $::fics::playing == 1 && $::fics::premoveSq1 != -1 } {
      addMove $::fics::premoveSq1 $::fics::premoveSq2
      set ::fics::premoveSq1 -1
    }
  }
  ################################################################################
  #   returns 1 if the player is allowed to enter a move (either playing or using puzzlebot)
  ################################################################################
  proc playerCanMove {} {

    if { ! [winfo exists .fics] } { return 1 }

    if { [sc_game info white] == "puzzlebot" || [sc_game info black] == "puzzlebot" } {
      return 1
    }

    if { $::fics::playing == 1 } { return 1 }

    if { $::fics::premoveEnabled && $::fics::playing == -1 } { 
        .main.board.bd delete mark
        set ::fics::premoveSq1 -1
        return 1 
    }
    return 0
  }
  ################################################################################
  # Handle mouse button 1 on console : observe the selected game
  # or handle commands (like <next>)
  ################################################################################
  proc consoleClick { x y win } {
    set idx [ $win index @$x,$y ]
    if { [ scan $idx "%d.%d" l c ] != 2 } {
      # should never happen
      return
    }
    set elt [$win get $l.0 $l.end]

    if { $elt ==  "Click or type \[next\] to see next page." } {
      writechan "next"
      return
    }

    regsub -all {\s+} $elt " " elt
    set elt [split $elt " "]
    set found 0

    if { [llength $elt] > 4} {
      # validate format
      set game [lindex $elt 0]
      set elow [lindex $elt 1]
      set white [lindex $elt 2]
      set elob [lindex $elt 3]
      set black [lindex $elt 4]

      if { [ scan $game "%d" tmp ] != 1 || \
            ( [ scan $elow "%d" tmp ] != 1 && $elow != "++++" ) || \
            ( [ scan $elob "%d" tmp ] != 1 && $elob != "++++" ) } {
      } else  {
        set found 1
      }
    }

    # Second chance : try to parse "tell relay listgames" (:104 GMxxxx GMyyyyy * B22)
    if { [llength $elt] == 5 && ! $found } {
      if { [ scan [lindex $elt 0] ":%d" game ] == 1 } {
        set white [lindex $elt 1]
        set black [lindex $elt 2]
        set elow "-"
        set elob "-"
        set found 1
      }
    }

    if { ! $found } {
      puts "$elt not a valid game"
      return
    }

    # warn the user before observing a game because it can interfere with a game played or
    # other that would be disturbed by observing a game
    set ans [tk_messageBox -title "Observe game" -icon question -type yesno \
        -message "[ ::tr FICSObserveconfirm ] $game\n$white ($elow) - $black ($elob)  ?" ]
    if { $ans == yes } {
      writechan "unobserve" "echo"
      writechan "observe $game" "echo"
    }

  }
  ################################################################################
  # updates the offers view if it is visible
  ################################################################################
  proc tabchanged {} {
    set nb .fics.f.top
    set w .fics.f.top.foffers

    if { [ $nb select ] == $w } {
      updateOffers
      set ::fics::graphon 1
    } else  {
      after cancel ::fics::updateOffers
      set ::fics::graphon 0
    }
  }
  ################################################################################
  #
  ################################################################################
  proc close { {mode ""} } {
    variable logged
    # stop recursive call
    bind .fics <Destroy> {}

    set ::fics::sought 0
    after cancel ::fics::updateOffers
    after cancel ::fics::stayConnected
    set logged 0

    if {$mode != "error"} {
      writechan "exit"
    }

    set ::fics::playing 0
    set ::fics::observedGame -1
    ::close $::fics::sockchan
    if { ! $::windowsOS } { catch { exec -- kill -s INT [ $::fics::timeseal_pid ] }  }
    ::win::closeWindow .fics
  }
}
###
### End of file: fics.tcl
###
