/*
 *   Generating moves
 */

#include "phalanx.h"


unsigned char pin[120];
unsigned char tar[120];



#undef debugsee
/* Static Exchange Evaluator */
int see( tsquare * b, int from, int square )
{

static int wl[16];
static int bl[16];
int wn, bn;
int color = color(b[from]);
int i,p;
int minwn, minbn;
unsigned static int backup;

#define ret(X) return ( color==WHITE ? (X) : -(X) )

wn=bn=1;
if( color == WHITE )
{ wl[0] = Values[ b[from] >> 4 ];
  bl[0] = Values[ b[square] >> 4 ];
}
else
{ wl[0] = Values[ b[square] >> 4 ];
  bl[0] = Values[ b[from] >> 4 ];
}

backup = b[from]; b[from] = 0;

if( b[square-9]==WP )  { wl[wn] = P_VALUE; wn++; }
if( b[square-11]==WP ) { wl[wn] = P_VALUE; wn++; }
if( b[square+9]==BP )  { bl[bn] = P_VALUE; bn++; }
if( b[square+11]==BP ) { bl[bn] = P_VALUE; bn++; }

for( i=0; i!=8; i++ ) if( ( p = b[square+N_moves[i]] ) )
switch( p )
{	case WN: wl[wn] = N_VALUE; wn++; break;
	case BN: bl[bn] = N_VALUE; bn++; break;
}

wl[ minwn=wn ] = CHECKMATE;
bl[ minbn=bn ] = CHECKMATE;

for( i=0; i!=8; i++ )
{	int step = RB_dirs[i];
	int minw = minwn;
	int minb = minbn;
	int dest = square;
	for(;;)
	{
		dest += step;
		while( b[dest] == 0 ) dest += step;
		p = b[dest];
		if( p==3 || piece(p)==KNIGHT ) break;
		if( i < 4 ) { if( piece(p) == BISHOP ) break; }
		else        { if( piece(p) == ROOK ) break; }
		if( p==WK )
		{
			if( dest-step==square )
			{ wl[wn] = CHECKMATE; wn++;
			  wl[wn] = CHECKMATE; }
			break;
		}
		else if( p==BK )
		{
			if( dest-step==square )
			{ bl[bn] = CHECKMATE; bn++;
			  bl[bn] = CHECKMATE; }
			break;
		}
		else if( p==WP )
		{
			if( dest-square == 9 || dest-square == 11 )
			continue; else break;
		}
		else if( p==BP )
		{
			if( dest-square == -9 || dest-square == -11 )
			continue; else break;
		}
		else
		{
			int j;
			int val = Values[p>>4];
			if( color(p) == WHITE )
			{
			  while( wl[minw] <= val ) minw++;
			  for( j=wn+1; j!=minw; j-- )
				wl[j] = wl[j-1];
			  wl[minw] = val;
			  wn++; wl[wn]=CHECKMATE;
			  minw++;
			}
			else
			{
			  while( bl[minb] <= val ) minb++;
			  for( j=bn+1; j!=minb; j-- )
				bl[j] = bl[j-1];
			  bl[minb] = val;
			  bn++; bl[bn]=CHECKMATE;
			  minb++;
			}
		}
	}
}
b[from] = backup;

#ifdef debugsee
printf("WHITE: "); for(i=0;i!=wn;i++) printf(" %hi",wl[i]); puts("");
printf("BLACK: "); for(i=0;i!=bn;i++) printf(" %hi",bl[i]); puts("");
#endif

/*** Let's count it ***/
{
	int wi=0; int bi=0; int result=0;
	int alpha=-CHECKMATE, beta=CHECKMATE;

	if( color == BLACK )
	{ result -= wl[wi]; wi++; alpha = result; }

	for(;;)
	{
		if( wi == wn ) ret(alpha);
		else
		{
			result += bl[bi]; bi++;
			if( result < beta )
			{ if( result < alpha ) ret(alpha); beta = result; }
		}
#ifdef debugsee
		printf("[w,%hi,%hi,%hi]",alpha,result,beta);
#endif

		if( bi == bn ) ret(beta);
		else
		{
			result -= wl[wi]; wi++;
			if( result > alpha )
			{ if( result > beta ) ret(beta); alpha = result; }
		}
#ifdef debugsee
		printf("[b,%hi,%hi,%hi]",alpha,result,beta);
#endif
	}
}

}



