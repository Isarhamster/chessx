########################################################################
# Copyright (C) 2020 Fulvio Benini
#
# This file is part of Scid (Shane's Chess Information Database).
# Scid is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation.

### Window for chess engine configuration and position analysis

namespace eval enginewin {}
array set ::enginewin::engState {} ; # closed idle run locked

# Sends the updated position to the active engines
proc ::enginewin::onPosChanged { {ids ""} } {
    set position ""
    foreach {id state} [array get ::enginewin::engState] {
        if {$state ne "run"} { continue }
        if {$ids ne "" && $id ni $ids} { continue }
        if {$position eq ""} {
            set position [sc_game UCI_currentPos]
        }
        ::enginewin::updateDisplay $id ""
        ::engine::send $id Go [list $position]
    }
}

proc ::enginewin::toggleStartStop {id} {
    if {![winfo exists .engineWin$id]} {
        ::enginewin::Open $id
        after 300 ".engineWin$id.config.header.go invoke"
        return
    }
    if {$::enginewin::engState($id) eq "idle"} {
        ::enginewin::changeState $id run
        ::enginewin::onPosChanged $id
    } else {
        ::enginewin::changeState $id idle
        ::engine::send $id StopGo
    }
}

proc ::enginewin::Open { {id ""} } {
    if {$id == ""} {
        set id 1
        while {[winfo exists .engineWin$id]} {
            incr id
        }
    }
    set w .engineWin$id
    if {! [::win::createWindow $w ""]} {
        ::win::makeVisible $w
        return
    }
    set btn1 [::enginewin::frameConfig [ttk::frame $w.config] $id]
    $btn1 configure -command "
        grid forget $w.config
        grid $w.display -sticky news
        ::enginewin::changeState $id run
        ::enginewin::onPosChanged $id
    "
    set btn2 [::enginewin::frameDisplay [ttk::frame $w.display] $id]
    $btn2 configure -command "
        grid forget $w.display
        grid $w.config -sticky news
    "
    grid columnconfigure $w 0 -weight 1
    grid rowconfigure $w 0 -weight 1

    # The engine should be closed before the debug .text is destroyed
    bind $w.config <Destroy> "
        unset ::enginewin::engState($id)
        unset ::enginewin::position($id)
        ::engine::close $id
        ::enginelist::save \$::enginewin::engConfig($id)
        unset ::enginewin::engConfig($id)
    "
    grid $w.config -sticky news

    options.persistent ::enginewin_lastengine($id) ""
    set lastEngine [lsearch -exact -inline -index 0 $::engines(list) $::enginewin_lastengine($id)]
    set ::enginewin::engState($id) {}
    ::enginewin::changeState $id closed
    if {$lastEngine ne ""} {
        catch { ::enginewin::connectEngine $id $lastEngine }
    }
}

proc ::enginewin::frameConfig {w id} {
    ttk::frame $w.header
    ttk::button $w.header.go -text go
    ttk::combobox $w.header.engine -state readonly -postcommand "
        $w.header.engine configure -values \[lmap elem \$::engines(list) { lindex \$elem 0 } \]
    "
    bind $w.header.engine <<ComboboxSelected>> "
        ::enginewin::connectEngine $id \[lindex \$::engines(list) \[$w.header.engine current\]\]
    "
    ttk::button $w.header.addpipe -text "add local" -command "::enginewin::addLocal $id"
    ttk::button $w.header.addnetwork -text "add remote" -command "::enginewin::addNetwork $id"
    ttk::button $w.header.clone -text "clone"
    ttk::button $w.header.delete -text "delete" -command "
        $w.header.engine configure -values \[lmap elem \$::engines(list) { lindex \$elem 0 } \]
        ::enginelist::delete \[$w.header.engine current\]
        ::engine::close $id
        ::enginewin::changeState $id closed
    "
    grid $w.header.go $w.header.engine $w.header.addpipe $w.header.addnetwork \
         $w.header.clone $w.header.delete -sticky news
    grid columnconfigure $w.header 1 -weight 1

    autoscrollText both $w.options $w.options.text Treeview
    $w.options.text configure -wrap none

    grid columnconfigure $w 0 -weight 1
    grid rowconfigure $w 1 -weight 1
    grid $w.header -sticky news
    grid $w.options -sticky news

    return $w.header.go
}

