####################
# File finder window

set ::file::finder::data(dir) [pwd]
set ::file::finder::data(sort) name
set ::file::finder::data(recurse) 0
set ::file::finder::data(stop) 0
set ::file::finder::data(Scid) 1
set ::file::finder::data(PGN) 1
set ::file::finder::data(Rep) 1
set ::file::finder::data(EPD) 1
set ::file::finder::data(Old) 1

proc ::file::finder::Open {} {
  set w .finder
  if {[winfo exists $w]} { return }
  
  win::createDialog $w
  wm title $w "Scid: $::tr(FileFinder)"
  bind $w <F1> {helpWindow Finder}
  setWinLocation $w
  bind $w <Configure> "recordWinSize $w"
  
  ttk::frame $w.p
  ttk::labelframe $w.p.label -text $::menuLabel($::language,FinderSortType)
  foreach type {Scid Old PGN Rep EPD} {
    ttk::checkbutton $w.p.label.[string tolower $type] -text $type \
        -variable ::file::finder::data($type) -onvalue 1 -offvalue 0 \
        -command ::file::finder::Refresh
    ::utils::tooltip::Set $w.p.label.[string tolower $type] $::menuLabel($::language,FinderTypes$type)
    pack $w.p.label.[string tolower $type] -side left -anchor w -padx 2 -fill x
  }
  ttk::button $w.p.stop -textvar ::tr(Stop) -command {set finder(stop) 1 }
  ttk::checkbutton $w.p.sub -text [tr FinderFileSubdirs] \
      -variable ::file::finder::data(recurse) -onvalue 1 -offvalue 0 \
      -command ::file::finder::Refresh
  pack $w.p.stop -side right -padx "5 0"
  pack $w.p.label $w.p.sub -side left -padx "0 5" -pady "0 4"

  ttk::frame $w.d
  ttk::label $w.d.label -text "$::tr(FinderDir):" -font font_Small
  set ::file::finder::data(menu) [tk_optionMenu $w.d.mb ::file::finder::data(dir) ""]
  # use ttk instead of tk_optionbutton, but use the menu
  ttk::menubutton $w.d.mbn -text $::file::finder::data(dir) -menu $::file::finder::data(menu)

  ttk::button $w.d.up -image tb_updir -command {::file::finder::Refresh ..}
  ttk::button $w.d.help -image tb_help_small -command {helpWindow Finder}
  pack $w.d.label -side left
  pack $w.d.help $w.d.up -side right -padx "5 0"
  pack $w.d.mbn -side left -fill x -expand yes
  
  autoscrollText both $w.t $w.t.text Treeview
  $w.t.text configure -width 65 -height 25 -font font_Small -wrap none \
      -setgrid 1 -cursor top_left_arrow
  $w.t.text tag configure Dir -foreground brown
  $w.t.text tag configure Vol -foreground gray25
  $w.t.text tag configure PGN -foreground blue
  $w.t.text tag configure Scid -foreground red
  $w.t.text tag configure Old -foreground black
  $w.t.text tag configure Rep -foreground darkGreen
  $w.t.text tag configure EPD -foreground orange
  $w.t.text tag configure bold -font font_SmallBold
  $w.t.text tag configure center -justify center
  set xwidth [font measure [$w.t.text cget -font] "x"]
  set tablist {}
  foreach {tab justify} {15 r 30 r 32 l 50 l} {
    set tabwidth [expr {$xwidth * $tab} ]
    lappend tablist $tabwidth $justify
  }
  $w.t.text configure -tabs $tablist
  
  bind $w <Escape> {
    if {[winfo exists .finder.t.text.ctxtMenu]} {
      destroy .finder.t.text.ctxtMenu
      focus .finder
    } else {
      .finder.p.stop invoke
    }
  }
  # Bind left button to close ctxt menu:
  bind $w <ButtonPress-1> {
    if {[winfo exists .finder.t.text.ctxtMenu]} {
      destroy .finder.t.text.ctxtMenu
      focus .finder
    }
  }

  grid $w.d -sticky we
  grid $w.p -sticky we
  grid $w.t -sticky nswe
  grid rowconfigure $w 2 -weight 1
  grid columnconfigure $w 0 -weight 1
  ::file::finder::Refresh
}

