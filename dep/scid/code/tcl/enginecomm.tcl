########################################################################
# Copyright (C) 2020 Fulvio Benini
#
# This file is part of Scid (Shane's Chess Information Database).
# Scid is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation.

### Manage the communication with chess engines

# Communication takes place through the exchange of messages.
# Every local or remote client can send a message and the reply will be
# broadcasted to all the clients.
# Events:
# connection local or remote    >> InfoConfig
# net client disconnect         >> InfoConfig
# engine crash/local disconnect >> Disconnected
# Messages:
# SetOptions >> InfoConfig
# NewGame    >> InfoReady
# StopGo     >> InfoReady
# Go         >> InfoGo
#            >> the engine will repeatedly send InfoPV replies until a new
#               message is received or one of the Go's limits is reached.


namespace eval engine {}

# Sets the hooks for logging the received and sent i/o (default: none).
# Should be invoked before connecting to the engine.
proc ::engine::setLogCmd {id {recv ""} {send ""}} {
    set ::engconn(logRecv_$id) $recv
    set ::engconn(logSend_$id) $send
}

# Starts a new engine process or connects to a remote engine.
# If protocols is not provided uses "uci" first and after 3s "xboard".
# If protocols is "network" opens a socket (indicated as host:port).
# Messages from the engine will be sent to @p callback.
# An exception is raised in case of error.
proc ::engine::connect {id callback exe_or_host args {protocols {uci xboard}}} {
    if {![info exists ::engconn(logSend_$id)]} {
        error "Set the log commands with ::engine::setLogCmd"
    }
    ::engine::close $id
    if {$protocols eq "network"} {
        set channel [socket {*}[split $exe_or_host :]]
    } else {
        set channel [open "| [list $exe_or_host] $args" "r+"]
    }
    ::engine::init_ $id $channel $callback
    chan configure $channel -buffering line -blocking 0
    ::engine::handshake_ $id $protocols
    chan event $channel readable "::engine::onMessages_ $id $channel"
}

# If @p port is empty stops accepting network connection. Otherwise accept network
# connection on the specified port (if port is 0 use an automatic port).
# An exception is raised in case of error.
# Returns the listening port (empty if none).
proc ::engine::netserver {id {port ""}} {
    ::engine::closeServer_ $id
    if {$port == ""} {
        return ""
    }
    set ::engconn(serverchannel_$id) \
        [socket -server [list ::engine::connectd_ $id] $port]

    set sockname [chan configure $::engconn(serverchannel_$id) -sockname]
    return [lindex $sockname 2]
}

# Close the engine
proc ::engine::close {id} {
    if {[info exists ::engconn(channel_$id)]} {
        chan event $::engconn(channel_$id) readable {}
        if {$::engconn(protocol_$id) ne "network"} {
            if {$::engconn(waitReply_$id) eq "Go"} {
                {*}$::engconn(StopGo$id)
            }
            ::engine::rawsend $id "quit"
        }
        ::engine::destroy_ $id
    }
}

# Sends a message to the engine.
# If the engine is local and a reply to a previous message is expected,
# the message is queued and sent after the reply has arrived.
# Sending a message also cancels queued "Go" messages.
# The "StopGo" message is always sent immediately, but only if the engine is thinking.
proc ::engine::send {id msg {msgData ""}} {
    if {![info exists ::engconn(channel_$id)]} {
        error "The engine is not open"
    }

    if {$::engconn(protocol_$id) eq "network"} {
        ::engine::rawsend $id [list $msg $msgData]
        return
    }
    if {$::engconn(waitReply_$id) eq "Go"} {
        set ::engconn(waitReply_$id) "StopGo"
        {*}$::engconn(StopGo$id)
    }
    if {[set idx [lsearch -index 0 $::engconn(sendQueue_$id) "Go"]] != -1} {
        set ::engconn(sendQueue_$id) [lreplace $::engconn(sendQueue_$id) $idx $idx]
    }
    if {$msg eq "StopGo"} {
        set ::engconn(sendQueue_$id) [linsert $::engconn(sendQueue_$id) 0 [list $msg $msgData]]
    } else {
        lappend ::engconn(sendQueue_$id) [list $msg $msgData]
    }
    if {$::engconn(waitReply_$id) == ""} {
        ::engine::done_ $id
    }
}

