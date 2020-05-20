
#include "phalanx.h"



int onepawn( int wk, int wp, int bk, int wtm )
{
#define PWINS R_VALUE
#define PDRAWS (1-P_VALUE)

int wpfile = wp%10;
int target = wpfile + 90;
int steps = target/10 - wp/10 + ( wk>wp && (wk-wp)%10==0 ) - ( wp <= H2 );
tdist * tardist = dist+120*target;
int rookpawn;

/**
***  If black king is out of the square, it's won.
**/
if( (int)(tardist[bk].max) > steps-wtm+1 ) return PWINS-steps;

/**
***  Now, a draw rule: if black king is much closer to the pawn than
***  the white king, it must be a draw.  We already know it is in the
***  square.  Sometimes pawn can stop enemy's diagonal run, so let's
***  add a step.  With rook pawn it's even easier for the defending side
***  - let's add a step too.
**/
if(    dist[120*wp+bk].max + wtm + 1 + ( (bk-wp)%9==0 || (bk-wp)%11==0 )
    <=   dist[120*wp+wk].max
       + ( rookpawn = ( wpfile==FILE_A || wpfile==FILE_H ) )        )
return PDRAWS;

/**
***  Rook-pawn.
**/
if(rookpawn)
{
	if( tardist[bk].max+wtm-1 > tardist[wk].max && abs(wpfile-wk%10)==1 )
	return PWINS-P_VALUE-2*steps;
	return PDRAWS;
}

/**
***  Look at the critical squares, according to the endgames textbook
***  by IGM Ludek Pachman.
**/
if( wk-wp==19 || wk-wp==20 || wk-wp==21 ) return PWINS-4*steps;
if( wk-wp==9 || wk-wp==10 || wk-wp==11 ) if( wp>A5 || ( wtm != (bk-wk==20) ) )
return PWINS-8*steps;

/**
***  Some simple drawish knowledge - white cannot access critical square.
**/
if( wp<A7 && bk-wp==10 ) return PDRAWS;
if( wp<A6 )
{
	if( bk-wp==20 ) return PDRAWS;
	if( ( bk-wp==19 || bk-wp==21 ) && bk-wk==20 ) return PDRAWS;
}

/**
***  We still don't know :-(
***  Lets try to return something reasonable.
**/
{
	int d;
	int d1 = 120 * ( wp + ( wp<A5 ? 19 : 9 ) );
	int d2 = 120 * ( wp + ( wp<A5 ? 21 : 11 ) );
	if( dist[wk+d1].max < dist[wk+d2].max )
		d = wtm + dist[bk+d1].max - dist[wk+d1].max;
	else
	if( dist[wk+d1].max > dist[wk+d2].max )
		d = wtm + dist[bk+d2].max - dist[wk+d2].max;
	else
	if( dist[bk+d1].max < dist[bk+d2].max )
		d = wtm + dist[bk+d2].max - dist[wk+d2].max;
	else	d = wtm + dist[bk+d1].max - dist[wk+d1].max;

	if(d>0) return PWINS-P_VALUE-4*(steps-d);
	else
	if(d<0) return PDRAWS+20+2*d;

	return 32 * ( dist[(wp+10)*120+bk].taxi - dist[(wp+20)*120+wk].taxi );
}

}



	int Oppb[17] =  /* outside passed pawn bonus */
	/*       +10  +8  +6  +4             */
	{  0,  0, 40, 50, 58, 64, 68, 68,
	  68, 68, 68, 68, 68, 68, 68, 68, 68 };