void find_pins( int check )
{
int d, square;

if(check) { memset( pin+20, 1, 80*sizeof(char) ); return; }

memset( pin+20, 0, 80*sizeof(char) );

square = L[Color].next;

pin[square] = 1;

for(d=0;d!=8;d++)
{
	int step = RB_dirs[d];
	int destination = square;
	int mark = 0;
	for(;;)
	{	destination += step;
		if( B[destination] )
		{
			if( color(B[destination]) == Color )
			{
				if(mark) break;
				mark = destination;
			}
			else
			{
				if(mark)
				{
					switch( piece(B[destination]) )
					{
					  case QUEEN:
						pin[mark]=abs(step);
					  break;
					  case ROOK:
						if(d<4) pin[mark]=abs(step);
					  break;
					  case BISHOP:
						if(d>3) pin[mark]=abs(step);
					  break;
					}
				}
				break;
			}
		}
	}
}

#undef debug
#ifdef debug
printboard();
for(d=0;d!=120;d++)
{
  if(d%10==0) puts("");
  printf("%3i",pin[d]);
}
getchar();
#endif

}



void find_targets(void)
{
int d,square;

/* memset( tar, 0, 120*sizeof(char) ); */
memset( tar+20, 0, 80*sizeof(char) );

square = L[enemy(Color)].next;

for(d=0;d!=8;d++)
{
	int step = RB_dirs[d];
	int destination = square;
	for(;;)
	{	destination += step;
		if( B[destination] ) break;
		if( d<4 ) tar[destination]=ROOK;
		else tar[destination]=BISHOP;
	}

	if( B[(destination=square+N_moves[d])] == 0 ) tar[destination]=KNIGHT;
}

#undef debug
#ifdef debug
printboard();
for(d=0;d!=120;d++)
{
  if(d%10==0) puts("");
  printf("%04X ",tar[d]);
}
getchar();
#endif

}



/*
 *    LINKING GENERATED MOVE
 */

int square, destination;
#define From square
#define To destination

void lm(
	tmove *m,
	unsigned char In2a, unsigned char Special,
	int *i )
{

m->in1 = B[m->from=From];
m->in2 = B[m->to=To];
m->in2a = In2a;
m->dch = 100;
m->value = 0;
m->special = Special;
if( m->special != 0 || pin[m->from] )
{
	do_move(m);
	if( ! checktest(enemy(Color)) ) (*i)++;
	undo_move(m);
} else (*i)++;

}

# define link_move(In2a,Special) \
 { lm( moves+i, In2a, Special, &i ); }

#define link_castling(C) \
 { register tmove* m = &moves[i]; \
   m->from = A1; /* anything different from 0 */ \
   m->to = A1; \
   m->dch = 100; \
   m->special = C; \
   i++; \
 }