proc ::file::finder::Refresh {{newdir ""}} {
  variable data
  set w .finder
  if {! [winfo exists $w]} { return }
  set t $w.t.text
  
  # When parameter is "-fast", just re-sort the existing data:
  set fastmode 0
  if {$newdir == "-fast"} {
    set fastmode 1
    set newdir ""
  }
  if {$newdir == ".."} { set newdir [file dirname $data(dir)] }
  if {$newdir != ""} { set data(dir) $newdir }
  
  busyCursor .
  set data(stop) 0
  $w.d.help configure -state disabled
  $w.p.sub configure -state disabled
  $w.p.stop configure -state normal
  catch {grab $w.p.stop}
  $t configure -state normal
  update
  
  if {$fastmode} {
    set flist $data(flist)
  } else {
    set flist [::file::finder::GetFiles $data(dir)]
    set data(flist) $flist
  }
  
  switch $data(sort) {
    "none" {}
    "type" { set flist [lsort -decreasing -index 1 $flist] }
    "size" { set flist [lsort -integer -decreasing -index 0 $flist] }
    "name" { set flist [lsort -dict -index 2 $flist] }
    "path" { set flist [lsort -dict -index 3 $flist] }
    "mod"  { set flist [lsort -integer -decreasing -index 4 $flist] }
  }
  
  set hc yellow
  $t delete 1.0 end
  set dcount 0
  $t insert end "$::tr(FinderDirs)\n" {center bold}
  set dlist {}
  
  # Insert drive letters, on Windows:
  if {$::windowsOS} {
    foreach drive [lsort -dictionary [file volume]] {
      $t insert end " $drive " [list Vol v$drive]
      $t insert end "    "
      $t tag bind v$drive <1> [list ::file::finder::Refresh $drive]
      $t tag bind v$drive <Any-Enter> \
          "$t tag configure [list v$drive] -background $hc"
      $t tag bind v$drive <Any-Leave> \
          "$t tag configure [list v$drive] -background {}"
    }
    $t insert end "\n"
  }
  
  # Insert parent directory entry:
  lappend dlist ..
  
  # Generate other directory entries:
  set dirlist [lsort -dictionary [glob -nocomplain [file join $data(dir) *]]]
  foreach dir $dirlist {
    if {[file isdir $dir]} {
      lappend dlist $dir
    }
  }
  foreach dir $dlist {
    if {$dcount != 0} {
      set sep "\n"
      if {$dcount % 2 != 0} { set sep "\t\t\t" }
      $t insert end $sep
    }
    incr dcount
    if {$dir == ".."} {
      set d ..
      $t insert end " .. ($::tr(FinderUpDir)) " [list Dir d..]
    } else {
      set d [file tail $dir]
      $t insert end " $d " [list Dir d$d]
    }
    $t tag bind d$d <1> [list ::file::finder::Refresh $dir]
    $t tag bind d$d <Any-Enter> \
        "$t tag configure [list d$d] -background $hc"
    $t tag bind d$d <Any-Leave> \
        "$t tag configure [list d$d] -background {}"
  }
  
  # Add File section headings:
  $t insert end "\n\n"
  if {[llength $flist] != 0} {
    foreach i {Type Size Mod Name Path} v {type size mod name path} {
      $t tag configure s$i -font font_SmallBold
      $t tag bind s$i <1> "set ::file::finder::data(sort) $v; ::file::finder::Refresh -fast"
      $t tag bind s$i <Any-Enter> "$t tag config s$i -foreground red"
      $t tag bind s$i <Any-Leave> "$t tag config s$i -foreground {}"
    }
    $t insert end "$::tr(FinderFiles)\n" {center bold}
    $t insert end " "
    $t insert end "[tr FinderSortType]" sType
    $t insert end "\t"
    $t insert end "[tr FinderSortSize]" sSize
    $t insert end "\t"
    $t insert end "[tr FinderSortMod]" sMod
    $t insert end "\t"
    $t insert end "[tr FinderSortName]" sName
    $t insert end "\t"
    $t insert end "[tr FinderSortPath]" sPath
    $t insert end "\n"
  }
  
  # Add each file:
  foreach i $flist {
    set size [lindex $i 0]
    set type [lindex $i 1]
    set fname [lindex $i 2]
    set path [lindex $i 3]
    set mtime [lindex $i 4]
    set est [lindex $i 5]
    $t insert end "\n "
    $t insert end $type [list $type f$path]
    set  esize [::utils::thousands $size];
    if {$est} { append esize " kB" }
    $t insert end "\t$esize" f$path
    $t insert end "\t[clock format $mtime -format {%b %d %Y}]" f$path
    $t insert end "\t$fname\t" f$path
    set dir [file dirname $path]
    set tail [file tail $path]
    if {$dir == "."} {
      set fullpath $data(dir)/$tail
    } else  {
      set fullpath $data(dir)/$dir/$tail
    }
    
    $t tag bind f$path <ButtonRelease-1> "::file::Open [list $fullpath]"
    # Bind right button to popup a contextual menu:
    $t tag bind f$path <ButtonPress-$::MB3> "::file::finder::contextMenu .finder.t.text [list $fullpath] %x %y %X %Y"
    
    $t tag bind f$path <Any-Enter> \
        "$t tag configure [list f$path] -background $hc"
    $t tag bind f$path <Any-Leave> \
        "$t tag configure [list f$path] -background {}"
    if {$dir == "."} {
      set fullpath "$data(dir)/$tail"
    } else {
      $t tag configure p$path -foreground darkblue
      $t insert end "$dir/" [list p$path f$path]
    }
    $t tag configure t$path -foreground blue
    $t insert end $tail [list t$path f$path]
  }
  $t configure -state disabled
  
  # Update directory menubutton:
  $data(menu) delete 0 end
  set mlist {}
  set d {}
  foreach subdir [file split $data(dir)] {
    set d [file join $d $subdir]
    lappend mlist $d
  }
  foreach m $mlist {
    $data(menu) add command -label $m -command "::file::finder::Refresh [list $m]"
  }
  
  #store actual directory string in menubutton
  .finder.d.mbn configure -text [lindex $mlist [ expr { [llength $mlist] - 1}]]
  catch {grab release $w.p.stop}
  $w.p.stop configure -state disabled
  $w.p.sub configure -state normal
  $w.d.help configure -state normal
  unbusyCursor .
  
}
################################################################################
#
################################################################################
proc ::file::finder::contextMenu {win fullPath x y xc yc} {
  
  update idletasks
  
  set mctxt $win.ctxtMenu
  
  if { [winfo exists $mctxt] } { destroy $mctxt }
  
  menu $mctxt
  $mctxt add command -label [tr FinderCtxOpen ] -command "::file::Open [list $fullPath]"
  $mctxt add command -label [tr FinderCtxBackup ] -command "::file::finder::backup [list $fullPath]"
  $mctxt add command -label [tr FinderCtxCopy ] -command "::file::finder::copy [list $fullPath]"
  $mctxt add command -label [tr FinderCtxMove ] -command "::file::finder::move [list $fullPath]"
  $mctxt add separator
  $mctxt add command -label [tr FinderCtxDelete ] -command "::file::finder::delete $fullPath"
  
  $mctxt post [winfo pointerx .] [winfo pointery .]
  
}
################################################################################
# will backup a base in the form name-date.ext
################################################################################
proc ::file::finder::backup { f } {
  set r [file rootname $f]
  set d [clock format [clock seconds] -format "-%Y.%m.%d-%H%M" ]
  set ext [string tolower [file extension $f]]
  if { $ext == ".si4" } {
    if { [catch { file copy "$r.sg4" "$r$d.sg4" ; file copy "$r.sn4" "$r$d.sn4" } err ] } {
      tk_messageBox -title Scid -icon error -type ok -message "File copy error $err"
      return
    }
    catch { file copy "$r.stc" "$r$d.stc" }
  }
  
  if { [catch { file copy "$r[file extension $f]" "$r$d[file extension $f]" } err ] } {
    tk_messageBox -title Scid -icon error -type ok -message "File copy error $err"
    return
  }
  
  ::file::finder::Refresh
}
################################################################################
#
################################################################################
proc ::file::finder::copy { f } {
  if {[sc_base slot $f] != 0} {
    tk_messageBox -title Scid -icon error -type ok -message "Close base first"
    return
  }
  set dir [tk_chooseDirectory -initialdir [file dirname $f] ]
  if {$dir != ""} {
    if { [string tolower [file extension $f]] == ".si4" } {
      if { [catch { file copy "[file rootname $f].sg4" "[file rootname $f].sn4" $dir } err ] } {
        tk_messageBox -title Scid -icon error -type ok -message "File copy error $err"
        return
      }
      
      catch { file copy "[file rootname $f].stc" $dir }
    }
    
    if { [catch { file copy $f $dir } err ] } {
      tk_messageBox -title Scid -icon error -type ok -message "File copy error $err"
      return
    }
    
  }
}
################################################################################
#
################################################################################
proc ::file::finder::move { f } {
  if {[sc_base slot $f] != 0} {
    tk_messageBox -title Scid -icon error -type ok -message "Close base first"
    return
  }
  set dir [tk_chooseDirectory -initialdir [file dirname $f] ]
  if {$dir != ""} {
    if { [string tolower [file extension $f]] == ".si4" } {
      
      if { [catch { file rename "[file rootname $f].sg4" "[file rootname $f].sn4" $dir } err ] } {
        tk_messageBox -title Scid -icon error -type ok -message "File rename error $err"
        return
      }
      catch { file rename "[file rootname $f].stc" $dir }
    }
    
    if { [catch { file rename $f $dir } err ] } {
      tk_messageBox -title Scid -icon error -type ok -message "File rename error $err"
      return
    }
  }
  ::file::finder::Refresh
}
################################################################################
#
################################################################################
proc ::file::finder::delete { f } {
  if {[sc_base slot $f] != 0} {
    tk_messageBox -title Scid -icon error -type ok -message "Close base first"
    return
  }
  set answer [tk_messageBox -title Scid -icon warning -type yesno -message "Are you sure you want to permanently delete $f ?"]
  if {$answer == "yes"} {
    if { [string tolower [file extension $f]] == ".si4" } {
      file delete "[file rootname $f].sg4" "[file rootname $f].sn4" "[file rootname $f].stc"
    }
    file delete $f
  }
  ::file::finder::Refresh
}

