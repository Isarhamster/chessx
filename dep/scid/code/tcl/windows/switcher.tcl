###
### windows/switcher.tcl: part of Scid
### Copyright (C) 2000-2004 Shane Hudson.
### Copyright (C) 2006-2009 Pascal Georges
### Copyright (C) 2008-2011 Alexander Wagner
### Copyright (C) 2013 Fulvio Benini

set icons {
{Unknown}
{
R0lGODlhIAAgAIQQACYjYUooEzUsWUU5T3JAHWlJGFtPS3FgQ4VwPJR3LZ9/
KbKUL7ueK9SvGOrAE/rLAP//////////////////////////////////////
/////////////////////////yH5BAEKABAALAAAAAAgACAAAAX+ICSOZGme
aKquKtE8MMzMtAOzpBvL9Gw/OBFh8YM5erNYkJCYGWgPxsPRqDYcDhwhsJgt
DIuXwvCK0rSK7mwgKCsE0x6LUFB72dLH4tCQrwJXVFUzAgJ5AAN2LApFRgwI
AAJhCYhqKwSNPAsClQwOnAhAKXQ7MTMHAAAHYQ+cBlmjZaVfqWCeCqkIsCd0
DFVYNg2cAqszkABkoiZbCkgJBpEHvsapfDe8dV4IA6kDV1ML3AMIvle8XMac
iAk2DgnqiXYMy+jvqmoPDbWecUglBEwYbLo3ow8ndjH6+CMRIABAdQkWdBEG
AKEpJPMYNgyQCkBDgNwAeMLCY+GIjepLPG7s+JGOggRpJM4osZJlw5AqN+oE
SMdEAJwCUNrUSbThiY6pRiAtyvRoRxJIaTY1EVUEUgAsAlCtCuFqkBJerXL9
2nXsWLJoWYQAADs=
}
  {Temporary database}
  {
  R0lGODlhIAAgAMYaACEhISQkJC0tLTExMTU1NTk5OTo6Oj09PT4+PkJCQkND
  Q0REREZGRklJSUtLS0xMTE1NTU5OTlFRUVJSUlNTU1RUVFVVVVdXV1hYWFlZ
  WVpaWltbW1xcXF5eXl9fX2BgYGFhYWJiYmNjY2RkZGVlZWhoaGlpaWpqamtr
  a2xsbG1tbW5ubm9vb3BwcHJycnNzc3R0dHV1dXZ2dnd3d3h4eHl5eXp6ent7
  e3x8fH19fX5+fn9/f4GBgYKCgoODg4SEhIWFhYaGhoeHh4iIiImJiYqKiouL
  i4yMjI2NjY6OjpGRkZKSkpOTk5SUlJaWlpeXl5iYmJmZmZqamp2dnaKioqam
  pqmpqaqqqre3t7m5ubq6uru7u729vVpaWlpaWlpaWlpaWlpaWlpaWlpaWlpa
  WlpaWlpaWlpaWlpaWlpaWlpaWlpaWlpaWlpaWlpaWlpaWlpaWlpaWlpaWlpa
  WlpaWlpaWlpaWlpaWlpaWlpaWlpaWlpaWlpaWlpaWlpaWlpaWiH5BAEKAH8A
  LAAAAAAgACAAAAf+gH+Cg4SFhoeIiYqLhVtZVlhYjJOCV1VTUlmJT4JJg02K
  XFpUjok2Nzc1pzZHilFQSlFUiky1tVGIPSq7vLsuiENLQkdHUog5GMnKySeI
  KEMnNTVBiRIP19gPrYcjIUQhITyJMgnl5hOJKjU7Ly9DihAE8gQGSIkULxbg
  MYo0HP8cSiSK0sCBiwYNVihqkaFhBg+JiGjIwKKhQkQ4HCjYuHHEoSYaOSpg
  cNFQAgQoUyKAYoiESpWUYsqcSbOmTZtJfkzioUMnoxsuXOhgFBQGIx8lSvBj
  JMTEu0UdNnQQ5EEcrUkVJFTg4UGCjURAVIhYYTXRhwVoFzgANS5FDkZDRgzI
  NQBi0QEDESY5YMB3iaImAgQMKMKoBovDi4JcWFz2UBQLBSIXWAAE0QzJkV8k
  EgCgs2cATgw5+dw5wLabqFEHAgA7
  }
  {Clipbase}
{
R0lGODlhIAAgAOekAGBBE2lGFHBLFnFMFXFMFnJMFXFNFnJNFXJNFnVNFndP
FlxcXF1dXF5eXl9fX2BgXolgH4thH41iH45jH5BlIJJmIJNnIJVoIJdpIJhq
IZprIZxsIZ1tIZ9uIaFvIqJxIqNxIqRyIqZzIqh0I6l1I6t2I39/ca13I654
I4CAfbB5JIGDfrJ7JIGDf7N8JIKEgIKFgLV9JLd+Jbh/JYyGeLqAJYeJhIaK
hIeKhIeKhYqKfbyBJYiKhIiKhYeLhYeLhpCJeb2CJb2CJpGKeb+DJsGEJouO
icKFJoyPipaNeo6Oi8SHJsWHJ42Ri46RjMaIJ5GRjZeXipmZjZiYl5ubjqGh
laOjoKSkmaOjoqSkmqionaqqoKqqoauroKuroqmrqaysoaysoq2toq6uo7a2
rLa2rbe3rbe3rri4rri4r7m5r7m5sLq6sby8s8PDw8TEw8TExMTFxMXFxMXF
xcbGvsbGxcbGxsfHxsjIx8nJwtDQz9DQ0NTU1NXV1dXW1NbW1tjY2Nnb2dzc
297e3uPj4ubm5ejo6Onp6Onp6erq6erq6uvr6uvr6+zs6+zs7O3t7O3t7e7u
7e7u7u/v7u/v7/Dw7/Dw8PHx8fLy8vPz8/T09Pb29vf39/j4+Pn5+Pn5+fr6
+vv7+/39/f7+/v//////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////////////////////yH+FUNyZWF0ZWQgd2l0aCBUaGUgR0lNUAAh+QQB
CgD/ACwAAAAAIAAgAAAI/gD/CRxIUOCCgwgPFlzIcOECKVQiUpGyoKFFgg8Q
6jDB0YQOhA8u/lNwgIBJkwyqXFkZpWWUlVeqMBBwkkCBBAQRMHnCk+cCNl3A
CAUjpqiYMW0WBOn5pAgCggR4JulBdUGeNWrSoEFzxkwZMmToLFhBdUiIJwSg
8uxBqq0DLWHCeJnLpe6WLVkcYGrb4wNatU/YVqqEpUHCwwsaTClUiVQPD38H
Rl0ieDAhQJgzawYkKFLjHh0iC4x6pPLg06hTT5rkmIPof1GLmE5Ne/WkSI43
vI5KpDKe38CD3xleJxIkxxp2Pwkym3al1ZGiP3rkOIPyHc1Rr5YUHdIjR40c
/mNQXsO3cOJ15siRM6gRI8cXlM/IDt34dPCMGC1ybEG5jNn1RXKfe/opoohj
FSgXQ2XD2VFHenLA8cYbbrhhYCKIiNIDBcq5wJZt3Q1YoCIYImJIKD1MoBwL
H4b4HYGLkIiIiYYY8kkPEiinAlvRrSdhhRXuMeMhNSLiyI0RKIcCW96B516M
MtJoCCOSXAJKDxAod0IPo7w4Yok1GqIIJJZoYkkgOGSZlmRPlNCDKDBGSaQh
iEByw503IHGDD2qqRUIPoYw4ZJGOUIKJD3388YceTmCp3Ag9eHLhjGFSeYkm
nNygaBxIvODomqM9IUIPm4BZ45hlcgKKJk3w8cUPUy0oYUOfbIbQgyaZYHLJ
rphoskknoIgiih83GAFDClBYMatyIABB1bPQQpsDDjxQZYMNNNAqkAFFMOXt
t+DyJIQBBAUwQE3opquuSQMAINK7IgUEADs=
}
  {PGN format file}
 {
R0lGODlhIAAgAOeiADMzM0tLS0xMTE5OTk9PT1BQUFFRUVJSUlNTU1RUVFVV
VVZWVldXV1hYWFlZWVpaWltbW1xcXH5YHV1dXX9ZH4BaIGBgYIFbIoJdJINe
JWNjY4RfJ2RkZIRgKGZmZoZiK4ZjLGhoaIhlL2pqamtra21tbW9vb41rN3Bw
cI1sOHJyco9uPHNzc5BvPI9wP5FwPpFxP3Z2dnd3d5R0Q5J1R3p6epV2RpZ4
SX19fZh6S5l8TYCAgJp9T4GBgZt+UYODg4SEhJ2BVJ6CVoaGhqCEWYiIiImJ
iYuLi6OIXoyMjKOJX6SKYY6Ojo+Pj6aNZJCQkKeNZZKSkpOTk6qQapaWlpmZ
mZycnLGadp6enrijg7qlhrunibGxscW0mse3nsm6ob6+vr+/v8DAwM2+qMHB
wc7AqcLCws/Bq8PDw8TExMbGxsfHx8jIyNXItd3TxN/Vx9/WyODXydva19va
2Nzc3N/f3+Dg4OHh4eLi4uji2OPj4+Tk5OXl5ebm5ufn5+jo6Ovo5Onp6erq
6u/q4+vr6+zs7PDs5e3t7e7u7vLu6O/v7/Dw8PPw6/Hx8fLy8vPy8PPz8/T0
9PX19ff18vb29vj39Pf39/n39fj4+Pn5+fr59/r6+vv6+Pv7+/z7+vz8/P39
/f7+/v//////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////////////////////yH5BAEKAP8ALAAAAAAgACAAAAj+AP8JHEiwoMGD
CBMqXCjQisOHVqhIaZKkCJAeOGrEUGGCRAgOHBQgtAIKVKhNoUqW/MSyk8tN
MDPJxCTyIEk6onLq3MlTJx6aI0PRkCABg4gyob6suNBihRdROYi+QSLhRk2D
VkLNkdCiEogPYyRc4TQICg9RmooKSfvoakGSdSS8MNRBhA8JiXJ6cpNTQhYJ
cSQAtQkqLtEUbWZI8CQqCFEdoiQwykBEsFuCJO1IgKFzioQ8oiaJwBtZlBai
mB4gHAJKjoQUoXK+kbDkEhwKEtBK0ARog2AHCH+AckG0i84zJyrYcJI7qhJR
WwQ3QLjjE56e2HUWEkUIEwOEOD7+6clOXlShwQZriN9zh5AfMmDAhJk/X4x9
MWgOHcKUAKGMTnvwcUchfwQSiCCEFFKIfog06CAimByAEAsACkggGfTdJwYZ
HJKRhiKKYFIAQih0wkcfeBxyYIIMgrjIizAuYskACJVgIooqIrggIoq82MiP
QMooAEIjbNKHH3ocIogZYdjXoRlQmoGGGo40YkkACHlgJBd8KMkij4s04ggk
kEQSCZmOXImQBkYKkiQhaDhJBpRo1FnnGmeqeZAFmRy5ByJf9ihmmZJQQokk
kkSip0ET9PnHn3A6SWedaVTKRqKLFuRAJn48CuiCgo4ZSaGWYGLJoZkStAAm
nUIaJ4dKk65RqqmoYnkQApj04emXYYpK6qmY2moQAbnuumOPvhZqaLAIGXAq
JZCYasm01FZrrSUGIKTAttwu4O234Ia7AAMAMGTuueieGxAAOw==
}
  {My games}
  {
  R0lGODlhIAAgAOeOACAgICUlJSoqKi4uLjMzMzY2Njg4ODo6Ojw8PD09PT49
  PT8+Pj8/P0BAQEJCQkREREVFRUZGRklISEpKSk1LS05OTk9PT1FRUVVSUlNT
  U1RUVFZWVllVVVhYWFtYWFxaWltbW1xcXF5cXF9cXF1dXWJfX2BgYGFhYWVi
  YmVjY2RkZGhlZWZmZmllZWhoaGtnZ2xoaGxpaWpqam9sbG9vb3Jvb3FxcXVy
  cnR0dHZ2dnl2dnd3d3h4eH13d3p6en55eXt7e4B6enx8fIB8fIN9fX9/f4CA
  gIR/f4GBgYKCgomDg4WFhYaGhoqFhYyGhoqKiouLi5CQkJWOjpaOjpGRkZKS
  kpeQkJSUlJWVlZuUlJ6Wlp6Xl5qampycnKKamp2dnaKbm56enqScnKCgoKef
  n6KioqmgoKqhoaWlpa2kpKioqLCmpqurq6ysrLGxsbKysrOzs7S0tLyysra2
  tr6zs7+1tbq6usO4uLy8vMO5ub29vcW7u8e8vMDAwMi9vcfHx8nJyczMzM7O
  zs/Pz9LS0tTU1NbW1tra2t3d3eLi4uTk5Ojo6Orq6uzs7P//////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  /////////////////////yH5BAEKAP8ALAAAAAAgACAAAAj+AP8JHEiwoMGD
  CBMqXMiwocOHECMqtEOxokU7Ev+p+cKxo8eObSLC0YMHz5iTbkrGOVlGop1G
  MHnI7ANzkEweSSLOYcQTh089PAP5xHElitGjSJFyEfhmkVMaUO04BQSVxpMu
  WLNq1YpGIBMfYA2J7QKWilhDPGSoXcuWrRGBQljIVUQXi9wldBXlkMu3r18c
  AnmcGJyoMJXBSAonsjG4sePHNARCCUO5suXKKkJo3syZswyBYxCJ7kCajZ/T
  TUh3SHHaj44OrWOfFtjlkO0MuNW0JoM7w5DWNzKcFiOlePEsAqv8WW6i+ZjT
  e/JcqFBhS53TNSpgp86dhEATE8L9ny1yGoyfGOHpiDk9YwL78PA7CAQBoX6h
  +0BOE/HjBIIIP0ecBgMEAtZnoAYCbdDAgoQ0uMNpLcixRgM/+PHBaSs0cJoW
  TnTYoQUCAdHGiM2BdxoKU/ghgRd0JHAiArK1NoFXgtRowI0GnFbCC368cIcV
  OfpRQpBD4mhABAIlEciSBDRJwGkjLMCHGX7A8KQfI1yZpZMEOCCQC0cNIOYA
  p3kwwBmnKUCmH2aWOaaYCQjEgAB01inAaRwI0IMfadCJ551+WEHEoIMqIdAB
  ASSqaACnYRAABX4EkWijjMY42z8TAKDpppx26umnBQz0gIGklmrqqfVlpOqq
  rEIUEAA7
  }
  {Large database}
  {
  R0lGODdhIAAgAKEAAP8AAP///wAA/wAAACwAAAAAIAAgAAACiYSPqcvtH6KctFpks544iA9+
  WRhGHQmOqGAeUblR8AnH3uzeNxunYkB7/VS8X1DIq6SELQNTspxEdcCcZTqldIjIzZaYrXzB
  tvE1LLaChxmz9IeWuLtQtjbHDmN5xx33eRRHZ4THIlhnyFd4+LZEY9fjuEiyJum0Q3mG0gRg
  4ymn9un1QFpqeloAADs=
  }
  {Correspondence chess}
  {
  R0lGODdhIAAgANMAAAAAAAAAqgCqAACqqqoAAKoAqqpVAKqqqlVVVVVV/1X/VVX///9VVf9V
  ////Vf///ywAAAAAIAAgAAAEuPDJ+YC1NOs97fkfxo0UcDxgKpIcgL5p6LLZPJ24atM7leu8
  1waHAo5MMM4PtKoliRniwTPdmFLJ4ZWpuVI/2U6sWgsxvk/UltrzMs5f6TpEFk/fF/RpLivf
  zzJUaipqbTKATBd0XFNta1uJi5E9EnFjTJd1XZCXnFwkliqZlJueU52kNQYGlpxNoKusX480
  JbEeuI21qqsmvhe7Xbe/wS23p6nBAMO6xcK9yM7Pss3SGdAuydKvNBEAOw==
  }
  {Computer chess}
  {
  R0lGODlhIAAgAOezABVBgRtFhRFKiSBHiBNLihVMjCVMhzpmokNpmj9qpzhuqkFsqUxrl0FupE1s
  mEpspDpwrERurE5tmTtxrUNwpj1yrkRxp0VyqD90sEt3rkV5tUx4r055sVB7s1F8tFJ9tVN+tlV/
  t0yCuHx6flt/slaAuX17f1yAs1eBuk+Eu12BtFiCu1CFvFaEtVyDsF6CtVmDvFGGvV2EsV+DtlqE
  vVKHvoF/g2CEt1uFvlOIv1mHuGaEsmGFuFSJwIOBhVqIuVuJulaKwWOHulyKvF2LvViMw1mNxF+M
  vmWLuGCNv2GOwGeNulyQyGKPwWOQwmmPvGSRw2qQvWWSxGaTxWyTwGeUxm2UwWiVx26VwmmWyZKQ
  lG+Ww2qXyo6SlZORlXWWvnCXxGyYy5SSlnaXv3GYxW2ZzHeYwHKZxpaUmHOax3mZwpeVmWeez3Sb
  yJKXmXqaw3Ccz5iWmnubxJmXm3edynydxX6fx56coIChyZ+doaGeoqKfo6Wip6ekqamnq7K0sbO1
  srS2s7a4tby+u72/vL7BvcDCvsHDv8XHxLvK3rzL38jKx73M4MnLyMrMycvOys3Py87QzM/RztLU
  0dPV0tTW09XX1MzY5tLY2tbY1c7a6NXa3Nja1s/b6tbb3dnb19rc2dvd2tne4d3f3N7g3eDi39rj
  6+Hk4OPl4eTm4+Xn5Obo5efp5ujq5+nr6PL08fn7+Pr8+f7//P//////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////yH+EUNyZWF0ZWQgd2l0aCBH
  SU1QACH5BAEKAP8ALAAAAAAgACAAAAj+AP8JHEiwoMGDCBMqXDjQi5iHECNKlNhFYSNZGDNq3LgR
  kkJHlwAMECCAAIECBQyoXLkyAKNIHzVZeHFDCJIlUaiAGTMGzJctQLfISAQzoSNND0io4AEEp5Ut
  ZM6oIQMGKBYXRGM2IFEzyRMqW8CkkUP2jZozZHZkNaqJQosfQJREwSL2jRw7dtpsiQJkw1qERynM
  AJIEihUwZ9rQwQNmRYcNGCocUFQUsKYIN4A0kYKFjOI0OT6I7qAhQwLKMS/8OOLkcJo2TXrkqMEC
  xQcPHBagZquadZUsUo4YUZKkSBAcLEp4gLDbMoQUNXoQbgJlyhQpTZIEyZEiBPPKB4/+KhABo8eR
  JlOyhFlfxYmRHskvNA+vSUEJFkGUMKkShg2cMldAoUQQNKBQwV/hmYKABA40yMCDED7YYIMIdAKe
  QY7IEsuGHHboIYeyeJTQIY2UaKIjKKaoYoqPIKKQKJHEGCMkNNL4yI2OmLjIIo14kpAenPwByJCA
  BGLkkUgi+QcndxxkgiiDJCnllEYOwskIBsUByypcouLlKad88gknZHJCyplokjLKK2sYtIkrrHC5
  ipeonFKIIHgKQgiYfPKJCiYFudEKnX2eMoohhxiC6CGkFMonK2gQFMcnMlYa4ySUZErJJJZW+kkc
  BM0xSiWklkoqJXpooaoWe1hiqqkio8xBkA98+GHrrbj2oWsfuPbqBx82MCTssMQWa+yxyBoUEAA7
  }
  {Sorted Index of games}
  {
  R0lGODdhIAAgANMAAAAAAAAAqgCqAACqqqoAAKoAqqpVAKqqqlVVVVVV/1X/VVX///9VVf9V
  ////Vf///ywAAAAAIAAgAAAEovDJSau9OOvNu/9gKALkE5wPqa5A10pJHGtvxrImqkrAcWC1
  i2zGS/kut1wglUwZf5UgbIh09qCT0ml5w1mflNcwsWmZwbyuuvQ1H52hs/z9WkvbVvoIfy0y
  11F8enFtCQGDgBZyKYeDIHKGPo4fco1vW5iZmDVmkZI8Y6FjAEtWllh2ajqebxSir2OnGJq0
  mpJYF7e6u7scvL+3IsLDxMUUEQA7
  }
  {Player collection}
  {
  R0lGODlhIAAgAOfIAABO/gBQ/AJU8QBU/gNV8QBW/QBb/gFb/gBc/QBg/gBh
  /Qxe7wFh/gBl/gFm/gBn/QBr/gBs/RRk9gNt/hJo9wBy/gBz/gB3/gB4/gB9
  /gt4/gB+/gJ+/gCC/gt9/gCD/gCJ/gCK/gGK/hKD+wCP/g6J/gGQ/hKI/CV/
  /gWR/gCU/jV97wGV/jZ8/hCQ/gCZ/hyL/h2L/gGa/i6F/g+V/huP/hCV/i+G
  /hyQ/gCg/iSP/gGh/gud/gai/iSV/B2b/gCr/gGr/iea/hCm/gCw/ham/gGx
  /kiQ+A+v/hCv/hCw/jad/iml/i+i/huu/hSy/RC1/RC1/kiZ/liR/gq6/lmS
  /gu7/kid/iO0/hW9/Ra+/jqs/l2d9Ce3/k6k/lyf9Su3+iy4+T6u/jmx/i65
  +Se8/j2x/lyj9ju1/lym9yXC/lyp+Hmh3GWl/ki1/lys+XCk/lyw+mWs+km6
  /nCn/lW2/lyz+zbH/TfH/TjI/V22/DDN/l25/Wq1/kbI/E/E/kjI/F69/V/A
  /lvE/lzE/my8/lvF/lzF/mDD/nC7/kvQ/WHG/oG3+XK+/UvS/W/A/oG5+VfQ
  /WPK/oe4/lvP/FnQ/oC9/om5/lrS/XfE/lzS/X3G/n7G/nvI/nDP/pC//nDR
  /m3T/pDC/oPK/n7S7m/b/pXJ/nvY/JrK/pvK/pDR/o3T/pvM/p3M/oTc/p/S
  /oze/rbO/qfW/qnW/qLa/rbQ/rjP/q3W/rjR/qfa/rbU/rjU/rbW/rjW/rba
  /rjb/rDg/q3i/rLg/rbf/q3k/rjg/rjm/rjn/q6urq6urq6urq6urq6urq6u
  rq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6u
  rq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6u
  rq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6urq6u
  rq6urq6urq6urq6urq6uriH5BAEKAP8ALAAAAAAgACAAAAj+AP8JHEiwoMGD
  CBMqXMiwocOHECMmJIUpksWLGDVJ/KcIz52PIEPmcSSxVLFilaxQUeMKFiwt
  VKzskXjnJCgjRMoQO3YsChEiWfwAGkq06FBKBGsW8xQESJdgxowpAQIECpgw
  WLNqxUrm1MA/gwgNOkSWkFlDZLEgScK2rVu2T0INdNKjxxxgwlYNqUtLmDA0
  OQILHix4h6SBRV68cDOsmCoeinOdNKO4smXLMhYNHLNlS51NnB6J6ZyJEycm
  KlKrXq2aBaKBnWTNKmSDRpNXs2b9oEEjBYnfwIMDNyFo4ChfvxKJACHk1q9f
  LkBIn069OogQgQY2YtWqD44aS0zPtWpVooP58+jTd/jAZ6APGDG8oEplSUeM
  GBwy6N/Pv3+GDXoMdMIFF1zBSy+ieEDgggw22CAGdgw0QgUVSKHLLp9oQOGG
  HHbYoQVxDCTHJJe0ccMMKEwAwYostuhiixG8MRAkteBChwMN5Kjjjjz22MAD
  awzESCy2wMFAAkgmqeSSTCagQBoDHTFFFS0cYMCVWGap5ZYGIHDGQBQMIOaY
  ZJZpZpkFfDGQBAC06eabcMYJZwBcDLQCAQLkqeeefPa55wJsbCTooIQWauih
  BwUEADs=
  }
  {Tournament: All-play-all}
  {
  R0lGODdhIAAgAKEAAP///wAAAAAA//8AACwAAAAAIAAgAAACZoSPqcvtH6KMYM6HM7RVdq+F
  4kg2gnEC6YqW7usIsmwExnDnAL7rmA1rAF0zWq+i4yl9mmFQ4QwVU0vkscojRZ+HbayY1AGx
  QS/XnJiGr7X1E13+ztZka5ZrxSO8ara/CqcnOJhQAAA7
  }
  {Tournament: Swiss}
  {
  R0lGODdhIAAgAIAAAP8AAP///ywAAAAAIAAgAAACRYSPqcvtD6OctFoQss437d0hnxYeI1di
  Z6qO7BlU8Pw59B03OG3vcO9z6YJCBrG4ONYov1Sz9AxFO9NL1XJlabfcrldRAAA7
  }
  {Grandmaster games}
  {
  R0lGODlhIAAgAOevACAgICUlJSoqKi4uLjEwMDMzMzY2Njg4ODo6Ojw8PD09
  PT49PT8+Pj8/P0BAQEREREZEREVFRUlHR0lISEpKSk1KSk1LS0xMTE5OTlFR
  UVVSUlNTU1ZWVllVVVhYWFtYWFxaWltbW1xcXF1cXF5cXF9cXF5dXWJfX2Bg
  YGFhYWViYmVjY2RkZGZmZmllZWplZWhoaGtnZ2xoaGxpaWpqam1qam9ra3Ft
  bW9vb3FxcXR0dHhzc3Z2dnd3d3p2dnh4eH13d3x4eHp6en15eX55eXt7e4B6
  enx8fIB8fIN9fX9/f4CAgIR/f4GBgYZ/f4KCgomCgomDg4WFhYaGhoqFhYyG
  ho6Hh4qKio+IiIuLi5CJiZCLi5KMjJCQkJaOjpGRkZKSkpeQkJeRkZSUlJWV
  lZqTk5uUlJ6Xl5qampycnKGamqKamp2dnaKbm6Sbm56enqScnKWdnaCgoKef
  n6KioqmgoKqhoaWlpayjo62kpKioqK+lpbCmpqurq6ysrLOpqbOqqrasrLet
  rbeurrGxsbKysrmvr7Ozs7S0tLyysra2tr2zs76zs7+1tbq6usK4uMO4uLy8
  vMO5ucS5ub68vL29vcS6usW6usW7u8a7u8e8vMDAwMe9vci9vcfExMfHx8nJ
  yczMzM7OztLNzc/Pz9LS0tTU1NbW1tra2t3d3eLi4uTk5Ojo6Orq6uzs7P//
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  /////////////////////yH5BAEKAP8ALAAAAAAgACAAAAj+AP8JHEiwoMGD
  CBMqXMiwocOHECMqdESxokVHEv/pYcOxo8eOfiIeqhQpkpyThEoiOklHoiNX
  MH/I3ASTlMwfTyIqasVTh89KPEP51DGmi9GjSJGiEViIlVMcUB05BQUVx5U0
  WLNq1XpH4BQhYE+JTQP2i9hTP2ioXcuW7RKBR1rIXUWXjFwpdFfxkMu3r18d
  An+kGKyq8JfBTQqryjG4sePHOARmeUO5suXKLERo3syZMw2BclKJ9kC6j2hK
  ahJhkgRoiwnSnWJTIe1hRexO/9Kg2r2htx5UozR1wiMmTqZOg0ZsuD2n9wYk
  t/+B+UQdhXU5njh18oGhe43jXDD+xF4kKUP3M41i/0NBof1ZJWU6wWlPf8iN
  CxRit+k0oz0jOOqFEMGAphRYhCCdBDHgggzGlkQnVURAQidMqMeBAxiWomEP
  k3QSA4YghhibC4nw4QARnYCgXhF+tGgde5B04kICNN4WWwKxqeBFJxOswYgC
  6k0hypAHFHnAH53sYGQUUZgR2wGxnRCDh5CEASVuT4SiZQFcFmBFJ250yWUJ
  sRUQWwkMaFJHJzKYiRsMRw0g5wASXNKJDXMS8GAnA8T2wQB2xLZAn7g1IMCh
  iB76gnBwOKGFIbF5IUBsHQgARCd5HKoeAgF06qmnEGARiCWP7AFFBZ3GpkEA
  FnRiRKoduFEAwKy01mrrrbgaMNADDPbq66+9ZiTssMRCFBAAOw==
  }
  {International Master games}
  {
  R0lGODlhIAAgAOeNACAgICUlJSoqKi4uLjMzMzQ0NDY2Njg4ODo6Ojw8PD09
  PT49PT8+Pj8/P0BAQEJCQkREREVFRUZGRklISEpKSk1LS05OTk9PT1FRUVVS
  UlNTU1RUVFZWVllVVVhYWFtYWFxaWltbW1xcXF5cXF9cXF1dXWJfX2BgYGFh
  YWViYmVjY2RkZGhlZWZmZmllZWhoaGtnZ2xoaGxpaWpqam9sbG9vb3Jvb3Fx
  cXVycnR0dHZ2dnl2dnd3d3h4eH13d3p6en55eXt7e4B6enx8fIB8fIN9fX9/
  f4CAgIR/f4GBgYKCgoWFhYaGhoqFhYyGhoqKiouLi5CQkJaOjpGRkZKSkpeQ
  kJSUlJWVlZ6Xl5qampycnKKamp2dnaKbm56enqScnKCgoKefn6KioqmgoKqh
  oaWlpa2kpKioqKmpqbCmpqurq6ysrLGxsbKysrOzs7S0tLyysra2tr2zs76z
  s7i4uL+1tbq6usO4uLy8vMO5ub29vce8vMDAwMi9vcfHx8nJyczMzM7Ozs/P
  z9LS0tTU1NbW1tra2t3d3eLi4uTk5Ojo6Orq6uzs7P//////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  /////////////////////yH5BAEKAP8ALAAAAAAgACAAAAj+AP8JHEiwoMGD
  CBMqXMiwocOHECMqtEOxokU7Ev+d4cKxo8eOayK60YMHD5iTbEq+OSlGoh1G
  MHvI5ANTkMweSiLGWcQzh089PAH5zGElitGjSJFmEdhGkdMaUO04/QO1xhMt
  WLNq1VpGIJMfYAuJ1QJ2ithCPWaoXcuW7RGBQ1rITUT3itwldBPpkMu3r98c
  AnugGIyo8JTBSQojujG4sePHNQRC8UK5suXKK0Ro3syZ8wyBYA6J9kBajWg8
  Hvr02ZFadRPSHlSoXu2hhUAthnJr2H0mNx0NqnEAVx1mtwYis4WjEEjFj/MT
  0ME4R2NBtY3qfeTkwWDBApY61i3+lBB4goL5s0bMm1dNg4LqLn1kmJ/zhT0F
  DwJDRNhPqH+Q/fupFkMEqhXRhxMRjNAHEgJGsIFAHDgg4SAU8iChhKqx4IBq
  LsCRhgNA9AFChg5cIFAQa6QIXXkJtJiAaim82EcKUvQxwRZzKABjAhR4FciP
  BwQpZJCqmXBAkTD0AcMdVRzZh5ESCKQEIFQSYOWVVqpGAgFaMrDHGH3EwGUf
  Wz4g0AtHDaDmmmqq9sEAbg5AhmoLwNnHmwoI1IAAfPbppwCqdQBoH4L60IcZ
  fAYqwAECIRDAo5BGGoBqGUzaR6UV9CHEo5QGUIBAFAAg6qiklmrqqQYMBAGA
  rLbq6qsJ+2Uk66y0QhQQADs=
  }
  {Blitz (fast) games}
  {
  R0lGODlhIAAgAOfxACEhISUlJSoqKisqKS4uLjQyLDMzMzQ0NDY2Njg4ODk5OTs7O0I8Hjw8Ozw8
  PD8+Oj4+Pj8/P0FBQUdCLEJCQk5FG0REREhGPUZGRkdHR0pKSk1LQktLS09PT1BQUFpSK1RUVFtV
  O2FXKFdXV1pYT1hYV1hYWFlZWVxcXGNdQl1dXWBgYGNhWHNjFmFhYWJiYmljRXBmM2VlZWZmZmlp
  aX1sGmtra3ptL3NuWW9vb3BwcIRzHnJycnR0dIB2R3x2WHZ2dnh4eHl5eXp6epJ+HIp9P3x8fH19
  fX9/f5uDDoKBfIKCgpaFMIODg4aGhoeHh6CLJoqKiqmOCqeOE4yMjJCQkJGRkZKSkpOTk7CXGreZ
  B5WVlbqdEb+fAr6gCcOhBZ2dnb+hFL6hF56enp+fn8OkDKCgoMWlA8elAaGhoaKiosunAMioBMup
  AsypAsupBsypBsuqBc2qAs6qAMyrAqampsysBM6rC9CsANGsANGtANKtAM6uCKmpqaqqqtCvCtSx
  BKysrNOyBdSxD9GzBdW0DtW0G7Kyste6A9m4E7S0tNm5Gtm5Idq8AbW1tdq6Jre3t7i4uNq8Lbm5
  udu9Lt7BCrq6utzAG7u7u9vBJtrAOLy8vL29vb6+vsDAwMHBwcPDw+PLKODIU+nPEsjIyOzTCMrK
  ys3NzezVLe/YDe3VNO/YFM/Pz/DZEPDYHNDQ0PLbFvLbF9PT0/PdG/PdHPTdHdbW1vLeP9fX1/Xg
  KPXgLNjY2OvcgvThP/biMffiNfXiPffkOPXjSfbjSPbjSdzc3PPjV97e3vjnTPXmZ/noTPnoTvfn
  XuLi4vrpXPrqYOTk5Pjqc/vrYfTolPvrYvvraPrscPnsfPrtdPntfvvucejo6PzucvfsmvzvdPnu
  kf7wfuvr6/3xhPrwnP3yi+3t7f3ymPjyxPv0sP32tv33wP351P/60f364P/84v797v/+8///////
  /////////////////////////////////////////////////////yH+EUNyZWF0ZWQgd2l0aCBH
  SU1QACH5BAEKAP8ALAAAAAAgACAAAAj+AP8JHEiwoMGDCBMqXMiwocOHCj95mkixoidQEAUeGsOx
  o8eOkTJO6sSJk5mTh0o6OqkGU0ZM5WIKmfkp5quZQtJkhBSuZ4+fnXqe+tlji5WjSJMmrSNQ0ban
  OaJiemoqao4oYLJq3bqV6b8nQ8LiGgsmrJWxuILYWGvjxyBKkh4xWpQIUBglAo/M2Aut75a9TvpC
  A7J3r5hz8NyxS/fN2B8cPQQGeUG5meUrlJdYbsaD8osiot6REwfOWrJCPl7oEEiFjGuPrzvKUKGC
  xZ125rxx0xbtEhPaNgSmOUbchPFAxDcZX24CCi91z6hJwxYqSwnjMwSCKcYdhPc+3C3+eR8Pw9A6
  Z8qWVVNVhsT4FwKxkJq/on6a+X466NfPZVq2X8Egc0scKezXgQoCraDBgrs0iMSCEC54gybo6JKL
  L70IMoccbrTxRQwnCIQCBiTaYqIRJKaIwQZvdDPONcwAk8swxAiTiRchYACCQCNI4KMsQAbh45AS
  ELGHHnnggcostNTiCh9TXOCjBwIZEciVLmSpgQNcdtnlA3CsAksso7CxQwNdciCQE6y0mcCbcMYJ
  5wd2pNJKJWeIIGcGAjVxyp8GBCrooILWgEgphGgxAaEGUCAQDVVESsCklFZKqRSN0JFEAZZOCoFA
  EQgg6qikljoAGmu0UCqpCgi0QAAtsMYq66wMdFHBrLMeIJAFAPTq66/ABissAgOhQNuxyCar7LIZ
  Nevss9BG21BAADs=
  }
  {Tactics}
  {
  R0lGODlhIAAgAOeKACAgICUlJSoqKi4uLjMzMzQ0NDY2Njg4ODo6Ojw8PD09
  PT8/P0A/P0BAQEJCQkREREZEREVFRUZGRkpKSkxLS05OTk9PT1VSUlNTU1RU
  VFZWVllVVVhYWF1aWltbW1xcXF1dXV9eXmBfX2JfX2BgYGFhYWNhYWRiYmVi
  YmVjY2RkZGhlZWZmZmllZWhnZ2hoaGtnZ2toaGxoaGpqam1qam9ra29sbHJt
  bW9vb3Jvb3Rvb3FxcXZxcXVycnR0dHdzc3Z2dnl2dnd3d3h4eH14eHp6ent7
  e3x8fH9/f4CAgIGBgYKCgoWFhYaGhoqKiouLi5KMjJCQkJGRkZKSkpiSkpSU
  lJmTk5WVlZqampycnJ2dnaObm56enqCgoKKioqWlpayjo6ykpKioqK+mpqmp
  qaurq7KpqaysrLGxsbKysrOzs7S0tLuxsbyzs7a2tri4uL+1tcG2trq6ury8
  vMO5ub29vcS6usa7u8e8vMDAwMi9vcfHx8nJyczMzM7Ozs/Pz9LS0tTU1NbW
  1tra2t3d3eLi4uTk5Ojo6Orq6uzs7P//////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  /////////////////////yH5BAEKAP8ALAAAAAAgACAAAAj+AP8JHEiwoMGD
  CBMqXMiwocOHECMqlEOxokU5Ev+J0cKxo8eOZyKqqTNnTpeTaEquOelFopxE
  MIfIzAPzj8whSyK6QcTTh886PPv49FElitGjSJFiEZjmkFMcUOU45QMVh5Ms
  WLNq1fpFYJMiYAWJzQJWilhBQ2aoXcuWbRKBR1jINUT3ilwmdA0Bkcu3r18f
  AoeUGFyosJTBSgoX2jG4sePHOAQ+4UK5suXKKj5o3syZ8wyBXQiJ5kC6jOg5
  pFPr0ROEg4stcO6woRKCNAuBWQbpxsBbjO43vIOv7iECjp4xVtroCcO7hMAp
  e6KTmN4lOpkK2LGvzgFFDxjsJ1bYp6gAQiCJCejPIkHPnv1qGzWImEBfYXWM
  CRwEeojAP5B/I/wFGOBqMggYwQ96xEFBBBkIpEEDEAIioRAQVljhaitYSAMd
  dsAAoQUCGXHGiNOdl8CJKJ64GgootoBHHB2gOIFXftR4wI045njjaiPgaIYe
  POQogUBL9GEkAUgmqSSSOugAQZI36MCAkg4I9MJRA2Sp5ZZcdrmlAgItIMCY
  ZJZp5mobkIlmmQcIhEAAcMYp55yrXRBnnXIWINAEAPTp55+ABiqoAQM9YOCh
  iCYaQUaMNuooRAEBADs=
  }
  {Endgames}
  {
  R0lGODdhIAAgAKEAAP///wAAAP/73wAAACwAAAAAIAAgAAAChYSPqcsZDWOYsa5ns5lYXwoE
  AsZ54TQ+orqCFToK6RynVsulONpBO6/L9Ro/Hkd4Kx6FQ2JLBpUZPbBltfmyWjeg0ql3jAY3
  wTJYuySLxec1tIs+OuJD0RpLRZsUdvbe4Yan0RclmEH49oeAKKV4wDjiyKUlCcNm6EPnMghZ
  iMmn6SXJUAAAOw==
  }
  {Openings for White}
  {
  R0lGODdhIAAgANMAAAAAAAAAqgCqAACqqqoAAKoAqqpVAKqqqlVVVVVV/1X/VVX///9VVf9V
  ////Vf///ywAAAAAIAAgAAAErvDJSau9F+jNu9cYMBxDaZ4oCoRj6rqgJZJvXQLHWs22jec6
  Ca/3+gWCjyExZUQqURLf4XCkPE2UWtPaSmWL0+rkWvrCws6u9yGlpmnQiRbNhWMrYHd9bVbR
  x2plF2d6gHYDGIRiQoGIg0x/jIdLN5FJjZQDW4aZfoWSnSeboKGVn5eTlKOopaaLSRqtmpY/
  H7YetD9Tu7y9vpawv8LCr0IbAcjJysvMSFa30BwSEQA7
  }
  {Openings for Black}
  {
  R0lGODdhIAAgANMAAAAAAAAAqgCqAACqqqoAAKoAqqpVAKqqqlVVVVVV/1X/VVX///9VVf9V
  ////Vf///ywAAAAAIAAgAAAErvDJSau9F+jNu9cYMBxDaZ4oCoRj6rqgJZJvXQLHWs22jec6
  Ca/3+gWCjyExZUQqVaJak/I0caSHw5HaYm6wWmcXqgFvJ1VrtJg9C8enKzvMpcU7c3cSfsPD
  2mJ2fR5/dGh8H2txgHVkfiqMh4JLVpFvk5QDU5KZkIaXnYufe5iUm6ChN5akqaqjX62nSVmJ
  tRyrGlm6u7y9u3pJP77DvcDBGgHJysvMzUhUttExEQA7
  }
  {Openings for either color}
  {
  R0lGODdhIAAgANMAAAAAAAAAqgCqAACqqqoAAKoAqqpVAKqqqlVVVVVV/1X/VVX///9VVf9V
  ////Vf///ywAAAAAIAAgAAAEuvDJSau9F+jNu9cYMBxDaZ4oCoRj6rqgJZKq+JrAsVZzrd2l
  nG4n6Z04wIEwQHwYcZvksvmEJpWHA5NSVUaBU24LiryFJ1UPOLtFj73q17n4/tgkKja1/hlQ
  jnpiNEB/OIFugzeFQYd0iYoPeVp7jy6LWJOCSRaGmYibFZ1tjoQXjJ6kkJyYo05vlqaslFdy
  ja6VtLJcP7mSrUJ2wRy2GlnGx8jJx61OQsrPyczNGgHV1tfY2U27wt07EQA7
  }
  {Theory: 1.c4}
  {
  R0lGODdhIAAgAKEAAP/739jGpgAAAP///ywAAAAAIAAgAAACfYRvoauIzNyBSyYaLMDZcv15
  GDiKFHmaELqqkVvBXCN3UniXebq3/fuLBWlDmwN31CV5S18T+BRqokSqTSCYKB8lwWCQ3Wy1
  PCzZyU15wRdmerX+ZpHvCPY+H7fRZ2h9VvUX2CclaGTI4kc4uKfYWLh4GJkI+cgo5jZZo1EA
  ADs=  
  }
  {Theory: 1.d4}
  {
  R0lGODdhIAAgAKEAAP/739jGpgAAAP///ywAAAAAIAAgAAACf4RvoauIzNyBSyYaLMDZcv15
  GDiKFHmaELqqkVvBXCN3UniXebq3/fuLBWlDmwN31CV5S18T+BRqokSq8aEQCKRYJlYwGGyv
  E2VXyy17y2Bxtet8tMNbFlSuzZMva35cPXMHKOhHuGE2mFaoeNjX+Lf4ligZufdoiASHiVip
  UQAAOw==
  }
  {Theory: 1.d4 d5}
  {
  R0lGODdhIAAgAKEAAP/739jGpgAAAP///ywAAAAAIAAgAAACh4RvoauIzNyBSyYaLMDZcv15
  GDiKFHmaELqqkVvBXCN3UggIwlTepa67pHzDoLBFbAF3m57jswzinqPobvpwZocaKneL9Opi
  yZdPMBgIaGWyeM32hrNodRxsftbTa1b+YWUj98fzVThDeJTYZKjohvd4uIglGel4V4l5KQip
  yTiXyRnqZwlQAAA7
  }
  {Theory: QGD: 1.d4 d5 2.c4 e6}
  {
  R0lGODdhIAAgAKEAAP/739jGpgAAAP///ywAAAAAIAAgAAACmYRvoauIzNyB6wgxaZBJm3t1
  FAdo24eJEGmiYelxLQjCo+zRYYvfksn7xYS+l23VQ9JUkWQTYAw6eMvjc5rTWSvOig4jfQyx
  PhIZSURHFRduuikYDFJsuaDRxUNTtrb+DGfXETf3JzZCKBiQWMiStbSoc/LmBlh5CHSFqZlR
  tnnZiWZpGMrJBHpKmjo5yvqpugUbVooa63pQAAA7
  }
  {Theory: Slav: 1.d4 d5 2.c4 c6}
  {
  R0lGODdhIAAgAKEAAP/739jGpgAAAP///ywAAAAAIAAgAAACmIRvoauIzI4QBy5XbTBzJo19
  lkSJUAhoG9CVKvq23hvGcgrWOUbz6u7oBXceU0Q3YnWMF+SJU8IlfckWs+GMyCjCB2grfVLF
  wymqTPZOi9JJc3wUDAaugJyOhb/ZC3ceHTdXdyfwp3ZyJ5iSiAfzdWMns6JnmGF2+PMGqInJ
  aZn2eUQ5uVl5RdppGiZaihrqeep4+aradVAAADs=	  
  }
  {Theory: 1.d4 Nf6}
  {
  R0lGODdhIAAgAKEAAP/739jGpgAAAP///ywAAAAAIAAgAAACioRvoauIKqJoLrFVrYwhg4tl
  wcZ5IFWNZCeeJiexqdtCZX29q0lXZNxznGTC4MN4GPJAyxyO0YQ+QzPmFFW0Vp1baZfqyXLF
  3iwHe9QWBYPBhJgep8/wJFLTfkfBD3bbXWeh1rdzFfhxhzg0aMcoKNcI+VgWRxlpOcl3qZmJ
  tunZeahkuAeqKAlQAAA7	  
  }
  {Theory: 1.e4}
  {
  R0lGODdhIAAgAKEAAP/739jGpgAAAP///ywAAAAAIAAgAAACfYRvoauIzNyBSyYaLMDZcv15
  GDiKFHmaELqqkVvBXCN3UniXebq3/fuLBWlDmwN31CV5S18T+BRqokSq8aETCCZKbEowGGw3
  XS5Pa3Z6W2DxhbmOtMNbZLyizdfLb3V6JnVXJXj1F2g4iFjYB0XI0qj4eMg4SQYXWSOZaFAA
  ADs=
  }
  {Theory: Sicilian: 1.e4 c5}
  {
  R0lGODdhIAAgAKEAAP/739jGpgAAAP///ywAAAAAIAAgAAAChYRvoauIzNyBSyYaLMDZcv15
  GDiKFHmaELqqkVvBXCN30iII8V3yeU7jpYTAoCPk+xkfveNPyXrdnsDo7thkDjXYrbYlBOcm
  WXJKMBgILl6zeM0Gd1todVw6j9TTa2S+QtVXdnf1NVPohmeISLiUyLgxGNnWaPPnWGmFOSm3
  uOnnaRmqUQAAOw==
  }
  {Theory: Sicilian: 1.e4 c5 2.Nf3 d6 3.d4}
  {
  R0lGODdhIAAgAKEAAP/739jGpgAAAP///ywAAAAAIAAgAAACo4RvoauIzNxZQiyZYMOq1i1p
  oENZ4yNiQOedU8p5rSoGtPzRcLiTdY/S6AIfV0bIIbZsSSRPeWlCdDLjCjjBWYbTGI757IYj
  UrJqHEVbS+nKy+kTDAYmg5x+FPugIHfeHDdXB3An8JdGcidoo4hXc/NFiHMFF6Rn+YOIGSVz
  uJbSaPhYRjFnKorlSURXSHn5BuFB6JjqatY6qsaoiFoJy6BVUQAAOw==
  }
  {Theory: Caro-Kann: 1.e4 c6}
  {
  R0lGODdhIAAgAKEAAP/739jGpgAAAP///ywAAAAAIAAgAAACh4RvoauIzI4QBy5XbTBzJo19
  lkSJUAhoG9CVKvq23hvGcgrWOUbz6u7oBYEPYmakQ/qUQ2bReTwlpUtq0/rERiNTLqrqvYaz
  Xo9pvL2wBgOX8axmlXBQeGPdtq/AFwGbTSFE1ncjmHbH97MnFseI6Lg4+CgZeVipB/NGp4XZ
  1UiZWbeJ1olRAAA7
  }
  {Theory: French: 1.e4 e6}
  {
  R0lGODdhIAAgAKEAAP/739jGpgAAAP///ywAAAAAIAAgAAACiIRvoauIzNyB6wgxaZBJm3t1
  FAdo24eJEGmiYelxLQjCo+zRYYvfksn7xYQ+R9A4RBYfSeYy81RFeivqlFjFXpVZ7pbk7Ia/
  0OzLJi5PBYNB6jiuoNRyrZztlta9FXwbA0fXoAPYJHhiB4TI12C1F9eYuAg5echiqBfJWJnJ
  iUbWeRn1+WjJUQAAOw==
  }
  {Theory: Open Games: 1.e4 e5}
  {
  R0lGODdhIAAgAKEAAP/739jGpgAAAP///ywAAAAAIAAgAAACh4RvoauIzNyBSyYaLMDZcv15
  GDiKFHmaELqqkVvBXCN3UngrghDnab7b0Xwtn3DowCUDQZ5t+Us2hazXbcqr9qBFDdf63T5K
  gN2EHKYJBgPBJTr+md/deGvdpoPtETzbrcRXgbWBJoh0OKOFeAbXWPe4Fyk2yahHeWlZ6Jj5
  lLjoWRnaCapRAAA7
  }
}

