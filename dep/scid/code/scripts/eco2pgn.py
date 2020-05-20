#!/usr/bin/env python
#
# eco2pgn
# This file converts the Scid openings classification file (scid.eco)
# and converts it into a PGN file. The main advantage of having it as
# a PGN file is the ability to explore it with the Tree window in Scid.
#
# Usage: eco2pgn scid.eco > scideco.pgn
#
# Contributed by John Wiegley, 2001.08.13
#

import re
import sys

if len(sys.argv) < 2:
    sys.stderr.write("usage: eco2pgn file.eco > file.pgn\n")
    sys.stderr.write("eco2pgn takes a Scid openings classification file,\n")
    sys.stderr.write("and writes it in PGN format to standard output.\n")
    sys.exit(1)

fd = open(sys.argv[1], "r")

while 1:
    line = fd.readline()
    if not line: break
    line = line[:-1]

    if re.match("\s*(#|$)", line): continue

    match = re.match("(\S+)\s+\"([^\"]+)\"(\s+.+)?$", line)
    if match:
        eco, variation, cont = match.groups()

        if not cont:
            cont = ''

        while not cont or cont[-1] != '*':
            cont = cont + ' ' + fd.readline()[:-1]

        if cont[-1] == '*' and cont[-2] != ' ':
            cont = cont[:-1] + ' *'

    print '[ECO "%s"]' % eco
    print '[Variation "%s"]' % variation
    print '[Result "*"]'
    print
    print cont
    print

