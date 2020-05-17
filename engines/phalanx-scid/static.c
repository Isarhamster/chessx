/**
***   STATIC EVALUATION
**/

#include "phalanx.h"

/**
***   command `score' shows detailed information about static
***   evaluation of actual position. If you want this to work,
***   SCORING must be defined. Then, static evaluation function
***   is somewhat slower.
**/
#undef SCORING
int Scoring;

/**
***   If the material value is between ENDMATERIAL and MIDMATERIAL,
***   Phalanx computes both endgame and middlegame evaluation.
***   The evaluation is proportionally computed from these two values.
**/
#define ENDMATERIAL 3000 /* minimum is 0 */
#define MIDMATERIAL 6000 /* maximum is 8600 */

tknow Wknow, Bknow;


/* isolated pawn penalty by rank */
static const int isofile[10] =
{ 0, -4, -6, -8, -10, -10, -8, -6, -4, 0 };
/***  A   B   C    D    E   F   G   H  ***/

/*** Square color ***/
signed char sqcolor_[80] =
{
  0, 2, 1, 2, 1, 2, 1, 2, 1, 0,
  0, 1, 2, 1, 2, 1, 2, 1, 2, 0,
  0, 2, 1, 2, 1, 2, 1, 2, 1, 0,
  0, 1, 2, 1, 2, 1, 2, 1, 2, 0,
  0, 2, 1, 2, 1, 2, 1, 2, 1, 0,
  0, 1, 2, 1, 2, 1, 2, 1, 2, 0,
  0, 2, 1, 2, 1, 2, 1, 2, 1, 0,
  0, 1, 2, 1, 2, 1, 2, 1, 2, 0
}; signed char * sqcolor = sqcolor_-20;

/*** Pawn in middlegame ***/
int pmpb_[80] =
{
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  2,  3,  4,  6,  6,  4,  3,  2,  0,
  0,  0,  1,  5,  8,  8,  5,  1,  0,  0,
  0,  0,  1,  6, 16, 12,  6,  1,  0,  0,
  0,  1,  5,  7, 16, 12,  7,  5,  1,  0,
  0,  5,  8, 10, 16, 12, 10,  8,  5,  0,
  0,  5,  5,  5,  6,  6,  5,  5,  5,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
}; const int * pmpb = pmpb_-20;

/*** queenside pawn attack ***/
int qstormtable_[80] =
{
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,-12,-12,-12,  0,  0, 10, 10, 10,  0,
  0, -9,-10, -9,  0,  6,  8,  8,  8,  0,
  0, -6, -6, -6,  6, 10,  4,  4,  4,  0,
  0, -3, -3, -3, 10,  4,  0,  0,  0,  0,
  0,  0,  0,  5,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
}; const int * qstormtable = qstormtable_-20;

/*** kingside pawn attack ***/
int kstormtable_[80] =
{
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0, 10, 10, 10,  0,  0,-12,-12,-12,  0,
  0,  8,  8,  8,  6,  0, -9,-10, -9,  0,
  0,  4,  4,  4, 10,  6, -6, -6, -6,  0,
  0,  0,  0,  0,  4, 10, -3, -3, -3,  0,
  0,  0,  0,  0,  0,  0,  5,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
}; const int * kstormtable = kstormtable_-20;

/*** knight in middlegame ***/
int nmpb_[80] =
{
  0,-16,-10, -8, -8, -8, -8,-10,-16,  0,
  0,-10, -3,  1,  3,  3,  1, -3,-10,  0,
  0, -8,  1,  5,  5,  5,  5,  1, -8,  0,
  0, -8,  2,  5,  8,  8,  5,  2, -8,  0,
  0, -8,  4,  6, 10, 10,  6,  4, -8,  0,
  0, -8,  4,  7,  8,  8,  7,  4, -8,  0,
  0,-10,  0,  3,  4,  4,  3,  0,-10,  0,
  0,-16,-10, -8, -8, -8, -8,-10,-16,  0
}; const int * nmpb = nmpb_-20;

/*** bishop in middlegame ***/
int bmpb_[80] =
{
  0,  10,  8,  6,  4,  4,  6,  8, 10,  0,
  0,   8, 12,  8,  9,  9,  8, 12,  8,  0,
  0,  10, 10, 11, 11, 11, 11, 10, 10,  0,
  0,  11, 12, 13, 14, 14, 13, 12, 11,  0,
  0,  12, 13, 15, 17, 17, 15, 14, 12,  0,
  0,  13, 14, 16, 16, 16, 16, 14, 13,  0,
  0,  11, 14, 12, 12, 12, 12, 14, 11,  0,
  0,  13, 10, 10, 10, 10, 10, 10, 13,  0
}; const int * bmpb = bmpb_-20;

/*** bishop in endgame ***/
int bepb_[80] =
{
  0, 24, 21, 18, 15, 15, 18, 21, 24,  0,
  0, 21, 24, 21, 18, 18, 21, 24, 21,  0,
  0, 18, 21, 24, 24, 24, 24, 21, 18,  0,
  0, 15, 18, 24, 30, 30, 24, 18, 15,  0,
  0, 15, 18, 24, 30, 30, 24, 18, 15,  0,
  0, 18, 21, 24, 24, 24, 24, 21, 18,  0,
  0, 21, 24, 21, 18, 18, 21, 24, 21,  0,
  0, 24, 21, 18, 15, 15, 18, 21, 24,  0
}; const int * bepb = bepb_-20;

/*** rook in middlegame ***/
int rmpb_[80] =
{
  0,  0,  1,  2,  3,  3,  2,  1,  0,  0,
  0,  0,  1,  2,  3,  3,  2,  1,  0,  0,
  0,  0,  1,  2,  3,  3,  2,  1,  0,  0,
  0,  0,  1,  2,  3,  3,  2,  1,  0,  0,
  0,  0,  1,  2,  3,  3,  2,  1,  0,  0,
  0,  7,  8,  9, 10, 10,  9,  8,  7,  0,
  0, 10, 11, 12, 13, 13, 12, 11, 10,  0,
  0, 10, 11, 12, 13, 13, 12, 11, 10,  0
}; const int * rmpb = rmpb_-20;

/*** queen in middlegame ***/
int qmpb_[80] =
{
  0,  0,  1,  2,  3,  3,  2,  1,  0,  0,
  0,  4,  5,  6,  7,  7,  6,  5,  4,  0,
  0,  5,  6,  7,  8,  8,  7,  6,  5,  0,
  0,  6,  7,  8,  9,  9,  8,  7,  6,  0,
  0,  8,  9, 10, 11, 11, 10,  9,  8,  0,
  0, 10, 11, 12, 13, 13, 12, 11, 10,  0,
  0,  8, 10, 11, 12, 12, 11, 10,  8,  0,
  0,  5,  6,  7,  8,  8,  7,  6,  5,  0
}; const int * qmpb = qmpb_-20;