proc ::file::finder::GetFiles {dir {len -1}} {
  variable data
  set dlist {}
  set flist {}
  if {$len < 0} {
    set len [expr {[string length $dir] + 1} ]
  }
  
  foreach f [glob -nocomplain [file join $dir *]] {
    if {[file isdir $f]} {
      lappend dlist $f
    } elseif {[file isfile $f]} {
      set ext [string tolower [file extension $f]]
      if {[catch {set mtime [file mtime $f]}]} { set mtime 0 }
      set showFile 0
      set rootname [file rootname $f]
      set type PGN
      set fsize [file size $f]
      set est 0
      # if it is not a scid database show size in kb
      set size "[expr {$fsize/1024}]"
      if {$ext == ".si4"} {
        set showFile 1
        set size [expr {($fsize - 182)/47}]
        set type Scid
      } elseif {$ext == ".si3"} {
        set showFile 1
        set size [expr {($fsize - 128)/46}]
        set type Old
      } elseif {$ext == ".sor"} {
        set showFile 1
        set est 1
        set type Rep
      } elseif {$ext == ".epd"} {
        set type EPD
        set est 1
        set showFile 1
      } elseif {$ext == ".pgn"} {
        set est 1
        set showFile 1
      }
      if {$showFile  &&  [info exists data($type)]  &&  $data($type)} {
        set path [string range $f $len end]
        if {[file dirname $path] == "."} { set path "./$path" }
        lappend flist [list $size $type [file tail $rootname] $path $mtime $est]
      }
    }
    update
    if {$data(stop)} { break }
  }
  if {$data(recurse)} {
    foreach f $dlist {
      foreach i [::file::finder::GetFiles $f $len] {
        lappend flist $i
        update
        if {$data(stop)} { break }
      }
    }
  }
  return $flist
}