proc ::enginewin::frameDisplay {w id} {
    ttk::frame $w.header
    autoscrollText y $w.header.info $w.header.info.text TLabel
    $w.header.info.text configure -wrap word -height 1
    ttk::button $w.header.config -style Toolbutton -image tb_tabmenu
    grid $w.header.config $w.header.info -sticky news
    grid columnconfigure $w.header 1 -weight 1

    set tab [font measure font_Regular "xxxxxxx"]
    autoscrollText both $w.pv $w.pv.lines Treeview
    $w.pv.lines configure -tabs [list [expr {$tab * 2}] right [expr {int($tab * 2.2)}]]
    $w.pv.lines tag configure pv -lmargin2 [expr {$tab * 3}]
    grid propagate $w.pv 0

    autoscrollText y $w.debug $w.debug.lines TLabel
    $w.debug.lines configure -state normal -relief sunken
    grid propagate $w.debug 0

    ttk::frame $w.btn
    ttk::button $w.btn.startStop -image [list tb_eng_on pressed tb_eng_off] \
        -command "::enginewin::toggleStartStop $id"
    ttk::button $w.btn.threats -text "Threats"
    ttk::spinbox $w.btn.multipv -increment 1 -width 4
    grid $w.btn.startStop $w.btn.threats $w.btn.multipv

    grid $w.header -sticky news
    grid $w.pv -sticky news
    grid $w.debug -sticky news
    grid $w.btn -sticky news

    grid columnconfigure $w 0 -weight 1
    grid rowconfigure $w 1 -weight 1

    return $w.header.config
}

proc ::enginewin::changeDisplayLayout {id param value} {
    set w .engineWin$id.display
    switch $param {
        "scoreside" {
            set idx 0
        }
        "notation" {
            set idx 1
        }
        "wrap" {
            set idx 2
            $w.pv.lines configure -wrap $value
        }
        "debug" {
            set idx 3
            if {$value} {
                grid rowconfigure $w 2 -weight 1
                ::engine::setLogCmd $id \
                    [list ::enginewin::logEngine $w.debug.lines "" ""]\
                    [list ::enginewin::logEngine $w.debug.lines header ">>"]
            } else {
                grid rowconfigure $w 2 -weight 0
                ::engine::setLogCmd $id "" ""
            }
        }
        "multipv" {
            if {$value eq ""} {
                $w.btn.multipv configure -state disabled
            } else {
                lassign $value idx value min max
                # TODO: bind focusout
                $w.btn.multipv configure -state normal -from $min -to $max \
                  -command "after idle {
                    set go \[expr { \$::enginewin::engState($id) in {run lock} }\]
                    ::enginewin::updateConfigSetOption $id $idx $w.btn.multipv
                    if {\$go} {
                        ::enginewin::onPosChanged $id
                    }
                }"
            }
            $w.btn.multipv set $value
            return
        }
        default { error "changeDisplayLayout unknown $param" }
    }
    lset ::enginewin::engConfig($id) 6 $idx $value
}

