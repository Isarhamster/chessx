#include "phalanx.h"

typedef struct { unsigned f, t; } killentry;
killentry RK[H9][H9]; /* reaction killer */

unsigned HK[H9][H9]; /* history killer table */
unsigned Maxhist = 1;


void init_killers(void)
{
memset( RK, 0, H9*H9*sizeof(killentry) );
memset( HK, 0, H9*H9*sizeof(unsigned) );
Maxhist = 1;
}



void write_killer( int from, int to )
{

if( B[to] == 0 )
{
	RK[G[Counter-1].m.from][G[Counter-1].m.to].f = from;
	RK[G[Counter-1].m.from][G[Counter-1].m.to].t = to;

	HK[from][to] += Depth/10 + 1;
	if( HK[from][to] > Maxhist )
	{
		Maxhist = HK[from][to];
		if( Maxhist > 16000 )
		{
			int f,t;
			Maxhist /= 2;
			for( f=A1; f!=H9; f++ )
			for( t=A1; t!=H9; t++ )
				HK[f][t] /= 2;
		}
	}
}

}



void add_killer( tmove *m, int n, thashentry *ht )
{

int i, f=0, t=0;
tmove *mpv = &PV[0][Ply];
tmove *mlv = &PV[Ply][Ply];

if( Depth > 0 )
{
	f = RK[G[Counter-1].m.from][G[Counter-1].m.to].f;
	t = RK[G[Counter-1].m.from][G[Counter-1].m.to].t;
}

for( i=0; i!=n; i++ )
{
	register tmove *m1 = m+i;

	if( FollowPV )
	{
	  if( m1->from == mpv->from
	    && m1->to == mpv->to
	    && m1->in2a == mpv->in2a ) m1->value = CHECKMATE;
	}
	else
	{
	  if( m1->from == mlv->from
	    && m1->to == mlv->to
	    && m1->in2a == mlv->in2a ) m1->value += 350;

	  if( ht != NULL ) if( ht->move != 0 )
	  if( ht->move == smove(m1) ) m1->value = CHECKMATE;
	}

	if( m1->value != CHECKMATE )
	{
		if( m1->in2 ) /* capture */
		{
			m1->value += Values[ m1->in2>>4 ];
			if( Color==WHITE )
			{ if( P[m1->to]&0xFF00 )
			  m1->value -= ( Values[ m1->in1>>4 ] >> 2 );
			  else m1->value += 100; }
			else
			{ if( P[m1->to]&0x00FF )
			  m1->value -= ( Values[ m1->in1>>4 ] >> 2 );
			  else m1->value += 100; }
			if( m1->value < 30 ) m1->value = 30;
		}

		/* leaving attacked square */
		if( Color==WHITE )
		{ if(P[m1->from]&0xFF00) m1->value += 50; }
		else
		{ if(P[m1->from]&0x00FF) m1->value += 50; }

		if( m->in1 != m->in2 ) /* pawn promotion */
			m->value += Values[ m->in2>>4 ] - Values[ m->in1>>4 ];

		if( Depth > 200 ) m1->value += see( B, m1->from, m1->to );
		else /* last moved piece */
		if( m1->to == G[Counter-1].m.to ) m1->value += 150;

		if( Depth > 0 )
		{
			if( m1->from == f && m1->to == t )
				m1->value += max(Depth,50);
			if( HK[ m1->from ][ m1->to ] )
			m1->value +=
			 1 + HK[ m1->from ][ m1->to ] * 500 / Maxhist;
		}

	}
}

}