variable ::windows::switcher::base_types {}

#TODO: remove the no-icons option
if {! [info exists ::windows::switcher::icons] } { set ::windows::switcher::icons 1 }
after idle "options.save ::windows::switcher::icons"

# Initialise icons nicely

set i 0
foreach {icon data} $icons {
  lappend ::windows::switcher::base_types $icon
  image create photo dbt$i -format gif -data $data
  incr i
}

set numBaseTypeIcons [llength $::windows::switcher::base_types]

set temp_dbtype 0

proc getBaseType {baseId} {
  foreach {tagname tagvalue} [sc_base extra $baseId] {
    if {$tagname eq "type"} {
      return $tagvalue
    }
  }
  return 0
}

proc selectBaseType {type} {
  global temp_dbtype
  set w .btypeWin
  if {![winfo exists $w]} { return }
  set temp_dbtype $type
}

proc changeBaseType {baseNum} {
  global temp_dbtype ::windows::switcher::base_types numBaseTypeIcons
  if {$baseNum > [sc_info limit bases]} { return }
  set temp_dbtype [getBaseType $baseNum]
  if {$temp_dbtype >= $numBaseTypeIcons} { set temp_dbtype 0 }
  set w .btypeWin
  win::createDialog $w
  wm title $w "Scid: $::tr(ChangeIcon)"

  ttk::style configure btypeWin.Treeview -rowheight 36
  ttk::treeview $w.t -columns {} -show tree -selectmode browse \
        -yscrollcommand "$w.yscroll set" -style btypeWin.Treeview
  bind $w.t <<TreeviewSelect>> "selectBaseType \[$w.t selection\]"
  $w.t configure -height 9

  ttk::scrollbar $w.yscroll -command "$w.t yview" -takefocus 0
  pack [ttk::frame $w.b] -side bottom -anchor e
  pack $w.yscroll -side right -fill y
  pack $w.t -side left -fill both -expand yes

  dialogbutton $w.b.set -text "OK" -command \
    "catch {sc_base extra $baseNum type \$temp_dbtype}; ::windows::switcher::Refresh; ::maint::Refresh;
     focus .; destroy $w"

  dialogbutton $w.b.cancel -text $::tr(Cancel) -command "focus .; destroy $w"
  packdlgbuttons $w.b.cancel $w.b.set

  set numtypes [llength $base_types]
  for {set i  0} {$i < $numtypes} {incr i} {
      $w.t insert {} end -id $i -image dbt$i -text " [lindex $base_types $i]"
  } 

  bind $w <Up> {
    if {$temp_dbtype != 0} { selectBaseType [expr $temp_dbtype - 1] }
    break
  }

  bind $w <Down> {
    if {$temp_dbtype < [expr [llength $::windows::switcher::base_types] - 1]} {
      selectBaseType [expr $temp_dbtype + 1]
    }
    break
  }

  bind $w <Home> { selectBaseType 0 }
  bind $w <End> { selectBaseType [expr [llength $::windows::switcher::base_types] - 1] }
  bind $w <Escape> "$w.b.cancel invoke"
  bind $w <Return> "$w.b.set invoke"

  focus $w.t
  grab $w
  update
  selectBaseType $temp_dbtype
}