int pawns(void)
{

extern int wpf[10];
extern int bpf[10];
int result;
int color;
int wbest=32, bbest=32;
static int passed[16] =
	{ 0, 100, 75, 50, 30, 10, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static int kbonus_[80] =
{
  0,  0,  1,  2,  3,  3,  2,  1,  0, 0,
  0, 10, 11, 12, 13, 13, 12, 11, 10, 0,
  0, 20, 21, 22, 23, 23, 22, 21, 20, 0,
  0, 30, 31, 32, 33, 33, 32, 31, 30, 0,
  0, 40, 41, 42, 43, 43, 42, 41, 40, 0,
  0, 50, 51, 52, 53, 53, 52, 51, 50, 0,
  0, 50, 51, 52, 53, 53, 52, 51, 50, 0,
  0, 42, 45, 47, 48, 48, 47, 45, 42, 0
}; const int * kbonus = kbonus_-20;

if( Totmat == P_VALUE )  /* KPK routine */
{
	int wk, wp, bk, wtm;

	if( Color==WHITE ) wtm=1; else wtm=0;

	if( L[WKP].next != 0 )
	{	wk = WKP; wp = L[WKP].next; bk = BKP;
		result = onepawn(wk,wp,bk,wtm);
	}
	else
	{	wk = Th[BKP]; wp = Th[L[BKP].next]; bk = Th[WKP];
		wtm = !wtm;
		result = -onepawn(wk,wp,bk,wtm);
	}

	if(Color==WHITE) return result; else return -result;
} 

#define bonus(x) ( color==WHITE ? (x) : -(x) )

result = kbonus[WKP]-kbonus[Th[BKP]];

for( color=1; color!=3; color++ )
{
	int step = ( color==WHITE ? 10 : -10 );
	int best = 32;
	unsigned pawn = PAWN+color;
	unsigned xpawn = PAWN+enemy(color);
	unsigned kp = L[color].next;
	unsigned xkp = L[enemy(color)].next;
	int * pf = ( color==WHITE ? wpf : bpf );
	int s;  /* square */
	int i;

	for( i=1; i!=9; i++ ) pf[i]=0;

	for( s = L[kp].next; s != 0; s = L[s].next )
	{
		int b, togo;
		int blocks = 0;
		int cannotmove;
		int protected;
		int stepstosquare;

		pf[s%10]++;

		/* doubled pawn */
		if( pf[s%10] > 1 ) result -= bonus(8);

		if( B[s+step] ) cannotmove = 1;
		else
		{
			int f=0;
			if( B[s+step+step+1] == xpawn ) f++;
			if( B[s+step+step-1] == xpawn ) f++;
			if(f)
			{
				if( B[s+1] == pawn ) f--;
				if( B[s-1] == pawn ) f--;
				cannotmove = (f>0);
			} else cannotmove=0;
		}

		if(cannotmove) result -= bonus(6);

		protected = ( B[s-step-1]==pawn || B[s-step+1]==pawn );

		/* lets make kings active */
		if( ! protected )
		{
			int di = dist[120*xkp+s].max;
			if(cannotmove) b = bonus( 32-4*di );
			else b = bonus( 16-2*di );
			if(di==1)
			{ if(cannotmove) b+=30; else b+=20; }

			result -= b;
		}

		if( B[s+step] == xpawn ) goto end_passed;
		for( i=s+step+step; B[i]!=3; i+=step )
		{
			if( B[i] ) blocks++;
			if( B[i]==xpawn || B[i-1]==xpawn || B[i+1]==xpawn )
				goto end_passed;
		}

		/*** BEGIN: passed pawn evaluation ***/

		i -= step;                  /* the queening square */
		togo = blocks+abs(s-i)/10;  /* steps to queen */
		b = bonus(passed[togo]);
		result += b;

		/* protected passer -> more bonus */
		if( protected ) result += b+b;

		/* out of enemy's king square */
		stepstosquare=(int)(dist[xkp*120+i].max)-togo+(Color==color);

		if( stepstosquare>1 )
		{
			result += b+b;
			if( togo < best ) best = togo;
		}

		/* distance from enemy king */
		b = dist[xkp*120+s+step].max * (12-togo);
		result += bonus(b);

		/*** END: passed pawn evaluation ***/

		end_passed:;

		/*** some points for advancement ***/
		result += (s/10-4-color)*2;
	}
	if( color == WHITE ) wbest=best; else bbest=best;
}

if( wbest!=32 )
  if( bbest!=32 )
    { if(      wbest-bbest > 2 ) result += -300 + (bbest-wbest)*60;
      else if( bbest-wbest > 2 ) result +=  300 + (bbest-wbest)*60;
      else                       result += (bbest-wbest)*40;
    }
  else
    result += (30-wbest)*20;
else
  if( bbest!=32 )
    result -= (30-bbest)*20;

/* a simple detection of outside passed pawn */
if( G[Counter].mtrl && G[Counter].xmtrl )    /* both sides must have pawns */
{
	int whas=0, bhas=0;

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

	result += Oppb[whas]-Oppb[bhas];
}

if( Color == BLACK ) result = -result;

/* consistency with trade down bonus: 5 points per pawn */
if( G[Counter].mtrl > G[Counter].xmtrl ) result += G[Counter].mtrl/20;
else
if( G[Counter].mtrl < G[Counter].xmtrl ) result -= G[Counter].xmtrl/20;

return result;

}



/* table for king bishop knight endings */
int nb_[80] =
{
	  0,100, 90, 80, 70, 60, 50, 40, 30,  0,
	  0, 90, 80, 70, 57, 45, 32, 20, 40,  0,
	  0, 80, 70, 50, 37, 23, 10, 32, 50,  0,
	  0, 70, 57, 37, 10,  0, 23, 45, 60,  0,
	  0, 60, 45, 23,  0, 10, 37, 57, 70,  0,
	  0, 50, 32, 10, 23, 37, 50, 70, 80,  0,
	  0, 40, 20, 32, 45, 57, 70, 80, 90,  0,
	  0, 30, 40, 50, 60, 70, 80, 90,100,  0
}; int * nb = nb_-20;



int e_mp(void)  /*** minor piece and pawn ***/
{

if( G[Counter].mtrl && G[Counter].xmtrl )
{	/*** minor piece against pawn ***/
	int p;
	if( G[Counter].mtrl == P_VALUE )
	{
		if( Color == WHITE ) p = L[WKP].next/10;
		else p = 11 - L[BKP].next/10;
	}
	else
	{
		if( Color == WHITE ) p = - ( 11 - L[BKP].next/10 );
		else p = - L[WKP].next/10;
	}
	return G[Counter].xmtrl - G[Counter].mtrl + 3*p;
}
else
{	/*** check for bad bishop ***/
	int ss;  /* stronger side */
	int wb, wp, wk, bk;
	extern signed char * sqcolor;

	if( L[WKP].next )
	{
		wk=WKP;
		if( B[L[wk].next]==WP ) { wp=L[wk].next; wb=L[wp].next; }
		else                    { wb=L[wk].next; wp=L[wb].next; }
		if( B[wb] == WN )
		{
			if(   ( B[A7]==WP && ( B[A8]==BK || B[B7]==BK ) )
			    ||( B[H7]==WP && ( B[H8]==BK || B[G7]==BK ) )
			)    return   G[Counter].xmtrl - G[Counter].mtrl;
			else return   0;
		}
		bk=BKP; ss=WHITE;
	}
	else
	{
		wk=BKP;
		if( B[L[wk].next]==BP ) { wp=L[wk].next; wb=L[wp].next; }
		else                    { wb=L[wk].next; wp=L[wb].next; }
		bk=WKP; ss=BLACK;
		if( B[wb] == BN )
		{
			if(   ( B[A2]==BP && ( B[A1]==WK || B[B2]==WK ) )
			    ||( B[H2]==BP && ( B[H1]==WK || B[G2]==WK ) )
			)    return   G[Counter].xmtrl - G[Counter].mtrl;
			else return   0;
		}
		wk=Th[wk]; wp=Th[wp]; wb=Th[wb]; bk=Th[bk];
	}

	switch( wp%10 )
	{ case FILE_A: break;
	  case FILE_H: wp=Tv[wp]; wk=Tv[wk]; wb=Tv[wb]; bk=Tv[bk]; break;
	  default: return 0; /* this is not a rook pawn */
	}

	if( sqcolor[wb] == 1 ) return 0;  /* the bishop is ok */

	/* Draw!! */
	if( bk > wp )
	if( bk%10<FILE_C && bk>=A7 )
	{
		return   G[Counter].xmtrl - G[Counter].mtrl;
	}
}

return 0;

}



int e_nb( int color )  /*** mating with knight and bishop ***/
{
	int kdist = 8 * dist[WKP*120+BKP].max;
	int kp=A1;
	int i, npd=0;
	extern signed char * sqcolor;

	if( color == WHITE )
	{
		for( i=L[WKP].next; i!=0; i=L[i].next )
		if( B[i] == WN ) npd=2*dist[120*i+BKP].taxi;
		else if( B[i] == WB )
		{
		     if( sqcolor[i] == 1 ) kp = Th[BKP]; else kp = BKP;
		}
	}
	else
	{
		for( i=L[BKP].next; i!=0; i=L[i].next )
		if( B[i] == BN ) npd=2*dist[120*i+WKP].taxi;
		else if( B[i] == BB )
		{
		     if( sqcolor[i] == 1 ) kp = Th[WKP]; else kp = WKP;
		}
	}

	if( color == Color ) return -kdist-npd+nb[kp];
	                else return  kdist+npd-nb[kp];
}



int e_rpr(void)
{
	int wk, wr, wp, bk, br;
	int result = 0;
	int sstm;    /* it's stronger's side move */

	if( G[Counter].mtrl > G[Counter].xmtrl )
	{ sstm=1; wk=L[Color].next; bk=L[enemy(Color)].next; br=L[bk].next; }
	else
	{ sstm=0; wk=L[enemy(Color)].next; bk=L[Color].next; br=L[bk].next; }

	if( piece(B[L[wk].next]) == ROOK )
	{ wr=L[wk].next; wp=L[wr].next; }
	else
	{ wp=L[wk].next; wr=L[wp].next; }

	if( B[bk] == WK )
	{ wk=Th[wk]; wr=Th[wr]; wp=Th[wp]; bk=Th[bk]; br=Th[br]; }

	if( wp%10 > FILE_D )
	{ wk=Tv[wk]; wr=Tv[wr]; wp=Tv[wp]; bk=Tv[bk]; br=Tv[br]; }

	result = wp/10-5;
	if( wp%10 == FILE_A ) result -= 15;

	result -= 2*dist[br*120+wp].max + 3*abs(br%10-wp%10);

	if( bk > wp )
	switch( wp%10-bk%10 )
	{ case -2: break;
	  case -1: result -= 20; break;
	  case  0: result -= 50; break;
	  case  1: result -= 40; break;
	  case  2: if( br%10-wp%10 > 3 ) result -= 35;
	                            else result -= abs(br%10-wp%10)*3;
	  break;
	  default: result += 10*abs(wp%10-bk%10);
	}
	else result += 5*(wp/10-bk/10);

	if( wk < wp ) result -= 5*(wp/10-wk/10);

	if( br<wp && (br-wp)%10 == 0 ) { result -= 20; }

	if(sstm) return result; else return -result;
}

