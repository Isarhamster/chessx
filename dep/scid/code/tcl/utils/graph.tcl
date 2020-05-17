# utils/graph.tcl: Graph plotting package for Scid.
#

namespace eval ::utils::graph {}

# Configuration options, specific to each graph:
#
#  -width:     width of graph in canvas units.
#  -height:    height of graph in canvas units.
#  -xtop:      x-coord of top-left graph corner in canvas.
#  -ytop:      y-coord of top-left graph corner in canvas.
#  -background: background color in graph.
#  -font:      font of axis text.
#  -textcolor: color of axis text.
#  -ticksize:  length of ticks on axes, in canvas units.
#  -tickcolor: color to draw x-axis and y-axis ticks.
#  -textgap:   distance from graph border to text, in canvas units.
#  -xtick:     distance between x-axis ticks, in graph units.
#  -ytick:     distance between y-axis ticks, in graph units.
#  -xlabels, -ylabels: lists of {value,label} pairs to print on each axis.
#              If a list has no pairs, values are printed at each tick.
#  -xmin, -xmax, -ymin, -ymax:  minimum/maximum graph units to plot.
#  -canvas:    canvas to plot the graph in.
#  -vline, -hline: list of vertical/horizontal lines to plot. Each
#              element is a list of four items: {color width type value}
#              where color is the line color, width is its width in
#              pixels, type is "each" or "at", and value is the value.
#  -brect: list of background rectangles. Each element is a list of 5 items:
#              the graph coordinates of a rectangle, and its color.
#
set ::utils::graph::_options(graph) {
  width height xtop ytop background font ticksize textgap xtick ytick
  xmin xmax ymin ymax canvas vline hline textcolor tickcolor
  xlabels ylabels brect
}
set ::utils::graph::_defaults(graph) \
  { -width 400 -height 300 -xtop 50 -ytop 30 -ticksize 5 -textgap 2 \
    -xtick 5 -ytick 5 -tickcolor black -font fixed -background white \
      -canvas {} -hline {} -vline {} -textcolor black \
      -xlabels {} -ylabels {} -brect {} }

# Data options, specific to each data set within a graph:
#
#   -points:  1 to display data points.
#   -lines:   1 to display data line.
#   -bars:    1 to display vertical bars.
#   -color:   color to display points, lines and bars in.
#   -outline: color for outline of bars or points. Not used for lines.
#   -radius:  radius of points in canvas units.
#   -linewidth: width of line in canvas units.
#   -barwidth:  width of bars -- in GRAPH units, NOT canvas units.
#   -key:     key name to print by line.
#   -coords:  actual data to plot; should be a list containing an
#             EVEN number of numeric values.
#
set ::utils::graph::_options(data) {
  points lines bars color outline radius linewidth barwidth coords key
}
set ::utils::graph::_defaults(data) \
  { -points 0 -lines 1 -bars 0 -color red -outline black -radius 2 \
    -linewidth 1 -barwidth 1.0 -key {} -coords {} }

set ::utils::graph::_graphs {}
array set ::utils::graph::_data {}


# create:
#    Create a new graph. Sets up the graph configuration and creates a
#    new proc (in the global namespace) with the same name as the graph.
#
proc ::utils::graph::create args {
  set graph [lindex $args 0]
  lappend ::utils::graph::_graphs $graph
  
  # Remove any existing data for this graph name:
  foreach key [array names ::utils::graph::_data] {
    if {[string match "$graph,*" $key]} { unset ::utils::graph::_data($key) }
  }
  set ::utils::graph::_data($graph,sets) {}

  set args [concat graph $graph $::utils::graph::_defaults(graph) [lrange $args 1 end]]
  set extraArgs [eval "::utils::graph::_configure $args"]
  if {$extraArgs != ""} {
    error "Unrecognised arguments: $extraArgs"
  }
  return $graph
}


# delete:
#    Removes all privately stored information about a graph.
#
proc ::utils::graph::delete {graph} {
  # Remove from the list of available graphs:
  set index [lindex $::utils::graph::_graphs $graph]
  if {$index < 0} { return }
  set ::utils::graph::_graphs [lreplace $::utils::graph::_graphs $index $index]
  # Remove all configuration data for the graph:
  foreach key [array names ::utils::graph::_data] {
    if {[string match "$graph,*" $key]} {
      unset ::utils::graph::_data($key)
    }
  }
}


# isgraph:
#    Returns true if the named graph exists.
#
proc ::utils::graph::isgraph {graph} {
  if {[lsearch $::utils::graph::_graphs $graph] >= 0} { return 1 }
  return 0
}