proc ::windows::switcher::pressMouseEvent {i {w .baseWin}} {
  if {! [winfo exists $w]} {return}
  foreach win {"" .img .name .ngames} {
    $w.c.f$i$win configure -cursor exchange
  }
}

proc ::windows::switcher::releaseMouseEvent {fromBase x y {w .baseWin}} {
  if {! [winfo exists $w]} {return}
  foreach win {"" .img .name .ngames} {
    $w.c.f$fromBase$win configure -cursor {}
  }
  set dropPoint [winfo containing $x $y]
  if {! [string match "$w.c.f*" $dropPoint]} {return}
  regexp -all {[0-9]} $dropPoint toBase
  if {$toBase == $fromBase} {
    if { [info exists ::sw_LinkedGList_($w)] } {
      if {[::windows::gamelist::GetBase $::sw_LinkedGList_($w)] != "$fromBase"} {
        ::windows::gamelist::SetBase $::sw_LinkedGList_($w) "$fromBase"
        ::windows::switcher::Update_ $w
      } else {
        popupmenu $w "$w.c.f$fromBase" $x $y $fromBase
      }
    } else {
      ::file::SwitchToBase $toBase
    }
  } else {
    set glist ""
    if { [info exists ::sw_LinkedGList_($w)] } { set glist $::sw_LinkedGList_($w) }
    ::windows::gamelist::CopyGames $glist $fromBase $toBase
  }
}

