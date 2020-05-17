//////////////////////////////////////////////////////////////////////
//
//  FILE:       nagtext.h
//              Translationtable for NAG values to text
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.6
//
//  Notice:     Copyright (c) 2000-2003 Shane Hudson.  All rights reserved.
//
//  Author:     A. Wagner
//
//
//////////////////////////////////////////////////////////////////////

// 7-bit rewritten 
// following http://www.exeterchessclub.org.uk/nags.html

const char * evalNagsRegular [] = 
{ 
   "",                //         0 null annotation
   "!",               // $1      1 good move (traditional "!")
   "?",               // $2      2 poor move (traditional "?")
   "!!",              // $3      3 very good move (traditional "!!")
   "??",              // $4      4 very poor move (traditional "??")
   "!?",              // $5      5 speculative move (traditional "!?")
   "?!",              // $6      6 questionable move (traditional "?!")
   "forced",          // $7      7 forced move (all others lose quickly)
   "□",               // $8      8 singular move (no reasonable alternatives)
   "worst",           // $9      9 worst move
   "=",               // $10    10 drawish position
   "=, quiet",        // $11    11 equal chances, quiet position (DR: traditional =)
   "↹",               // $12    12 equal chances, active position (DR: ECO ->/<-)
   "~",               // $13    13 unclear position (DR: emerging &)
   "+=",              // $14    14 White has a slight advantage (DR: +=)
   "=+",              // $15    15 Black has a slight advantage (DR: =+)
   "+/-",             // $16    16 White has a moderate advantage (DR: +/-)
   "-/+",             // $17    17 Black has a moderate advantage (DR: -/+)
   "+-",              // $18    18 White has a decisive advantage (DR: +-)
   "-+",              // $19    19 Black has a decisive advantage (DR: -+)
   "+--",             // $20    20 White has a crushing advantage (Black should resign) (DR: +--)
   "--+",             // $21    21 Black has a crushing advantage (White should resign) (DR: --+)
   "⊙",               // $22    22 White is in zugzwang (DR: zz)
   "⊙",               // $23    23 Black is in zugzwang (DR: zz)
   "◯",               // $24    24 White has a slight space advantage
   "◯",               // $25    25 Black has a slight space advantage
   "◯◯",              // $26    26 White has a moderate space advantage (DR: O)
   "◯◯",              // $27    27 Black has a moderate space advantage (DR: O)
   "◯◯◯",             // $28    28 White has a decisive space advantage
   "◯◯◯",             // $29    29 Black has a decisive space advantage
   "↻",               // $30    30 White has a slight time (development) advantage
   "↺",               // $31    31 Black has a slight time (development) advantage
   "↻↻",              // $32    32 White has a moderate time (development) advantage (DR: @)
   "↺↺",              // $33    33 Black has a moderate time (development) advantage (DR: @)
   "↻↻↻",             // $34    34 White has a decisive time (development) advantage
   "↺↺↺",             // $35    35 Black has a decisive time (development) advantage
   "↑",               // $36    36 White has the initiative (DR: ^)
   "↓",               // $37    37 Black has the initiative (DR: ^)
   "⇑",               // $38    38 White has a lasting initiative
   "⇓",               // $39    39 Black has a lasting initiative
   "→",               // $40    40 White has the attack (DR: ->)
   "←",               // $41    41 Black has the attack (DR: ->)
   "&/-",             // $42    42 White has insufficient compensation for material deficit
   "&/+",             // $43    43 Black has insufficient compensation for material deficit
   "=/&",             // $44    44 White has sufficient compensation for material deficit (DR: =/&)
   "=/&",             // $45    45 Black has sufficient compensation for material deficit (DR: =/&)
   "+/&",             // $46    46 White has more than adequate compensation for material deficit
   "-/&",             // $47    47 Black has more than adequate compensation for material deficit
   "⊞",               // $48    48 White has a slight center control advantage
   "⊞",               // $49    49 Black has a slight center control advantage
   "⊞⊞",              // $50    50 White has a moderate center control advantage (DR: #)
   "⊞⊞",              // $51    51 Black has a moderate center control advantage (DR: #)
   "⊞⊞⊞",             // $52    52 White has a decisive center control advantage
   "⊞⊞⊞",             // $53    53 Black has a decisive center control advantage
   "⟩",               // $54    54 White has a slight kingside control advantage
   "⟩",               // $55    55 Black has a slight kingside control advantage
   "⟫",               // $56    56 White has a moderate kingside control advantage (DR: >>)
   "⟫",               // $57    57 Black has a moderate kingside control advantage (DR: >>)
   "⋙",               // $58    58 White has a decisive kingside control advantage
   "⋙",               // $59    59 Black has a decisive kingside control advantage
   "⟨",               // $60    60 White has a slight queenside control advantage
   "⟨",               // $61    61 Black has a slight queenside control advantage
   "⟪",               // $62    62 White has a moderate queenside control advantage (DR: <<)
   "⟪",               // $63    63 Black has a moderate queenside control advantage (DR: <<)
   "⋘",               // $64    64 White has a decisive queenside control advantage
   "⋘",               // $65    65 Black has a decisive queenside control advantage
   "$66",             // $66    66 White has a vulnerable first rank
   "$67",             // $67    67 Black has a vulnerable first rank
   "$68",             // $68    68 White has a well protected first rank
   "$69",             // $69    69 Black has a well protected first rank
   "$70",             // $70    70 White has a poorly protected king
   "$71",             // $71    71 Black has a poorly protected king
   "$72",             // $72    72 White has a well protected king
   "$73",             // $73    73 Black has a well protected king
   "$74",             // $74    74 White has a poorly placed king
   "$75",             // $75    75 Black has a poorly placed king
   "$76",             // $76    76 White has a well placed king
   "$77",             // $77    77 Black has a well placed king
   "$78",             // $78    78 White has a very weak pawn structure
   "$79",             // $79    79 Black has a very weak pawn structure
   "$80",             // $80    80 White has a moderately weak pawn structure (DR: x a5)
   "$81",             // $81    81 Black has a moderately weak pawn structure (DR: x a5)
   "$82",             // $82    82 White has a moderately strong pawn structure
   "$83",             // $83    83 Black has a moderately strong pawn structure
   "$84",             // $84    84 White has a very strong pawn structure
   "$85",             // $85    85 Black has a very strong pawn structure
   "$86",             // $86    86 White has poor knight placement
   "$87",             // $87    87 Black has poor knight placement
   "$88",             // $88    88 White has good knight placement
   "$89",             // $89    89 Black has good knight placement
   "$90",             // $90    90 White has poor bishop placement
   "$91",             // $91    91 Black has poor bishop placement
   "↗",               // $92    92 White has good bishop placement (DR: diagonal //)
   "↖",               // $93    93 Black has good bishop placement
   "$94",             // $94    94 White has poor rook placement
   "$95",             // $95    95 Black has poor rook placement
   "⇈",               // $96    96 White has good rook placement (DR: rank <=> file ||)
   "⇊",               // $97    97 Black has good rook placement
   "$98",             // $98    98 White has poor queen placement
   "$99",             // $99    99 Black has poor queen placement
   "$100",            // $100  100 White has good queen placement
   "$101",            // $101  101 Black has good queen placement
   "$102",            // $102  102 White has poor piece coordination
   "$103",            // $103  103 Black has poor piece coordination
   "$104",            // $104  104 White has good piece coordination
   "$105",            // $105  105 Black has good piece coordination
   "$106",            // $106  106 White has played the opening very poorly
   "$107",            // $107  107 Black has played the opening very poorly
   "$108",            // $108  108 White has played the opening poorly
   "$109",            // $109  109 Black has played the opening poorly
   "$110",            // $110  110 White has played the opening well
   "$111",            // $111  111 Black has played the opening well
   "$112",            // $112  112 White has played the opening very well
   "$113",            // $113  113 Black has played the opening very well
   "$114",            // $114  114 White has played the middlegame very poorly
   "$115",            // $115  115 Black has played the middlegame very poorly
   "$116",            // $116  116 White has played the middlegame poorly
   "$117",            // $117  117 Black has played the middlegame poorly
   "$118",            // $118  118 White has played the middlegame well
   "$119",            // $119  119 Black has played the middlegame well
   "$120",            // $120  120 White has played the middlegame very well
   "$121",            // $121  121 Black has played the middlegame very well
   "$122",            // $122  122 White has played the ending very poorly
   "$123",            // $123  123 Black has played the ending very poorly
   "$124",            // $124  124 White has played the ending poorly
   "$125",            // $125  125 Black has played the ending poorly
   "$126",            // $126  126 White has played the ending well
   "$127",            // $127  127 Black has played the ending well
   "$128",            // $128  128 White has played the ending very well
   "$129",            // $129  129 Black has played the ending very well
   "⇄",               // $130  130 White has slight counterplay
   "⇆",               // $131  131 Black has slight counterplay
   "⇄⇄",              // $132  132 White has moderate counterplay (DR: ->/<-)
   "⇆⇆",              // $133  133 Black has moderate counterplay
   "⇄⇄⇄",             // $134  134 White has decisive counterplay
   "⇆⇆⇆",             // $135  135 Black has decisive counterplay
   "⊕",               // $136  136 White has moderate time control pressure
   "⊖",               // $137  137 Black has moderate time control pressure
   "⊕⊕",              // $138  138 White has severe time control pressure
   "⊖⊖",              // $139  139 Black has severe time control pressure
   "△",               // $140 with the idea of
   "$141",            // $141
   "⌓",               // $142 better is
   "$143",            // $143
   "R",               // $144 various moves
   "RR",              // $145 editorial comment
   "N",               // $146 Novelty
   "×",               // $147 weak square
   "⊥",               // $148 endgame
   "⟺",               // $149 file
   "⇗",               // $150 diagonal
   "◫",               // $151 bishop pair
   "$152",            // $152
   "◨",               // $153 opposite coloured bishops
   "⊶",               // $154 bishops of the same colour
   "$155",            // $155
   "$156",            // $156
   "$157",            // $157
   "$158",            // $158
   "$159",            // $159
   "$160",            // $160
   "$161",            // $161
   "$162",            // $162
   "$163",            // $163
   "$164",            // $164
   "$165",            // $165
   "$166",            // $166
   "$167",            // $167
   "$168",            // $168
   "$169",            // $169
   "$170",            // $170
   "$171",            // $171
   "$172",            // $172
   "$173",            // $173
   "$174",            // $174
   "$175",            // $175
   "$176",            // $176
   "$177",            // $177
   "$178",            // $178
   "$179",            // $179
   "$180",            // $180
   "$181",            // $181
   "$182",            // $182
   "$183",            // $183
   "$184",            // $184
   "$185",            // $185
   "$186",            // $186
   "$187",            // $187
   "$188",            // $188
   "$189",            // $189
   "ǁ",               // $190 etc.
   "⡁",               // $191 doubled pawn
   "⚯",               // $192 separated pawn
   "⚮",               // $193 connected pawn
   "$194",            // $194
   "$195",            // $195
   "$196",            // $196
   "$197",            // $197
   "$198",            // $198
   "$199",            // $199
   "$200",            // $200
   "D",               // $201 Diagram
   "$202",            // $202
   "$203",            // $203
   "$204",            // $204
   "$205",            // $205
   "$206",            // $206
   "$207",            // $207
   "$208",            // $208
   "$209",            // $209
   "—",               // $210 see
   "#",               // $211 mate
   "⚨",               // $212 passed pawn
   "$213",            // $213 more pawns
   "⌊",               // $214 with
   "⌋",               // $215 without
   "$216",            // $216
   "$217",            // $217
   "$218",            // $218
   "$219",            // $219
   "$220",            // $220
   "$221",            // $221
   "$222",            // $222
   "$223",            // $223
   "$224"             // $224
};


//-------------------- UTF-8 Edition --------------------
// to follow