# data:
#    Adds a new data set to the graph, or modifies an existing one.
#
proc ::utils::graph::data args {
  variable _data
  variable _defaults
  set graph [lindex $args 0]
  set dataset [lindex $args 1]

  set args [concat data $graph,$dataset $_defaults(data) \
              [lrange $args 2 end]]

  set extraArgs [eval "::utils::graph::_configure $args"]
  if {$extraArgs != ""} {
    error "Unrecognised graph data options: $extraArgs"
  }

  set marklist $_data($graph,sets)
  if {[lsearch -exact $marklist $dataset] < 0} {
    lappend _data($graph,sets) $dataset
  }

  set datalength 0
  set ncoords [llength $_data($graph,$dataset,coords)]
  if {$ncoords % 2 != 0} {
    error "Error: coordinates list must have an even length"
  }

  # Redraw graph: do we want to do this here?
  #::utils::graph::redraw $graph
}


# cget:
#    Return a stored attribute of a graph.
#
proc ::utils::graph::cget {graph opt} {
  variable _data
  # Remove initial "-" if necessary:
  if {[string index $opt 0] == "-"} { set opt [string range $opt 1 end] }

  # If asking for axmin/axmax/aymin/aymax, set ranges first:
  if {[string match "a?m??" $opt]} { ::utils::graph::set_range $graph }

  if {! [info exists _data($graph,$opt)]} {
    error "No such graph option: $opt"
  }
  return $_data($graph,$opt)
}

# configure:
#    Modify stored attributes for a graph.
#
proc ::utils::graph::configure args {
  set newargs [concat "graph" [lindex $args 0] [lrange $args 1 end]]
  eval "::utils::graph::_configure $newargs"
}


# _configure:
#    Handle configuration of both the graph, and individual data sets.
#    The first arg (type) should be "graph" or "data". The second should
#    be a graph name for graph configuration, or a "graph,set" pair
#    for dataset configuration.
#
proc ::utils::graph::_configure args {
  variable _data
  set type [lindex $args 0]
  set dataset [lindex $args 1]
  set args [lrange $args 2 end]

  set optionList $::utils::graph::_options($type)
  set option {}

  if {[llength $args] % 2} { error "Error: odd-length options list: $args" }

  for {set i 0} {$i < [llength $args]} {incr i 2} {
    set option [lindex $args $i]
    if {[string index $option 0] != "-"} { return [lrange $args $i end] }
    set option [string range $option 1 end]
    if {[lsearch $optionList $option] >= 0} {
      set _data($dataset,$option) [lindex $args [expr {$i + 1}]]
    }
  }
}

# redraw:
#    Redraw the entire graph, axes and data.
#
proc ::utils::graph::redraw {graph} {
  if {! [::utils::graph::isgraph $graph]} { error "$graph: no such graph" }
  if {! [info exists ::utils::graph::_data($graph,canvas)]} { return }
  $::utils::graph::_data($graph,canvas) delete -withtag g$graph
  ::utils::graph::plot_axes $graph
  ::utils::graph::plot_data $graph
}

