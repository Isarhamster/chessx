![Icon](https://github.com/Isarhamster/chessx/raw/master/data/images/chessx-64.png)ChessX
=================================================================

# 1. Description

A free and open source chess database application for Linux, Mac OS X and
Windows.

[ChessX Web Site](http://chessx.sourceforge.net/)

# 2. Features

* Multi-platform. Supports Windows, Linux and Mac OS X
* Load and save PGN files
* Work with multiple databases simultaneously
* Browse chess games, including variations
* Enter moves, variations, comments
* Setup board, copy/paste FEN
* Search in Databases for text or positions
* Display tree of moves for the current position
* Analyze using UCI and Winboard/Xboard Chess engines
* Prepare for openings or opponents
* Training mode (next move is hidden)
* Integrated Stockfish engine
* FICS Support

# 3. Requirements
  
To compile ChessX, you need zlib, qmake and **Qt5 version 5.14.1** or above.

**known issue:** FICS does not work properly with Qt4

# 4. Compilation

Compiling the source is simple. Execute:

     qmake

and then

     make

If the compilation is successful you should be able to run ChessX
executable in **bin** (or **release**) subdirectory. It is called: **chessx** on Linux, **chessx.app** on Mac OS, and **chessx.exe** on Windows

## 4.1 Compilation in Linux x64 systems

On Linux x64, the  following commands are required for successful compilation:

Build the translations

     lrelease-qt5 chessx.pro

Run qmake, specifically qt5 version

     qmake-qt5

Compile with

     make

If compilation finishes successfully, the executable will be found as:

     release/chessx

## 4.2 Compilation of timeseal in Linux x64 systems

In order to compile timeseal, to be used in Linux x64, prepare with

    qmake-qt5 timeseal.pro

then compile with
 
    make

# 5. Problems


You may check your Qt version by running:

     qmake -v

If you erroneously tried to build with Qt3, be sure to recreate
makefiles using:

     qmake -recursive

If your compiler does not support C++11, comment out ```CONFIG += c++11```, see file ***chessx.pro***.

If your Qt libraries are compiled w/o Multimedia, comment out ```CONFIG += sound```, see file **chessx.pro**

Depending on your installed libraries, you may have to experiment with

```
# DEFINES += ZLIB_WINAPI
# LIBS += -lz
# INCLUDEPATH += <path to qt>/QtZlib
```

# 6. Debugging


If you want to help debugging ChessX, rebuild with debug option enabled:

     qmake -recursive -after "CONFIG+=debug"
     make

*************

**License:** GPLv2

:copyright: 2021 -- 2005, Maintained by Jens Nissen and Ejner Borgbjerg <br>
:copyright: 2021 -- 2005, Other developers: Michal Rudolf, Bruno Rizzuti, Marius Roets, Sean Estabrooks, Rico Zenklusen, William Hoggarth, and Heinz Hopfgartner

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