proc ::enginewin::changeState {id newState} {
    if {$::enginewin::engState($id) eq $newState} { return }

    set w .engineWin$id
    if {$::enginewin::engState($id) eq "closed"} {
        $w.config.header.go configure -state normal
        $w.config.header.clone configure -state normal
        $w.config.header.delete configure -state normal
        $w.display.btn.startStop configure -state normal
    }
    switch $newState {
      "closed" {
          $w.config.header.go configure -state disabled
          $w.config.header.clone configure -state disabled
          $w.config.header.delete configure -state disabled
          $w.display.btn.startStop state !pressed
          $w.display.btn.startStop configure -state disabled

          set ::enginewin_lastengine($id) ""
          set ::enginewin::engConfig($id) {}
          set ::enginewin::position($id) ""
          ::setTitle .engineWin$id "Engine Window"
          $w.config.header.engine set "[tr Engine]:"
          $w.config.options.text configure -state normal
          $w.config.options.text delete 1.0 end
          $w.config.options.text configure -state disabled
      }
      "idle" {
          $w.display.btn.startStop state !pressed
      }
      "run" {
          $w.display.btn.startStop state pressed
      }
    }
    set ::enginewin::engState($id) $newState
}

proc ::enginewin::logEngine {widget tag prefix msg} {
    $widget insert end "[set prefix]$msg\n" $tag
    $widget see end
}

proc ::enginewin::connectEngine {id config} {
    ::enginelist::save $::enginewin::engConfig($id)

    lassign $config name cmd args dir elo time url uci options
    if {[llength $url] != 5} {
        set url [list white SAN word false ""]
    }
    lassign $url scoreside notation pvwrap debugframe netport
    set ::enginewin::engConfig($id) [list $name $cmd $args $dir $elo $time $url $uci {}]

    ::setTitle .engineWin$id "[tr Engine]: $name"
    ::enginewin::updateDisplay $id ""
    ::enginewin::changeDisplayLayout $id scoreside $scoreside
    ::enginewin::changeDisplayLayout $id notation $notation
    ::enginewin::changeDisplayLayout $id wrap $pvwrap
    ::enginewin::changeDisplayLayout $id debug $debugframe
    ::enginewin::changeDisplayLayout $id multipv ""

    .engineWin$id.config.options.text configure -state normal
    .engineWin$id.config.options.text delete 1.0 end
    .engineWin$id.config.options.text insert 1.0 "Connecting..."
    .engineWin$id.config.options.text configure -state disabled
    update idletasks

    switch $uci {
      0 { set protocol "xboard" }
      1 { set protocol "uci" }
      2 { set protocol "network" }
      default { set protocol [list uci xboard] }
    }
    if {[catch {
        ::engine::connect $id [list ::enginewin::callback $id] $cmd $args $protocol
    }]} {
        ERROR::MessageBox
        ::enginelist::delete [lsearch -exact -index 0 $::engines(list) $name]
        ::enginewin::changeState $id closed
        return
    }

    if {[catch {
        if {[string match "auto_*" $netport]} {
            lset ::enginewin::engConfig($id) 6 4 "auto_[::engine::netserver $id 0]"
        } else {
            lset ::enginewin::engConfig($id) 6 4 [::engine::netserver $id $netport]
        }
    }]} {
        ERROR::MessageBox
    }

    if {[llength $options]} {
        ::engine::send $id SetOptions $options
    }
    ::engine::send $id NewGame [list analysis post_pv post_wdl]
}

proc ::enginewin::callback {id msg} {
    lassign $msg msgType msgData
    switch $msgType {
        "InfoConfig" {
            ::enginewin::updateConfig $id $msgData
            ::enginewin::changeState $id idle
        }
        "InfoGo" {
            lassign $msgData ::enginewin::position($id)
            ::enginewin::changeState $id run
        }
        "InfoPV" {
            ::enginewin::updateDisplay $id $msgData
        }
        "InfoReady" {
            ::enginewin::changeState $id idle
        }
        "Disconnected" {
            tk_messageBox -message "The connection with the engine terminated unexpectedly."
            ::enginewin::changeState $id closed
        }
    }
}

proc ::enginewin::addLocal {id} {
    if {[set fName [tk_getOpenFile]] != ""} {
        ::engine [list Name $fName Cmd $fName Dir .]
        lset ::engines(list) end 7 {} ; # Change protocol from xboard to auto
        ::enginewin::connectEngine $id [lindex $::engines(list) end]
    }
}