void generate_legal_moves( tmove moves[], int *n, int check )
{

int i = 0;

find_pins( check );

for( square=L[Color].next; square!=0; square=L[square].next )
{
	unsigned pbs=piece(B[square]);
	switch ( pbs )
	{
	case PAWN:
	 if(Color==WHITE)
	 {	destination = square+10;
		if( B[destination] == 0 )
		{
			if( destination < A8 ) /* not a promotion */
			{ link_move(WP,0); }
			else /* pawn promotion */
			{
				unsigned piece;
				for( piece=WQ; piece!=WP; piece-=PAWN )
				link_move(piece,0);
			}
			if( square < A3 )
			{	destination += 10;
				if( B[destination] == 0 )
				link_move(B[square],0);
			}
		}
		for( destination = square+9;
		     destination != square+13;
		     destination += 2 )
		  if( color(B[destination]) == BLACK )
		  {
			if( destination < A8 )
			{ link_move(WP,0); }
			else
			{
				unsigned piece;
				for( piece=WQ; piece!=WP; piece-=PAWN )
				link_move(piece,0);
			}
		  }
	 }
	 else /* This is a black pawn */
	 {	destination = square-10;
		if( B[destination] == 0 )
		{
			if( destination>H1 ) /* not a promotion */
			{ link_move(BP,0); }
			else /* pawn promotion */
			{
				unsigned piece;
				for( piece=BQ; piece!=BP; piece-=PAWN )
				link_move(piece,0);
			}

			if( square>H6 )
			{	destination -= 10;
				if( B[destination] == 0 )
					link_move(BP,0);
			}
		}
		for( destination = square-9;
		     destination != square-13;
		     destination -= 2 )
		if( color(B[destination]) == WHITE )
		{
			if( destination > H1 )
			{ link_move(BP,0); }
			else
			{
				unsigned piece;
				for( piece=BQ; piece!=BP; piece-=PAWN )
				link_move(piece,0);
			}
		}
	 }
	break;
	case KNIGHT: case KING:
	 {	int j;
		int *mov = pbs==KING ? K_moves : N_moves;
		for( j=0; j!=8; j++ )
		{	destination = square+mov[j];
			if( ! ( color(B[destination]) & Color ) )
				link_move(B[square],0);
		}
	 }
	break;
	default: /* BISHOP, ROOK, QUEEN */
	 {	int d;
		int d1 = pbs==BISHOP ? 4 : 0;
		int d2 = pbs==ROOK   ? 4 : 8;
		for( d=d1; d!=d2; d++ )
		{	int step=RB_dirs[d];
			destination=square;
			for(;;)
			{	destination += step;
				if( ! ( color(B[destination]) & Color ) )
				link_move(B[square],0);
				if( B[destination] ) break;
			}
		}
	 }
	}
}

/* Special moves: en passant, castling */
/* En passant capture first */
if( Counter != 0 )
{
	int c = Counter-1;
	if( piece(G[c].m.in1) == PAWN  &&  abs(G[c].m.from-G[c].m.to) == 20 )
	/* The last move was a pawn double step */
	{
		for( square = G[c].m.to-1; square != G[c].m.to+3; square += 2 )
		if( B[square] == PAWN+Color )
		{
			int special = G[c].m.to;
			destination = Color==WHITE ? special+10 : special-10;
			link_move( B[square], special )
		}
	}
}

/* Castling */
if( ! check )
{
	if( Color == WHITE )
	{
		/****** Short ******/
		if( ( G[Counter].castling & WSHORT ) == 0 )
		 if( B[F1]==0 && B[G1]==0 )
		  if( ! attacktest(F1,BLACK) && ! attacktest(G1,BLACK) )
		   link_castling( SHORT_CASTLING );
		/****** Long ******/
		if( ( G[Counter].castling & WLONG ) == 0 )
		 if( B[B1]==0 && B[C1]==0 && B[D1]==0 )
		  if( ! attacktest(C1,BLACK) && ! attacktest(D1,BLACK) )
		   link_castling( LONG_CASTLING );
	}
	else /* Black */
	{
		/****** Short ******/
		if( ( G[Counter].castling & BSHORT ) == 0 )
		 if( B[F8]==0 && B[G8]==0 )
		  if( ! attacktest(F8,WHITE) && ! attacktest(G8,WHITE) )
		   link_castling( SHORT_CASTLING );
		/****** Long ******/
		if( ( G[Counter].castling & BLONG ) == 0 )
		 if( B[B8]==0 && B[C8]==0 && B[D8]==0 )
		  if( ! attacktest(C8,WHITE) && ! attacktest(D8,WHITE) )
		   link_castling( LONG_CASTLING );
	}
}

*n = i;

}



void lc( tmove *m, int *i )
{

if( Depth < -200 )
{
if( Values[ B[From]>>4 ] > Values[ B[To]>>4 ] )
{
  if( Color == BLACK )
  { if( (P[To]&0x00FF) ) return; }
  else
  { if( (P[To]&0xFF00) ) return; }
}
}
else
if( Values[ B[From]>>4 ] > Values[ B[To]>>4 ] )
{
	if( see(B,From,To) < 0 ) return;
}

m->in1 = m->in2a = B[m->from=From];
m->in2 = B[m->to=To];
m->dch = 100;
m->value = 0;
m->special = 0;
if( pin[m->from] )
{
	do_move(m);
	if( ! checktest(enemy(Color)) ) (*i)++;
	undo_move(m);
} else (*i)++;

}


# define link_capture { lc( moves+i, &i ); }



