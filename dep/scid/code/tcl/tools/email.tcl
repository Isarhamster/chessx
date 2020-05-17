###
### tools/email.tcl: part of Scid.
### Copyright (C) 1999-2003  Shane Hudson.
###

# Email manager window: closed by default
set emailWin 0


# ::tools::email
#
#   Opens the email chess manager window, for sending moves to opponents.
#
proc ::tools::email {} {
  global emailWin emailData
  set w .emailWin
  if {[winfo exists $w]} {
    destroy .emailWin
    set emailWin 0
    return
  }
  set emailWin 1
  win::createDialog $w
  wm title $w "Scid: Email Manager"
  wm minsize $w 25 10

  bind $w <Destroy> { set .emailWin 0 }
  bind $w <F1> { helpWindow Email }

  ttk::frame $w.f
  ttk::frame $w.b
  pack $w.f -side left -fill y
  pack $w.b -side right -fill y

  set f $w.f
  ttk::label $f.title -text "Opponent list" -font font_Bold
  ttk::treeview $f.list -columns {0} -show {} -selectmode browse \
        -yscrollcommand "$f.scroll set" -height 9
  $f.list column 0 -width 150

  ttk::scrollbar $f.scroll -command "$w.list yview" -takefocus 0
  pack $f -side left -expand true -fill both
  pack $f.title -side top
  pack $f.scroll -side right -fill y
  pack $f.list -side right -expand true -fill both

  bind $f.list <ButtonRelease-1> ::tools::email::refreshButtons
  bind $f.list <Enter> ::tools::email::refreshButtons
  bind $f.list <Key-Up> ::tools::email::refreshButtons
  bind $f.list <Key-Down> ::tools::email::refreshButtons

  bind $f.list <Key-a> {.emailWin.b.add invoke}
  bind $f.list <Key-e> {.emailWin.b.edit invoke}
  bind $f.list <Key-d> {.emailWin.b.delete invoke}
  bind $f.list <Key-l> {.emailWin.b.load invoke}
  bind $f.list <Key-s> {.emailWin.b.send invoke}
  bind $f.list <Key-t> {.emailWin.b.time.m post [winfo pointerx .] [winfo pointery .]}

  set b .emailWin.b

  ttk::button $b.add -text "Add..." -underline 0 -command {
    set idx [llength $emailData]
    lappend emailData [list "" "" "" "" ""]
    .emailWin.f.list insert {} end -id $idx -values ""
    modifyEmailDetails $idx
    ::tools::email::refresh
  }

  ttk::button $b.edit -text [tr Edit] -underline 0 -command ::tools::email::EditButton
  ttk::button $b.delete -text "$::tr(Delete)..." -underline 0 -command ::tools::email::DeleteButton
  ttk::button $b.load -text $::tr(LoadGame) -underline 0 -command ::tools::email::LoadButton
  ttk::button $b.send -text "Send email..." -underline 0 -command ::tools::email::SendButton
  ttk::menubutton $b.time -text $::tr(Time) -menu $b.time.m
  menu $b.time.m
  $b.time.m add command -label "Received today" -underline 0 \
    -command {::tools::email::TimesButton r}
  $b.time.m add command -label "Sent today" -underline 0 \
    -command {::tools::email::TimesButton s}
  $b.time.m add command -label [tr Edit] -underline 0 \
    -command {::tools::email::TimesButton e}

  ttk::button $b.config -text "$::tr(GlistEditField)..." -command ::tools::email::config
  ttk::button $b.help -text $::tr(Help) -command { helpWindow Email }
  ttk::button $b.close -text $::tr(Close) -command { destroy .emailWin }
  pack $b.add $b.edit $b.delete $b.load $b.send $b.time \
    -side top -pady 2 -padx "10 0" -fill x
  pack $b.close $b.help $b.config -side bottom -pady 2 -padx "10 0"  -fill x

  bind $w <Destroy> { set emailWin 0 }
  set emailData [::tools::email::readOpponentFile]
  set idx 0
  foreach i $emailData {
    set name [lindex $i 0]
    set time ""
    if {[llength $i] == 6} {
      set timeList [lindex $i 5]
      set time [lindex $timeList end]
    }
    $f.list insert {} end -id $idx -values [format "%-14s %s" $name $time]
    incr idx
  }
  focus $w.f.list
  ::tools::email::refresh
}

