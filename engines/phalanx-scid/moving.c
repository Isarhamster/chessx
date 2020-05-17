/*
 * executing and unexecuting generated moves
 */

#include "phalanx.h"


#undef DEBUG_MLIST
#ifdef DEBUG_MLIST
int checklist(tmove *m)
{
	int j;
	int err=0;
	int wp=0, bp=0;
	for( j=WKP; j!=0 && wp<18; j=L[j].next )
	{ wp++; if(color(B[j])!=WHITE) err=1; }
	for( j=A1; j!=H9; j++ ) if( color(B[j])==WHITE ) wp--;

	for( j=BKP; j!=0 && bp<18; j=L[j].next )
	{ bp++; if(color(B[j])!=BLACK) err=1; }
	for( j=A1; j!=H9; j++ ) if( color(B[j])==BLACK ) bp--;

	if( wp!=0 || bp!=0 || err )
	{
		int x=18;
		puts("");
		for( j=WKP; j!=0 && x!=0; j=L[j].next )
			{ x--; if(color(B[j])!=WHITE) printf("!"); printf("%i,",j); }
		puts("");
		x=18;
		for( j=BKP; j!=0 && x!=0; j=L[j].next )
			{ x--; if(color(B[j])!=BLACK) printf("!"); printf("%i,",j); }
		puts("");
		printboard(NULL);
		printf("wp=%i, bp=%i\n",wp,bp);
		for(j=0;j!=Counter;j++) printm(G[j].m,NULL);
		puts(""); if( m!=NULL) printm(*m,NULL);
		getchar();
		return 1;
	}
	return 0;
}
#endif



#define update_hash(p,s) { G[Counter].hashboard ^= H[ HP[p] ][ HS[s] ]; }



void do_move(register tmove * m)
{

G[Counter].m = *m;
Counter++;
G[Counter].castling = G[Counter-1].castling;
G[Counter].rule50 = G[Counter-1].rule50 + 1;
G[Counter].hashboard = HASH_COLOR ^ G[Counter-1].hashboard;
G[Counter].mtrl = G[Counter-1].xmtrl;
G[Counter].xmtrl = G[Counter-1].mtrl;
Ply++;

switch(m->special)
{
  case 0: /* most of the moves */

	update_hash( m->in1, m->from );
	update_hash( m->in2a, m->to );
	if( m->in2 != 0 ) /* capture */
	{
		update_hash( m->in2, m->to );
		G[Counter].mtrl -= Values[ m->in2 >> 4 ];
		G[Counter].rule50 = 0;

		/* remove captured piece from the list */
		L[ L[m->to].prev ].next = L[m->to].next;
		L[ L[m->to].next ].prev = L[m->to].prev;
	} else
	if( piece(m->in1) == PAWN ) G[Counter].rule50 = 0;

	/* update piece list */
	L[m->to] = L[m->from];
	L[ L[m->to].prev ].next = L[ L[m->to].next ].prev = m->to;

	if( m->in2a != m->in1 ) /* pawn promotion */
	G[Counter].xmtrl += Values[ m->in2a >> 4 ] - P_VALUE;

	B[m->from] = 0;
	B[m->to] = m->in2a;

{
	static unsigned char cb_[80] =
	{
		0, WLONG, 0, 0, 0, (WSHORT|WLONG), 0, 0, WSHORT, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, BLONG, 0, 0, 0, (BSHORT|BLONG), 0, 0, BSHORT, 0
	}; static unsigned char * cb = cb_-20;

	G[Counter].castling |= cb[m->from] | cb[m->to];
}

	break;

  case SHORT_CASTLING:
	if( Color == WHITE )
	{
		B[E1]=0; B[F1]=WR; B[G1]=WK; B[H1]=0;
		G[Counter].castling |= ( WSHORT | WLONG );
		update_hash( WK, E1 );
		update_hash( WK, G1 );
		update_hash( WR, H1 );
		update_hash( WR, F1 );
		L[G1] = L[E1]; WKP = L[L[G1].next].prev = G1;
		L[F1] = L[H1]; L[L[F1].prev].next = L[L[F1].next].prev = F1;
	}
	else
	{
		B[E8]=0; B[F8]=BR; B[G8]=BK; B[H8]=0;
		G[Counter].castling |= ( BSHORT | BLONG );
		update_hash( BK, E8 );
		update_hash( BK, G8 );
		update_hash( BR, H8 );
		update_hash( BR, F8 );
		L[G8] = L[E8]; BKP = L[L[G8].next].prev = G8;
		L[F8] = L[H8]; L[L[F8].prev].next = L[L[F8].next].prev = F8;
	}
	break;
  case LONG_CASTLING:
	if( Color == WHITE )
	{
		B[A1]=0; B[C1]=WK; B[D1]=WR; B[E1]=0;
		G[Counter].castling |= ( WSHORT | WLONG );
		update_hash( WK, E1 );
		update_hash( WK, C1 );
		update_hash( WR, A1 );
		update_hash( WR, D1 );
		L[C1] = L[E1]; WKP = L[L[C1].next].prev = C1;
		L[D1] = L[A1]; L[L[D1].prev].next = L[L[D1].next].prev = D1;
	}
	else
	{
		B[A8]=0; B[C8]=BK; B[D8]=BR; B[E8]=0;
		G[Counter].castling |= ( BSHORT | BLONG );
		update_hash( BK, E8 );
		update_hash( BK, C8 );
		update_hash( BR, A8 );
		update_hash( BR, D8 );
		L[C8] = L[E8]; BKP = L[L[C8].next].prev = C8;
		L[D8] = L[A8]; L[L[D8].prev].next = L[L[D8].next].prev = D8;
	}
	break;
  default: /* en passant pawn capture */

	update_hash( m->in1, m->from );
	update_hash( m->in2a, m->to );
	update_hash( B[m->special], m->special );

	B[m->from] = 0;
	B[m->to] = m->in2a;
	B[m->special] = 0; /* captured pawn */

	/* update piece list */
	L[m->to] = L[m->from];
	L[ L[m->to].prev ].next = L[ L[m->to].next ].prev = m->to;
	L[L[m->special].next].prev = L[m->special].prev;
	L[L[m->special].prev].next = L[m->special].next;

	G[Counter].mtrl -= P_VALUE;
	G[Counter].rule50 = 0; /* this is an irreversible move */
}

Color = enemy(Color);
Depth -= m->dch;

#ifdef DEBUG_MLIST
checklist(m);
#endif

}