# plot_axes:
#    Replot the graph axes.
#
proc ::utils::graph::plot_axes {graph} {
  variable _data
  # Set ranges and scaling factors:
  ::utils::graph::set_range $graph
  ::utils::graph::rescale $graph

  set xmin $_data($graph,axmin)
  set xmax $_data($graph,axmax)
  set ymin $_data($graph,aymin)
  set ymax $_data($graph,aymax)

  set xminc [xmap $graph $xmin]
  set xmaxc [xmap $graph $xmax]
  set yminc [ymap $graph $ymin]
  set ymaxc [ymap $graph $ymax]
  #Klimmek: additional space for 6.and 7.number
  set fontsize [font configure font_Small -size]
  if { $ymax > 99999 } { set xminc [expr {$xminc + $fontsize}] }
  if { $ymax > 999999 } { set xminc [expr {$xminc + $fontsize}] }

  set canvas $_data($graph,canvas)
  set tag g$graph

  # Extract the graph attributes we will need to use:
  foreach attr {ticksize font textcolor tickcolor textgap \
                  xtick ytick xlabels ylabels} {
    set $attr $_data($graph,$attr)
  }

  $canvas create rectangle $xminc $yminc $xmaxc $ymaxc -outline $tickcolor \
    -fill $_data($graph,background) -tag $tag

  set brect $_data($graph,brect)
  for {set i 0} {$i < [llength $brect]} {incr i} {
    set item [lindex $brect $i]
    set x1 [xmap $graph [lindex $item 0]]
    set y1 [ymap $graph [lindex $item 1]]
    set x2 [xmap $graph [lindex $item 2]]
    set y2 [ymap $graph [lindex $item 3]]
    if {$x1 < $xminc} { set x1 $xminc }
    if {$x1 > $xmaxc} { set x1 $xmaxc }
    if {$x2 < $xminc} { set x2 $xminc }
    if {$x2 > $xmaxc} { set x2 $xmaxc }
    if {$y1 > $yminc} { set y1 $yminc }
    if {$y1 < $ymaxc} { set y1 $ymaxc }
    if {$y2 > $yminc} { set y2 $yminc }
    if {$y2 < $ymaxc} { set y2 $ymaxc }
    $canvas create rectangle $x1 $y1 $x2 $y2 -fill [lindex $item 4] -width 0 \
      -tag $tag
  }

  # Plot vertical guide lines:
  foreach vline $_data($graph,vline) {
    set color [lindex $vline 0]
    set width [lindex $vline 1]
    set type [lindex $vline 2]
    set inc [lindex $vline 3]
    set xminvalue [xmap $graph $xmin]
    set xmaxvalue [xmap $graph $xmax]
    if {$type == "at"} {
      # Plot just one line:
      set xvalue [xmap $graph $inc]
      if {$xvalue != $xminvalue  &&  $xvalue != $xmaxvalue} {
        $canvas create line $xvalue $yminc $xvalue $ymaxc -width $width \
          -fill $color -tag $tag
      }
    } elseif {$inc > 0} {
      # Plot a line at each multiple of "inc" units:
      set x [expr {int($xmin/$inc) * $inc + $inc}]
      while {$x < $xmax} {
        set xvalue [xmap $graph $x]
        if {$xvalue != $xminvalue  &&  $xvalue != $xmaxvalue} {
          $canvas create line $xvalue $yminc $xvalue $ymaxc -width $width \
            -fill $color -tag $tag
        }
        set x [expr {$x + $inc}]
      }
    }
  }

  # Plot horizontal guide lines:
  foreach hline $_data($graph,hline) {
    set color [lindex $hline 0]
    set width [lindex $hline 1]
    set type [lindex $hline 2]
    set inc [lindex $hline 3]
    set yminvalue [ymap $graph $ymin]
    set ymaxvalue [ymap $graph $ymax]
    if {$type == "at"} {
      set yvalue [ymap $graph $inc]
      if {$yvalue != $yminvalue  &&  $yvalue != $ymaxvalue} {
        $canvas create line $xminc $yvalue $xmaxc $yvalue -width $width \
          -fill $color -tag $tag
      }
    } elseif {$inc > 0} {
      set y [expr {int($ymin/$inc) * $inc + $inc}]
      while {$y < $ymax} {
        set yvalue [ymap $graph $y]
        if {$yvalue != $yminvalue  &&  $yvalue != $ymaxvalue} {
          $canvas create line $xminc $yvalue $xmaxc $yvalue -width $width \
            -fill $color -tag $tag
        }
        set y [expr {$y + $inc}]
      }
    }
  }

  # Plot x ticks and y ticks:
  set nxlabels [llength $xlabels]
  set nylabels [llength $ylabels]

  if {$xtick > 0} {
    set x [expr {int($xmin/$xtick) * $xtick}]
    while {$x < $xmin} { set x [expr {$x + $xtick}] }
    while {$x <= $xmax} {
      set xc [xmap $graph $x]
      $canvas create line $xc $yminc $xc [expr {$yminc - $ticksize}] \
        -tag $tag -fill $tickcolor
      $canvas create line $xc $ymaxc $xc [expr {$ymaxc + $ticksize}] \
        -tag $tag -fill $tickcolor
      if {$nxlabels == 0} {
        $canvas create text $xc [expr {$yminc + $textgap}] -font $font \
          -text [::utils::graph::round $x] -anchor n -tag $tag -fill $textcolor
      }
      set x [expr {$x + $xtick}]
    }
  }
  for {set i 0} {$i < $nxlabels} {incr i} {
    set label [lindex $xlabels $i]
    set x [lindex $label 0]
    set text [lindex $label 1]
    set xc [xmap $graph $x]
    $canvas create text $xc [expr {$yminc + $textgap}] -font $font \
      -text $text -anchor n -tag $tag -fill $textcolor -justify center
  }

  if {$ytick > 0} {
    set y [expr {int($ymin/$ytick) * $ytick}]
    while {$y < $ymin} { set y [expr {$y + $ytick}] }
    while {$y <= $ymax} {
      set yc [ymap $graph $y]
      $canvas create line $xminc $yc [expr {$xminc + $ticksize}] $yc \
        -tag $tag -fill $tickcolor
      $canvas create line [expr {$xmaxc - $ticksize}] $yc $xmaxc $yc \
        -tag $tag -fill $tickcolor
      if {$nylabels == 0} {
        $canvas create text [expr {$xminc - $textgap}] $yc -font $font \
          -text [::utils::graph::round $y] -anchor e -tag $tag -fill $textcolor
      }
      set y [expr {$y + $ytick}]
    }
  }
  for {set i 0} {$i < $nylabels} {incr i} {
    set label [lindex $ylabels $i]
    set y [lindex $label 0]
    set text [lindex $label 1]
    set yc [ymap $graph $y]
    $canvas create text [expr {$xminc - $textgap}] $yc -font $font \
      -text $text -anchor e -tag $tag -fill $textcolor
  }
}