proc ::engine::init_ {id channel callback} {
    set ::engconn(protocol_$id) {}
    set ::engconn(callback_$id) $callback
    set ::engconn(channel_$id) $channel
    set ::engconn(serverchannel_$id) {}
    set ::engconn(netclients_$id) {}
    set ::engconn(waitReply_$id) {}
    set ::engconn(sendQueue_$id) {}

    set ::engconn(options_$id) {}
}

proc ::engine::destroy_ {id} {
    after cancel "::engine::done_ $id"

    chan close $::engconn(channel_$id)
    ::engine::closeServer_ $id

    unset ::engconn(protocol_$id)
    unset ::engconn(callback_$id)
    unset ::engconn(channel_$id)
    unset ::engconn(serverchannel_$id)
    unset ::engconn(netclients_$id)
    unset ::engconn(waitReply_$id) ; # the message to be answered
    unset ::engconn(sendQueue_$id) ; # the queue of messages waiting to be sent

    # When the engine's output is parsed its options and PV infos are stored in this vars:
    unset ::engconn(options_$id)
    unset -nocomplain ::engconn(InfoPV_$id)

    unset -nocomplain ::engconn(nextHandshake_$id)

    # Functions that converts messages to uci or xboard.
    unset -nocomplain ::engconn(SetOptions$id)
    unset -nocomplain ::engconn(NewGame$id)
    unset -nocomplain ::engconn(Go$id)
    unset -nocomplain ::engconn(StopGo$id)
    unset -nocomplain ::engconn(parseline$id)
}

proc ::engine::closeServer_ {id} {
    if {$::engconn(serverchannel_$id) != ""} {
        chan close $::engconn(serverchannel_$id)
    }
    set ::engconn(serverchannel_$id) ""
    foreach netchannel $::engconn(netclients_$id) {
        chan close [lindex $netchannel 0]
    }
    set ::engconn(netclients_$id) {}
}

proc ::engine::handshake_ {id protocols} {
    set ::engconn(protocol_$id) [lindex $protocols 0]
    switch $::engconn(protocol_$id) {
      "uci" {
        set ::engconn(SetOptions$id) [list ::uci::sendOptions $id]
        set ::engconn(NewGame$id) [list ::uci::sendNewGame $id]
        set ::engconn(Go$id) [list ::uci::sendGo $id]
        set ::engconn(StopGo$id) [list ::engine::rawsend $id "stop"]
        set ::engconn(parseline$id) "::uci::parseline"

        set ::engconn(waitReply_$id) "hello"
        ::engine::rawsend $id "uci"
      }
      "xboard" {
        set ::engconn(SetOptions$id) [list ::xboard::sendOptions $id]
        set ::engconn(NewGame$id) [list ::xboard::sendNewGame $id]
        set ::engconn(Go$id) [list ::xboard::sendGo $id]
        set ::engconn(StopGo$id) [list ::xboard::sendStopGo $id]
        set ::engconn(parseline$id) "::xboard::parseline"

        set ::engconn(waitReply_$id) "hello"
        ::engine::rawsend $id "xboard"
        ::engine::rawsend $id "protover 2"
      }
      "network" {
      }
      default {
        error "Unknown engine protocol"
      }
    }
    if {[llength $protocols] > 1} {
        set next [list [lrange $protocols 1 end]]
        set ::engconn(nextHandshake_$id) [after 2000 ::engine::handshake_ $id $next]
    } else {
        unset -nocomplain ::engconn(nextHandshake_$id)
        after 2000 "::engine::done_ $id"
    }
}

# Accept a network connection and creates the corresponding channel.
proc ::engine::connectd_ {id channel clientaddr clientport} {
    lappend ::engconn(netclients_$id) [list $channel $clientaddr $clientport]
    chan configure $channel -buffering line -blocking 0
    chan event $channel readable "::engine::forwardNetMsg_ $id $channel"
    ::engine::replyInfoConfig $id
}

# Forward the messages received from the network.
# When a network client disconnects a InfoConfig message is sent to the local
# callback and the other network clients.
proc ::engine::forwardNetMsg_ {id channel} {
    chan event $channel readable {}

    # A disconnected channel creates a readable event with no input
    if {[chan eof $channel]} {
        chan close $channel
        set idx [lsearch -exact -index 0 $::engconn(netclients_$id) $channel]
        set ::engconn(netclients_$id) [lreplace $::engconn(netclients_$id) $idx $idx]
        ::engine::replyInfoConfig $id
        return
    }
    while {[set msg [chan gets $channel]] != ""} {
        ::engine::send $id {*}$msg
    }
    chan event $channel readable "::engine::forwardNetMsg_ $id $channel"
}