proc ::tools::email::config {} {
  global email
  set w .emailConfig
  win::createDialog $w
  wm title $w "Scid"
  ttk::labelframe $w.use -text "Send email using"
  ttk::frame $w.smtp
  ttk::radiobutton $w.smtp.b -text "SMTP server:" -variable email(smtp) -value 1
  ttk::entry $w.smtp.s -width 30 -textvar email(server)
  ttk::frame $w.sm
  ttk::radiobutton $w.sm.b -text "sendmail process:" -variable email(smtp) -value 0
  ttk::entry $w.sm.s -width 30 -textvar email(smproc)
  pack $w.use -side top -fill x
  pack $w.smtp $w.sm -side top -anchor e -in $w.use
  pack $w.smtp.s $w.smtp.b -side right
  pack $w.sm.s $w.sm.b -side right

  ttk::labelframe $w.addr -text "Email address fields"
  ttk::frame $w.from
  ttk::label $w.from.lab -text "From:"
  ttk::entry $w.from.e -textvar email(from) -width 30
  ttk::frame $w.bcc
  ttk::label $w.bcc.lab -text "Bcc:"
  ttk::entry $w.bcc.e -textvar email(bcc) -width 30
  pack $w.addr -side top -fill x -pady "10 0"
  pack $w.from $w.bcc -side top -fill x -in $w.addr
  pack $w.from.e $w.from.lab -side right
  pack $w.bcc.e $w.bcc.lab -side right

  pack [ttk::frame $w.b] -side top -fill x
  ttk::button $w.b.ok -text [tr OptionsSave] -command {
    options.write
    catch {grab release .emailConfig}
    destroy .emailConfig
  }
  ttk::button $w.b.cancel -text $::tr(Cancel) \
    -command "catch {grab release $w}; destroy $w"
  packdlgbuttons $w.b.cancel $w.b.ok
  wm resizable $w 1 0
  catch {grab $w}
}

proc ::tools::email::EditButton {} {
  global emailData
  set sel [.emailWin.f.list selection]
  if {[llength $sel] == 1} {
    set idx [lindex $sel 0]
    if {[llength $emailData] > $idx} {
      modifyEmailDetails $idx
    }
  }
  ::tools::email::refresh
}

proc ::tools::email::DeleteButton {} {
  global emailData
  set sel [.emailWin.f.list selection]
  if {[llength $sel] != 1} { return }
  set idx [lindex $sel 0]
  if {[llength $emailData] <= $idx} { return }
  set confirm [tk_messageBox -icon question -type yesno -default yes \
                 -parent .emailWin -title "Really delete opponent?" \
                 -message "Do you really want to delete this opponent?"]
  if {$confirm == "yes"} {
      set emailData [lreplace $emailData $idx $idx]
    ::tools::email::writeOpponentFile $emailData
    .emailWin.f.list delete $sel
    ::tools::email::refresh
  }
}

proc ::tools::email::LoadButton {} {
  global emailData
  set sel [.emailWin.f.list selection]
  if {[llength $sel] != 1} { return }
  set idx [lindex $sel 0]
  if {[llength $emailData] <= $idx} { return }
  set details [lindex $emailData $idx]
  if {[llength [lindex $details 3]] > 0} {
    if {[catch {::game::Load [lindex [lindex $details 3] 0]} result]} {
      tk_messageBox -type ok -icon warning -title "Scid" -message $result
    } else {
      sc_move end
    }
  }
}