proc ::windows::switcher::popupmenu { {switcherWin} {w} {abs_x} {abs_y} {baseIdx} } {
  $w.menu delete 0 end
  $w.menu add command -label "[tr NewGameListWindow]" -command "::windows::gamelist::Open $baseIdx"
  $w.menu add separator
  $w.menu add command -label [tr FileOpen] -command ::file::Open
  if {![sc_base isReadOnly $baseIdx]} {
    $w.menu add command -label [tr ToolsImportFile] -command "importPgnFile $baseIdx"
  }
  if { [info exists ::sw_LinkedGList_($switcherWin)] && \
       [::windows::gamelist::GetBase $::sw_LinkedGList_($switcherWin)] == "$baseIdx"} {
    $w.menu add command -label "[tr ToolsExpFilter]" -command \
        "::windows::gamelist::FilterExport $::sw_LinkedGList_($switcherWin)"
  }
  if { $baseIdx != $::clipbase_db } {
    $w.menu add command -label [tr FileClose] -command [list ::file::Close $baseIdx]
    $w.menu add command -label $::tr(CompactDatabase) -command "compactDB $baseIdx"
    if { [::file::autoLoadBases.find $baseIdx] == "-1" } {
      set ::sw_DummyCheckbutton 0
      $w.menu add checkbutton -label "[tr LoadatStartup]" -variable ::sw_DummyCheckbutton \
        -command "::file::autoLoadBases.add $baseIdx"
    } else {
      set ::sw_DummyCheckbutton 1
      $w.menu add checkbutton -label "[tr LoadatStartup]" -variable ::sw_DummyCheckbutton \
        -command "::file::autoLoadBases.remove $baseIdx"
    }
  } else {
    $w.menu add command -label [tr EditReset] -command ::windows::gamelist::ClearClipbase
  }

  $w.menu add separator
  $w.menu add command -label [tr ChangeIcon] -command "changeBaseType $baseIdx"
  $w.menu add checkbutton -label "Icons" -variable ::windows::switcher::icons \
    -command ::windows::switcher::Refresh
  tk_popup $w.menu $abs_x $abs_y
}