void undo_move(register tmove * m)
{

Color = enemy(Color);
Depth += m->dch;
Ply--; Counter--;

switch(m->special)
{
  case 0: /* most of the moves */
	B[m->from] = m->in1;
	B[m->to] = m->in2;

	/* update piece list */
	L[m->from] = L[m->to];
	L[ L[m->from].prev ].next = L[ L[m->from].next ].prev = m->from;
	if( m->in2 != 0 ) /* return captured piece to the list */
	{
		if( color(m->in2) == WHITE )
		{
			L[m->to].next = L[WKP].next;
			L[m->to].prev = WKP;
			L[L[WKP].next].prev = m->to;
			L[WKP].next = m->to;
		}
		else
		{
			L[m->to].next = L[BKP].next;
			L[m->to].prev = BKP;
			L[L[BKP].next].prev = m->to;
			L[BKP].next = m->to;
		}
	}
  break;
  case SHORT_CASTLING:
	if( Color == WHITE )
	{
		B[E1]=WK; B[F1]=0; B[G1]=0; B[H1]=WR;
		L[E1] = L[G1]; WKP = L[L[E1].next].prev = E1;
		L[H1] = L[F1]; L[L[H1].prev].next = L[L[H1].next].prev = H1;
	}
	else
	{
		B[E8]=BK; B[F8]=0; B[G8]=0; B[H8]=BR;
		L[E8] = L[G8]; BKP = L[L[E8].next].prev = E8;
		L[H8] = L[F8]; L[L[H8].prev].next = L[L[H8].next].prev = H8;
	}
  break;
  case LONG_CASTLING:
	if( Color == WHITE )
	{
		B[A1]=WR; B[C1]=0; B[D1]=0; B[E1]=WK;
		L[E1] = L[C1]; WKP = L[L[E1].next].prev = E1;
		L[A1] = L[D1]; L[L[A1].prev].next = L[L[A1].next].prev = A1;
	}
	else
	{
		B[A8]=BR; B[C8]=0; B[D8]=0; B[E8]=BK;
		L[E8] = L[C8]; BKP = L[L[E8].next].prev = E8;
		L[A8] = L[D8]; L[L[A8].prev].next = L[L[A8].next].prev = A8;
	}
  break;
  default: /* en passant pawn capture */
	B[m->to] = 0;
	L[m->from] = L[m->to];
	L[ L[m->from].prev ].next = L[ L[m->from].next ].prev = m->from;
	if( Color ==  WHITE )
	{
		B[m->from] = WP;
		B[m->special] = BP; /* captured pawn */
		L[m->special].next = L[BKP].next;
		L[m->special].prev = BKP;
		L[L[BKP].next].prev = m->special;
		L[BKP].next = m->special;
	}
	else
	{
		B[m->from] = BP;
		B[m->special] = WP; /* captured pawn */
		L[m->special].next = L[WKP].next;
		L[m->special].prev = WKP;
		L[L[WKP].next].prev = m->special;
		L[WKP].next = m->special;
	}
}

#ifdef DEBUG_MLIST
checklist(m);
#endif

}



#ifdef ETTC
/**
***   Doing and undoing only hash keys. To be used for ETC
***   (enhanced transpostion cutoffs); see evaluate.c
**/

void do_hash(register tmove * m)
{

Counter++;
G[Counter].hashboard = HASH_COLOR ^ G[Counter-1].hashboard;

switch(m->special)
{
  case 0: /* most of the moves */

	update_hash( m->in1, m->from );
	update_hash( m->in2a, m->to );
	if( m->in2 != 0 ) /* capture */
		update_hash( m->in2, m->to );

	break;

  case SHORT_CASTLING:
	if( Color == WHITE )
	{
		update_hash( WK, E1 );
		update_hash( WK, G1 );
		update_hash( WR, H1 );
		update_hash( WR, F1 );
	}
	else
	{
		update_hash( BK, E8 );
		update_hash( BK, G8 );
		update_hash( BR, H8 );
		update_hash( BR, F8 );
	}
	break;
  case LONG_CASTLING:
	if( Color == WHITE )
	{
		update_hash( WK, E1 );
		update_hash( WK, C1 );
		update_hash( WR, A1 );
		update_hash( WR, D1 );
	}
	else
	{
		update_hash( BK, E8 );
		update_hash( BK, C8 );
		update_hash( BR, A8 );
		update_hash( BR, D8 );
	}
	break;
  default: /* en passant pawn capture */

	update_hash( m->in1, m->from );
	update_hash( m->in2a, m->to );
	update_hash( B[m->special], m->special );
}

Depth -= m->dch;

}



void undo_hash(register tmove * m)
{
Depth += m->dch;
Counter--;
}

#endif /* ETTC */

