#include "phalanx.h"

tflag Flag;

tsquare B[120];         /* The chessboard */
tlist L[120];                  /* List of pieces */
/*** L[1].next ... white king position; L[2].next ... black king position */

tmove PV[MAXPLY][MAXPLY];      /* Principal var. */
tmove Pondermove;

tgamenode G[MAXCOUNTER];

int Counter;

int Color;
int LastIter, Depth, Ply, FollowPV, Totmat, Abort, NoAbort;
int DrawScore = -20;
long AllDepth = 0;
int64 AllNPS = 0;
int64 Nodes;

/* params that cannot be pushed via SIGALM handler */
int A_n, A_i, A_d;
tmove * A_m;

int N_moves[8] = { -21, -19, -12, -8, 21, 19, 12, 8 };
int RB_dirs[8] = { 1, -1, 10, -10, 11, -11, 9, -9 };

int Values[7] = { 0, P_VALUE, N_VALUE, B_VALUE, R_VALUE, Q_VALUE, 0 };


signed char Th_[80] =
{
  0, 91, 92, 93, 94, 95, 96, 97, 98,  0,
  0, 81, 82, 83, 84, 85, 86, 87, 88,  0,
  0, 71, 72, 73, 74, 75, 76, 77, 78,  0,
  0, 61, 62, 63, 64, 65, 66, 67, 68,  0,
  0, 51, 52, 53, 54, 55, 56, 57, 58,  0,
  0, 41, 42, 43, 44, 45, 46, 47, 48,  0,
  0, 31, 32, 33, 34, 35, 36, 37, 38,  0,
  0, 21, 22, 23, 24, 25, 26, 27, 28,  0
}; signed char *Th = Th_-20;

signed char Tv_[80] =
{
  0, 28, 27, 26, 25, 24, 23, 22, 21,  0,
  0, 38, 37, 36, 35, 34, 33, 32, 31,  0,
  0, 48, 47, 46, 45, 44, 43, 42, 41,  0,
  0, 58, 57, 56, 55, 54, 53, 52, 51,  0,
  0, 68, 67, 66, 65, 64, 63, 62, 61,  0,
  0, 78, 77, 76, 75, 74, 73, 72, 71,  0,
  0, 88, 87, 86, 85, 84, 83, 82, 81,  0,
  0, 98, 97, 96, 95, 94, 93, 92, 91,  0
}; signed char *Tv = Tv_-20;

int HP[100] = /* hash codes for pieces */
{
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0, /* PAWN */
  0,  3,  2,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0, /* KNIGHT */
  0,  5,  4,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0, /* BISHOP */
  0,  7,  6,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0, /* ROOK */
  0,  9,  8,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0, /* QUEEN */
  0, 11, 10,  0                                                   /* KING */
};

int HS_[80] = /* hash codes for squares */
{
  0,  0,  1,  2,  3,  4,  5,  6,  7,  0,
  0,  8,  9, 10, 11, 12, 13, 14, 15,  0,
  0, 16, 17, 18, 19, 20, 21, 22, 23,  0,
  0, 24, 25, 26, 27, 28, 29, 30, 31,  0,
  0, 32, 33, 34, 35, 36, 37, 38, 39,  0,
  0, 40, 41, 42, 43, 44, 45, 46, 47,  0,
  0, 48, 49, 50, 51, 52, 53, 54, 55,  0,
  0, 56, 57, 58, 59, 60, 61, 62, 63,  0
}; int * HS=HS_-20;


tdist dist[120*120];

int taxi_dist( int a, int b )
{ return abs( a%10 - b%10 ) + abs( a/10 - b/10 ); }

int diag_dist( int a, int b )
{
	int fdi = abs( a%10 - b%10 );
	int rdi = abs( a/10 - b/10 );
	return abs(rdi-fdi) + max(rdi,fdi);
}

int max_dist( int a, int b )
{
	int fdi = abs( a%10 - b%10 );
	int rdi = abs( a/10 - b/10 );
	return max(rdi,fdi);
}

int min_dist( int a, int b )
{
	int fdi = abs( a%10 - b%10 );
	int rdi = abs( a/10 - b/10 );
	return min(rdi,fdi);
}

void initdist(void)
{
	int i,j;
	for( i=A1; i!=H9; i++ ) for( j=A1; j!=H9; j++ )
	{
		dist[120*i+j].taxi = taxi_dist(i,j);
		dist[120*i+j].diag = diag_dist(i,j);
		dist[120*i+j].max = max_dist(i,j);
		dist[120*i+j].min = min_dist(i,j);
	}
}

