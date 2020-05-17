

# thousands, percentFormat:
#   Functions to format integer numbers.
#   thousands inserts the thousands separator (usually "," or ".") for
#   every three digits before the decimal separator in the number.
#   percentFormat does the same as thousands, but also adds a percentage.
#   If "kilo" is true, numbers >= 100,000 are divided by 1000 and have
#   the unit "K" appended while values over 1 million appear as "1.00M"
#
proc ::utils::thousands {n {kilo 0}} {
  global locale
  set commaChar [string index $locale(numeric) 1]
  set unit ""
  if {$kilo} {
    if {$n >= 1000000} {
      set decimalChar [string index $locale(numeric) 0]
      set decimalPart [format "%02d" [expr {(int($n / 10000)) % 100}]]
      set n [expr {int($n) / 1000000}]
      set unit "${decimalChar}${decimalPart}M"
    } elseif {$n >= 100000} {
      set unit "K"
      set n [expr {int($n / 1000)} ]
    }
  }
  if {$commaChar == ""} { return "$n$unit" }
  while {[regsub {^([-+]?[0-9]+)([0-9][0-9][0-9])} $n "\\1$commaChar\\2" n]} {}
  return "$n$unit"
}

proc ::utils::percentFormat {num denom} {
  # Ensure denominator is not zero:
  if {$denom == 0} {set denom 1}
  return "[::utils::thousands $num] ([expr $num * 100 / $denom]%)"
}