proc ::enginewin::addNetwork {id} {
    set w .enginewinAddNetwork
    win::createDialog $w
    grid [ttk::label $w.option_name -text "<hostname>:<port>"] [ttk::entry $w.new_value]
    dialogbutton $w.cancel -text "cancel" -command "destroy $w"
    dialogbutton $w.ok -text "OK" -command "
        set host \[$w.new_value get\]
        destroy $w
        ::enginewin::connectEngine $id \[list \$host \$host {} {} {} {} {} 2 \]
    "
    grid $w.cancel $w.ok
}

proc ::enginewin::createConfigWidgets {id options} {
    set w .engineWin$id.config.options.text

    $w insert end "[tr EngineName]:\t"
    ttk::entry $w.name
    $w window create end -window $w.name

    $w insert end "\n[tr EngineCmd]:\t"
    $w insert end "\n" engcmd

    $w insert end "[tr EngineArgs]:\t"
    $w insert end "\n" engargs

    $w insert end "Protocol:\t"
    ttk::combobox $w.protocol -width 9 -values {xboard uci}
    bind $w.protocol <<ComboboxSelected>> "
        lset ::enginewin::engConfig($id) 7 \[ $w.protocol current \]
        lset ::enginewin::engConfig($id) 8 {}
        ::enginewin::connectEngine $id \$::enginewin::engConfig($id)
    "
    $w window create end -window $w.protocol

    foreach {layout label values} [list \
        scoreside "Score perspective" [list engine white] \
        notation "Move lines notation" [list engine SAN] \
        wrap "Wrap move lines" [list word char none] \
        debug "Show debug frame" [list false true]
    ] {
        $w insert end "\n$label:\t"
        ttk::combobox $w.$layout -state readonly -width 9 -values $values
        bind $w.$layout <<ComboboxSelected>> "
            ::enginewin::changeDisplayLayout $id $layout \[ $w.$layout get \]
        "
        $w window create end -window $w.$layout
    }

    $w insert end "\nAccept Network Connections:\t"
    ttk::combobox $w.netd -state readonly -width 9 -values {off on auto_port}
    $w window create end -window $w.netd
    $w insert end "  port:"
    ttk::entry $w.netport -width 6 -validate key -validatecommand { string is integer %P }
    $w window create end -window $w.netport
    bind $w.netd <<ComboboxSelected>> "::enginewin::updateConfigNetd $id $w"
    $w insert end "\n" netclients

    if {[llength $options]} {
        $w insert end "\n"
        ttk::button $w.reset -style Pad0.Small.TButton -text "Reset Options" \
            -command "::enginewin::updateConfigReset $id"
        $w window create end -window $w.reset
    }
    for {set i 0} {$i < [llength $options]} {incr i} {
        lassign [lindex $options $i] name value type default min max var_list internal
        if {$internal} { continue }

        $w insert end "\n$name\t"
        set btn ""
        if {$type eq "button" || $type eq "save" || $type eq "reset"} {
            set btn $name
        } else {
            if {$type eq "spin" || $type eq "slider"} {
                ttk::spinbox $w.value$i -width 12 -from $min -to $max -increment 1
            } elseif {$type eq "combo"} {
                lassign [lsort -decreasing -integer [lmap elem $var_list { string length $elem }]] maxlen
                ttk::combobox $w.value$i -width [incr maxlen] -values $var_list
            } elseif {$type eq "check"} {
                ttk::combobox $w.value$i -width 6 -values {false true}
            } else {
                ttk::entry $w.value$i
                if {$type eq "file" || $type eq "path"} {
                    set btn "..."
                }
            }
            bind $w.value$i <FocusOut> "::enginewin::updateConfigSetOption $id $i $w.value$i"
            $w window create end -window $w.value$i
        }
        if {$btn ne ""} {
            ttk::button $w.button$i -style Pad0.Small.TButton -text $btn \
                -command "::enginewin::updateConfigBtn $id \"$name\" $type"
            $w window create end -window $w.button$i
        } elseif {$type eq "spin" || $type eq "slider"} {
            $w insert end " (Range: $min ... $max)"
        }
    }
    $w configure -tabs [font measure font_Regular "Accept Network Connections:XXXXX"]
}