set baseWin 0
set sw_nBases_ [sc_info limit bases]
set ::windows::switcher::wins {}

proc ::windows::switcher::Open {{w .baseWin}} {
  if {[::createToplevel $w] == "already_exists"} {
    focus .
    destroy $w
    set ::baseWin 0
    return
  }
  
  ::setTitle $w "Scid: [tr WindowsSwitcher]"
  ::windows::switcher::Create $w
  bind $w <Escape> "focus .; destroy $w"
  bind $w <Destroy> "+ if {\[string equal $w %W\]} {set ::baseWin 0}"
  bind $w <F1> { helpWindow Switcher }
  ttk::label $w.status -width 1 -anchor w -relief sunken -borderwidth 1
  grid $w.status -columnspan 2 -sticky we
  ::createToplevelFinalize $w
  after idle "::windows::switcher::Update_ $w"
}

proc ::windows::switcher::Create {{w} {gamelist ""}} {
  if {$gamelist == ""} {
    catch { unset ::sw_LinkedGList_($w) }
  } else {
    set ::sw_LinkedGList_($w) $gamelist
  }

  ttk::frame $w.border
  grid $w.border -sticky news
  grid rowconfigure $w 0 -weight 1
  grid columnconfigure $w 0 -weight 1
  autoscrollframe -bars y $w.border canvas $w.c -highlightthickness 0

  for {set i 1} {$i <= $::sw_nBases_} {incr i} {
    set f [ttk::frame $w.c.f$i -borderwidth 1 -relief raised]
    $w.c create window 0 0 -window $w.c.f$i -anchor nw -tag tag$i

    set f $w.c.f$i
    label $f.img -image dbt0
    label $f.name -width 12 -anchor w -font font_Small
    label $f.ngames -text "0" -width 12 -anchor e -font font_Tiny
    grid $f.img -row 0 -column 0 -rowspan 2
    grid $f.name -row 0 -column 1 -sticky we
    grid $f.ngames -row 1 -column 1 -sticky we

    foreach win {"" .img .name .ngames} {
      bind $f$win <ButtonPress-1> [list ::windows::switcher::pressMouseEvent $i $w]
      bind $f$win <ButtonRelease-1> [list ::windows::switcher::releaseMouseEvent $i %X %Y $w]
    }

    menu $f.menu -tearoff 0
    foreach win {"" .img .name .ngames} {
      bind $f$win <ButtonPress-$::MB3> "::windows::switcher::popupmenu $w $f %X %Y $i"
    }
  }
  bind $w <Configure> "+::windows::switcher::Update_ $w"
  bind $w <Destroy> { set idx [lsearch $::windows::switcher::wins %W]; set ::windows::switcher::wins [lreplace $::windows::switcher::wins $idx $idx] }
  lappend ::windows::switcher::wins $w

  after idle "::windows::switcher::Update_ $w"
}