/*** king in middlegame ***/
int kmpb_[80] =
{
  0,  0,  0, -3, -6, -6, -3,  0,  0,  0,
  0, -2, -4, -8,-12,-12, -8, -4, -2,  0,
  0, -6,-12,-20,-25,-25,-20,-12, -6,  0,
  0,-15,-20,-30,-40,-40,-30,-20,-15,  0,
  0,-30,-40,-50,-60,-60,-50,-40,-30,  0,
  0,-35,-45,-55,-65,-65,-55,-45,-35,  0,
  0,-30,-40,-50,-60,-60,-50,-40,-30,  0,
  0,-20,-30,-40,-50,-50,-40,-30,-20,  0
}; const int * kmpb = kmpb_-20;

/*** king in endgame ***/
int kepb_[80] =
{
  0, -5,  5, 10, 15, 15, 10,  5, -5,  0,
  0,  6, 11, 16, 21, 21, 16, 11,  6,  0,
  0, 12, 17, 22, 27, 27, 22, 17, 12,  0,
  0, 18, 24, 30, 34, 34, 28, 24, 18,  0,
  0, 20, 27, 33, 48, 48, 33, 27, 20,  0,
  0, 16, 25, 34, 40, 40, 34, 25, 16,  0,
  0,  9, 20, 28, 35, 35, 28, 20,  9,  0,
  0,  3, 10, 17, 24, 24, 17, 10,  3,  0,
}; const int * kepb = kepb_-20;

/*** knight in endgame ***/
int c_bonus_[80] =
{
  0,-16,-10, -6, -2, -2, -6,-10,-16,  0,
  0, -9, -3,  1,  4,  4,  1, -3, -9,  0,
  0, -4,  1,  5,  8,  8,  5,  1, -4,  0,
  0,  0,  4,  8, 12, 12,  8,  4,  0,  0,
  0,  1,  5,  9, 13, 13,  9,  5,  1,  0,
  0, -2,  2,  6,  9,  9,  6,  2, -2,  0,
  0, -6, -1,  2,  5,  5,  2, -1, -6,  0,
  0, -9, -6, -3,  0,  0, -3, -6, -9,  0
}; int *c_bonus = c_bonus_-20;

int outpost_[80] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 2, 2, 1, 0, 0, 0,
  0, 0, 1, 2, 3, 3, 2, 1, 0, 0,
  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0
}; const int *outpost = outpost_-20;

int B_mobi[20] =
 { -36, -28, -20, -14, -6, -2, 1, 2, 3, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8, 8 };
int R_mobi[16] =
 { -9, -5, -2, 0, 2, 4, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6 };

/* power table */
unsigned short P[120];

int *pf, *xpf, wpf[10], bpf[10]; /* # of w&b pawns on a file */