proc ::enginewin::updateConfig {id msgData} {
    lassign $msgData protocol netclients options
    lassign $::enginewin::engConfig($id) name cmd args dir elo time url uci
    lassign $url scoreside notation pvwrap debugframe netport

    if {$uci == ""} {
        if {[set idx [lsearch -index 0 $options "myname"]] >=0} {
            set name [::enginelist::rename $name [lindex $options $idx 1]]
            lset ::enginewin::engConfig($id) 0 $name
        }
        lset ::enginewin::engConfig($id) 7 [expr { $protocol eq "uci" }]
    }

    .engineWin$id.config.header.engine set $name
    set ::enginewin_lastengine($id) $name

    set w .engineWin$id.config.options.text
    $w configure -state normal

    set oldOptions [lindex $::enginewin::engConfig($id) 8]
    lset ::enginewin::engConfig($id) 8 $options
    if {![winfo exists $w.name] || ![string equal \
        [lmap elem $oldOptions {lreplace $elem 1 1}] \
        [lmap elem $options {lreplace $elem 1 1}] \
    ]} {
        set oldOptions {}
        $w delete 1.0 end
        ::enginewin::createConfigWidgets $id $options
    }

    $w.name delete 0 end
    $w.name insert end "$name"
    set wd [string length $name]
    if {$wd < 24} { set wd 24 } elseif {$wd > 60} { set wd 60 }
    $w.name configure -width $wd
    bind $w.name <FocusOut> "
        if {\[ lindex \$::enginewin::engConfig($id) 0 \] ne \[ $w.name get \]} {
            lset ::enginewin::engConfig($id) 0 \[::enginelist::rename \[ lindex \$::enginewin::engConfig($id) 0 \] \[ $w.name get \]\]
            .engineWin$id.config.header.engine set \[ lindex \$::enginewin::engConfig($id) 0 \]
            set ::enginewin_lastengine($id) \[ lindex \$::enginewin::engConfig($id) 0 \]
        }
    "

    $w replace engcmd.first engcmd.last "$cmd\n" engcmd

    $w replace engargs.first engargs.last "$args\n" engargs

    $w.protocol set $protocol
    $w.protocol configure -state [expr { $uci == 2 ? "disabled" : "readonly" }]

    $w.scoreside set $scoreside

    $w.notation set $notation
    foreach elem [lsearch -all -inline -index 0 $options "san"] {
        # Check if it is an internal option (an xboard engine with san=1)
        if {[lindex $elem 7]} {
            $w.notation set "engine"
            $w.notation configure -state disabled
            ::enginewin::changeDisplayLayout $id notation "engine"
            break
        }
    }

    $w.wrap set $pvwrap

    $w.debug set $debugframe

    if {$netport eq ""} {
        $w.netd set "off"
        $w.netport configure -state disabled
        bind $w.netport <FocusOut> {}
    } elseif {[string match "auto_*" $netport]} {
        $w.netd set "auto_port"
        $w.netport insert 0 [string range $netport 5 end]
        $w.netport configure -state readonly
        bind $w.netport <FocusOut> {}
    } else {
        $w.netd set "on"
        $w.netport insert 0 $netport
        bind $w.netport <FocusOut> "::enginewin::updateConfigNetd $id $w"
    }
    set strclients "\n"
    if {[llength $netclients]} {
        append strclients "Network Connections:"
        foreach client $netclients {
            append strclients "\t$client\n"
        }
    }
    $w replace netclients.first netclients.last $strclients netclients

    set statereset 2
    for {set i 0} {$i < [llength $options]} {incr i} {
        lassign [lindex $options $i] name value type default min max var_list internal
        if {$internal || $type in [list button save reset]} { continue }
        if {$statereset} {
            if {$value ne $default} {
                $w.reset configure -state normal
                set statereset 0
            } elseif {$statereset != 1} {
                $w.reset configure -state disabled
                set statereset 1
            }
        }
        if {$oldOptions ne "" && $value eq [lindex $oldOptions $i 1]} { continue }

        if {$value eq $default} {
            $w tag remove header "$w.value$i linestart" $w.value$i
        } else {
            $w tag add header "$w.value$i linestart" $w.value$i
        }
        if {$type eq "string" || $type eq "file" || $type eq "path"} {
            set wd [string length $value]
            if {$wd < 24} { set wd 24 } elseif {$wd > 60} { set wd 60 }
            $w.value$i configure -width $wd
        }
        $w.value$i delete 0 end
        $w.value$i insert 0 $value

        if {[string equal -nocase $name "multipv"]} {
            ::enginewin::changeDisplayLayout $id multipv [list $i $value $min $max]
        }
    }

    $w configure -state disabled
}