proc ::windows::switcher::calcSpace {{w} {selected}} {
  global numBaseTypeIcons
  variable icons
  if {! [winfo exists $w]} { return }

  for {set i 1} {$i <= $::sw_nBases_} {incr i} {
      $w.c itemconfigure tag$i -state hidden
  }
  set n_bases 0
  foreach i [sc_base list] {
      incr n_bases
      set color white
      if {$i == $selected} { set color lightSteelBlue }

#      $w.c.f$i configure -background $color
      if {$icons} {
        set dbtype [getBaseType $i]
        if {$dbtype >= $numBaseTypeIcons} { set dbtype 0 }
        $w.c.f$i.img configure -image dbt$dbtype -background $color
		grid $w.c.f$i.img -row 0 -column 0 -rowspan 2
      } else {
        grid forget $w.c.f$i.img
      }
      if {$i == $::clipbase_db} {
        set name [sc_base filename $i]
      } else {
        set name "[file tail [sc_base filename $i]]"
      }
      $w.c.f$i.name configure -background $color -text $name
      if {[info exists ::sw_LinkedGList_($w)]} {
        set txt [::windows::gamelist::filterText $::sw_LinkedGList_($w) $i]
      } else {
        set txt [::windows::gamelist::filterText "" $i]
      }
      $w.c.f$i.ngames configure -background $color -text "$txt"
      $w.c itemconfigure tag$i -state normal
  }

  set iconWidth [winfo width $w.c.f$::clipbase_db]
  incr iconWidth 5
  set iconHeight [winfo height $w.c.f$::clipbase_db]
  incr iconHeight 5

  return [list $n_bases $iconWidth $iconHeight]
}

