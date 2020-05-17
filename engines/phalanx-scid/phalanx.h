#ifndef PHALANX_INCLUDED
#define PHALANX_INCLUDED

#define VERSION "XXII-pg"

#ifdef GNUFUN
  #include <getopt.h>
#endif

#ifdef _WIN32
  #include <windows.h>

  #ifndef GNUFUN
    extern int opterr;
    extern int optind;
    extern char *optarg;
    int getopt (int argc, char **argv, char *opts);
  #endif

  #define	R_OK	4		/* Test for read permission.  */
  #define	W_OK	2		/* Test for write permission.  */
  #define access(a,b) 0

  #define setTimeout(FN, SEC)
  #define clearTimeout(DUMMY)

#else
  #include <unistd.h>
  #include <sys/time.h>
  #define setTimeout(FN, SEC) signal(SIGALRM,FN); alarm(SEC);
  #define clearTimeout(DUMMY) alarm(0);
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
 * COMPATIBILITY
 */

#ifndef CLOCKS_PER_SEC
# define CLOCKS_PER_SEC CLK_TCK
#endif

#ifdef _WIN32
# define inline __inline
  typedef __int64 int64;
#else
  typedef long long int64;
#endif

/*
 * TYPES
 */

typedef unsigned char tsquare;

/*** Moves are generated into and executed from struct tmove ***/
typedef struct
{
  unsigned char
	from,		/* square */
	to,		/* destination square */
	special;	/* Castling or en passant capture, see moving.c */
  unsigned char
	in1,		/* moving piece */
	in2,		/* captured piece or 0 */
	in2a;		/* moving piece or promoted piece */
  signed short
	value;		/* heuristic value - the order key - before search */
			/* value returned - after search */
  short
	dch;		/* depth change -100...100 */
  short
	shift;		/* alphabeta window shift at root search */
} tmove;          /* 12 bytes */

typedef struct
{
  tmove
	m;
  unsigned
	hashboard;
  unsigned short
	rule50,		/* number of half-moves since last irreversible */
	castling:8,	/* 4 castling flags: */
			/*   white short, w. long, b. s., b. l. */
	check:8;
  short
	mtrl,		/* material count of side on move */
	xmtrl;		/* material count of opposite side */
} tgamenode;      /* 12+12 bytes */

typedef struct
{
  unsigned
	hashboard;
  short
	value,
	depth: 14;
  unsigned
	result: 2;
  unsigned short
	move,
	age;
} thashentry;     /* 12 bytes */

/* Positional features of one side */
typedef struct
{
  short hung;
  signed char khung, kshield;
  unsigned char p, n, b, r, q;   /* number of given pieces */
  unsigned char kp;              /* king position */
  unsigned char r7r;             /* # of rooks on 7-th row */
  signed char prune;             /* is it ok to prune search in this pos ? */
  signed char kstorm, qstorm;    /* diffside castling: king attack q/k side */
  unsigned char devel;           /* development needed?, 0 ... no */
  unsigned char castling;
  unsigned char bishopcolor,     /* square color of friendly bishops, or-red */
               xbishopcolor;     /* square color of enemy bishops */
  signed char worsebm;           /* mobility of the worse bishop */
  unsigned char lpf, rpf;        /* leftmost/rightmost pawn file */
} tknow;

typedef enum
	{ timecontrol, averagetime, fixedtime, fixeddepth }
tlevel;

typedef struct
{
  int	centiseconds;
  unsigned
	moves,            /* time levels */
	increment;
  int	depth;            /* fixed depth level */
  int
	noise;
  int resign;
  int
	bench;
  int
	machine_color,
	post,
	xboard,
	book,
	learn,
	cpu,           /* use cpu time */
	ponder,
	analyze,
	polling;       /* polling input */
  tlevel level;
  unsigned char easy;
  FILE	* log;
} tflag;

typedef struct
{ unsigned char prev, next; }
tlist;

typedef struct
{ FILE *
	f;
  unsigned
	firstkey,
	lastkey;
  int
	filesize;
} tsbook;

typedef struct
{ FILE * f; int filesize; }
tpbook;

typedef struct
{ unsigned taxi, max, diag, min; }
tdist;



/*
 * DEFINITIONS
 */

#ifndef VERSION
# define VERSION "unknown"
#endif

/* use SAN for output */
#undef SANOUT

/* primary (text, small, user) book */
#ifndef PBOOK_FILE
#define PBOOK_FILE "pbook.phalanx"
#endif
#ifndef PBOOK_DIR
#define PBOOK_DIR "/usr/local/lib/phalanx"
#endif