proc ::tools::email::SendButton {} {
  global emailData
  set sel [.emailWin.f.list selection]
  if {[llength $sel] != 1} { return }
  set idx [lindex $sel 0]
  if {[llength $emailData] <= $idx} { return }
  set details [lindex $emailData $idx]
  emailMessageEditor $idx [lindex $details 0] [lindex $details 1] \
    [lindex $details 2] [lindex $details 3] [lindex $details 4]
}

set emailTimesIdx 0

proc ::tools::email::TimesButton {type} {
  global emailData emailTimesIdx
  set sel [.emailWin.f.list selection]
  if {[llength $sel] != 1} { return }
  set idx [lindex $sel 0]
  if {[llength $emailData] <= $idx} { return }
  set details [lindex $emailData $idx]
  while {[llength $details] < 6} { lappend details {} }
  set timeList [lindex $details 5]
  set last [lindex $timeList end]

  if {$type == "r"  || $type == "s"} {
    ::tools::email::addSentReceived $idx $type
    return
  }

  set emailTimesIdx $idx
  set w .emailTimesWin
  if {[winfo exists $w]} { return }
  win::createDialog $w
  wm title $w "Scid: Email Times"
  ttk::label $w.title -text "Email Times for [lindex $details 0]"
  autoscrollText y $w.t $w.t.text Treeview
  $w.t.text configure -height 15 -width 30 -font font_Fixed -setgrid 1 -state normal
  ttk::frame $w.b
  ttk::button $w.b.ok -text "OK" -command {
    set details [lindex $emailData $emailTimesIdx]
    set timeList [split [string trim [.emailTimesWin.t.text get 1.0 end]] "\n"]
    set details [lreplace $details 5 5 $timeList]
    set emailData [lreplace $emailData $emailTimesIdx $emailTimesIdx $details]
    ::tools::email::writeOpponentFile $emailData
    grab release .emailTimesWin
    ::tools::email::refresh 0
    catch {focus .emailWin}
    destroy .emailTimesWin
  }
  ttk::button $w.b.cancel -text $::tr(Cancel) \
    -command "grab release $w; catch {focus .emailWin}; destroy $w"
  pack $w.title -side top -fill x
  pack $w.t -side top -fill both
  pack $w.b -side bottom -fill x
  packdlgbuttons $w.b.cancel $w.b.ok
  foreach i $timeList {
    $w.t.text insert end "$i\n"
  }
  grab $w
}

proc ::tools::email::addSentReceived {idx type} {
  global emailData
  if {[llength $emailData] <= $idx} { return }
  set details [lindex $emailData $idx]
  while {[llength $details] < 6} { lappend details {} }
  set timeList [lindex $details 5]
  set last [lindex $timeList end]

  set new ""
  if {$type == "r"} { append new "Received " } else { append new "Sent     " }

  set oppGList [lindex $details 3]
  if {[llength $oppGList] > 0} {
    set oppGNum [lindex $oppGList 0]
    sc_game push
    set mnum "     "
    if {[catch {::game::Load $oppGNum}]} {
    } else {
      sc_move end
      set m [llength [split [sc_game moves coord list]]]
      if {$m > 0} {
        set m [expr int(($m+1)/2)]
        set mnum [format "%3d  " $m]
      }
    }
    sc_game pop
    append new $mnum
  }
  append new [::utils::date::today]
  if {! [string compare $last $new]} { return }
  lappend timeList $new
  set details [lreplace $details 5 5 $timeList]
  set emailData [lreplace $emailData $idx $idx $details]
  ::tools::email::writeOpponentFile $emailData
  ::tools::email::refresh 0
}

proc ::tools::email::refreshButtons {} {
  set sel [.emailWin.f.list selection]
  if {[llength $sel] > 0} {
    .emailWin.b.edit configure -state normal
    .emailWin.b.delete configure -state normal
    .emailWin.b.load configure -state normal
    .emailWin.b.send configure -state normal
  } else {
    .emailWin.b.edit configure -state disabled
    .emailWin.b.delete configure -state disabled
    .emailWin.b.load configure -state disabled
    .emailWin.b.send configure -state disabled
  }
}