void generate_legal_captures( tmove moves[], int *n, int minval )
/* we are sure we are not in check */
{

int i = 0;

find_pins( 0 );

for( square=L[Color].next; square!=0; square=L[square].next )
{
	unsigned pbs=piece(B[square]);
	switch ( pbs )
	{
	case PAWN:
	 if(Color==WHITE)
	 {	destination = square+10;
		if( B[destination] == 0 && destination>=A8 )
		{ link_move(WQ,0); }
		for( destination = square+9;
		     destination != square+13;
		     destination += 2 )
		if( color(B[destination]) == BLACK )
		{
			if( destination < A8 )
			{
				if( Values[ B[destination] >> 4 ] >= minval )
					link_move(WP,0);
			}
			else
			{
				link_move(WQ,0);
			}
		}
	 }
	 else /* This is a black pawn */
	 {	destination = square-10;
		if( B[destination] == 0 && destination<=H1 )
		{ link_move(BQ,0); }
		for( destination = square-9;
		     destination != square-13;
		     destination -= 2 )
		if( color(B[destination]) == WHITE )
		{
			if( destination > H1 )
			{
				if( Values[ B[destination] >> 4 ] >= minval )
					link_move(BP,0);
			}
			else
			{
				link_move(BQ,0);
			}
		}
	 }
	break;
	case KNIGHT: case KING:
	 {	int j;
		int *mov = pbs==KING ? K_moves : N_moves;
		for( j=0; j!=8; j++ )
		{	destination = square+mov[j];
			if( color(B[destination]) == enemy(Color) )
			if( Values[ B[destination] >> 4 ] >= minval ) link_capture;
		}
	 }
	break;
	default: /* BISHOP, ROOK, QUEEN */
	 {	int d;
		int d1 = pbs==BISHOP ? 4 : 0;
		int d2 = pbs==ROOK   ? 4 : 8;
		for( d=d1; d!=d2; d++ )
		{	int step=RB_dirs[d];
			destination=square;
			for(;;)
			{	destination += step;
				if( color(B[destination]) == enemy(Color) )
				if( Values[ B[destination] >> 4 ] >= minval )
					link_capture;
				if( B[destination] ) break;
			}
		}
	 }
	}
}

*n = i;

}



#ifndef QCAPSONLY
/* generating checks AND captures */
void generate_legal_checks( tmove moves[], int *n )
/* we are sure we are not in check */
/* pawn checks should be added */
{

int i = 0;

find_pins( 0 );
find_targets();

for( square=L[Color].next; square!=0; square=L[square].next )
{
	unsigned pbs=piece(B[square]);
	switch ( pbs )
	{
	case PAWN:
	 if(Color==WHITE)
	 {	destination = square+10;
		if( B[destination] == 0 && ( destination>=A8 ) )
		{
			link_move(WQ,0);
			link_move(WN,0);
		}
		for( destination = square+9;
		     destination != square+13;
		     destination += 2 )
		if( color(B[destination]) == BLACK )
		{
			if( destination < A8 )
			{ link_move(WP,0); }
			else
			{
				link_move(WQ,0);
				link_move(WN,0);
			}
		}
	 }
	 else /* This is a black pawn */
	 {	destination = square-10;
		if( B[destination] == 0 && destination<=H1 )
		{
			link_move(BQ,0);
			link_move(BN,0);
		}
		for( destination = square-9;
		     destination != square-13;
		     destination -= 2 )
		if( color(B[destination]) == WHITE )
		{
			if( destination > H1 )
			{ link_move(BP,0); }
			else
			{
				link_move(BQ,0);
				link_move(BN,0);
			}
		}
	 }
	break;
	case KNIGHT: case KING:
	 {	int j;
		int *mov = pbs==KING ? K_moves : N_moves;
		for( j=0; j!=8; j++ )
		{	destination = square+mov[j];
			if( color(B[destination]) == enemy(Color)
			  || ( B[destination]==0 && tar[destination] == pbs ) )
			link_capture;
		}
	 }
	break;
	default: /* BISHOP, ROOK, QUEEN */
	 {	int d;
		int d1 = pbs==BISHOP ? 4 : 0;
		int d2 = pbs==ROOK   ? 4 : 8;
		for( d=d1; d!=d2; d++ )
		{	int step=RB_dirs[d];
			destination=square;
			for(;;)
			{	destination += step;
				if( color(B[destination]) == enemy(Color)
				  || ( B[destination]==0 && tar[destination]
				       &&
				       ( tar[destination]==pbs
				         || ( tar[destination]!=KNIGHT
				            && pbs==QUEEN )
				       )
				     )
				  )
				link_capture;
				if( B[destination] ) break;
			}
		}
	 }
	}
}

*n = i;

}

#endif /* QCAPSONLY */