# plot_data:
#    Plot the lines/points/bars for each data set in the graph.
#
proc ::utils::graph::plot_data {graph} {
  variable _data
  set canvas $_data($graph,canvas)

  foreach dataset $_data($graph,sets) {
    set color $_data($graph,$dataset,color)
    set outline $_data($graph,$dataset,outline)
    set tag g$graph
    set coords [scale_data $graph $_data($graph,$dataset,coords)]
    set ncoords [expr {[llength $coords] - 1}]

    # Draw key:
    if {$_data($graph,$dataset,key) != ""} {
      set key $_data($graph,$dataset,key)
      if {$ncoords >= 1} {
        set dy 3
        set anchor nw
        set x [expr {[lindex $coords 0] + 3}]
        set y [lindex $coords 1]
        if {$ncoords >= 3} {
          set nexty [lindex $coords 3]
          if {$nexty > $y} { set dy -3; set anchor sw }
        }
        incr y $dy
        catch {$canvas create text $x $y -fill $color -tag $tag \
                 -text $_data($graph,$dataset,key) \
                 -font $_data($graph,font) -anchor $anchor}
      }
    }

    # Plot line:
    if {$_data($graph,$dataset,lines)} {
      # Catch errors drawing line in case the data set contains no data:
      catch {eval "$canvas create line $coords -fill $color \
                   -width $_data($graph,$dataset,linewidth) -tag $tag"}
    }

    # Plot points:
    if {$_data($graph,$dataset,points)} {
      for {set i 0} {$i < $ncoords} {incr i 2} {
        set x [lindex $coords $i]
        set y [lindex $coords [expr {$i + 1}]]
        set p $_data($graph,$dataset,radius)
        $canvas create oval [expr {$x-$p}] [expr {$y-$p}] \
          [expr {$x+$p}] [expr {$y+$p}] \
          -fill $color -outline $outline -width 1 -tag $tag
      }
    }

    # Plot bars:
    if {$_data($graph,$dataset,bars)} {
      set base [ymap $graph $_data($graph,aymin)]
      # type Bars 2: draw from 0 to Value
      if {$_data($graph,$dataset,bars) == 2} {
	  set base [expr {int(($_data($graph,aymax) - 0) * $_data($graph,yfac) + $_data($graph,ytop))}]
      }
      set hwidth [xmap $graph $_data($graph,$dataset,barwidth)]
      set hwidth [expr {$hwidth - [xmap $graph 0]}]
      set hwidth [expr {$hwidth / 2}]
      if {$hwidth < 1} { set hwidth 1 }

      for {set i 0} {$i < $ncoords} {incr i 2} {
        set x [lindex $coords $i]
        set y [lindex $coords [expr {$i + 1}]]
	  if {$_data($graph,$dataset,bars) == 2} {
	      #Bars 2: ajust
	      if { $hwidth > 4 } {
		  set x1 [expr {$x+(($i+1)%2)*$hwidth-1}]
		  set x [expr {$x-($i % 2)*$hwidth+1}]
	      } else { #Bars 2: ajust for small window size: make bar smaller
		  set x1 [expr {$x+(($i+1)%2)*$hwidth}]
		  set x [expr {$x-($i % 2)*$hwidth}]
	      }
	  } else {
	      set x1 [expr {$x+$hwidth}]
	      set x [expr {$x-$hwidth}]
	  }
        $canvas create rectangle \
           $x $y $x1 $base \
          -fill $color -outline $outline -tag $tag
      }
    }
  }
}


# round
#
#    Returns a value n rounded to the nearest integer if it is
#    within 0.1 of n, or to one decimal place otherwise.
#    Used to print axis values to a sensible precision.
#
proc ::utils::graph::round {n} {
  set intn [expr {int($n)}]
  if {[expr {$n - $intn}] < 0.1  &&  [expr {$intn - $n}] < 0.1} {
    return [expr {round($n)}]
  }
  return [expr {double(round($n * 10.0)) / 10.0}]
}