/* secondary (binary, large, generated from pgn) book */
#ifndef SBOOK_FILE
#define SBOOK_FILE "sbook.phalanx"
#endif
#ifndef SBOOK_DIR
#define SBOOK_DIR "/usr/local/lib/phalanx"
#endif

#ifndef ECO_FILE
#define ECO_FILE "eco.phalanx"
#endif
#ifndef ECO_DIR
#define ECO_DIR "/usr/local/lib/phalanx"
#endif

#ifndef LEARN_FILE
#define LEARN_FILE "learn.phalanx"
#endif
#ifndef LEARN_DIR
#define LEARN_DIR "/var/local/lib/phalanx"
#endif

#define ENV_PBOOK "PHALANXPBOOKDIR"
#define ENV_SBOOK "PHALANXSBOOKDIR"
#define ENV_LEARN "PHALANXLEARNDIR"
#define ENV_ECO "PHALANXECODIR"

#define max(x,y) (((x)>(y))?(x):(y))
#define min(x,y) (((x)<(y))?(x):(y))

#define NOVALUE 32123
#define CHECKMATE 30000
#define MAXPLY 40
#define MAXCOUNTER 1024 /* 512 moves */

#define HASH_COLOR ((unsigned)0xFDB97531)
#define no_cut     1
#define alpha_cut  2
#define beta_cut   3

/* Castling special move codes and flags */

#define SHORT_CASTLING 1
#define LONG_CASTLING 2

#define WSHORT 1 /* if set, white short castling is impossible */
#define WLONG  2
#define BSHORT 4
#define BLONG  8

/* Colors / sides */

#define WHITE 1
#define BLACK 2

#define empty(X) (((X)&3)==0)
#define white(X) (((X)&3)==WHITE)
#define black(X) (((X)&3)==BLACK)
#define color(X) ((X)&3)
#define enemy(X) ((X)^3)

/* Values */

#define P_VALUE  100
#define N_VALUE  350
#define B_VALUE  350
#define R_VALUE  550
#define Q_VALUE 1050

/* Pieces */

#define piece(X) ((X)&0xFC)

#define PAWN   0x10
#define KNIGHT 0x20
#define BISHOP 0x30
#define ROOK   0x40
#define QUEEN  0x50
#define KING   0x60

#define WP (WHITE|PAWN)
#define WN (WHITE|KNIGHT)
#define WB (WHITE|BISHOP)
#define WR (WHITE|ROOK)
#define WQ (WHITE|QUEEN)
#define WK (WHITE|KING)

#define BP (BLACK|PAWN)
#define BN (BLACK|KNIGHT)
#define BB (BLACK|BISHOP)
#define BR (BLACK|ROOK)
#define BQ (BLACK|QUEEN)
#define BK (BLACK|KING)

/*** Bit masks for power tables ***/

#define WPM 0x0001
#define WNM 0x0002
#define WBM 0x0004
#define WRM 0x0008
#define WQM 0x0010
#define WWW 0x0020

#define BPM 0x0100
#define BNM 0x0200
#define BBM 0x0400
#define BRM 0x0800
#define BQM 0x1000
#define BBB 0x2000

/* Files */

#define FILE_A 1
#define FILE_B 2
#define FILE_C 3
#define FILE_D 4
#define FILE_E 5
#define FILE_F 6
#define FILE_G 7
#define FILE_H 8

/* Squares */

#define A1 21
#define B1 22
#define C1 23
#define D1 24
#define E1 25
#define F1 26
#define G1 27
#define H1 28

#define A2 31
#define B2 32
#define C2 33
#define D2 34
#define E2 35
#define F2 36
#define G2 37
#define H2 38

#define A3 41
#define B3 42
#define C3 43
#define D3 44
#define E3 45
#define F3 46
#define G3 47
#define H3 48

#define A4 51
#define B4 52
#define C4 53
#define D4 54
#define E4 55
#define F4 56
#define G4 57
#define H4 58

#define A5 61
#define B5 62
#define C5 63
#define D5 64
#define E5 65
#define F5 66
#define G5 67
#define H5 68

#define A6 71
#define B6 72
#define C6 73
#define D6 74
#define E6 75
#define F6 76
#define G6 77
#define H6 78

#define A7 81
#define B7 82
#define C7 83
#define D7 84
#define E7 85
#define F7 86
#define G7 87
#define H7 88

#define A8 91
#define B8 92
#define C8 93
#define D8 94
#define E8 95
#define F8 96
#define G8 97
#define H8 98
#define H9 99 /* a bumper: we are out of board */