proc ::enginewin::updateConfigSetOption {idEngine idx widgetValue} {
    lassign [lindex [lindex $::enginewin::engConfig($idEngine) 8] $idx] name oldValue type default min max
    set value [$widgetValue get]
    if {$value eq $oldValue} {
        return
    }
    if {$value eq ""} {
        set value $default
    } elseif {$min != "" && $max != ""} {
        if {$value < $min || $value > $max} {
            $widgetValue configure -style Error.TSpinbox
            return
        }
    } elseif {![catch { set values [$widgetValue cget -values] }]} {
        if {[set idx [lsearch -exact -nocase $values $value]] != -1} {
            set value [lindex $values $idx]
        } else {
            $widgetValue configure -style Error.TCombobox
            return
        }
    }
    $widgetValue configure -style {}
    ::engine::send $idEngine SetOptions [list [list $name $value]]
}

proc ::enginewin::updateConfigBtn {idEngine name type} {
    if {$type eq "file"} {
        set value [tk_getOpenFile]
        if {$value == ""} { return }
    } elseif {$type eq "path"} {
        set value [tk_chooseDirectory]
        if {$value == ""} { return }
    } else {
        set value ""
    }
    ::engine::send $idEngine SetOptions [list [list $name $value]]
}

proc ::enginewin::updateConfigReset {id} {
    if {[lindex $::enginewin::engConfig($id) 7] != 2} {
        # Local engine: re-open
        lset ::enginewin::engConfig($id) 8 {}
        ::enginewin::connectEngine $id $::enginewin::engConfig($id)
        return
    }

    set options {}
    foreach option [lindex $::enginewin::engConfig($id) 8] {
        lassign $option name value type default min max var_list internal
        if {! $internal && $value ne $default} {
            lappend options [list $name $default]
        }
    }
    if {[llength $options]} {
        ::engine::send $id SetOptions $options
    }
}

proc ::enginewin::updateConfigNetd {id w} {
    bind $w.netport <FocusOut> {}
    $w.netport configure -state normal -style {}
    set port [$w.netport get]
    $w.netport delete 0 end
    if {[catch {
        switch [$w.netd get] {
          "auto_port" {
              set port [::engine::netserver $id 0]
              $w.netport insert 0 $port
              $w.netport configure -state readonly
              lset ::enginewin::engConfig($id) 6 4 "auto_$port"
          }
          "on" {
              bind $w.netport <FocusOut> "::enginewin::updateConfigNetd $id $w"
              set port [::engine::netserver $id $port]
              $w.netport insert 0 $port
              lset ::enginewin::engConfig($id) 6 4 $port
          }
          default {
              set port [::engine::netserver $id ""]
              $w.netport insert 0 $port
              $w.netport configure -state disabled
              lset ::enginewin::engConfig($id) 6 4 $port
          }
        }
    }]} {
        $w.netport configure -style Error.TEntry
        if {[$w.netd get] ne "on"} {
            $w.netport configure -state disabled
        }
        ERROR::MessageBox
    }
}