int score_position(void)
{

int i;

/**
***  We always compute both 'endgame' value and 'middlegame' position
***  value.  After they are computed, they are balanced by total material.
***  This prevents evaluation to do big jumps when searching a position
***  on the edge of middlegame and endgame.
***
***  Evaluation is done in two passes:
***  1) Extract knowledge (see typedef tknow), prepare mirrored board.
***  2) Use knowledge.  Thanks to mirroring board in first pass, we don't
***     have to write evaluation of specific things twice (for white and
***     black separately).  This might also make better use of internal
***     cache on small machines.
**/  

int midresult, endresult, result;

int whp, bhp; /* sum of hung pieces. >=2 means troubles -> big penalty. */

int kp, xkp; /* this side and other side king position */

tknow * know, * xknow;

static int *l, wl[34], bl[34];   /* w&b piece list. Indexes to B[] (mB[]) */
static int n, wn, bn;

static int pin[32];
int npin;

/* mirrored board */
static tsquare mB[120] =
{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };

static tsquare mBinit[80] =
{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 3 };

tsquare *b;

/***************************************************************/

if( Totmat <= (R_VALUE+R_VALUE+P_VALUE) )
{
	/* if there are no pieces, return special pawn endgame evaluation */
	/* [moved, computed later] */

	/* Now, check some special endgame positions that are known to be
	 * hopeless draw (this might also cause pruning of the search). */
	switch(Totmat)
	{
	case (B_VALUE+P_VALUE): /* minor piece and pawn */
		{	int r = e_mp();
			if( r ) return DRAW+r;
		}
	break;
	case (N_VALUE+N_VALUE): /* minor against minor or two knights */
		if( G[Counter].mtrl == N_VALUE ) /* minor:minor */
		if( G[Counter].m.in2 == 0 ) return DRAW;
		{ int nn=0;
		  for( i=L[WKP].next; i!=0; i=L[i].next )
		  if( B[i] == WN ) nn++;
		  if( nn == 1 ) return e_nb(WHITE);
		  if( nn == 2 ) return
			Color==WHITE ?
				  DRAW-(N_VALUE+N_VALUE)
				: DRAW+(N_VALUE+N_VALUE);
		  if( nn == 0 )
		  {
		  for( i=L[BKP].next; i!=0; i=L[i].next )
		  if( B[i] == BN ) nn++;
		  if( nn == 1 ) return e_nb(BLACK);
		  if( nn == 2 ) return
			Color==WHITE ?
				  DRAW+(N_VALUE+N_VALUE)
				: DRAW-(N_VALUE+N_VALUE);
		  }
		}
	break;
	case (R_VALUE+R_VALUE+P_VALUE):  /* RPr */
		if( abs(G[Counter].mtrl-G[Counter].xmtrl) == P_VALUE )
		{
			int i; int np=0;
			for( i=L[WKP].next; i!=0; i=L[i].next ) np++;
			for( i=L[BKP].next; i!=0; i=L[i].next ) np++;
			if( np == 3 ) return e_rpr();
		}
	break;
	}
}

result = midresult = endresult = whp = bhp = npin = 0;

memcpy( mB+20, mBinit, 80*sizeof(tsquare) );

wn = bn = 1; /* King ... at least */
for(i=0;i!=10;i++) wpf[i]=bpf[i]=0;

memset( &Wknow, 0, sizeof(tknow) );
memset( &Bknow, 0, sizeof(tknow) );
memset( P, 0, 120*sizeof(short) );

Wknow.kp = WKP; Wknow.worsebm = 100;
Wknow.lpf = 9;
Bknow.kp = BKP; Bknow.worsebm = 100;
Bknow.lpf = 9;

/************************************************/

i=WKP; mB[ Th[i] ] = BK; wl[0] = i;
for( i=L[i].next; i!=0; i=L[i].next )
{
	int j; int mobi;
	switch( B[i] )
	{
	case WP: mB[ Th[i] ] = BP; wl[wn] = i; wn++;
		wpf[i%10]++;
		if(i%10<Wknow.lpf) Wknow.lpf=i%10;
		if(i%10>Wknow.rpf) Wknow.rpf=i%10;
		Wknow.p++;
		for( j=i+9; j<=i+11; j+=2 ) { P[j] |= WPM; P[j] += WWW; }
		if( i>=A6 && B[i+10]==0 ) /* Passed pawn */
		{ Bknow.hung+=4;
		  if(B[i+20]==0||B[i+20]==3) Bknow.hung+=16;
		}
	break;
	case WN: mB[ Th[i] ] = BN; wl[wn] = i; wn++;
		Wknow.n++;
		for( j=0; j!=8; j++ )
		{ int d = i+N_moves[j]; P[d] |= WNM; P[d] += WWW; }
	break;
	case WB: mB[ Th[i] ] = BB; wl[wn] = i; wn++;
		Wknow.b++;
		Wknow.bishopcolor  |= sqcolor[i];
		Bknow.xbishopcolor |= 3-sqcolor[i];
		mobi = 4;
		for( j=4; j!=8; j++ )
		{ int step = RB_dirs[j]; int d = i;
		  do { d+=step; mobi++; P[d] |= WBM; P[d] += WWW; }
		  while( B[d] == 0 );
		  if( B[d] & WHITE ) /* edge or friendly piece */
		  {
			mobi--;
			if( B[d]==3 || ( B[d]==WP && B[d+10] ) ) mobi--;
		  }
		  else
		  if( B[d]==BN || B[d]==BR || B[d]==BQ )
		  { pin[npin]=d;
		    do d+=step; while( B[d]==0 );
		    if( B[d]==BQ || B[d]==BK ) npin++;
		  }
		}
		result += B_mobi[mobi];
		if( mobi < Wknow.worsebm ) Wknow.worsebm = mobi;
	break;
	case WR: mB[ Th[i] ] = BR; wl[wn] = i; wn++;
		if( i>=A7 && i<=H7 ) Wknow.r7r+=2;
		Wknow.r++;
		mobi = 0;
		for( j=0; j!=4; j++ )
		{ int step = RB_dirs[j]; int d = i;
		  do { d += step; mobi++; P[d] |= WRM; P[d] += WWW; }
		  while( B[d] == 0 );
		  if( B[d] & WHITE ) mobi--;
		  else
		  if( B[d]==BN || B[d]==BB || B[d]==BQ )
		  { pin[npin]=d;
		    do d+=step; while( B[d]==0 );
		    if( B[d]==BQ || B[d]==BK ) npin++;
		  }
		}
		result += R_mobi[mobi];
	break;
	case WQ: mB[ Th[i] ] = BQ; wl[wn] = i; wn++;
		if( i>=A1 && i<=H3 ) Wknow.r7r++;
		Wknow.q++;
		for( j=0; j!=8; j++ )
		{ int step = RB_dirs[j]; int d = i;
		  do { d += step; P[d] |= WQM; P[d] += WWW; }
		  while( B[d] == 0 );

		  if( B[d]==BN || ( B[d]==BB && d<4 ) || ( B[d]==BR && d>=4 ) )
		  { pin[npin]=d;
		    do d+=step; while( B[d]==0 );
		    if( B[d]==BK ) npin++;
		  }
		}
	break;
	}
}

i=BKP; mB[ Th[i] ] = WK; bl[0] = Th[i];
for( i=L[i].next; i!=0; i=L[i].next )
{
	int j;
	int mobi;
	switch( B[i] )
	{
	case BP: mB[ Th[i] ] = WP; bl[bn] = Th[i]; bn++;
		bpf[i%10]++;
		if(i%10<Bknow.lpf) Bknow.lpf=i%10;
		if(i%10>Bknow.rpf) Bknow.rpf=i%10;
		Bknow.p++;
		for( j=i-11; j<=i-9; j+=2 ) { P[j] |= BPM; P[j] += BBB; }
		if( i<=H3 && B[i-10]==0 )
		{ Wknow.hung+=4;
		  if(B[i-20]==0||B[i-20]==3) Wknow.hung+=16;
		}
	break;
	case BN: mB[ Th[i] ] = WN; bl[bn] = Th[i]; bn++;
		Bknow.n++;
		for( j=0; j!=8; j++ )
		{ int d = i+N_moves[j]; P[d] |= BNM; P[d] += BBB; }
	break;
	case BB: mB[ Th[i] ] = WB; bl[bn] = Th[i]; bn++;
		Bknow.b++;
		Bknow.bishopcolor  |= 3-sqcolor[i];
		Wknow.xbishopcolor |= sqcolor[i];
		mobi = 4;
		for( j=4; j!=8; j++ )
		{ int step = RB_dirs[j]; int d = i;
		  do { d += step; mobi++; P[d] |= BBM; P[d] += BBB; }
		  while( B[d] == 0 );
		  if( B[d] & BLACK )
		  {
			mobi--;
			if( B[d]==3 || ( B[d]==BP && B[d-10] ) ) mobi--;
		  }
		  else
		  if( B[d]==WN || B[d]==WR || B[d]==WQ )
		  { pin[npin]=d;
		    do d+=step; while( B[d]==0 );
		    if( B[d]==WQ || B[d]==WK ) npin++;
		  }
		}
		result -= B_mobi[mobi];
		if( mobi < Bknow.worsebm ) Bknow.worsebm = mobi;
	break;
	case BR: mB[ Th[i] ] = WR; bl[bn] = Th[i]; bn++;
		if( i>=A2 && i<=H2 ) Bknow.r7r+=2;
		Bknow.r++;
		mobi = 0;
		for( j=0; j!=4; j++ )
		{ int step = RB_dirs[j]; int d = i;
		  do { d += step; mobi++; P[d] |= BRM; P[d] += BBB; }
		  while( B[d] == 0 );
		  if( B[d] & BLACK ) mobi--;
		  else
		  if( B[d]==WN || B[d]==WB || B[d]==WQ )
		  { pin[npin]=d;
		    do d+=step; while( B[d]==0 );
		    if( B[d]==WQ || B[d]==WK ) npin++;
		  }
		}
		result -= R_mobi[mobi];
	break;
	case BQ: mB[ Th[i] ] = WQ; bl[bn] = Th[i]; bn++;
		if( i>=A1 && i<=H3 ) Bknow.r7r++;
		Bknow.q++;
		for( j=0; j!=8; j++ )
		{ int step = RB_dirs[j]; int d = i;
		  do { d += step; P[d] += BBB; P[d] |= BQM; }
		  while( B[d] == 0 );
		  if( B[d]==WN || ( B[d]==WB && d<4 ) || ( B[d]==WR && d>=4 ) )
		  { pin[npin]=d;
		    do d+=step; while( B[d]==0 );
		    if( B[d]==WK ) npin++;
		  }
		}
	break;
	}
}

if( Totmat == P_VALUE*(Wknow.p+Bknow.p) )
{ /* only pawns */ return pawns(); }

#undef debug
#ifdef debug
{ int i,j; puts("");
  for(i=110;i>=0;i-=10) { for(j=0;j!=10;j++)
                          printf(" %04X",P[i+j]);puts(""); } }
#endif

#ifdef SCORING
if( Scoring ) printf(" (w/b) mobility = %i\n",result);
#endif

/**
***   Scan pinned pieces
**/
{ int pini;
for( i=0; i!=npin; i++ )
switch( B[ pini = pin[i] ] )
{
case WQ: result -= 50; Wknow.hung += 20;
break;
case BQ: result += 50; Bknow.hung += 20;
break;
case WN: case WB: case WR:
	if( (P[pini] & WPM) == 0 ) /* not pawn-protected */
	{ Wknow.hung += 6; result += -15*(P[pini]>>13); }
	else result += -6*(P[pini]>>13);
	if( P[pini] & BPM ) { Wknow.hung += 5; result -= 15; }
break;
case BN: case BB: case BR:
	if( (P[pini] & BPM) == 0 )
	{ Bknow.hung += 6; result +=  15*((0x00F0&P[pini])>>5); }
	else result +=  6*((0x00F0&P[pini])>>5);
	if( P[pini] & WPM ) { Bknow.hung += 5; result += 15; }
break;
}
}

/**
***   Look for hung pieces & weak pawns
**/
{
	static unsigned m[112] =
	{ 0,   0,   0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0x0, 0x0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0x1, 0x1, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0x1, 0x1, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0x7, 0x7, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0xF, 0xF, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0xF, 0xF, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0 };

	for( i=L[WKP].next; i!=0; i=L[i].next )
	if(P[i]&0xFF00) /* Black attacks the square 'i' */
	{
		if( (P[i]&0x00FF)==0 /* undefended */
		 || ( m[B[i]] & (P[i]>>8) ) /* attacked by less valuable */ )
			switch(B[i])
		{ case WP: Wknow.hung += 8; break;
		  case WN:
		  case WB: whp++; Wknow.hung+=12; break;
		  case WR: whp++; Wknow.hung+=14; break;
		  case WQ: whp++; Wknow.hung+=16;
		}
		else if( (P[i]&WPM) == 0 ) /* not pawn-protected */
		{
		  if(B[i]==WP) result += (P[i]>>13)*5;
		  else Wknow.hung += 6;
		}
	}

	for( i=L[BKP].next; i!=0; i=L[i].next )
	if(P[i]&0x00FF) /* white attacks this square */
	{
		if( (P[i]&0xFF00)==0 || ( m[B[i]] & P[i] ) )
			switch(B[i])
		{ case BP: Bknow.hung += 8; break;
		  case BN:
		  case BB: bhp++; Bknow.hung+=12; break;
		  case BR: bhp++; Bknow.hung+=14; break;
		  case BQ: bhp++; Bknow.hung+=16;
		}
		else if( (P[i]&BPM) == 0 ) /* not pawn-protected */
		{
		  if(B[i]==BP) result -= ((P[i]>>5)&0x7)*5;
		  else Bknow.hung += 6;
		}
	}

	if( whp>1 ) Wknow.hung += 8*whp;
	if( bhp>1 ) Bknow.hung += 8*bhp;
}

/*** King safety ***/
for(i=0;i!=8;i++)
{
	int direction = K_moves[i];
	int d=Wknow.kp+direction;
	if(B[d]!=3)
	{
		int ataks = (P[d]>>13) & 0x7;
		if(ataks)
		{
			int defs = (P[d]>>5) & 0x7;
			Wknow.khung += ataks;
			if( defs == 0 )
			{
				Wknow.khung ++;
				if( ataks>1 && (P[d]&BQM) )
				Wknow.khung += 2;
			}
		}

		/*** Add number of safe checks available ***/
		if( B[d]==0 )
		{
			d += direction;
			while( (B[d]&BLACK) == 0 )
			{
				if( ( P[d] & 0xE0 ) == 0 )
				{
					if( i<4 )
					{ if( P[d] & (BQM|BRM) )
					  { Wknow.khung +=2; break; } }
					else
					{ if( P[d] & (BQM|BBM) )
					  { Wknow.khung +=2; break; } }
				}
				if( B[d] ) break;
				d += direction;
			}
		}
		d = Wknow.kp+N_moves[i];
		if( (P[d]&BNM) && (B[d]&BLACK) == 0 && ( P[d] & 0xE0 ) == 0 )
		Wknow.khung += 2;
	}

	d=Bknow.kp+direction;
	if(B[d]!=3)
	{
		int ataks = (P[d]>>5) & 0x7;
		if(ataks)
		{
			int defs = (P[d]>>5) & 0x7;
			Bknow.khung += ataks;
			if( defs == 0 )
			{
				Bknow.khung ++;
				if( ataks>1 && (P[d]&WQM) )
				Bknow.khung += 2;
			}
		}

		/*** Add number of safe checks available ***/
		if( B[d]==0 )
		{
			d += direction;
			while( (B[d]&WHITE) == 0 )
			{
				if( ( P[d] & 0xE000 ) == 0 )
				{
					if( i<4 )
					{ if( P[d] & (WQM|WRM) )
					  { Bknow.khung +=2; break; } }
					else
					{ if( P[d] & (WQM|WBM) )
					  { Bknow.khung +=2; break; } }
				}
				if( B[d] ) break;
				d += direction;
			}
		}
		d = Bknow.kp+N_moves[i];
		if( (P[d]&WNM) && (B[d]&WHITE) == 0 && ( P[d] & 0xE000 ) == 0 )
		{ Bknow.khung += 2; }
	}
}

if( Totmat >= ENDMATERIAL+Q_VALUE )
{
	if( Wknow.kp%10 < FILE_E && Bknow.kp%10 > FILE_E )
	{
		Wknow.kstorm = 1; Bknow.qstorm = 1;
	}
	else
	if( Wknow.kp%10 > FILE_E && Bknow.kp%10 < FILE_E )
	{
		Wknow.qstorm = 1; Bknow.kstorm = 1;
	}
}

if( Wknow.hung || Bknow.hung )
{
	int hresult = Bknow.hung - Wknow.hung;
	midresult += hresult;
	endresult += hresult;
#ifdef SCORING
	if( Scoring ) printf(" (w/b) hung pieces = %i\n",hresult);
#endif
}

Wknow.castling = ( G[Counter].castling & 3 );
Bknow.castling = ( G[Counter].castling >> 2 );

if( (P[G1]|P[F1]) & 0xFF00 ) Wknow.castling |= WSHORT;
if( (P[C1]|P[D1]) & 0xFF00 ) Wknow.castling |= WLONG;

if( (P[G8]|P[F8]) & 0x00FF ) Bknow.castling |= WSHORT; /* WSHORT is correct! */
if( (P[C8]|P[D8]) & 0x00FF ) Bknow.castling |= WLONG;  /* We're mirroring */

/**
***   PASS 2
**/

n = wn; l = wl; b = B;
pf = wpf; xpf = bpf;
know = &Wknow; xknow = &Bknow;
kp = Wknow.kp; xkp = Bknow.kp;
for(;;)
{
	tdist *kdist = dist+120*kp;
	tdist *xkdist = dist+120*xkp;

	int j;
	int mr = 0, er = 0;

	/* bishop pair */
	if( know->bishopcolor == 3 )
	{
		int bb = 18 + 2*know->worsebm;
		mr += bb;
		er += bb*2;
#ifdef SCORING
		if( Scoring )
		if(l==wl) printf(" (whi) bishop pair bonus = %i\n",bb);
		else      printf(" (bla) bishop pair bonus = %i\n",bb);
#endif
	}

	/* rooks on 7th row */
	/* r7r: 2 pts for rook, 1 pt for queen on 6-8th row */
	if( know->r7r > 1 )
	{
		if( xkp >= A8 )
		{ er += 12 * know->r7r; mr += 5 * know->r7r; }
		mr += 10 * know->r7r; er += 4 * know->r7r;
		if( know->r7r > 2 ) mr += know->r7r * know->r7r + 4;
	}

	if( Totmat >= ENDMATERIAL )
	{
		int d = 0;

		/*** play Pc2-c4 (black Pc7-c5) in closed games ***/
		if( b[D5]==BP )
		{
			if( b[C4]==WP )
			{	mr+=10;
				if( b[E6]==BP && B[D4]==WP ) mr+=10;
				/*** defender in stonewall must keep pressure
				 *** to opponent's d-pawn ****/
				if( b[F5]==BP ) mr+=15;
			}
			if( b[C2]==WP )
			{
				if( b[C3]!=0 ) mr -= 7;
				else           mr -= 3;
				if( b[E2]==WP || b[E3]==WP ) mr -= 5;
				if( b[E6]==BP || b[C6]==BP ) mr -= 4;
			}
		}

		/*** development ***/
		if( b[A1] == WR && pf[FILE_A] != 0 ) d+=2;
		if( b[B1] == WN ) d+=5;
		if( b[C1] == WB ) { d+=3; if( b[B2] && b[D2] ) d+=4; }
		if( b[D1] == WQ ) d++;
		if( b[F1] == WB ) { d+=3; if( b[E2] && b[G2] ) d+=4; }
		if( b[G1] == WN ) d+=5;
		if( b[H1] == WR && pf[FILE_H] != 0 ) d+=2;

		know->devel = d; /* maximum is 29, <10 is okay */

		/*** central pawns ***/
		if( b[D4]==WP && b[E4]==WP )
		{
			if( b[E5]!=BP && b[D5]!=BP ) d = 12; else d = 6;
			if( b[C4]==WP || b[F4]==WP ) d += 4;

			mr += d;

#ifdef SCORING
			if( Scoring )
				if(l==wl)
				printf( " (whi) pawn center = %i\n", d );
				else
				printf( " (bla) pawn center = %i\n", d );
#endif
		}

		{
		/*** castling ***/
		int cneed;
		d = 0;
		if( kp%10 < FILE_D || kp%10 > FILE_E ) cneed=0;
		else
		{
			cneed = 4;
			if( !  pf[FILE_E] ) cneed += 3;
			if( ! xpf[FILE_E] ) cneed += 5;
		}

		switch( know->castling )
		{
		case 0:               /* both castlings possible */
			if( b[F1] ) d -= 2*cneed;
			if( b[G1] ) d -= 2*cneed;
			if( b[B1] ) d -= cneed;
			if( b[C1] ) d -= cneed;
			if( b[D1] ) d -= cneed;
		break;
		case WSHORT:          /* only long castling possible */
			d-=20;
			if( b[B1] ) d -= cneed;
			if( b[C1] ) d -= cneed;
			if( b[D1] ) d -= cneed;
		break;
		case WLONG:           /* only short castling possible */
			d-=8;
			if( b[F1] ) d -= 2*cneed;
			if( b[G1] ) d -= 2*cneed;
		break;
		case (WSHORT|WLONG):  /* castling impossible */
			if( kp%10 > FILE_E )
			{
			   if( b[G2]!=WP && b[G3]!=WP ) d -= 10;
			   if( b[H2]!=WP && b[H3]!=WP ) d -= 8;
			}
			else if( kp%10 < FILE_D )
			{
			    d -= 15;
			    if( b[B2]!=WP && b[B3]!=WP ) d -= 10;
			    if( b[A2]!=WP && b[A3]!=WP ) d -= 8;
			}
			else
			{ d -= 5*cneed; }
		break;
		}

		if( Totmat < 5000 ) d /= 2;

		mr += d;

		}

#ifdef SCORING
		if( Scoring )
			if(d!=0)
			if(l==wl)
			printf( " (whi) castling = %i\n", d );
			else
			printf( " (bla) castling = %i\n", d );
#endif
	}

	/*** pawn structure ***/
	for( i=1; i!=9; i++ ) if( pf[i] )
	{
		if( pf[i] > 1 ) /* doubled */ { mr-=5; er-=10; }

		if( pf[i-1]==0 && pf[i+1]==0 ) /* isolated */
		{
			/* knights are strong against an isolated pawn
			 * since they can block it. */
			int pen = isofile[i]
			          + (2+isofile[i]) * xknow->n
			          + 4*know->b;

			/* rooks are strong against an isolated pawn on
			 * open file */
			if( xpf[i]==0 && xknow->r )
			{
				int j;
				static const int rfip[10] =
				{ 0, -2, -3, -4, -5, -5, -4, -3, -2, 0 };
				for( j=i+20; j<H9; j+=10 ) if( b[j]==BR )
					pen += rfip[i];
				pen += (rfip[i]-2)*xknow->r;
			}

			if( pf[i] > 1 ) pen = pen*pf[i] + 2*isofile[i];

			mr += pen; er += pen;
#ifdef SCORING
			if( Scoring )
				if(l==wl)
				printf( " (whi) isolated pawn = %i\n", pen );
				else
				printf( " (bla) isolated pawn = %i\n", pen );
#endif
		}
	}

	for( i=0; i!=n; i++ ) /* scan pieces */
	{
	static int sq;
	sq = l[i];
	switch( b[ sq ] )
	{
	case WP:
		/* is it passed? */
		for( j=sq+10; j<A8; j+=10 )
		if( b[j-1]==BP || b[j]==BP || b[j+1]==BP ) goto not_passed;

		{ /* yes, the pawn is passed */

	/* passed pawn advancement bonus */
	static const int P_advance[12] =
	       {  0,  0,  0, 10, 15, 25, 40, 60, 95,  0,  0,  0 };
	/* ROW:   -   -   1   2   3   4   5   6   7   8   -   -   */

			int row = sq/10;
			int abonus = P_advance[row];
			int ppeval = abonus;

			if( b[sq+10]==0 )
			{
				if( row==8 )
				{ if(see(b,sq,sq+10)>=0) ppeval+=80; }
				else
				if( row==7 && b[sq+20]==0 )
				if( see(b,sq,sq+10)>=0 && see(b,sq,sq+20)>=0 )
				ppeval+=50;
			}
			else /* blocked */
			if( color(b[sq+10]) == BLACK ) ppeval-=abonus/2;

			/* blocked - reserve blocking square */
			if( color(b[sq+20]) == BLACK ) ppeval-=abonus/4;

			/* covered or a member of a phalanx */
			if( b[sq-9]==WP || b[sq-11]==WP
			 || b[sq-1]==WP || b[sq+1]==WP )
			{ ppeval += 2*abonus; }

			/* rook behind - support or brake */
			for( j=sq-10;
			     b[j]==0 || b[j]==WR || b[j]==BR
			             || b[j]==BQ || b[j]==WQ;
			     j-=10 )
				if( b[j] == WR )
				{ ppeval += abonus/2 + 4; }
				else if( b[j] == BR )
				{ ppeval -= abonus/2 + 9; }

			if( Totmat < ENDMATERIAL )
			{
				/* distance from enemy king */
				int difdis
				= 2*xkdist[sq+10].max - ( 9 - row );
				if( difdis >= 0 ) ppeval += 8+4*difdis;

				/* king supports the pawn */
				if( kdist[sq+10].max < 2 ) ppeval += abonus;
			}

			er += ppeval;
			mr += ppeval/2;

#ifdef SCORING
			if( Scoring && ppeval != 0 )
			{
				if(l==wl) printf(" (whi) ");
				else printf(" (bla) ");
				printf("passed pawn = %i ... %i\n",
				ppeval/2, ppeval );
			}
#endif
		}

		not_passed:;

		/* backward */
		if(     sq <= H3
		     && ( b[sq+19]==BP || b[sq+21]==BP )
		     && b[sq+10] != BP
		     && b[sq-1]  != WP
		     && b[sq+1]  != WP
		     && b[sq-9]  != WP
		     && b[sq-11] != WP    )
		{
			int pen = 0;

/*
printboard(); printf("%02i",sq); getchar();
*/
			if( xpf[sq%10]==0 && xknow->r )
			{
				int j;
				for( j=sq%10+20; j<H9; j+=10 ) if( b[j]==BR )
					pen += -2 + isofile[sq%10]/2;
				pen += isofile[sq%10]*xknow->r;
			}

			mr += isofile[ sq%10 ] + pen/2;
			er += -15 + pen;
#ifdef SCORING
			if( Scoring && pen != 0 )
			{
				if(l==wl) printf(" (whi) ");
				else printf(" (bla) ");
				printf("backward pawn = %i ... %i\n",
				isofile[ sq%10 ] + pen/2, -15 + pen );
			}
#endif
		}

		/* cannot move */
		{
			int cannotmove = 0;

			if( b[ sq + 10 ] != 0 )
			{
				mr -= (12-sq/10); er -= (12-sq/10);
				if( sq == E2 || sq == D2 )
				{ mr -= 10; er -= 8; }

				if( b[ sq + 10 ] )
				{	/* blocked pawn is an attack brake */
					if( know->kstorm && sq%10 > FILE_E )
						mr -= 12;
					else
					if( know->qstorm && sq%10 < FILE_D )
						mr -= 12;

					if( b[sq+10]==BP
					 && pf[sq%10-1]==0
					 && pf[sq%10+1]==0 )
					/* this pawn was already penalized
					 * as isolated. But this is a pawn
					 * ram - blocked by enemy pawn
					 * face to face. It is not so
					 * critical; take back a small part
					 * of the penalty. *********/
					{ mr+=5; er+=8; }
				}

				cannotmove = 1;
			}
			else
			{
				int ata=0;
				if( b[ sq - 1 ] == WP ) ata++;
				if( b[ sq + 1 ] == WP ) ata++;
				if( b[ sq + 9 ] == BP ) ata--;
				if( b[ sq +11 ] == BP ) ata--;

				if( ata < 1 )
				{
					mr += -4+ata; er += -8+2*ata;
					cannotmove = 1;
				}
			}

			if( cannotmove )
			{
				int p = 1 + (b[sq+10]==BP);

				if( Totmat < 1500 ) er -= 6*p;

				/* penalize friendly bishop that stands
				 * on a square of this color */
				if( sqcolor[sq] & know->bishopcolor )
				{
					if(    sq==E4 || sq==D4
					    || sq==E3 || sq==D3   )
					{ er-=7*p; mr-=5*p; }
					else
					{ er-=5*p; mr-=4*p; }
				}
			}
		}

		/* pawn phalanx or chain, both midgame & endgame */
		if( b[sq-1]==WP || b[sq-9]==WP || b[sq-11]==WP )
		{
			if( sq >= A4 )
			{ mr += 6; er += 4; }
			else
			{ mr += 2; er += 2; }
		}

		if( Totmat >= ENDMATERIAL )
		{
			/*** outpost "spacemaker" pawn in midgame */
			if( outpost[sq] )
			{
				int o = outpost[sq] + (sq==D4);
				if( b[sq-1]==WP ) mr += 2*o;
				if( b[sq+1]==WP ) mr += 2*o;
				if( b[sq-9]==WP || b[sq-11]==WP ) mr += 3*o;
				if( b[sq+9]==BP || b[sq+11]==BP ) mr -= 3*o;
				if(b==B) mr += (P[sq]&0x00FF)/0x0020 *  5*o;
				else     mr += (P[sq]>>8)    /0x0020 *  5*o;
			}

			/*** storm */
			if( know->qstorm ) mr += qstormtable[sq];
			else if( know->kstorm ) mr += kstormtable[sq];

			/*** `table' bonus */
			mr += pmpb[sq];

			/*** don't play a4/b4/g4/h4 in opening */
			if( sq >= A4 )
			{
			  if( know->castling == (WSHORT|WLONG) )
			  {
			    if( kp%10 > FILE_E )
			    {
				    if( sq%10 > FILE_F )
					    mr -= 10;
			    }
			    else
			    {
				    if( kp%10 < FILE_D && sq%10 < FILE_C )
				      mr -= 8;
			    }
			  }
			  else
			  {
                            if( sq%10 < 3 )
				    mr -= know->devel;
                            else if( sq%10 > 6 ) mr -= 4+know->devel;
			  }
			}
		}

	break;
	case WK:
		er += kepb[sq];

		if( Totmat >= ENDMATERIAL )
		{
			mr += kmpb[sq];
			/* SHELTER */

			switch( b[sq-1] )
			{ case WP: mr += 3; know->kshield++; break;
		  	  case WB: mr += 2; know->kshield++;
			           if( b[sq+9]==WP ) mr += 2; }

			switch( b[sq+1] )
			{ case WP: mr += 3; know->kshield++; break;
		  	  case WB: mr += 2; know->kshield++;
			           if( b[sq+11]==WP ) mr += 2; }

			switch( b[sq+10] )
			{ case WP: mr += 6; know->kshield+=2;
				if( b[sq+9]==WP || b[sq+11]==WP )
				{ mr += 3; know->kshield+=2; }
		  	  break;
		  	  case WB: mr += 2; know->kshield++;
				if( b[sq+9]==WP || b[sq+11]==WP )
				{
				  know->kshield++;
				  if( b[sq+20]==WP )
				  { mr += 2; know->kshield++; }
				}
			}

			if( b[sq+20]==WP )
			{ know->kshield++;
			  if( b[sq+19]==WP || b[sq+21]==WP )
			    know->kshield++;
			}

			if( b[sq+9]==WP ) { know->kshield++; mr += 4; }

			if( b[sq+11]==WP ) { know->kshield++; mr += 4; }

			if( xknow->q || xknow->r >= 2 )
			{	int file = sq%10;
			  	if( pf[file] == 0 ) mr -= 6;
				if( pf[file-1] == 0 ) mr -= 3;
				if( pf[file+1] == 0 ) mr -= 3;
			  
				if( file > FILE_C && file < FILE_F )
				{ /* open center makes us cry */
					if( xpf[file] == 0 )
					{
					  if(pf[file]==0) mr -= 8;
					  else            mr -= 5;
					}
				}
			}
		}

	break;
	case WR:
	{
		int j;
		int r;
		int mrr = 0; int err = 0;
		static int frb[12] =
			{ 0, 0, -5, -5, -2, 0, 2, 4, 8, 10, 10, 10 };

		for( j = sq+10; b[j]==0 || b[j]>=ROOK; j += 10 ) {};

		r=j/10;		/* 2...10 */

		if( b[j]==WP && b[j+1] ) r--;

		mrr = err = frb[r];

		if( xpf[sq%10] != 0 ) mrr -= 5;

		if( pf[sq%10] != 0 )
		{
		  mrr -= 5;
		  if( b[j]==WP )
		  {
			/* avoid pseudocastling, that's moving king
			 * e1-f1-g1, satisfying king safety but blocking
			 * the rook undeveloped */
			if( sq==H1 || sq==H2 || sq==G1 )
			if( kp==F1 || kp==G1 )
			{ mrr -= 33; err -= 33; }
		  }
		}

		{
			static int bon[8] =
			{ 5, 3, 1, -2, -2, -2, -2, -2 };
			int b = bon[ xkdist[sq].min ];
			if(b>0)
			{
				b += r;
				if( know->qstorm || know->kstorm ) mrr += b;
				if( b > 5 )
				{
					if( b > 8 ) xknow->khung += 2;
					else         xknow->khung ++;
				}
			}
		}

		mrr += rmpb[sq];

		mr += mrr; er += err;
#ifdef SCORING
		if( Scoring )
			if(l==wl)
			printf( " (whi) rook = %i,%i\n", mrr, err );
			else
			printf( " (bla) rook = %i,%i\n", mrr, err );
#endif
	}
	break;
	case WB:
		if(    b[sq+10]==BP
		    || (    outpost[sq]
		         && ( b[sq-9]==WP || b[sq-11]==WP )
		       )
		  )
		{
			int j;
			int bonus;

			for( j=sq+9; j<A8; j+=10 )
			if( b[j]==BP || b[j+2]==BP ) goto no_bishop_outpost;

				bonus = 0;
				if( b[sq+10]==BP ) bonus += 12*xpf[sq%10];
				if( b[sq+20]==BP ) bonus += 8;
				if( outpost[sq] ) bonus += 10 + 4*outpost[sq];
				if( b[sq-9]==WP ) bonus += 10;
				if( b[sq-11]==WP ) bonus += 10;

			mr += bonus; er += bonus/2;

#ifdef SCORING
			if( Scoring )
				if(l==wl)
				printf( " (whi) strong bishop = %i\n", bonus );
				else
				printf( " (bla) strong bishop = %i\n", bonus );
#endif

			no_bishop_outpost:;
		}

		/*** trapped bishop - it's not worth the pawn ***/
		if( sq==A7 )
		{
			if( b[B6] == BP && see(b,A7,B6) < -200 )
			{ mr -= 140; er -= 140; }
		}
		else
		if( sq==H7 )
		{
			if( b[G6] == BP && see(b,H7,G6) < -200 )
			{ mr -= 140; er -= 140; }
		}

		if( know->qstorm || know->kstorm )
		{
			static int bon[16] =
			{ 8,  8,  8,  7,  5,  3,  1,  0,
			 -1, -2, -3, -4, -5, -6, -7, -8 };
			mr += bon[ xkdist[sq].diag ];
		}

		mr += bmpb[sq]; er += bepb[sq];
	break;
	case WN:
		if(    b[sq+10]==BP
		    || (    outpost[sq]
		         && ( b[sq-9]==WP || b[sq-11]==WP )
		       )
		  )
		{
			int j;
			int bonus;

			for( j=sq+9; j<A8; j+=10 )
			if( b[j]==BP || b[j+2]==BP ) goto no_knight_outpost;

				bonus = 0;
				if( b[sq+10]==BP ) bonus += 15*xpf[sq%10];
				if( outpost[sq] ) bonus += 8 + 5*outpost[sq];
				if( b[sq-9]==WP ) bonus += 10;
				if( b[sq-11]==WP ) bonus += 10;

				/*** Look at opponent's minor pieces:
				 *** is it possible to attack the knight
				 *** with a minor piece? ******************/
				if( sq>=A4 && sq%10>=FILE_B && sq%10<=FILE_G
				 && xknow->n == 0
				 && ( know->xbishopcolor & sqcolor[sq] ) == 0
				 && ( b[sq-9]==WP || b[sq-11]==WP )
				  )
				{ mr+=8+2*outpost[sq]; er+=15+4*outpost[sq]; }

			mr += bonus; er += bonus/2;

#ifdef SCORING
			if( Scoring )
				if(l==wl)
				printf( " (whi) strong knight = %i\n", bonus );
				else
				printf( " (bla) strong knight = %i\n", bonus );
#endif

			no_knight_outpost:;
		}

		/*** trapped knight ***/
		if( sq==A8 )
		{
			if( b[A7] == BP && see(b,A8,C7) < -200 )
			{ mr -= 140; er -= 140; }
		}
		else if( sq==H8 )
		{
			if( b[H7] == BP && see(b,H8,F7) < -200 )
			{ mr -= 140; er -= 140; }
		}
		else if( sq==A7 )
		{
			if( b[B7]==BP )
			if( b[A6]==BP || b[C6]==BP )
			{ mr -= 45; er -= 45; }
		}
		else if( sq==H7 )
		{
			if( b[G7]==BP )
			if( b[H6]==BP || b[F6]==BP )
			{ mr -= 45; er -= 45; }
		}

		mr += nmpb[sq];

		if( know->qstorm || know->kstorm )
		{
			static int bon[8] =
				{ 8, /* 1-st entry is never used */
				     8, 8, 5, 2, -1, -4, -7 };
			mr += bon[ xkdist[sq].max ];
		}

		if( Totmat < ENDMATERIAL )
		{
			/* look at the leftmost and righmost pawn file -
			 * large distance between these files is bad
			 * for knight in endgame */
			int d =
			max(Wknow.rpf,Bknow.rpf)-min(Wknow.lpf,Bknow.lpf);
			if(d>3)  /* look at nonpawn material */
			{ if( Totmat-P_VALUE*(Wknow.p+Bknow.p)
			      <= N_VALUE+B_VALUE ) er-=5*d;
			  else                     er-=3*d;
			}
		}

		er += c_bonus[sq];
	break;
	case WQ:
		/* penalize early queen activity, */
		/* 10 to 31 pts., depending on development */
		if( know->devel >= 8 )
		if( sq >= A3 ) mr -= (2+know->devel);

		/* let's play aggressively - */
		/* penalize for distance from enemy king */
		/* active queen is very strong; especially in endgame */
		{
			static int bon[16] =
			{ 10, 10, 10,  9,  7,  5,  3,  0,
			  -2, -4, -8, -8, -8, -8, -8, -8 };
			int qb;

			if( !( B==b ? (P[sq]>>8) : (P[Th[sq]]&0x00FF) ) )
				qb = bon[ xkdist[sq].taxi ];
			else	qb = -8;

			if( know->qstorm || know->kstorm ) mr += 2*qb;
			else mr += qb;
			er += 3*qb;
		}

		mr += qmpb[sq];

	break;
	}
	}

	if( l == wl )
	{
		midresult += mr; endresult += er;
		l = bl; n = bn; b = mB;
		pf = bpf; xpf = wpf;
		know = &Bknow; xknow = &Wknow;
		kp = Th[Bknow.kp]; xkp = Th[Wknow.kp];
	}
	else
	{
		midresult -= mr; endresult -= er;
		break;
	}
}


/*********** Development */

if( Totmat >= ENDMATERIAL )
{

if( Color == WHITE ) { if( Wknow.devel>1 && Wknow.hung<10 ) Wknow.devel-=2; }
else                 { if( Bknow.devel>1 && Bknow.hung<10 ) Bknow.devel-=2; }

{
	int d = Bknow.devel-Wknow.devel;
	d = 4*d + d*max(Wknow.devel,Bknow.devel)/8;

	midresult += d;

#ifdef SCORING
	if( Scoring )
	{
		printf(" (   ) %i %i\n", Bknow.devel, Wknow.devel );
		if( d!=0 ) printf( " (w/b) development = %i\n", d );
	}
#endif
}

}
else  /* outside passed pawn */
{
	int whas=0, bhas=0;

	extern int Oppb[];  /* outside passed pawn bonus, see endgame.c */

	/* white/black left/right -most pawn file (0-10) */
	int wrm = Wknow.rpf;
	int wlm = Wknow.lpf;
	int brm = Bknow.rpf;
	int blm = Bknow.lpf;

	/*** look at leftmost pawn ***/
	if( wlm+1<blm && wrm>wlm+3 ) whas=blm-wlm;
	else
	if( blm+1<wlm && brm>blm+3 ) bhas=wlm-blm;

	/*** look at rightmost pawn ***/
	if( wrm-1>brm && wrm>wlm+3 ) whas+=wrm-brm;
	else
	if( brm-1>wrm && brm>blm+3 ) bhas+=brm-wrm;

	endresult +=
		(Oppb[whas]-Oppb[bhas]) * Q_VALUE
		/ (Q_VALUE+Totmat-P_VALUE*(Wknow.p+Bknow.p));
}

midresult += (Bknow.khung-Wknow.khung) * 2;

/*********** King danger */
Wknow.khung -= Wknow.kshield/3;
Bknow.khung -= Bknow.kshield/3;
if( Wknow.khung > 1 )
{
	midresult -= Wknow.khung * Wknow.khung;
#ifdef SCORING
	if( Scoring )
	printf( " (whi) king safety = %i\n", Wknow.khung * Wknow.khung * -4 );
#endif
}

if( Bknow.khung > 1 )
{
	midresult += Bknow.khung * Bknow.khung;
#ifdef SCORING
	if( Scoring )
	printf( " (bla) king safety = %i\n", Bknow.khung * Bknow.khung * -4 );
#endif
}

Wknow.prune = ( Wknow.hung < 10 && Wknow.khung < 2 );
Bknow.prune = ( Bknow.hung < 10 && Bknow.khung < 2 );

/**
***   Trade down bonus
**/
{
	int tbonus = ( G[Counter].mtrl - G[Counter].xmtrl );
	if( tbonus != 0 )
	{
		int r;

		if( Color == BLACK ) tbonus = -tbonus;

		if( tbonus > 0 ) /* white stronger */
		{
			if( Wknow.p || tbonus>=(N_VALUE+P_VALUE) )
			r = (   tbonus * Wknow.p
			      - tbonus * (2*Bknow.q+Bknow.b+Bknow.r+Bknow.n)
			    ) / 30;
			else /* white has no pawns! */
			r = - tbonus / 2;
		}
		else             /* black stronger */
		{
			if( Bknow.p || tbonus<=(N_VALUE+P_VALUE) )
			r = (   tbonus * Bknow.p
			      - tbonus * (2*Wknow.q+Wknow.b+Wknow.r+Wknow.n)
			    ) / 30;
			else
			r = - tbonus / 2;
		}

		endresult += r;
		midresult += r/2;

#ifdef SCORING
		if( Scoring )
		printf(" (w/b) traded pieces/pawns = %i\n", r );
#endif
	}
}

if( Totmat < ENDMATERIAL ) result += endresult;
else if( Totmat > MIDMATERIAL ) result += midresult;
else result += (Totmat-ENDMATERIAL) * midresult / (MIDMATERIAL-ENDMATERIAL)
            + (MIDMATERIAL-Totmat) * endresult / (MIDMATERIAL-ENDMATERIAL);

/*** different colored bishops in endgame lead to draw ***/
if( Totmat <  ENDMATERIAL )
if(   Wknow.bishopcolor
   && Wknow.xbishopcolor
   && Wknow.bishopcolor+Wknow.xbishopcolor == 3
   && Wknow.n==0 && Wknow.r==0 && Wknow.q==0
   && Bknow.n==0 && Bknow.r==0 && Bknow.q==0 )
{
	int difference = ( G[Counter].mtrl - G[Counter].xmtrl ) / 4;
	result/=2;
	if( difference > 0 )
	{ if(Color==WHITE) result -= 50+difference;
	              else result += 50+difference; }
	else
	if( difference < 0 )
	{ if(Color==WHITE) result += 50-difference;
	              else result -= 50-difference; }
}

#define TEMPO 4

if( Color == WHITE ) return result+TEMPO;
else return -result-TEMPO;

}

