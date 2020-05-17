

# ::utils::validate::Integer
#
#   Used to check the size of integers in entry boxes.
#
proc ::utils::validate::Integer {maxValue allowQuestionMarks name el op} {
  global $name ${name}_old
  if {[string comp {} $el]} {
    set old  ${name}_old\($el\)
    set name $name\($el\)
  } else {
    set old ${name}_old
  }

  if {$allowQuestionMarks > 0} {
    if {[regexp {^\?*$} [set $name]]} {
      # Accept this value:
      set $old [set $name]
      return
    }
  }

  # Only non-negative integers up to maxValue are allowed, unless the
  # value is negative:
  set allowNegatives 0
  if {$maxValue < 0} {
    set allowNegatives 1
    set maxValue [expr {0 - $maxValue}]
  }

  if {$allowNegatives} {
    if {![regexp {^[-+]?[0-9]*$} [set $name]]} {
      set $name [set $old]
      bell
      return
    }
  } else {
    if {![regexp {^[+]?[0-9]*$} [set $name]]} {
      set $name [set $old]
      bell
      return
    }
  }
  if {[set $name] > $maxValue} {
    set $name [set $old]
    bell
    return
  }
  #if {[expr {0 - [set $name]}] < [expr {0 - $maxValue}]} {
  #  set $name [set $old]
  #  bell
  #  return
  #}
  set $old [set $name]
}



# ::utils::validate::Date
#
#    Used to check the validity of a date string as it is entered.
#
proc ::utils::validate::Date {name el op} {
  global $name ${name}_old
  set old ${name}_old
  if {![sc_info validDate [set $name]]} {
    set $name [set $old]
    bell
    return
  }
  set $old [set $name]
}

# ::utils::validate::Result
#
#    Used to check the validity of a result entrybox value.
#    Result can be empty, "1", "0", "=", or "*".
#
proc ::utils::validate::Result {name el op} {
  global $name ${name}_old
  set old ${name}_old
  if {![regexp {^[1|0|=|\*]?$} [set $name]]} {
    set $name [set $old]
    bell
    return
  }
  set $old [set $name]
}

# ::utils::validate::Alpha
#
#    Used to check that an entrybox contains only letters.
#
proc ::utils::validate::Alpha {name el op} {
  global $name ${name}_old
  set old ${name}_old
  if {![regexp {^[A-Za-z]*$} [set $name]]} {
    set $name [set $old]
    bell
    return
  }
  set $old [set $name]

}

# ::utils::validate::Regexp
#
#    Used to check the validity of an entrybox given a regular expression.
#    Used to verify a file is "a-h", for example.
#
proc ::utils::validate::Regexp {expression name el op} {
  global $name ${name}_old
  set old ${name}_old
  if {![regexp $expression [set $name]]} {
    set $name [set $old]
    bell
    return
  }
  set $old [set $name]
}

# ::utils::validate::roundScale
# will round var to the next integer which is a multiple of tickinterval
# given its current value
proc ::utils::validate::roundScale { var tickinterval value } {
  set $var [expr int($value/$tickinterval ) * $tickinterval]
}

# ::utils::validate::floatScale
# returns a float value with 1 decimal
proc ::utils::validate::floatScale { var tickinterval value } {
  set $var [format "%.1f" [expr ($value/$tickinterval ) * $tickinterval] ]
}