proc ::enginewin::updateDisplay {id msgData} {
    lassign $msgData multipv depth seldepth nodes nps hashfull tbhits time score score_type score_wdl pv
    if {$time eq ""} { set time 0 }
    if {$nps eq ""} { set nps 0 }
    if {$hashfull eq ""} { set hashfull 0 }
    if {$tbhits eq ""} { set tbhits 0 }

    set w .engineWin$id.display
    $w.header.info.text configure -state normal
    $w.header.info.text delete 1.0 end
    $w.header.info.text insert end "[tr Time]: " header
    $w.header.info.text insert end [format "%.2f s" [expr {$time / 1000.0}]]
    $w.header.info.text insert end "   [tr Nodes]: " header
    $w.header.info.text insert end [format "%.2f Kn/s" [expr {$nps / 1000.0}]]
    $w.header.info.text insert end "   Hash: " header
    $w.header.info.text insert end [format "%.1f%%" [expr {$hashfull / 10.0}]]
    $w.header.info.text insert end "   TB Hits: " header
    $w.header.info.text insert end $tbhits
    $w.header.info.text configure -state disabled

    $w.pv.lines configure -state normal
    if {$msgData eq ""} {
        $w.pv.lines delete 1.0 end
        $w.pv.lines configure -state disabled
        return
    }

    # Debug:
    # if {[sc_game UCI_currentPos] ne $::enginewin::position($id)} {
    #     puts "PV line for a wrong position:"
    #     puts "Current: [sc_pos fen]"
    #     puts "Was: $::enginewin::position($id)"
    #     puts "Time: $time"
    # }
    lassign [lindex $::enginewin::engConfig($id) 6] scoreside notation
    if {$notation eq "SAN"} {
        set pv [::uci::formatPv $pv]
    }
    if {$score ne ""} {
        if {$scoreside eq "white" && [sc_pos side] eq "black"} {
            set score [expr { - $score }]
        }
        if {$score_type eq "mate"} {
            if {$score >= 0} {
                set score "+M$score"
            } else {
                set score "-M[string range $score 1 end]"
            }
        } else {
            set score [format "%+.2f" [expr {$score / 100.0}]]
            if {$score_type eq "lowerbound" || $score_type eq "upperbound"} {
                lappend extraInfo $score_type
            }
        }
    }
    if {$seldepth ne ""} {
        set depth "$depth/$seldepth"
    }
    if {$score_wdl ne ""} {
        lassign $score_wdl win draw lose
        if {$draw eq ""} { set draw 0 }
        if {$lose eq ""} { set lose 0 }
        lappend extraInfo [format "w%.1f%%" [expr {$win / 10.0}]]
        lappend extraInfo [format "d%.1f%%" [expr {$draw / 10.0}]]
        lappend extraInfo [format "l%.1f%%" [expr {$lose / 10.0}]]
    }
    if {$nodes ne ""} {
        if {$nodes > 100000000} {
            lappend extraInfo [format "%.2fM nodes" [expr {$nodes / 1000000.0}]]
        } else {
            lappend extraInfo [format "%.2fK nodes" [expr {$nodes / 1000.0}]]
        }
    }
    set firstMove [string wordend $pv 0]
    if {$multipv == 1} {
        set line $multipv
        $w.pv.lines tag remove header 1.0 1.end
    } else {
        #TODO: this assumes that the lines are sent in order
        set line $multipv
        $w.pv.lines delete $line.0 end
    }
    $w.pv.lines insert $line.0 "\n"
    $w.pv.lines insert $line.end "$depth"
    $w.pv.lines insert $line.end "\t$score" header
    $w.pv.lines insert $line.end "\t[string range $pv 0 $firstMove]" header
    $w.pv.lines insert $line.end "[string range $pv [incr firstMove] end]" pv
    if {[info exists extraInfo]} {
        $w.pv.lines insert $line.end "  ([join $extraInfo {  }])" pv
    }

    $w.pv.lines configure -state disabled
}