# Reads a line from the local engine or a reply message from a network engine.
# Parse local input accordingly to the engine protocol and sends the replies.
proc ::engine::onMessages_ {id channel} {
    chan event $channel readable {}

    # A disconnected channel creates a readable event with no input
    if {[chan eof $channel]} {
        ::engine::closeServer_ $id
        ::engine::reply $id [list Disconnected ""]
        ::engine::destroy_ $id
        return
    }
    while {[set line [chan gets $channel]] != ""} {
        if {$::engconn(logRecv_$id) != ""} {
            {*}$::engconn(logRecv_$id) $line
        }
        if {$::engconn(protocol_$id) eq "network"} {
            ::engine::reply $id $line
        } elseif {[$::engconn(parseline$id) $id $line]} {
            ::engine::done_ $id
        }
        if {[info exists ::engconn(InfoPV_$id)]} {
            if {$::engconn(waitReply_$id) ne "StopGo"} {
                ::engine::reply $id [list InfoPV $::engconn(InfoPV_$id)]
            }
            unset ::engconn(InfoPV_$id)
        }
    }
    chan event $channel readable "::engine::onMessages_ $id $channel"
}

proc ::engine::done_ {id} {
    after cancel "::engine::done_ $id"
    switch $::engconn(waitReply_$id) {
        "hello" {
            if {[info exists ::engconn(nextHandshake_$id)]} {
                after cancel $::engconn(nextHandshake_$id)
                unset ::engconn(nextHandshake_$id)
            }
            ::engine::replyInfoConfig $id
         }
        "SetOptions" { ::engine::replyInfoConfig $id }
        "NewGame" { ::engine::reply $id [list InfoReady ""] }
    }
    set ::engconn(waitReply_$id) ""

    while { [llength $::engconn(sendQueue_$id)] } {
        lassign [lindex $::engconn(sendQueue_$id) 0] msg msgData
        set ::engconn(sendQueue_$id) [lrange $::engconn(sendQueue_$id) 1 end]

        if {$msg eq "StopGo"} {
            # The "StopGo" message was already sent in ::engine::send
            ::engine::reply $id [list InfoReady ""]
            continue
        }
        set ::engconn(waitReply_$id) $msg
        if {$msgData eq ""} {
            {*}$::engconn($msg$id)
        } else {
            {*}$::engconn($msg$id) $msgData
        }
        if {$msg eq "Go"} {
            # Immediately send an InfoGo reply
            ::engine::reply $id [list InfoGo $msgData]
        }
        break
    }
}

# Sends a reply to the local callback and all the remote clients.
proc ::engine::reply {id msg} {
    {*}$::engconn(callback_$id) $msg
    foreach netchannel $::engconn(netclients_$id) {
        chan puts [lindex $netchannel 0] $msg
    }
}

proc ::engine::replyInfoConfig {id} {
    ::engine::reply $id [list InfoConfig \
        [list $::engconn(protocol_$id) $::engconn(netclients_$id) $::engconn(options_$id)]]
}

# Helper functions used by ::uci and ::xboard
# Return the type of the option
proc ::engine::updateOption {id name value} {
    set idx [lsearch -exact -index 0 $::engconn(options_$id) $name]
    if {$idx != -1} {
        set elem [lindex $::engconn(options_$id) $idx]
        set elem [lreplace $elem 1 1 $value]
        set ::engconn(options_$id) [lreplace $::engconn(options_$id) $idx $idx $elem]
        return [lindex $elem 2]
    }
    return ""
}

proc ::engine::rawsend {n msg} {
    chan puts $::engconn(channel_$n) $msg
    if {$::engconn(logSend_$n) != ""} {
        {*}$::engconn(logSend_$n) $msg
    }
}

namespace eval xboard {}

proc ::uci::sendOptions {id msgData}  {
    foreach option $msgData {
        lassign $option name value
        set type [::engine::updateOption $id $name $value]
        if {$type eq "button"} {
            ::engine::rawsend $id "setoption name $name"
        } else {
            ::engine::rawsend $id "setoption name $name value $value"
        }
    }
    ::engine::rawsend $id "isready"
}