# point_visible
#
#    Returns true if a point (in graph coordinates) is visible given
#    the current display boundaries.
#
proc ::utils::graph::point_visible {graph x y} {
  variable data
  set xmin $_data($graph,xtop)
  set ymin $_data($graph,ytop)
  set xmax [expr {$xmin + $_data($graph,width)}]
  set ymax [expr {$ymin + $_data($graph,height)}]

  if {$x >= $xmin && $x <= $xmax && $y >= $ymin && $y <= $ymax} { return 1 }
  return 0
}


# rescale:
#    Sets the scaling factors used for mapping graph to canvas coordinates.
#
proc ::utils::graph::rescale {graph} {
  variable _data
  set width $_data($graph,width)
  set height $_data($graph,height)
  set xdelta [expr {double($_data($graph,axmax) - $_data($graph,axmin))}]
  set ydelta [expr {double($_data($graph,aymax) - $_data($graph,aymin))}]
  # Ensure deltas are not zero or too close to it:
  if {$xdelta < 0.0001} { set xdelta 0.0001 }
  if {$ydelta < 0.0001} { set ydelta 0.0001 }

  set _data($graph,xfac) [expr {double($width)/$xdelta}]
  set _data($graph,yfac) [expr {double($height)/$ydelta}]
}


# xmap:
#    Map a graph X coordinate to its canvas unit equivalent.
#
proc ::utils::graph::xmap {graph x} {
  variable _data
  return [expr {int(($x - $_data($graph,axmin)) * \
            $_data($graph,xfac) + $_data($graph,xtop))}]
}

# ymap:
#    Map a graph Y coordinate to its canvas unit equivalent.
#
proc ::utils::graph::ymap {graph y} {
  variable _data
if {$y == ""} { error "y is empty" }
  return [expr {int(($_data($graph,aymax) - $y) * \
            $_data($graph,yfac) + $_data($graph,ytop))}]
}

# Xunmap:
#    Transform a canvas unit to its graph X coordinate equivalent.
#
proc ::utils::graph::xunmap {graph cx} {
  variable _data
  return [expr {$_data($graph,axmin) + \
            double($cx - $_data($graph,xtop)) / \
            double($_data($graph,xfac))}]
}

# Yunmap:
#    Transform a canvas unit to its graph Y coordinate equivalent.
#
proc ::utils::graph::yunmap {graph cy} {
  variable _data
  return [expr {$_data($graph,aymax) - \
            double($cy - $_data($graph,ytop)) / \
            double($_data($graph,yfac))}]
}

# scale_data:
#    Transforms an even-sized list of graph coordinates to canvas units.
#
proc ::utils::graph::scale_data {graph coords} {
  set result {}
  for {set i 0} {$i < [llength $coords] - 1} {incr i 2} {
    lappend result [xmap $graph [lindex $coords $i]]
    lappend result [ymap $graph [lindex $coords [expr {$i + 1}]]]
  }
  return $result
}

# set_range:
#    Sets any range boundaries that are not already set for a graph.
#
proc ::utils::graph::set_range {graph} {
  variable _data

  set xmin 1000000000; set xmax -100000000
  set ymin 1000000000; set ymax -100000000

  foreach dataset $_data($graph,sets) {
    set coords $_data($graph,$dataset,coords)
    for {set i 0} {$i < [llength $coords] - 1} {incr i 2} {
      set x [lindex $coords $i]
      set y [lindex $coords [expr {$i + 1}]]

      if {$x < $xmin} { set xmin $x }
      if {$x > $xmax} { set xmax $x }
      if {$y < $ymin} { set ymin $y }
      if {$y > $ymax} { set ymax $y }
    }
  }

  # Set sane values if no data coordinates exist at all:
  if {$xmax < $xmin} { set xmin 0; set xmax 1 }
  if {$ymax < $ymin} { set ymin 0; set ymax 1 }

  set xtick $_data($graph,xtick)
  set ytick $_data($graph,ytick)
  set _data($graph,axmin) [expr {floor($xmin/$xtick) * $xtick}]
  set _data($graph,axmax) [expr {floor($xmax/$xtick) * $xtick + $xtick}]
  set _data($graph,aymin) [expr {floor($ymin/$ytick) * $ytick}]
  set _data($graph,aymax) [expr {floor($ymax/$ytick) * $ytick + $ytick}]

  # Explicitly set boundaries override the detected ranges:
  foreach coord {xmin xmax ymin ymax} {
    if {[info exists _data($graph,$coord)]} {
      set _data($graph,a$coord) $_data($graph,$coord)
    }
  }
}