proc ::windows::switcher::Draw {{w} {numColumns} {iconWidth} {iconHeight} } {
  set numRows 0
  set column 0
  set x 0
  set y 0
  foreach i [sc_base list] {
      $w.c coords tag$i [expr $x + 2] [expr $y + 2]
      incr column
      if { $x == 0} { incr numRows }
      if {$column == $numColumns} {
        set column 0
        set x 0
        incr y $iconHeight
      } else {
        incr x $iconWidth
      }
  }

  if {$numRows < 1} { set numRows 1 }
  set right [expr {$numColumns * $iconWidth}]
  set bottom [expr {$numRows * $iconHeight}]
  set bgcolor [ttk::style lookup Button.label -background]
  $w.c configure -scrollregion [list 0 0 $right $bottom] -borderwidth 4 -relief flat
  ::applyThemeColor_background $w.c
}

proc ::windows::switcher::Refresh {} {
  foreach w $::windows::switcher::wins {
    if {[winfo exists $w]} { ::windows::switcher::Update_ $w }
  }
}

proc ::windows::switcher::Update_ {w} {
  if {[info exists ::sw_LinkedGList_($w)]} {
    set curr_base [::windows::gamelist::GetBase $::sw_LinkedGList_($w)]
  } else {
    set curr_base [sc_base current]
    if {![sc_base inUse $curr_base]} { return }
  }

  set space [::windows::switcher::calcSpace $w $curr_base]
  set n_bases [lindex $space 0]
  set iconWidth [lindex $space 1]
  set iconHeight [lindex $space 2]

  if {! [info exists ::sw_LinkedGList_($w)]} {
    set canvasWidth [winfo width $w.c]
    set numColumns [expr {int($canvasWidth / $iconWidth)}]
    if {$numColumns < 1} { set numColumns 1 }
    ::windows::switcher::Draw $w $numColumns $iconWidth $iconHeight
    set status [file nativename [sc_base filename $curr_base ] ]
    if {[sc_base isReadOnly $curr_base]} { append status " ($::tr(readonly))" }
    $w.status configure -text $status
  } else {
    #TODO: if $w parent is too small do not use 2 columns
    if {[winfo height $w.c] < [expr $iconHeight * ($n_bases)]} {
      $w.c configure -width [expr $iconWidth * 2]
      ::windows::switcher::Draw $w 2 $iconWidth $iconHeight
    } else {
      $w.c configure -width $iconWidth
	  ::windows::switcher::Draw $w 1 $iconWidth $iconHeight
    }
  }
}
