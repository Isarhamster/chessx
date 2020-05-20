#!/usr/bin/env python

# pgnfix, by John Wiegley <johnw@gnu.org>
# version 1.2, 2001.08.28
#
# This script will take a PGN file (typically gigantic), and perform
# as many normalizations and spelling corrections on the player's
# names as possible.  If there is embedded rating information in the
# player's name, it will be extracted and moved to the correct field.
# If you have a scid spelling.ssp file, spelling corrections will also
# take place.  It will even extract dates that have been mistakenly
# put in the Site or Event field, and move them to the Date field.

import sys
import string
import re

if len(sys.argv) < 2:
    sys.stderr.write("usage: pgnfix file.pgn [spelling.ssp]\n")
    sys.stderr.write("pgnfix cleans up a PGN file, writing clean games\n")
    sys.stderr.write("to stdout and games with errors to stderr.\n")
    sys.exit(1)

# If a field matches one of these values, discard the field altogether
bogus = {
    "Round": "[?-]",
    "Date":  "\?\?\?\?\.\?\?\.\?\?",
    "Site":  "\?",
    "Event": "\?",
}

# The preferred ordering for the known fields
order = [
    "Event",
    "Site",
    "Date",
    "Round",
    "White",
    "WhiteElo",
    "Black",
    "BlackElo",
    "TimeControl",
    "Result",
    "FEN",
    "ECO",
    "Opening",
    "Variation",
]

headers = {}
specials = {}

# Grab all the spelling names
spelling = {}

if len(sys.argv) == 3:
    spell = open(sys.argv[2], "r")
    in_player = 0
    while 1:
        line = spell.readline()
        if not line:
            break
        if re.match("@PLAYER", line):
            in_player = 1
        elif re.match("### END OF PLAYER SECTION", line):
            break
        elif in_player:
            match = re.match("^([A-Z][^,#]+)([^#]*)", line)
            if match:
                full = match.group(0)
                last, first = match.groups()
                first = string.strip(first)
                full  = string.strip(full)
                last  = string.strip(last)
                if first:
                    for name in (full, last,
                                 last + " " + first,
                                 last + ", " + first[0],
                                 last + " " + first[0]):
                        name = string.lower(name)
                        if spelling.has_key(name):
                            del spelling[name]
                        else:
                            spelling[name] = full

fd = open(sys.argv[1], "r")
current = []
bad_lines = []
in_body = 0
error = 0

while 1:
    line = fd.readline()
    if not line:
        break

    if not in_body and re.match("\s*$", line):
        in_body = 1

    if in_body:
        if line[0] == '[':
            in_body = 0
            # We've seen a header line; output whichever game was seen
            # last
            if current:
                out = sys.stdout
                if error:
                    out = sys.stderr
                    error = 0
                out.write(string.join(current, ""))
                current = []
                bad_lines = []
        else:
            if headers:
                for field, value in specials.items():
                    if not headers.has_key(field):
                        headers[field] = value
                for field in order:
                    if headers.has_key(field):
                        current.append('[%s "%s"]\n' % (field, headers[field]))
                        del headers[field]
                for field, value in headers.items():
                    current.append('[%s "%s"]\n' % (field, value))
                current.extend(bad_lines)
                headers = {}
                specials = {}
            current.append(line)
            continue

    # See whether we have a broken header line
    if line[0] == '[' and line[-2] != ']':
        next = fd.readline()
        if line[0] != '[' and line[-2] == ']':
            line = line[:-1] + next
        else:
            # Just skip both
            error = 1
            bad_lines.append(line)
            bad_lines.append(next)
            continue

    match = re.match('\[(\S+)\s+"(.*)"\]', line)
    if not match:
        error = 1
        bad_lines.append(line)
        continue

    field, value = match.groups()
    if value:
        if field in ("Event", "Site"):
            match = re.search("\(?([0-9]{4})\.([0-9]+)\.([0-9]+)\)?", value)
            if match:
                year = int(match.group(3))
                month = int(match.group(2))
                day = int(match.group(1))
                specials["Date"] = "%04d.%02d.%02d" % (year, month, day)
                value = re.sub("\(?([0-9]{4})\.([0-9]+)\.([0-9]+)\)?",
                               "", value)
            match = re.search("\(?([0-9]+)/([0-9]+)/([0-9]+)\)?", value)
            if match:
                year = int(match.group(3))
                day = int(match.group(2))
                month = int(match.group(1))
                if month > 12:
                    t = day
                    day = month
                    month = t
                if year < 100 and year > 20:
                    year = 1900 + year
                if year < 10:
                    year = 2000 + year
                specials["Date"] = "%04d.%02d.%02d" % (year, month, day)
                value = re.sub("\(?([0-9]+)/([0-9]+)/([0-9]+)\)?", "", value)

        value = string.strip(value)

        if field in ("White", "Black"):
            name = re.sub("[MW][0-9]{5}", "", value)

            # Extract any embedded Elo rating
            match = re.search("(^|[, {(])([1-9][0-9]{3})($|[, })])", name)
            if match:
                specials[field + "Elo"] = match.group(2)
                name = re.sub("(^|[ {(])([1-9][0-9]{3})($|[ })])", "", name)

            name = re.sub("(.+?),\s*(.+?)&", "\\2 & \\1", name)

            if not re.search("[A-Z]{4}", name):
                match = re.search("[-A-Za-z.'` _]+"
                                  "(,\s*[-A-Za-z.'` _]+)?(\s*[1-3])?", name)
                if match:
                    name = match.group(0)
                    name = re.sub("_", " ", name)
                    name = re.sub("\.", "", name)
                    name = re.sub("([A-Za-z])([0-9])", "\\1 \\2", name)
                    name = string.strip(name)
                    name = re.sub("^([A-Z]+?)\s*([A-Z][a-z].+)", "\\2, \\1",
                                  name)

                name = re.sub(",\s*([GI]M)?$", "", name)

            name = string.strip(name)
            t = string.lower(name)

            if spelling.has_key(t):
                name = spelling[t]

            headers[field] = name

        elif not bogus.has_key(field) or not re.search(bogus[field], value):
            headers[field] = value

if current:
    out = sys.stdout
    if error:
        out = sys.stderr
        error = 0
    out.write(string.join(current, ""))
    current = []
    bad_lines = []

fd.close()
print