proc ::tools::email::refresh {{clearSelection 1}} {
  global emailWin emailData
  if {! [winfo exists .emailWin]} { return }
  if {$clearSelection} {
    set sel ""
  } else {
    set sel [.emailWin.f.list selection]
  }
  if {$sel != ""} {
    .emailWin.f.list selection set $sel
  }
  ::tools::email::refreshButtons
}

#Initial values for globals:
set emailData {}
set emailData_index 0
set emailData_name ""
set emailData_addr ""
set emailData_subj ""
set emailData_glist ""
set emailData_dates ""
set emailData_helpBar {}
array set ::tools::email::helpBar ""

# Force the game numbers list to be digits and spaces only:
trace variable emailData_glist w {::utils::validate::Regexp {^[0-9\ ]*$}}


# emailCount: counter to give each email window a unique name.
set emailCount 0

# emailMessageEditor:
#    Constructs the email message to the opponent and
#    creates the editor window for editing and sending the message.
#
proc emailMessageEditor {idx name addr subj gamelist sig} {
  global emailCount emailData email
  incr emailCount
  if {$emailCount >= 10000} { set emailCount 1 }

  set w ".emailMessageWin$emailCount"
  win::createDialog $w
  wm title $w "Send email to $name"
  set f [ttk::frame $w.fields]

  ttk::label $f.fromlab -text "From: "
  ttk::entry $f.from
  $f.from insert end $email(from)

  ttk::label $f.tolab -text "To: "
  ttk::entry $f.to
  $f.to insert end $addr

  ttk::label $f.subjlab -text "Subject: "
  ttk::entry $f.subj
  $f.subj insert end $subj

  ttk::label $f.bcclab -text "Bcc: "
  ttk::entry $f.bcc
  $f.bcc insert end $email(bcc)

  grid $f.fromlab -row 0 -column 0 -sticky e
  grid $f.from -row 0 -column 1 -sticky ew
  grid $f.tolab -row 1 -column 0 -sticky e
  grid $f.to -row 1 -column 1 -sticky ew
  grid $f.subjlab -row 2 -column 0 -sticky e
  grid $f.subj -row 2 -column 1 -sticky ew
  grid $f.bcclab -row 3 -column 0 -sticky e
  grid $f.bcc -row 3 -column 1 -sticky ew
  grid columnconfigure $f 1 -weight 1

  set f $w.message
  autoscrollText both $f $f.text Treeview
  $f.text configure -width 72 -height 20 -wrap none -state normal
  pack $w.fields -fill x -padx 4 -pady 4
  pack $w.message -expand yes -fill both -padx 4 -pady 4

  ttk::frame $f.buttons
  ttk::button $f.send -text " Send " -command "::tools::email::processMessage $w $idx"
  ttk::button $f.cancel -text $::tr(Cancel) -command "destroy $w"
  grid $f.buttons -row 2 -column 0 -columnspan 2 -sticky e
  packdlgbuttons $f.cancel $f.send -side right -in $f.buttons

  grid rowconfig $w.message 0 -weight 1 -minsize 0
  grid columnconfig $w.message 0 -weight 1 -minsize 0

  # Right-mouse button cut/copy/paste menu:
  menu $f.text.edit -tearoff 0
  $f.text.edit add command -label "Cut" -command "tk_textCut $f.text"
  $f.text.edit add command -label "Copy" -command "tk_textCopy $f.text"
  $f.text.edit add command -label "Paste" -command "tk_textPaste $f.text"
  bind $f.text <ButtonPress-$::MB3> "tk_popup $f.text.edit %X %Y"

  set text $w.message.text
  # $text insert end "Hi $name,\n\n"
  $text insert end "\n"
  foreach i $gamelist {
    catch {set gamePgn [sc_game pgn -gameNumber $i -width 70 -tags 0 \
                          -variations 0 -comments 0]}
    $text insert end "$gamePgn\n"
  }
  $text insert end $sig
  return
}