proc ::xboard::sendOptions {id msgData} {
    foreach option $msgData {
        lassign $option name value
        set type [::engine::updateOption $id $name $value]
        if {$name in {memory cores egtpath} } {
            ::engine::rawsend $id "$name $value"
        } elseif {$value eq ""} {
            ::engine::rawsend $id "option $name"
        } else {
            ::engine::rawsend $id "option $name=$value"
        }
    }
    after 200 "::engine::done_ $id"
}

proc ::uci::sendNewGame {id msgData} {
    if {[lsearch -index 0 $::engconn(options_$id) "UCI_AnalyseMode"] != -1} {
        set analyze [expr {"analysis" in $msgData ? "true" :"false"}]
        ::engine::rawsend $id "setoption name UCI_AnalyseMode value $analyze"
    }
    if {[lsearch -index 0 $::engconn(options_$id) "UCI_Chess960"] != -1} {
        set chess960 [expr {"chess960" in $msgData ? "true" :"false"}]
        ::engine::rawsend $id "setoption name UCI_Chess960 value $chess960"
    }
    if {[lsearch -index 0 $::engconn(options_$id) "UCI_ShowWDL"] != -1} {
        set wdl [expr {"post_wdl" in $msgData ? "true" :"false"}]
        ::engine::rawsend $id "setoption name UCI_ShowWDL value $wdl"
    }
    ::engine::rawsend $id "ucinewgame"
    ::engine::rawsend $id "isready"
}

proc ::xboard::sendNewGame {id msgData} {
    ::engine::rawsend $id "new"
    if {"chess960" in $msgData} {
        ::engine::rawsend $id "variant fischerandom"
    }
    ::engine::rawsend $id "force"
    ::engine::rawsend $id [expr {"post_pv" in $msgData ? "post" :"nopost"}]
    ::engine::rawsend $id [expr {"ponder" in $msgData ? "hard" :"easy"}]
    #TODO: ::engine::rawsend $id "computer"
    #TODO: ::engine::rawsend $id "name ..."

    if {[lsearch -index 0 $::engconn(options_$id) "ping"] != -1} {
        ::engine::rawsend $id "ping 1"
    } else {
        after 500 "::engine::done_ $id"
    }
}

proc ::uci::sendGo {id msgData} {
    lassign $msgData position limits
    if {$limits == ""} {
        set limits "infinite"
    } else {
        set limits [join $limits]
    }
    ::engine::rawsend $id $position
    ::engine::rawsend $id "go $limits"
}

