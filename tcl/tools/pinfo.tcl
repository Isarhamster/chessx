
####################
# Player Info window

namespace eval pinfo {
set playerInfoName ""
set ::eloFromRating 0

proc setupDefaultResolvers { } {
   set optionF ""
   if {[catch {open [scidConfigFile resolvers] w} optionF]} {
      tk_messageBox -title "Scid: Unable to write file" -type ok -icon warning \
         -message "Unable to write options file: [scidConfigFile resolvers]\n$optionF"
   } else {
      puts $optionF "# Scid resolver configuration"
      puts $optionF "# Version: $::scidVersion, $::scidVersionDate"
      puts $optionF "# This file contains commands in the Tcl language format."
      puts $optionF "# If you edit this file, you must preserve valid Tcl"
      puts $optionF "# format or it will not set your Scid options properly."
      puts $optionF "#"
      puts $optionF "# Define how to interpret IDs stored in ssp files. Each ID has to"
      puts $optionF "# have a unique page to link to. It is currently not possible to link"
      puts $optionF "# to more than one page by means of a single ID, or to get more"
      puts $optionF "# than one button by any ID. FIDEID is an exception, see below"
      puts $optionF "#"
      puts $optionF "# Syntax of the following array's list entries:"
      puts $optionF "#"
      puts $optionF "# IDName: the tag to search for e.g. FIDEID searches for"
      puts $optionF "#         %Bio FIDEID -lines in the spellcheck file"
      puts $optionF "#      special IDNames:"
      puts $optionF "#         useFIDEID : Use the FIDEID"
      puts $optionF "#         useNAMEF: Use the name of the player as ID in the order First Name Last Namee.q. Magnus Carlsen"
      puts $optionF "#         useNAMEL: Use the name of the player as ID in the order Last Name,First Name e.q. Carlsen,Magnus"
      puts $optionF "#                 add an additional sign after the F/L to select a delimiter, see examples below "
      puts $optionF "# link target url: %ID% is replaced by the ID, %LANG% is replaced with the selected language code"
      puts $optionF "# icon name: the icons are specified below as base64 encoded images"
      puts $optionF "# Link text: what to display in pinfo window"
      puts $optionF ""
      puts $optionF "#resolver without icons"
      puts $optionF "set idlink(0) {{VIAF} {http://viaf.org/%ID%} {} {VIAF}}"
      puts $optionF "set idlink(1) {{FIDEID} {http://ratings.fide.com/card.phtml?event=%ID%} {} {FIDE}}"
      puts $optionF "set idlink(2) {{ICCFID} {https://www.iccf.com/player?id=%ID%} {} {ICCF}}"
      puts $optionF "set idlink(3) {{PND} {https://portal.dnb.de/opac.htm?query=%ID%\\&method=simpleSearch} {} {PND}}"
      puts $optionF "set idlink(4) {{BCF} {http://ecfgrading.org.uk/new/menu.php?ref=%ID%} {} {ECF}}"
      puts $optionF "#set idlink(5) {{useFIDEID} {https://chess-db.com/public/pinfo.jsp?id=%ID%} {} {chess-db.com}}"
      puts $optionF "#set idlink(6) {{useNAMEF+} {https://tools.wmflabs.org/persondata/index.php?name=%ID%} {} {PS}}"
      puts $optionF "#set idlink(7) {{useNAMEF_} {http://%LANG%.wikipedia.org/wiki/%ID%} {} {Wikipedia}}"
      puts $optionF "#set idlink(8) {{useNAMEF_} {https://www.365chess.com/players/%ID%} {} {365Chess}}"
      puts $optionF "#set idlink(9) {{useNAMEF-} {https://chess24.com/en/read/players/%ID%} {} {Chess24}}"
      puts $optionF ""
      puts $optionF "#resolver with icons"
      puts $optionF "#set idlink(0) {{VIAF} {http://viaf.org/%ID%} {VIAF} viaflnk }"
      puts $optionF "#set idlink(1) {{FIDEID} {http://ratings.fide.com/card.phtml?event=%ID%} fidelnk {FIDE}}"
      puts $optionF "#set idlink(2) {{ICCFID} {https://www.iccf.com/player?id=%ID%} iccflnk {ICCF}}"
      puts $optionF "#set idlink(3) {{PND} {https://portal.dnb.de/opac.htm?query=%ID%\\&method=simpleSearch} dnblnk {PND}}"
      puts $optionF "#set idlink(4) {{BCF} {http://ecfgrading.org.uk/new/menu.php?ref=%ID%} bcflnk {ECF}}"
      puts $optionF "#set idlink(5) {{useFIDEID} {https://chess-db.com/public/pinfo.jsp?id=%ID%} chessdblnk {chess-db.com}}"
      puts $optionF "#set idlink(6) {{useNAMEF+} {https://tools.wmflabs.org/persondata/index.php?name=%ID%} pslnk {PS}}"
      puts $optionF "#set idlink(7) {{useNAMEF_} {http://%LANG%.wikipedia.org/wiki/%ID%} wikiplnk {Wikipedia}}"
      puts $optionF "#set idlink(8) {{useNAMEF_} {https://www.365chess.com/players/%ID%} chess365lnk {365Chess}}"
      puts $optionF "#set idlink(9) {{useNAMEF-} {https://chess24.com/en/read/players/%ID%} {c24lnk} {Chess24}}"
      puts $optionF ""
      puts $optionF "#"
      puts $optionF "# Icons for the links"
      puts $optionF "#"
      puts $optionF ""
      puts $optionF "image create photo iccflnk -data {"
      puts $optionF "   R0lGODlhEAAQAMZqAAAA/wEB/wMD/wQE/wUF/wYG/wcH/wgI/woK/wsL/wwM/xAQ/xER/xIS/xMT"
      puts $optionF "   /xQU/xYW/xcX/xgY/xkZ/xoa/xsb/x4e/yEh/yIi/yUl/yYm/ycn/ykp/zIy/zQ0/zY2/zc3/zo6"
      puts $optionF "   /zs7/zw8/z09/0ZG/0lJ/01N/1hY/1lZ/1pa/1tb/1xc/2Nj/2Vl/2lp/2pq/3R0/3V1/3Z2/3d3"
      puts $optionF "   /3h4/3t7/3x8/39//4SE/4WF/4uL/42N/4+P/5CQ/5KS/5OT/5SU/5WV/5aW/5eX/5iY/5ub/5+f"
      puts $optionF "   /6Cg/6Gh/6en/6mp/6qq/66u/6+v/7S0/7i4/7u7/729/7+//8DA/8LC/8PD/8rK/8zM/83N/87O"
      puts $optionF "   /9PT/9fX/9jY/+Pj/+Tk/+bm/+jo/+vr/+zs//Hx//Ly//T0//b2//j4//z8////////////////"
      puts $optionF "   /////////////////////////////////////////////////////////////////////////yH+"
      puts $optionF "   EUNyZWF0ZWQgd2l0aCBHSU1QACH5BAEKAH8ALAAAAAAQABAAAAfjgFJTaF86R2dpXjlIZ2JQUFIV"
      puts $optionF "   GGMnAAVFaCaWRlQKCRULE1cGBwIgWwQIASFSAAAMDBJKAC4XEEwALRcWTwAUKAwTQwA0GQs9ADIZ"
      puts $optionF "   Dk0ALGmhw8ULPAAzGgvNK2nBQtYjHNUxJBxOABw/3dZcW0YANl1bUa4eod40aWlHADf5rQAegkmT"
      puts $optionF "   8QIIgBozXvQqoUWgNWM7kmVg0EwFtAlFiE30kWxDA23cJiwBAAODBFwvdvXaFsvKgAcGQGQJEGGA"
      puts $optionF "   iFYsJYT5ACBAEDMdAAggMi9FGiVMzmhhcYNMGiwrcJQBk6RKmkAAOw=="
      puts $optionF "}"
      puts $optionF "image create photo viaflnk -data {"
      puts $optionF "   R0lGODlhEAAQAIQaAP8AAP8ICP8QEP8YGP8hIf8pKf8xMf85Of9CQv9SUv9aWv9jY/9zc/97e/+E"
      puts $optionF "   hP+MjP+UlP+cnP+lpf+trf+1tf/Gxv/Ozv/W1v/e3v/v7////////////////////////yH+EUNy"
      puts $optionF "   ZWF0ZWQgd2l0aCBHSU1QACH5BAEKAB8ALAAAAAAQABAAAAWDoCZm1lVp1ZVhqSWOSnBpEJNlynC+"
      puts $optionF "   tCA2LwODJ4IENI+M6AKYESWACuXlIBBFFADwVdgSK9bRBCC5ajBOTYYiNWumIguFPSdmledA42JJ"
      puts $optionF "   8DIHABhYghoqPAxZTgsEEQwTPBILDFEiAhBIeIYOhgBTd0QZCEpZEBcJA2lLFyolKayqGiEAOw=="
      puts $optionF "}"
      puts $optionF ""
      puts $optionF "image create photo bcflnk -data {"
      puts $optionF "   R0lGODlhEAAQAIQRAAAAAAAAIAAAQAAgYCAgIDAwMABAgEBAQGBgYDBvbwCAgD9/fzCQkH9/f2Cg"
      puts $optionF "   oDCvr4/Pz////////////////////////////////////////////////////////////yH+EUNy"
      puts $optionF "   ZWF0ZWQgd2l0aCBHSU1QACwAAAAAEAAQAAAFWmCBjGRZEgixFE3kqqwbHUjCGK1r4zJdGINc5Bfs"
      puts $optionF "   IRrIpFJJkzUAUCcU0HQ9o9ZpNXIFSKE0mFBs3AnNRqJQbVy6k9uuE/IIxKdOh0Jwx3IDAlQEB4SF"
      puts $optionF "   hoYEIQA7"
      puts $optionF "}"
      puts $optionF ""
      puts $optionF "image create photo dnblnk -data {"
      puts $optionF "   R0lGODlhFAAUALMAAAAAAGZmZld/u8zMzP8AAP///wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      puts $optionF "   AAAAACH5BAEAAAQALAAAAAAUABQAAARkkMg5BaA4SxHs1RknjB5YdSQJfCGapismvjRLzDTM4vkI"
      puts $optionF "   BB9e7ze4CHPEIsDT8wUGUAsHuKw9o8YAIUCFXQeWzbNQ4K4s37BkOSCTzWkb4dx2x0F0N9a0ZhfA"
      puts $optionF "   ciZnanwYMSARAAA7"
      puts $optionF "}"
      puts $optionF ""
      puts $optionF "image create photo c24lnk -data {"
      puts $optionF "   iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAYAAACNiR0NAAAABmJLR0QA/wD/AP+gvaeTAAAACXBI"
      puts $optionF "   WXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH4wIBFisA8KPVIgAAABl0RVh0Q29tbWVudABDcmVhdGVk"
      puts $optionF "   IHdpdGggR0lNUFeBDhcAAAFaSURBVDjLrZSxSgNBEIa/2azmOKOgjWihYmGh4IOksIkISRpLm+Rh"
      puts $optionF "   8gKCSETINfoEYmXnMwQEsRCM0SSijsVdZE0uuSTeX+3O7v7M/P/MytHJpc3MZfLAAbCHyLLAEpAD"
      puts $optionF "   fCDDX3wCb8Croi3gSZR7hauX9seNFCuNsoickwJU9dAAZdLDsRGRjbTYRGTLRlpNVhI8XtQKa26s"
      puts $optionF "   VA26QDbaLpppCIGvmNi3s84ZYCFFDX3rpDsKnb5EQC9eiV94Nro4kqxeK/huoFhp3IlINmyTMCvX"
      puts $optionF "   FzuDk/uAF66Hz03S+4QShxCXYRMNnVPozkKoTiadeq2wOaDZLcgqoBLe88e1qo2c88aIti2wPqHE"
      puts $optionF "   PQO8Jw/IxOgYoJ1ggkxB2LYDhF6pGjw74yTAynSEqk1Edp3g8j8+xAejEKQ1yAqBVdUzwn7Li7AD"
      puts $optionF "   0v/+c9Gcz8e5GZnZVmiBNlGuUU5/AOgHXYLbHhimAAAAAElFTkSuQmCC"
      puts $optionF "}"
      puts $optionF ""
      puts $optionF "image create photo pslnk -data {"
      puts $optionF "   iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAYAAACNiR0NAAAABmJLR0QA/wD/AP+gvaeTAAAACXBI"
      puts $optionF "   WXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH4wIDCS0S6DMvKgAAABl0RVh0Q29tbWVudABDcmVhdGVk"
      puts $optionF "   IHdpdGggR0lNUFeBDhcAAAKPSURBVDjLrZRbSFRRGIW/vc85Mx6qKcaiiy+CVpLQhSGoiLAb9BAY"
      puts $optionF "   FBQVUg/NQBGCkfTQQ/ZQVERv0QxCBRVBVAQlSNCFIsEYg9AUFcxiMGsqL6PNjOfs3UOpM4NdLNfr"
      puts $optionF "   v/bPWazvPzDVCk/GHMxw7z6VPdt3AQABEIGSQcMosbTu9iple6Ft3+nGQbpeLUZI20h/We16/M8J"
      puts $optionF "   H2jKWrLz5ApmzhWkEu1cPZIAMH+OkrZSdq9lFS1Ip88PQAvHVm4j9OoMWi1T5vQFaPWGtbvKWLRm"
      puts $optionF "   E9LaipTzUG4RWiVQqgp4CCAB0mCitb+mcOFtoNkDG6o9eZtRbgwo0MIUQClL1r/D9F5Hyj3ARqRR"
      puts $optionF "   iNYluI5/9KNN/SN350VYd6m9pQL4JsAoHkktZUZ+K0N9o14HuAXiNb0djdw79ywr/t6zcK0aUwCH"
      puts $optionF "   wXChflDK0hFpnJjjjMRMKJ7Z01HX75s9AMInlJPUcIPaQ/UTFnatmrHICZAalvuFHjjujDSHoHIW"
      puts $optionF "   PDhx8/hLhu0gqbwq/Sm1n9pVj34FQCAcHG85axAJ3kXjcy0jLV1lGLguoEBIpYSV80YAH4Cj0VAk"
      puts $optionF "   BiAC4eDXDIMC/JNEWf8IiQtYpko5jswzZ6P/6z4MNZTukbZ12UzGhyvs+TNuCil8/7JJK31/+P3n"
      puts $optionF "   yrZTd7oAjPjT5s78NUsazGmePO0qW0jx15G1ox43HazdEn/W2lf2tIa3V55gAHx89Lq7oHxlnZNI"
      puts $optionF "   3pEeMx9oB+JA4e8Wfo7GdvQ3dfQEwkEatp8fbzkQDhINRXIxKAMe/25hNBQRmdhEQ5EJsAkHk4A3"
      puts $optionF "   o0Hxh+RxoDwairwYAztHKoezPynTP/a3yVQNYE+i6H7g3a/OR/wriKOnN+X6DoYV8zpy6frwAAAA"
      puts $optionF "   AElFTkSuQmCC"
      puts $optionF "}"
      puts $optionF ""
      puts $optionF "image create photo chess365lnk -data {"
      puts $optionF "   iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAYAAACNiR0NAAAABmJLR0QA/wD/AP+gvaeTAAAACXBI"
      puts $optionF "   WXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH4wEWESUKAdQSwQAAABl0RVh0Q29tbWVudABDcmVhdGVk"
      puts $optionF "   IHdpdGggR0lNUFeBDhcAAAStSURBVDjLhZRbbFRlFIW/ff5z5sxMp/ebbZWWBrFAFdQqQYvKk1ED"
      puts $optionF "   oVGiRgyGRIIxUR9UImj0SY1GQ2rig7eoiZd4SQniJSpaUiRUi5S01AIFxt6wnQ69TWc6Z875fx9A"
      puts $optionF "   ITHqet5Ze6+9114C0PvYJhp3vgdA4pN3ZPTLXRG3tv7WcNVlG6xIeCWiatABxs+N+FOTnfMjv3/k"
      puts $optionF "   TSTaJeykVry1y3AR5OgTW1j28hsAnH74LlcvvuYOVV71gq6pXzxbUEnGjRFYISwT4ObS5GcmCSeH"
      puts $optionF "   8ePH+7wzQ9tmew7vbdq1P9P9wDpWvLsb+Ys5/tSDBSxp2pa9dPH2ZHUDM27RuY4YBIM5X2rEwvUz"
      puts $optionF "   XDIzQnTgMP7A0WeTHd+/ev3XXalfWlajAIYeucf1lzc/fbb+2u1D1VeSDefjWILn+xit0dqgtSHQ"
      puts $optionF "   Bt8PyGKTcIvxi8ooUMEa11bWa+0Hf3izfxBlWh+X8eK69aMLr2s9Ub6UEkdTnx6lcHqUaDRCPKPI"
      puts $optionF "   BYZsYHBtYUGBgyVQEbUY0XlMuUWUZ882b6kuiL/e1dtt/TqaiY4VLXihv7SBWMiioihKNBGnZOAQ"
      puts $optionF "   SikuLw0x42nOzgcExnBVZZhVNVEaSl08L8dJq4TemuuJVFY+922NVageuXnV2p8bbn8oEythxexx"
      puts $optionF "   Sk53IzrA5DyUJRR0/8hU3VXEpzzSOUPG10x7mp7xLAOTOdJeQMIupHwyHiuLOmfsPqdqQ79TySXZ"
      puts $optionF "   HCRGye77DCksxYiF9BzGT88SW3k3E+kAR6C+KERtoYMx8xwcyTCX04gIh4sb1ZrcN3faxyKXrhxO"
      puts $optionF "   +WR9TbpyIbGNj+MNDlCcGifetI4rTv3EngmPWU/jCBwYTtM3ofgj5ZPMBMx5Ab6GCecyWZVOL7NT"
      puts $optionF "   kaJqMTCZzvG2LmNt+TzLDrXzR08XVZcvZ2feTfQNphCBpWUh7l1W+LeJ+5NjjM8ZRMCoEFOxsmJ7"
      puts $optionF "   3o0RCSkqXKElNExtvA8W1BEqr8CN93NvbIIPKlYxPO0RdhRTWU3W14SUhRJBzjvZiOA5UezacHBm"
      puts $optionF "   jTNYV5Gdpry9jTOdHYgbBhEIApSt2LS9lWO25vhYPluH0kzP+1gihG1ByYW3s/3cpN1y+ouDc3u6"
      puts $optionF "   63LZeabFRuVfkIQDBkPq+UepdRRLG6+hdvVmPldV5Lzc39MB2IFvCuYSR63EgX0fzOcCAuWAZfFP"
      puts $optionF "   CFrZeFqYOXKI6t691JZG0BdFgkGoSp7UhanxzyyJxvYh8hsI/woREMGIgNYoES6OGK1srj723eii"
      puts $optionF "   wd/et0QkBTzJ/0FrQmUVTC25kZPJDOq8GF85XDHYyaLhrm2qn1mrqa3DgNkLPPtffMoNkb36Ftrz"
      puts $optionF "   lpDNegigLUXp9Ag3HPn85aa2jo8BLICmtv0ZDK8CL/6r6uo6jt64kf7xOWwBLYri2TFuOfThS7G5"
      puts $optionF "   5A4B07W++Vx8da1fTdOuDm99Y+SHPJN3GlgBxDi3WLEiUbzNz/DmUJgQgUEsXXn21GjzkU+3bmx9"
      puts $optionF "   ZefC3hNB19pradrdeeESv7Q0c13bfgA6191QpJS6H2hB68b8lk1lrQs2kBxLzEX9dE/N+PFP7vtq"
      puts $optionF "   xzsNe0/MAHy96R5ue+9jAP4ETQUK89iGnNcAAAAASUVORK5CYII="
      puts $optionF "}"
      puts $optionF ""
      puts $optionF "image create photo chessdblnk -data {"
      puts $optionF "   iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAYAAACNiR0NAAAABmJLR0QA/wD/AP+gvaeTAAAACXBI"
      puts $optionF "   WXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH4wEWESIIoJvlKgAAABl0RVh0Q29tbWVudABDcmVhdGVk"
      puts $optionF "   IHdpdGggR0lNUFeBDhcAAAOPSURBVDjLlZQ9jFRVFMd/57735mOZ3WXJAooIikSRyjXZgkYsjKCF"
      puts $optionF "   YkdhaW9DaaKdBYmJMbHSxMpopRUhURGIIYpmDQZdRRZcVnd2Zmdmd2bevO97j8UbVpECPMW9yT3J"
      puts $optionF "   P/+Pc4+oqnKfpQoC4+O/zfLd53+V4nDcWj9HmK6yo3GAWrCTydo+fFNHEMz9Q5XUBMPN9tdcWf4M"
      puts $optionF "   pymp3cS6eExa781QxzpVFUFRlRJAhDjvUEUpO4LoPSSX9iqjpEl7+D2WlF2Np4mTnCK3pMUA39RB"
      puts $optionF "   LWMB95asqqz0vuH8zx/imwr96Dp7Z+eIo4Lc9snsABEzNuUuybqVJii3B0AAzwhZ1iPwGtgsRGOL"
      puts $optionF "   KSJEon9FIXcCqoJiMeITxU2sRtQqD2AKDz/KoRghzkCaYKICvxiB9jHIFpm7PFRrOfftuyTFFfY/"
      puts $optionF "   vJ9G/TGysEsQW4IiBFUkizFRQaWIIe2NExZAMP/M9fgWw2R1ku6fHSbUp1IUmDzGjwuCPESyISZL"
      puts $optionF "   MXFBLYvwkt7YwxLCB3CqGBF0/A0mKpPUrMNLBxgzhUlTJMypuQgrgqc5kjuqNsWOitvOo4AvInRX"
      puts $optionF "   v8JFlzH+FPWpOSpmG0GSUcsjRDbRUULSSVhfWEZ+6pNu5Dw6ilnvXoPHp+CIggEx41AuX/ySuScs"
      puts $optionF "   uBbRYB2bHGTYDik6A5yB65qhxw7xY98x+vQPjj7zIqwt0/z9Kg3pohKULFXLvK3dTmtxkXClSdLp"
      puts $optionF "   Y5OC1vKQuLnJ+eYSTbOJWMfOPbuZ/+QkC198zMriD0wfPsBDzx7ixoW3wRjcbQ+NN83atQ6VnZOM"
      puts $optionF "   VkPk4JO0VkLyZp+FG332HguYntpGf9QjWsxxkxPsOvkUv3rHueSfICmU5PMWb56YLRkG1SlWlzaI"
      puts $optionF "   V4ekrQEP7tpNNwzI1xI0s9xa6rFyq8VGNKSy1mB+/i0+uHqc1eA5ev0hg8GI3xYXUaf4zjmOPP8y"
      puts $optionF "   p995ncONBv7ufTgzw/tnL1D0Qx458x5LGwuYuqF+GiqVDu3gIq9pwUczB0kGQ+Jwg263h5ijZcr1"
      puts $optionF "   Wo03zvyCJcP3a+AH2CKkOjvNSy+8ytnvEoZ2mfZfId5swfZqg5maR3v5JtYKMzsmOPXKsXLotayt"
      puts $optionF "   STJbm228qmzGYNQm1jaDSwO0O8JkFt8Xlubm2V7z2DNTZXZ6gnrFvxNw6wep3LEojBGcs2RhQpFl"
      puts $optionF "   4MATId9WZ6Lq4Xs+6sq9+Tcv6fd6haBVDgAAAABJRU5ErkJggg=="
      puts $optionF "}"
      puts $optionF ""
      puts $optionF "image create photo wikiplnk -data {"
      puts $optionF "   iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAYAAACNiR0NAAAABmJLR0QA/wD/AP+gvaeTAAAACXBI"
      puts $optionF "   WXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH4wEWESoJH0VftAAAABl0RVh0Q29tbWVudABDcmVhdGVk"
      puts $optionF "   IHdpdGggR0lNUFeBDhcAAAKDSURBVDjLrZQ/SGNLFId/N6zwKrUIqKUgNqIW3kaQdKnFyKJio50h"
      puts $optionF "   bUBQUAyIpLAVsVLQIrt9RAXRVkJAZDUi2AQjioWuf+5Gc8+3xdP7srsKb3c9MDBzzpyPmTlnfpIk"
      puts $optionF "   M2szs09mdmNm/Oa4ec5tkyTneZKT9I/+zr5Jch0z+yTpo97HPjtmdiOp9p2AXx0zw3Gcd6EB+uB5"
      puts $optionF "   nkZGRpTP5wXI933F43E1NDQonU7L8zyFw2FNTk5qb29PmUxGdXV16uvrk+u6SiaTKpfLikQiWlhY"
      puts $optionF "   kHi2+fl5JJFOp19crKysIIm1tTUALi8vicVi7OzsBHsWFxdJJBLBWmYGwMXFBZ2dnXR1dQXB29tb"
      puts $optionF "   enp6aG1tpVKpsLW1RSqV4unpCTPj/v6e8fFx8vk8AGb2L/AFOjMzgySy2WwAXV5eRhJzc3Mkk0n2"
      puts $optionF "   9/eDWKFQYHh4OID9AiyVStTW1tLR0UG1tbS0UF9fz+zsbJAMkEgk2N3d/QEYqq5QU1OTBgcHdXR0"
      puts $optionF "   pPX19aB6ExMTur6+Vk1NjSTJcRxdXV3p4OBAkUhEwH+lrv5GAKenp0iiv78/OOH09DSNjY20t7dT"
      puts $optionF "   KpUAiMfjQXGqGfr5bwIMDQ0RDofJ5XIcHh4yOjrKxsYGklhaWuLs7Ize3t5fYG8Cj4+PkcTU1BRj"
      puts $optionF "   Y2OcnJzgeR7d3d1Eo1EGBgbI5XL/D2hmPDw8EIvFkEQqleLx8RHf98lkMkgiGo1yd3f3qvroLVnK"
      puts $optionF "   ZrM0NzdTKBSCtywWi7iuy+rqKr7vvwl8VQPPz8/Z3t6mXC4HvkqlwubmJsVi8dXrmtnNm/JVLRjV"
      puts $optionF "   bfHi/6FVfpKvdxXYUCgU+iLJlfRZ0tc/0cDnXDcUCn35Ds1OQnIj7LXqAAAAAElFTkSuQmCC"
      puts $optionF "}"
      puts $optionF ""
      puts $optionF "image create photo fidelnk -data {"
      puts $optionF "   iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAYAAACNiR0NAAAABmJLR0QA/wD/AP+gvaeTAAAACXBI"
      puts $optionF "   WXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH4wIEEx02hxDjFwAAABl0RVh0Q29tbWVudABDcmVhdGVk"
      puts $optionF "   IHdpdGggR0lNUFeBDhcAAAM9SURBVDjLrZRNbJRVFIaf+/NNyzAz/dOxU2vbGRoLVmikLUJUqMBC"
      puts $optionF "   iFEW4oJo4g9hpyZu0BpNg1GJbpSNiX8LXGDiChUUu7CJrYKJSiASEa1DM0OV32lph5nvu/e7LoZW"
      puts $optionF "   YwuJpic5q3PPyT3ved9X9Kzf4VjA0NcqOOcIQ4dzlQRACKQAISRSinn7JAscCz5wzsqBMQS+oa42"
      puts $optionF "   Tro1RWOynkQiijGW4pUyFy5OMpb7k/z4eSKexvM8hJhnYBg6ildKrF2zgq1b+ritI00sWs0f5y8x"
      puts $optionF "   UbhM78pl+IFhuliiVCqTy5/l4/1DDA59j5QCKSvLip71O1wYOlI31bPr+SdY3rkEayzaU5RKPv0v"
      puts $optionF "   v8Pw4eO0tTSy7aGNPLjpbhBgTUhVxGP0dJ6du94le3ocIQSqKd090Lm0jT2vPc2t7begpERrRbFY"
      puts $optionF "   4oVX3uPrb4+jtaIwMcXBwSNorVjd24mnFUII6usSPLzlXk7+OsZodhwZjVbT/+wjpBobMMZgjKUw"
      puts $optionF "   McVLuz9gaPgo7ZmbWVQdQQhBPB7lwOBhAKy1GGMJrvYM7HyM5Z0ZZM8dHXS0t1QA1RqlJBFPs3nj"
      puts $optionF "   ava+3c99G3qx1la4GTpqa2IIQEqJUhJPa7RWxGOL2frAOmSmJQVA2Q8o+wF+YFBasWFdN7kz59j7"
      puts $optionF "   0Zf4gUUIQWAsfXd14QdmNmf6AmO48YY6dH78HABVEW8Op/YfHGG6WAIhEFfXbG5KEvHmF9ilwiRy"
      puts $optionF "   5LufyJ05C4AxFj8wBIHBOUe6rZFkQ4ymZIJUMkFrcwOffTE8+7sgMBhrCcOQy1NFPvl8BJVs7hr4"
      puts $optionF "   5bccq1YuJRFfjFIVbEDQdXsGreDEySxCCNauWcH2R++nvq4GTyuUkkgpcc7xxp59FU464Idjp3jq"
      puts $optionF "   ubf48dipiloCA86RiMfou2cVo2MXyOYukqipZUmmBSVl5Q0wms2z/ZnX+fTQN2itKsSewcAPDHd2"
      puts $optionF "   L+PxbZtIJKLsfnMfJ37+fVYF1oYIIXj1xSdJtzbx/ocH+Gr46N9uNKOUfwJrjKXsByyqiqC0xLm5"
      puts $optionF "   tqaVpDA5TXVVBO9fB5pzLq0VWiucY86wiiUKjA2Jx6L/zb6EuLZFiesU9fWK/yf+AkkdVsIfTmX/"
      puts $optionF "   AAAAAElFTkSuQmCC"
      puts $optionF "}"
      close $optionF
   }
}

proc ::pinfo::splitName { playerName } {
  set fname $playerName
  set lname $playerName
  set countlen 2
  set count [string first ", " $playerName ]
  if { $count < 0 } {
    set count [string first " " $playerName ]
    set countlen 1
  }
  if { $count > 0 } {
    # create names "firstname lastname" and "lastname,firstname"
    set fname "[string range $playerName [expr $count + $countlen] end] [string range $playerName 0 [expr $count - 1]]"
    regsub -all "\[0-9\]" $fname "" fname
    set lname "[string range $playerName 0 [expr $count - 1]],[string range $playerName [expr $count + $countlen] end]"
    regsub -all "\[0-9\]" $lname "" lname
  }
  return [list $fname $lname]
}

# Replace the ID-Tags by proper links
proc ::pinfo::ReplaceIDTags { pinfo } {
  switch $::language {
    B {set wplanguage pt}
    C {set wplanguage cs}
    D {set wplanguage de}
    F {set wplanguage fr}
    H {set wplanguage hu}
    I {set wplanguage it}
    K {set wplanguage ca}
    N {set wplanguage nl}
    O {set wplanguage no}
    P {set wplanguage pl}
    R {set wplanguage ru}
    S {set wplanguage es}
    W {set wplanguage sv}
    Y {set wplanguage sr}
    default {set wplanguage en}
  }

  set fideid ""
  set linked "<br>"
  # Handle all id definitions and link resolvers from the config file
  foreach index [lsort [array names ::unsafe::idlink]] {
    set federation $::unsafe::idlink($index)
    set searchterm [lindex $federation 0]
    set url        [lindex $federation 1]
    set icon       [lindex $federation 2]
    set str        [lindex $federation 3]
    if { $icon ne "" && ($icon eq [info commands $icon]) } { set btn "<img $icon>" } else { set btn "" }

    # extract the ID
    if { [regsub -all ".*$searchterm " $pinfo "" id] } {
      regsub -all {<br>.*} $id "" id
      set id [string trim $id]
      if { $searchterm == "FIDEID" } { set fideid $id }
      regsub -all "%ID%" $url $id url
      regsub -all "%LANG%" $url $wplanguage url
      set link "<run openURL $url; ::windows::stats::Refresh><blue>$btn$str: $id  </blue></run>$linked"
      regsub -all "$searchterm $id<br>" $pinfo "$link" pinfo
      set linked ""
    } else {
      if { $searchterm == "useFIDEID" && $fideid != ""} {
        regsub -all "%ID%" $url $fideid url
      } elseif { [string range $searchterm 0 6] == "useNAME" } {
        if {![info exists fname]} { lassign [splitName $::playerInfoName] fname lname }
        if { [string index $searchterm 7] eq "L" } { set psname $lname } else { set psname $fname }
        regsub -all " " $psname [string index $searchterm 8] psname
        regsub -all " " $psname "%%20" psname
        regsub -all "%ID%" $url $psname url
        regsub -all "%LANG%" $url $wplanguage url
      }
      if { [string first "%ID%" $url ] < 0} {
        set link "<run openURL $url; ::windows::stats::Refresh><blue>$btn$str  </blue></run>"
        if { ![regsub "</blue></run>" $pinfo "</blue></run>$link$linked" pinfo] } {
          regsub "<br><br>" $pinfo "<br><br>$link$linked" pinfo
          set linked ""
        }
      }
    }
  }
  return $pinfo
}

proc playerInfo {{player ""}} {
  global playerInfoName eloFromRating
  if {$player == "" && [info exists playerInfoName]} { set player $playerInfoName }
  if {[catch {sc_name info -htext $player} pinfo]} { return }
  # add country flag
  set found [string first " \[" $pinfo]
  if { $found > 0 } {
    set countryID [string range $pinfo [expr $found - 3] [expr $found - 1]]
    set country [getFlagImage $countryID]
    if { $country ne "" } {
      set pinfo [string replace $pinfo [expr $found - 3] [expr $found - 1] "$countryID <img $country>"]
    }
  }
  # append Elo History
  append pinfo "<br><br><darkblue>$::tr(PInfoRating):</darkblue><br>"
  if { $::eloFromRating } {
    set eloList [sc_name elo $player]
  } else {
    set filter [sc_filter new $::curr_db]
    set eloList [sc_base player_elo $::curr_db $filter $player]
    sc_filter release $::curr_db $filter
  }
  set i 0
  foreach { date elo } $eloList {
    set d [expr {int($date)}]
    set m [expr {round(($date - $d) * 12 + 1)}]
    set d [format "%04d" $d]
    set m [format "%02d" $m]
    append pinfo "$d$m  $elo     "
    incr i
    if { $i == 3 } { append pinfo "<br>"; set i 0 }
  }

  set playerInfoName $player
  set ::rgraph(player) $player
  set w .playerInfoWin
  if {! [winfo exists $w]} {
    ::createToplevel $w
    wm title $w "Scid: [tr ToolsPInfo]"
    ::setTitle $w "Scid: [tr ToolsPInfo]"
    wm minsize $w 40 5
    pack [ttk::frame $w.b2] -side bottom -fill x
    pack [ttk::frame $w.b] -side bottom -fill x
    ttk::radiobutton $w.b.eloF -text $::tr(PInfoEloFile) -value 1 -variable ::eloFromRating -command {::pinfo::playerInfo $playerInfoName}
    ttk::radiobutton $w.b.eloD -text $::tr(Database) -value 0 -variable ::eloFromRating -command {::pinfo::playerInfo $playerInfoName}
    ttk::label $w.b.eloT  -text "$::tr(Rating):"
    ttk::button $w.b.graph -text [tr ToolsRating] \
      -command {::tools::graphs::rating::Refresh player $playerInfoName}
    ttk::button $w.b.edit -text $::tr(PInfoEditRatings) -command {
      makeNameEditor
      setNameEditorType rating
      set editName $playerInfoName
      set editNameSelect crosstable
    }
    ttk::button $w.b2.report -text [tr ToolsPlayerReport] \
      -command {::preport::preportDlg $playerInfoName}
    dialogbutton $w.b2.help -textvar ::tr(Help) -command {helpWindow PInfo}
    dialogbutton $w.b2.update -textvar ::tr(Update) -command {::pinfo::playerInfo $playerInfoName}
    dialogbutton $w.b2.close -textvar ::tr(Close) -command "focus .; destroy $w"
    packbuttons right $w.b2.close $w.b2.update $w.b2.help
    pack $w.b.eloT $w.b.eloF $w.b.eloD -side left -padx "5 0"
    packbuttons left $w.b.graph $w.b.edit
    packbuttons left $w.b2.report

    autoscrollText both $w.frame $w.text Treeview
    $w.text configure -font font_Regular -wrap none -state normal
    ttk::label $w.photo
    pack $w.frame -side top -fill both -expand yes
    bind $w <Escape> "focus .; destroy $w"
    ::htext::init $w.text
    ::htext::updateRate $w.text 0
    bind $w <Escape> "focus .; destroy $w"
    bind $w <F1> {helpWindow PInfo}
    ::createToplevelFinalize $w
  }
  set player [trimEngineName $player]
  set imgdata [getphoto $player]
  if {$imgdata != ""} {
    image create photo photoPInfo -data $imgdata
    $w.photo configure -image photoPInfo -anchor ne
    place $w.photo -in $w.text -relx 1.0 -x -1 -rely 0.0 -y 1 -anchor ne
  } else {
    place forget $w.photo
  }
  $w.text configure -state normal
  $w.text delete 1.0 end

  set pinfo [::pinfo::ReplaceIDTags $pinfo]

  # Display the player info
  ::htext::display $w.text $pinfo

  $w.text configure -state disabled
}

# Call in the idlink config file.
   if {[catch {safeSource [scidConfigFile resolvers]} ]} {
     ::splash::add "No configuration for link resolvers found. Creating default..."
     ::pinfo::setupDefaultResolvers
      if {[catch {safeSource [scidConfigFile resolvers]} ]} {
         ::splash::add "Oops there is something wrong with the resolvers file..."
      } else {
         ::splash::add "Default resolvers created and loaded."
      }
   } else {
     ::splash::add "ID resolvers found, extended player info available."
   }
}