proc ::tools::email::processMessage {w idx} {
  global emailData
  set from [$w.fields.from get]
  set to [$w.fields.to get]
  set subj [$w.fields.subj get]
  set bcc [$w.fields.bcc get]
  set message [$w.message.text get 1.0 end]
  if {[string trim $to] == ""} {
    tk_messageBox -icon error -type ok -title "Empty email address" \
      -message "You must specify an email address."
    return
  }
  set cmd {::tools::email::sendMessage $from $to $subj $bcc $message}
  if {[catch $cmd result] != 0} {
    tk_messageBox -icon error -type ok -title "Error sending email" \
      -message "Error sending email: $result"
  } else {
    ::tools::email::addSentReceived $idx s
    tk_messageBox -icon info -type ok -title "Scid" -message $result
    destroy $w
  }
}

proc ::tools::email::sendMessage {from to subject bcc message} {
  global email

  ### Uncomment following line for testing, to avoid sending email:
  # return "Testing, no email was actually sent"

  set copy_id ""
  catch {set copy_id [open [file nativename $email(logfile)] "a+"]}
  if {$copy_id == ""} {
    return -code error "Unable to open $email(logfile)"
  }
  if {$email(smtp)} {
    set cmdargs "-to {$to} -subject {$subject} "
    if {$email(server) != ""} { ::ezsmtp::config -mailhost $email(server) }
    if {$email(from) != ""} {
      if {[catch {::ezsmtp::config -from $from} result]} {
        close $copy_id
        return -code error "Error configuring SMTP: $result"
      }
      append cmdargs "-from {$from} "
    }
    if {$email(bcc) != ""} {
      append cmdargs "-bcc {$bcc} "
    }
    if {[catch {eval "::ezsmtp::send $cmdargs -body {$message}"} result]} {
      close $copy_id
      return -code error "Error sending mail with SMTP: $result"
    }
  } else {
    if {[catch {open "| $email(smproc) -oi -t" "w"} ::tools::email::id]} {
      close $copy_id
      return -code error "Scid could not find the sendmail program: $email(smproc)"
    }
    if {[string trim $from] != ""} {
      puts $::tools::email::id "From: $from"
    }
    puts $::tools::email::id "To: $to"
    puts $::tools::email::id "Subject: $subject"
    if {[string trim $bcc] != ""} {
      puts $::tools::email::id "Bcc: $bcc"
    }
    puts $::tools::email::id ""
    puts $::tools::email::id $message
    close $::tools::email::id
  }
  puts $copy_id  "To: $to"
  puts $copy_id  "Subject: $subject"
  puts $copy_id  ""
  puts $copy_id $message
  close $copy_id
  return "The email message was sent; a copy was appended to $email(logfile)"
}