/*
 * VARIABLES
 */

extern tflag Flag;                    /* Misc. settings, see typedef of tflag */

extern tsquare B[120];         /* The chessboard */
extern tlist L[120];                  /* List of pieces */
#define WKP (L[1].next)               /* White king position */
#define BKP (L[2].next)               /* Black king position */

extern signed char * Th;              /* horizontal axe transf., (H8<->H1) */
extern signed char * Tv;              /* vertical axe tr., (H8<->A8) */

extern tdist dist[120*120];
extern tknow Wknow, Bknow;            /* Things i know about actual position */
extern int Color;                /* c. to move: 1 ... white, 2 ... black */
extern int LastIter;                /* result of last iteration */
extern int Depth;                   /* depth to be searched, in % of ply */
extern int Ply;
extern int FollowPV;
extern int Totmat;                  /* total mtrl sum on the board */
extern int Abort, NoAbort;          /* abort the search when SIG_INT */
extern long AllDepth;                 /* Statistics for average Depth */
extern int64 AllNPS;

extern int DrawScore;
#define DRAW ( Ply%2 ? -DrawScore : DrawScore )

extern tgamenode G[MAXCOUNTER];
extern int Counter;                 /* game counter, points to G[] */

extern int64 Nodes;           /* statistics */
extern int Scoring;
extern int EasyMove;

extern long Otim;                     /* remaining opponent's time */

/* move generation tables */
extern int N_moves[8], RB_dirs[8];
#define K_moves RB_dirs

extern int Values[7];            /* piece values */
extern unsigned short P[120];      /* power table */

extern tmove PV[MAXPLY][MAXPLY];   /* principal variation */
extern tmove Pondermove;

/* Hashing codes */
extern unsigned H[12][64];
extern thashentry *HT;             /* Hash table itself */
extern int * HS;                 /* square trans-table */
extern int HP[];                 /* piece trans-table */
extern unsigned SizeHT;            /* Size of the hash table */
extern unsigned Age;

/* Opening book */
extern tpbook Pbook;               /* primary book flags */
extern tpbook Learn;
extern tsbook Sbook;               /* secondary book flags */
extern FILE * Eco;
extern int Bookout;

/* params that cannot be pushed thru SIGALM handler, */
/* must be pushed thru global variables */
extern int A_n, A_i, A_d;
extern tmove * A_m;



/*
 * FUNCTIONS
 */

extern void myfwrite( void*, int, FILE* );
extern void myfread( void*, int, FILE* );

extern int bcreate(int,char**);

extern void new_game(void);

/* input/output */
extern void printm(tmove,char*);
extern void printmSAN( tmove*, int, int, char* );
extern void printPV(int,int,char*);
extern void infoline(int,char*);
extern void verboseline(tmove*,int,int);
extern void printboard(char*);
extern int setfen(char*);
extern void shell(void);
extern int command(void);
extern tmove * sandex(char*,tmove*,int);

/* signal handlers */
extern void interrupt(int);

extern int attacktest(int,int);
#define checktest(side) attacktest( (side)==WHITE ? WKP : BKP, enemy(side))

extern void generate_legal_moves(tmove*,int*,int);
extern void generate_legal_captures(tmove*,int*,int);
extern void generate_legal_checks(tmove*,int*);
extern int see( tsquare *, int, int );

extern int search(tmove*,int,int,int);
extern int csearch(tmove*,int,int,int,int);
extern tmove root_search(void);

extern int repetition(int);
extern int material_draw(void);
extern int evaluate(int,int);

extern int score_position(void);

/* endgame knowledge */
extern int pawns(void);
extern int e_nb(int);           /* endgame: knight and bishop */
extern int e_mp(void);            /*          minor piece and pawn */
extern int e_rpr(void);           /*          R+P vs. R */

extern void do_move( tmove * );
extern void undo_move( tmove * );

extern unsigned hashboard(void);
extern thashentry * seekHT(void);
extern void writeHT(int,int,int);

/* levels */
extern void l_level(char *);
extern void l_startsearch(void);
extern int l_iterate(void);
extern long ptime(void);
extern long LastTurn;
extern int Turns;

/* book */
extern int bookmove(tmove*,int);
extern unsigned smove(tmove*);

/* learning feature */
extern int rlearn(void);
extern void wlearn(int,int);

/* killer move utils */
extern void init_killers(void);
extern void write_killer( int, int );
extern void add_killer( tmove *, int, thashentry * );

#endif