proc ::xboard::sendGo {id msgData} {
    lassign $msgData position
    regexp {^position(?: fen)? (.*?) moves(.*)$} $position -> fen moves
    if {$fen eq "startpos"} {
        set fen "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    }
    ::engine::rawsend $id "setboard $fen"
    ::engine::rawsend $id "force"
    set usermove ""
    if {[lsearch -index 0 $::engconn(options_$id) "usermove"] != -1} {
        set usermove "usermove "
    }
    foreach move $moves {
        ::engine::rawsend $id "$usermove$move"
    }
    ::engine::rawsend $id "analyze"
}

proc ::xboard::sendStopGo {id} {
    ::engine::rawsend $id "exit"
    after 500 "::engine::done_ $id"
}

proc ::xboard::parseline {id line} {
    if {[regexp {^\s*(\d+)\s+(-\d+|\d+)\s+(\d+)\s+(\d+)\s+(.*)$} $line -> depth score time nodes pv]} {
        set scoreType "cp"
        if {$score >= 100000} {
            set scoreType mate
            incr score -100000
        } elseif {$score <= -100000} {
            set scoreType mate
            incr score 100000
        }
        set ::engconn(InfoPV_$id) [list 1 $depth {} $nodes {} {} {} $time $score $scoreType {} $pv]
        return 0
    }

    if {[string match "pong *" $line]} {
        return 1
    }

    if {[string match "feature *" $line]} {
        set line [string range $line 8 end]
        foreach {feat name default} [regexp -all -inline {(\w+)\s*=\s*("[^"]*"|\d+)} $line] {
            set default [string trim $default \"]
            set type {}
            set min {}
            set max {}
            set var {}
            if {$name eq "option"} {
                set internal 0
                # everything before " -" is considered the name
                lassign [regexp -inline {^(.*?)\s+-(\w+)\s*(.*)$} $default] -> name type extra
                if {$type eq "check" || $type eq "spin" || $type eq "slider"} {
                    lassign [split $extra] default min max
                } elseif {$type eq "string" || $type eq "file" || $type eq "path"} {
                    set default $extra
                } elseif {$type eq "combo"} {
                    set var [split [string map [list " /// " \0] $extra] \0]
                    set idx [lsearch $var {\**}]
                    if {$idx >= 0} {
                        set default [string range [lindex $var $idx] 1 end]
                        lset var $idx $default
                    } else {
                        set default [lindex $var 0]
                    }
                } elseif {$type eq "button" || type eq "save"} {
                    set default ""
                } else {
                    # Unknown type: ignore
                    set type ""
                }
            } else {
                if {$name in {done ping setboard san usermove nps time reuse memory smp \
                              variants name myname egt } } {
                    set internal 1
                    ::engine::rawsend $id "accepted $name"
                } else {
                    ::engine::rawsend $id "rejected $name"
                    continue
                }
                if {$name eq "done"} {
                    after cancel "::engine::done_ $id"
                    if {$default} {
                        return 1
                    }
                }
                if {$name eq "time" || $name eq "reuse"} {
                    set default [expr {! $default }]
                }
                if {$default == 0} {
                    continue
                }
                if {$name eq "memory"} {
                    set internal 0
                    set type spin
                    set default 1
                    set min 1
                    set max 2147483646
                } elseif {$name eq "smp"} {
                    set internal 0
                    set name "cores"
                    set type spin
                    set default 1
                    set min 1
                    set max 2147483646
                } else {
                    set type "string"
                }
            }
            if {$name ne "" && $type ne ""} {
                lappend ::engconn(options_$id) \
                    [list $name $default $type $default $min $max $var $internal]
            }
        }
        return 0
    }

    #unknown
    return 0
}

proc ::uci::parseline {id line} {
    if {[string match "info *" $line]} {
        set beginPV [string first " pv " $line]
        if {$beginPV < 0} {
            return 0
        }
        set endPV end
        set pv [string range $line [expr {$beginPV + 4}] end]
        set tokens [list multipv depth seldepth nodes nps hashfull tbhits time score \
                         currmove currmovenumber currline cpuload string refutation]
        foreach token $tokens {
            set nextToken [string first $token $pv]
            if {$nextToken >= 0} {
                set endPV [expr {$beginPV + 3 + $nextToken}]
                set pv [string trim [string range $line [expr {$beginPV + 4}] $endPV]]
                break
            }
        }
        set ::engconn(InfoPV_$id) [list 1 {} {} {} {} {} {} {} {} {} {} $pv]
        set idx -1
        foreach elem [split [string replace $line $beginPV $endPV]] {
            if {[string is integer -strict $elem]} {
                if {$idx >= 0 && $idx <= 8} {
                    lset ::engconn(InfoPV_$id) $idx $elem
                } elseif {$idx == 10} {
                    lset ::engconn(InfoPV_$id) $idx end+1 $elem
                }
            } else {
                if {$idx == 8 && $elem in {cp mate lowerbound upperbound wdl}} {
                    if {$elem eq "wdl"} {
                        set idx 10
                    } else {
                        lset ::engconn(InfoPV_$id) 9 $elem
                    }
                } else {
                    set idx [lsearch -exact $tokens $elem]
                }
            }
        }
        return 0
    }

    if {[string match "bestmove*" $line]} {
        return 1
    }

    if {$line eq "readyok" || $line eq "uciok"} {
        return 1
    }

    if {[string match "option *" $line]} {
        set tokens {name type default min max var}
        set name {}
        set type {}
        set default {}
        set min {}
        set max {}
        set var {}

        set unknown {}
        set currToken "unknown"
        foreach word [split $line] {
            if {[set idx [lsearch -exact $tokens $word]] != -1} {
                if {$word ne "var"} {
                    # remove the tokens that should appear only once
                    set tokens [lreplace $tokens $idx $idx]
                }
                set currToken $word
            } else {
                lappend $currToken $word
            }
        }
        set internal [expr {$name in {UCI_AnalyseMode UCI_Chess960 UCI_ShowWDL}}]
        if {$type eq "string"} {
            if {[string match -nocase "*file*" $name]} {
                set type "file"
            } elseif {[string match -nocase "*path*" $name]} {
                set type "path"
            }
        }
        lappend ::engconn(options_$id) [list [join $name] [join $default] \
            [join $type] [join $default] [join $min] [join $max] $var $internal]
        return 0
    }

    if {[string match "id name *" $line]} {
        set name [string range $line 8 end]
        lappend ::engconn(options_$id) [list myname $name string $name {} {} {} 1]
        return 0
    }

    #unknown
    return 0
}