proc modifyEmailDetails {i} {
  global emailData emailData_name emailData_addr emailData_glist emailData_subj emailAdd
  global emailData_sig emailData_index emailData_helpBar ::tools::email::helpBar

  win::createDialog .emailEditor
  set w .emailEditor
  bind $w <F1> { helpWindow Email }
  set emailData_index $i
  if {[lindex [lindex $emailData $i] 0] == ""} {
    wm title $w "Add opponent details"
    set emailAdd 1
  } else {
    wm title $w "Edit opponent details"
    set emailAdd 0
  }
  set f [ttk::frame $w.name]
  ttk::label $f.label -text "Name: "
  ttk::entry $f.entry -width 30 -textvariable emailData_name
  set ::tools::email::helpBar(name) "Enter the opponent's name"

  set f [ttk::frame $w.addr]
  ttk::label $f.label -text "Email address: "
  ttk::entry $f.entry -width 30 -textvariable emailData_addr
  set ::tools::email::helpBar(addr) "Enter the opponent's email address"

  set f [ttk::frame $w.subj]
  ttk::label $f.label -text "Subject: "
  ttk::entry $f.entry -width 30 -textvariable emailData_subj
  set ::tools::email::helpBar(subj) "Enter the subject for each message"

  set f [ttk::frame $w.glist]
  ttk::label $f.label -text "Game Numbers: "
  ttk::entry $f.entry -width 30 -textvariable emailData_glist
  set ::tools::email::helpBar(glist) \
    "Enter opponent's game numbers, separated by spaces"

  foreach f {name addr subj glist} {
    pack $w.$f -side top -fill x
    pack $w.$f.entry $w.$f.label -side right -anchor e
    set e $w.$f.entry
    bind $e <FocusIn> " set emailData_helpBar \$::tools::email::helpBar($f)"
  }

  addHorizontalRule $w

  set f [ttk::frame $w.sig]
  ttk::label $f.label -text "Signature: " -anchor n
  text $f.entry -width 30 -height 5
  applyThemeStyle Treeview $f.entry
  bind $f.entry <FocusIn> " set emailData_helpBar {Enter the closing text for each message}"

  pack $f -side top -fill x -pady 5
  pack $f.entry $f.label -side right -anchor n

  addHorizontalRule $w

  set f [ttk::frame $w.buttons]
  ttk::button $w.buttons.save -text "OK" -command {
    set gNumberErr [::tools::email::validGameNumbers $emailData_glist]
    if {$gNumberErr != -1} {
      set nGames [sc_base numGames [sc_base current]]
      tk_messageBox -icon error -type ok -title "Invalid data" \
        -message "The games list contains an invalid game number: $gNumberErr; there are only $nGames games in this database."
    } else {
      set emailData [lreplace $emailData $emailData_index \
                       $emailData_index \
                       [list $emailData_name $emailData_addr $emailData_subj \
                          $emailData_glist \
                          [.emailEditor.sig.entry get 1.0 end-1c]]]
      .emailWin.f.list set $emailData_index 0 [format "%-14s" $emailData_name ]
      ::tools::email::writeOpponentFile $emailData
      destroy .emailEditor
      ::tools::email::refresh
    }
  }
  ttk::button $f.cancel -text $::tr(Cancel) -command {
    if { $emailAdd } {
        .emailWin.f.list delete $emailData_index
    }
    set emailData [::tools::email::readOpponentFile]
    destroy .emailEditor
    ::tools::email::refresh
  }

  ttk::label $w.helpBar -width 1 -textvariable emailData_helpBar \
    -font font_Small -anchor w
  pack $w.helpBar -side top -fill x
  pack $f -side top -anchor e
  packdlgbuttons $f.cancel $f.save

  # Set up the initial values in the entry boxes:
  set details [lindex $emailData $emailData_index]
  set emailData_name [lindex $details 0]
  set emailData_addr [lindex $details 1]
  set emailData_subj [lindex $details 2]
  set emailData_glist [lindex $details 3]
  $w.sig.entry insert 1.0 [lindex $details 4]
  grab .emailEditor
}

proc ::tools::email::validGameNumbers {numberList} {
  set nGames [sc_base numGames [sc_base current]]
  foreach i $numberList {
    if {$i < 1  ||  $i > $nGames} { return $i }
  }
  return -1
}

proc ::tools::email::opponentFilename {} {
  set filename [sc_base filename $::curr_db]
  append filename ".sem"
  return $filename
}

proc ::tools::email::readOpponentFile {} {
  set filename [::tools::email::opponentFilename]
  if {[catch {set f [open $filename "r"]} ]} {
    # puts "Unable to open opponent file"
    return {}
  }
  set data [read -nonewline $f]
  close $f
  return $data
}

proc ::tools::email::writeOpponentFile {data} {
  set filename [::tools::email::opponentFilename]
  if {[catch {set f [open $filename "w"]} ]} {
    # puts "Unable to write opponent file"
    return {}
  }
  puts $f $data
  close $f
}

