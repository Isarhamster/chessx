
#include "phalanx.h"

extern long Time;


char Inp[256] = "\0";
char piece[7] =
 { ' ', 'P', 'N', 'B', 'R', 'Q', 'K' };
char file[10] =
 { '<', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', '>' };
char row[12] =
 { '<', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '>' };

tmove Pm[256];
int Pn;


/**
*** Init board and structures
*** To be called after position is set (FEN or xboard's edit)
**/
void initbs(void)
{

int s;

Counter = 0;
DrawScore = 0;

G[0].hashboard = hashboard();

/* Do full material count */
G[0].mtrl = G[0].xmtrl = 0;
for( s=A1; s!=H9; s++ )
  if( color(B[s]) == Color ) G[0].mtrl += Values[ B[s]>>4 ];
  else G[0].xmtrl += Values[ B[s]>>4 ];

/* busted! S.A.
G[0].castling = 0;
if( B[E1]!=WK || B[A1]!=WR ) G[0].castling |= WLONG;
if( B[E1]!=WK || B[H1]!=WR ) G[0].castling |= WSHORT;
if( B[E8]!=BK || B[A8]!=BR ) G[0].castling |= BLONG;
if( B[E8]!=BK || B[H8]!=BR ) G[0].castling |= BSHORT;
*/

G[0].rule50 = 0;

/* create the piece list */
L[L[1].next].prev = 1;
L[L[2].next].prev = 2;
L[L[1].next].next = L[L[2].next].next = 0;
{
	int wlast = L[1].next, blast = L[2].next;
	for( s=A1; s!=H9; s++ )
	if( B[s]!=0 && B[s]!=3 && B[s]!=WK && B[s]!=BK )
	{
	if( color(B[s])==WHITE )
	{ L[wlast].next = s; L[s].prev = wlast; L[s].next = 0; wlast = s; }
	else
	{ L[blast].next = s; L[s].prev = blast; L[s].next = 0; blast = s; }
	}
}

Flag.machine_color = enemy(Color);

}



/**
*** Xboard compatible edit
**/
void edit(void)
{
int i;
int color = WHITE;
int squ, pie;
char s[8];
s[0] = '@';

for(;;)
{
	for( i=0; i!=3; i++ ) s[i] = toupper(s[i]);

	switch(*s)
	{
	case '#':
		for(i=A1;i!=H9;i++) if(B[i]!=3) B[i]=0;
	break;
	case 'C':
		color = enemy(color);
	break;
	case 'S':
		Color = enemy(Color);
	break;
	case '.': break;
	case 'P': case 'N': case 'B': case 'R': case 'Q': case 'K':
		squ = s[1]-'A' + 10*(s[2]-'1') + A1;
		if( squ<A1 || squ>H8 ) squ=0;
		pie = 0;
		for( i=1; i!=7; i++ ) if( s[0] == piece[i] )
		{ pie = i*16+color; break; }
		B[squ]=pie;
		if( *s == 'K' ) L[color].next = squ;
	break;
	}

	printboard(NULL); printf("\n");
	printf("       <piece><location>\n");
	printf("       .            exit\n");
	printf("       #           clear\n");
	printf("       c          change          ");
	if( color==WHITE ) printf("put white pieces\n");
	else printf("put black pieces\n");
	printf("       s    side to move\n");
	printf("edit > ");

	if( fgets(s,7,stdin) == NULL )
	{ strcpy(Inp,"quit\n"); return; }

	if( *s == '.' ) break;
}

initbs();
Inp[0]='\0'; return;

}



/**
*** Print move in full notation (Ng1-f3) to string 's' or to stdout.
*** SAN would be more complicated since we need a list of all possible
*** moves to find out whether to print 'Nbd2' or 'Nd2'.
**/
void printm( tmove m, char *s )
{

char ss[64];

switch( m.special )
{
  case LONG_CASTLING:  sprintf( ss, "O-O-O  " ); goto endprint;
  case SHORT_CASTLING: sprintf( ss, "O-O  " ); goto endprint;
}

sprintf( ss, "%c%c%c%c%c%c", piece[m.in1>>4],
	file[m.from%10], row[m.from/10],
	( m.in2 || m.special ) ? 'x' : '-',
	file[m.to%10], row[m.to/10] );

if( m.in2a != m.in1 ) sprintf( ss+strlen(ss), "%c", piece[m.in2a>>4] );

#ifdef SHOW_FORCED_MOVES
if( m.dch != 100 ) sprintf( ss+strlen(ss), "/%i",m.dch );
#endif

sprintf( ss+strlen(ss), "  " );

endprint:;

if( s == NULL ) printf(ss); else strcpy( s, ss );

}



#ifdef SANOUT

void printmSAN( tmove *m, int i, int n, char *s )
{
char ss[8];  /* longest: "Nb1xd2+\0" */
int j;
tmove *mi = m+i;

switch( mi->special )
{
  case LONG_CASTLING:  sprintf( ss, "O-O-O" ); goto endprint;
  case SHORT_CASTLING: sprintf( ss, "O-O" ); goto endprint;
}

if( piece(mi->in1) == PAWN )
{
	sprintf( ss, "%c", file[mi->from%10] );
	if( mi->in2 != 0 || mi->special )
		sprintf( ss+strlen(ss),
			"x%c%c",
			file[mi->to%10], row[mi->to/10] );
	else sprintf( ss+strlen(ss), "%c", row[mi->to/10] );
	if( mi->in1 != mi->in2a )
		sprintf( ss+strlen(ss), "=%c", piece[mi->in2a>>4] );
	goto endprint;
}

/* print NBRQ moves */
sprintf( ss, "%c", piece[mi->in1>>4] );

for( j=0; j!=n; j++ )
if( j!=i && mi->in1 == m[j].in1 && mi->to == m[j].to
 && mi->from%10 != m[j].from%10 )
	sprintf( ss+strlen(ss), "%c", file[mi->from%10] );

for( j=0; j!=n; j++ )
if( j!=i && mi->in1 == m[j].in1 && mi->to == m[j].to
 && mi->from%10 == m[j].from%10 )
	sprintf( ss+strlen(ss), "%c", row[mi->from/10] );

if( mi->in2 != 0 || mi->special ) sprintf( ss+strlen(ss), "x" );

sprintf( ss+strlen(ss), "%c%c", file[mi->to%10], row[mi->to/10] );

endprint:;

/* is this a checking move? */
do_move( m+i );
if( checktest(Color) ) sprintf( ss+strlen(ss), "+" );
undo_move( m+i );

if( s == NULL ) printf(ss); else strcpy( s, ss );

}



void pvSAN( tmove *v, char *s )
{
int n;
tmove m[256];
int i;

generate_legal_moves( m, &n, checktest(Color) );

for( i=0; i!=n; i++ )
if( smove(v) == smove(m+i) ) break;

if( i != n )
{
	printmSAN( m, i, n, s );
	do_move( m+i );
	v++;
	if( v->from )
	{
		sprintf( s+strlen(s), " " );
		pvSAN( v, s+strlen(s) );
	}
	undo_move( m+i );
}

}

#endif



/* GNU Chess `X' understands only moves in `g1f3' notation. We have to send
 * this to older versions of xboard when playing phalanx with gnu. */
void gnuprintm(tmove m)
{

switch( m.special )
{
  case LONG_CASTLING:  printf("o-o-o"); return;
  case SHORT_CASTLING: printf("o-o"); return;
}

printf( "%c%c%c%c%c",
	file[m.from%10], row[m.from/10],
	file[m.to%10], row[m.to/10],
	m.in2a == m.in1 ? ' ' : tolower(piece[m.in2a>>4]) );

}



/**
*** Return pointer to move in m[] if it matches the SAN representation.
*** If there is no such move, return NULL.
**/
tmove * sandex( char *inp, tmove *m, int n )
{
	char bufik[32];
	char *s = bufik;
	char * ss; char * sto;
	int move,to;
	int i;
	unsigned p=0;
	int frow=0, ffile=0;
	int in2a=0;
	int color = color(m->in1);
	int sc=-1; int lc=-1;

	strncpy(bufik,inp,30);

	for( i=0; i!=n; i++ ) if(m[i].special==SHORT_CASTLING) sc=i;
	else if(m[i].special==LONG_CASTLING) lc=i;

	/*** step 0: castlings ***/
	if( lc != -1 )
	if(  strncmp(s,"o-o-o",5)==0
	  || strncmp(s,"O-O-O",5)==0
	  || strncmp(s,"0-0-0",5)==0
	  || ( strncmp(s,"e1c1",4)==0 && color==WHITE )
	  || ( strncmp(s,"e8c8",4)==0 && color==BLACK )
	  ) return m+lc;

	if( sc != -1 && s[3]!='-' )
	if(  strncmp(s,"o-o",3)==0
	  || strncmp(s,"O-O",3)==0
	  || strncmp(s,"0-0",3)==0
	  || ( strncmp(s,"e1g1",4)==0 && color==WHITE )
	  || ( strncmp(s,"e8g8",4)==0 && color==BLACK )
	  ) return m+sc;

	/*** step 1: noncastling move must start with PNBRQKabcdefgh ***/
	/***         if it starts with pnrqk, do the letter upercase ***/
	{
		static char ok[] = "PNBRQKabcdefghpnrqk";
		static char up[] = "pnrqk";
		for( i=0; ok[i]!=*s; i++ ) if( ok[i] == '\0' ) return(NULL);
		for( i=0; up[i]!='\0'; i++ )
			if( up[i] == *s ) { *s = toupper(*s); break; }
	}

	/*** step 2: find the destination square ***/
	/* Find the last digit <1-8> in the input string.  It must
	 * be the row of the destination square and must be preceded
	 * by letter <a-h> of the dest. square **********************/
	sto=NULL;
	for( ss=s+1; *ss!='\0'; ss++ ) if( *ss>='1' && *ss<='8' ) sto = ss-1;
	if( sto == NULL )
	{
		if(    ( s[2]=='\0' || s[2]=='\n' )
		    && s[1]>='a' && s[1]<='h'
		    && s[0]>='a' && s[0]<='h' )
		{ /* exception: pawn capture can be typed as 'ef' */
			int mn=-1;
/* printf("trying exception %c %c %c\n", s[0], s[1], s[2] ); */
			for( i=0; i!=n; i++ )
				if( piece(m[i].in1) == PAWN )
					if( m[i].from%10 == s[0]+1-'a' )
						if( m[i].to%10 == s[1]+1-'a' )
						{
							if( mn==-1 )
								mn=i;
							else
								mn=-2;
						}
			if( mn>=0 ) return m+mn;
		}
		return NULL;  /* no digit 1-8 found */
	}

	for( i=1; i!=9; i++ ) if(file[i]==*sto) break;
	if(i==9) return NULL;
	to = i;
	for( i=2; i!=10; i++ ) if(row[i]==sto[1]) break;
	if(i==10) return NULL;
	to += i*10;

	/*** step 3: find the origin square ***/
	/* 3a - try to find full description of origin square (g1f3) */
	ss=sto-1;
	if(ss>=s)
	for( ss=sto-1; ss!=s; ss-- )
	if( *ss>='1' && *ss<='8' && *(ss-1)>='a' && *(ss-1)<='h' )
	{
		for( i=2; i!=10; i++ ) if(row[i]==*ss) break;
		frow = i;
		ss--; for( i=1; i!=9; i++ ) if(file[i]==*ss) break;
		ffile = i;
		for(i=0;i!=n;i++) if(m[i].from==frow*10+ffile)
		{ p=m[i].in1; break; }
		goto step4;
	}
	/* 3b - origin square must be computed from piece */
	{
		for(p=6;p!=1;p--) if( piece[p] == *s ) { s++; break; }
		p = (p<<4) + color;
		if( *s!='x' && s<sto ) /* init frow or ffile */
		{
			if( *s>='a' && *s<='h' ) ffile = *s-'a'+1;
			else
			if( *s>='1' && *s<='8' ) frow = *s-'1'+2;
		}

	}

	step4:;
	/*** step 4: is it a pawn promotion? determine piece ***/
	in2a = p;
	sto += 2; if( *sto == '=' ) sto++;
	*sto = toupper(*sto);
	for(i=2;i!=7;i++) if( piece[i]==*sto ) in2a=(i<<4)+color;

	/*** step 5: scan move list ***/
	move = -1;
	for(i=0;i!=n;i++) if( m[i].to==to && m[i].in1==p && m[i].in2a==in2a )
	if( ffile==0 || ffile==m[i].from%10 )
	if( frow==0  ||  frow==m[i].from/10 )
	{ if(move==-1) move=i; else return NULL; }
	if(move==-1) return NULL;

	return m+move;
}



/**
*** Is the string represented move ("Pe2-e4") same as the tmove move?
*** This correctly parses almost anything but not SAN - we don't have
*** full move list.
**/
int checkmove( char *s, tmove *m )
{
	int i;

	if( m->special == LONG_CASTLING )
	{
		if( strncmp(s,"o-o-o",5)==0
		  || strncmp(s,"O-O-O",5)==0
		  || strncmp(s,"0-0-0",5)==0
		  || ( Color == WHITE && strncmp(s,"e1c1",4) == 0 )
		  || ( Color == BLACK && strncmp(s,"e8c8",4) == 0 )
		  ) return 1;
		else return 0;
	}

	if( m->special == SHORT_CASTLING )
	{
		if(  ( strncmp(s,"o-o",3)==0 && strncmp(s,"o-o-o",5)!=0 )
		  || ( strncmp(s,"O-O",3)==0 && strncmp(s,"O-O-O",5)!=0 )
		  || ( strncmp(s,"0-0",3)==0 && strncmp(s,"0-0-0",5)!=0 )
		  || ( Color == WHITE && strncmp(s,"e1g1",4) == 0 )
		  || ( Color == BLACK && strncmp(s,"e8g8",4) == 0 )
		  ) return 1;
		else return 0;
	}

	for( i=0; s[i]!='\0' && ! islower(s[i]); i++ );
	if( s[i]=='\0' || s[i]!=file[m->from%10] ) return 0;

	i++;
	if( s[i]=='\0' || s[i]!=row[m->from/10] ) return 0;

	for(    ; s[i]!='\0' && ( s[i]=='x' || ! islower(s[i]) ); i++ );
	if( s[i]=='\0' || s[i]!=file[m->to%10] ) return 0;

	i++;
	if( s[i]=='\0' || s[i]!=row[m->to/10] ) return 0;

	i++;
	if( m->in2a != m->in1 )
	{
		if( s[i] == '=' || s[i] == ':' ) i++;
		if( toupper(s[i]) != piece[m->in2a>>4] ) return 0;
	}

	return 1;
}



void printPV( int mpl, int lid, char *s )
/* mpl ... moves per line */
{
	char ss[1024] = "";
	int j;

#ifdef SHOW_FORCED_MOVES
	if( mpl>2 ) mpl -= 2;
#endif

#ifdef SANOUT
	int k=0;
	int l=lid;
	char s1[1024] = "";
	pvSAN( PV[0], s1 );
	for( j=0; s1[j]!='\0'; j++,l++ )
	if( l <= 72 || s1[j] != ' ' ) { ss[k] = s1[j]; k++; }
	else
	{
		ss[k] = '\n'; k++;
		for( l=0; l!=lid; l++ ) { ss[k] = ' '; k++; }
	}
	ss[k] = '\0';
#else
	for(j=0;PV[0][j].from;j++)
	{
		if( j%mpl == 0 && j != 0 && Flag.xboard!=2 )
		{
			int i;
			sprintf( ss+strlen(ss), "\n" );
			for( i=0; i!=lid; i++ ) sprintf( ss+strlen(ss), " " );
		}
		printm( PV[0][j], ss+strlen(ss) );
	}

	if( j < 3 ) sprintf( ss+strlen(ss), "           " );
#endif

	sprintf( ss+strlen(ss), "\n" );

	if( s == NULL ) printf(ss); else strcpy(s,ss);
}



void infoline( int typ, char *s )
{
/** typ:
***  0 ... search finished, summary information
***  1 ... normal 'xboard' line, known eval, time, full PV
***  2 ... '!' - turn, new best move
***  3 ... summary informations at the end of ply level, no PV
***  4 ... '??' - first move fails low
***  5 ... '!!' - current move fails high, over window
**/

char ss[1024];
extern long T1;
long t = ptime();

if( typ!=0 && !Flag.analyze && ( Abort || t-T1 < Flag.noise ) ) return;

if( Flag.log && s==NULL )
{
	int x = Flag.xboard;
	Flag.xboard = 0;
	infoline(typ,ss);
	Flag.xboard = x;
	fprintf(Flag.log, ss);
}

if( Flag.xboard == 2 )
switch( typ )
{	case 0: /* ICS whispering */
		if( Flag.ponder<2 && Flag.xboard>1 && typ==0 )
		{
		  printf(
"tellics whisper depth=%i; eval=%c%i.%02i; nodes=%lli; nps=%i\n",
A_d,
PV[0][0].value>=0 ? '+' : '-',
abs(PV[0][0].value/100),
abs(PV[0][0].value)%100,
Nodes,
(int)( ((float)Nodes) / (((float)max(t-T1,1))/100) ) );
/*
		  printPV(8,5,ss);
		  printf("tellics whisper %s",ss);
*/
		} 
	return;
	case 3: typ=1; break;
}

if( typ >= 1 && typ <= 5 )
{
	sprintf( ss, "%3i", A_d );
	if( typ==1 || typ==2 || typ==4 || typ == 5 )
		sprintf( ss+strlen(ss), "%7i", PV[0][0].value );
	else if( typ == 3 ) sprintf( ss+strlen(ss), " -> " );

	if( typ == 3 )
	{
		long tt = (long) (ptime()-T1);
		long t = tt / 100;
		sprintf( ss+strlen(ss), "%3i:%02i.%02li",
		        (int)t/60, (int)t%60, tt*100/100-t*100 );
	}
	else
	{
		sprintf( ss+strlen(ss), " %5li", (long)(t-T1) );
	}
	sprintf( ss+strlen(ss), " %8lli  ", Nodes );
}
else
{
	sprintf( ss,
		"Depth=%i, Value=%i, Time=%i.%02i, Last turn=%i.%02i, Nodes=%i",
		(int) A_d,
		(int) PV[0][0].value,
		(int) (t-T1)/100,
		(int) (t-T1)%100,
		(int) (LastTurn-T1)/100,
		(int) (LastTurn-T1)%100,
		(int) Nodes );
	if( t-T1 != 0 )
	sprintf( ss+strlen(ss), ", N/s=%i\n",
		(int)
		( (float) Nodes / ( (float)(t-T1)/(float)100 ) )
	      );
	else sprintf( ss+strlen(ss), "\n" );
}

switch( typ )
{
case 0:
	sprintf( ss+strlen(ss), "PV = " ); printPV(8,5,ss+strlen(ss));
	if( PV[0][0].value > CHECKMATE-100 )
	sprintf( ss+strlen(ss),
		"     Mate in %i\n", (CHECKMATE-PV[0][0].value+1) / 2 );
break;
case 1: printPV(6,27,ss+strlen(ss));
break;
case 2:
	printm( PV[0][0], ss+strlen(ss) );
	sprintf( ss+strlen(ss), "!       \n" );
break;
case 3: sprintf( ss+strlen(ss), "%2i turn", Turns );
	if(Turns==1) sprintf( ss+strlen(ss), "                          \n");
	else         sprintf( ss+strlen(ss), "s                         \n");
break;
case 4: printm( PV[0][0], ss+strlen(ss) );
	sprintf( ss+strlen(ss), "??      \n" );
break;
case 5: printm( PV[0][0], ss+strlen(ss) );
	sprintf( ss+strlen(ss), "!!      \n" );
}

if( s != NULL ) strcpy( s, ss );
else
{
	printf(ss);
}

}



void verboseline( tmove* m, int i, int n )
{
	char s[256];
	int j;
	extern long T1;
	long t = (long) (ptime()-T1) / 100;

	sprintf( s, "(%2i)", A_d );
	sprintf( s+strlen(s), "   ");
	sprintf( s+strlen(s), "%3li:%02li   ", t/60, t%60 );
	sprintf( s+strlen(s), "%9lli  ", Nodes );
	sprintf( s+strlen(s), "(%2i/%2i) ", i+1, n );
	printm( m[i], s+strlen(s) );
	sprintf( s+strlen(s), "     " );
	for( j=0; j!=79; j++ ) sprintf( s+strlen(s), "" );

	printf(s);
}



void printboard(char *s)
{
	int i;
	char ss[2048];

	sprintf(ss, "  +---+---+---+---+---+---+---+---+\n  ");
	for(i=A8;i>=A1;i++)
	{
		switch(color(B[i]))
		{
		case WHITE:
			sprintf(ss+strlen(ss),"| %c ",piece[B[i]>>4]);
		break;
		case BLACK:
			sprintf(ss+strlen(ss),"| *%c",piece[B[i]>>4]);
		break;
		default: sprintf(ss+strlen(ss),"|   ");
		}
		if( i%10 == 8 )
		{
			i-=18;
			sprintf(ss+strlen(ss),
				"|\n  +---+---+---+---+---+---+---+---+");
			if(i!=10) sprintf(ss+strlen(ss),"\n  ");
		}
	}
	if( Color == WHITE ) sprintf(ss+strlen(ss),"   White to move\n");
	else sprintf(ss+strlen(ss),"   Black to move\n");

	if( s==NULL ) printf(ss); else strcpy(s,ss);

	/* printf("%08X\n",G[Counter].hashboard); */
}



/**
***  The setfen() function is one of the two ways to set a position.
***  The second one is edit().
**/
int setfen( char *f )
{

int s;

if( Flag.log != NULL )
{
	fprintf(Flag.log,"\n\nsetting position\n");
	fprintf(Flag.log,f);
	fprintf(Flag.log,"\n\n");
}

for( s=0; s!=120; s++ )
if( s>H8 || s<A1 || s%10==0 || s%10==9 ) B[s] = 3; else B[s] = 0;

for( s=A8; s>=A1; f++ )
{
	if( *f == '\0' )
	{ puts("error: unexpected end of fen line"); return 1; }

	if( isdigit(*f) ) /* skip squares */
	{ s += *f - ('1'-1); continue; }

	switch( *f )
	{
	case 'K': B[s] = WK; L[1].next=s; break;
	case 'k': B[s] = BK; L[2].next=s; break;
	case 'Q': B[s] = WQ; break;
	case 'q': B[s] = BQ; break;
	case 'R': B[s] = WR; break;
	case 'r': B[s] = BR; break;
	case 'B': B[s] = WB; break;
	case 'b': B[s] = BB; break;
	case 'N': B[s] = WN; break;
	case 'n': B[s] = BN; break;
	case 'P': B[s] = WP; break;
	case 'p': B[s] = BP; break;
	case '/': case ' ': case ';': s -= s%10 + 10; break;
	default: puts("error in fen line"); return 1;
	}

	s++;
}

switch( *f )
{
	case 'w': Color = WHITE; break;
	case 'W': Color = WHITE; break;
	case 'b': Color = BLACK; break;
	case 'B': Color = BLACK; break;
	default:
		puts("error: side to move must be either 'w' or 'b'");
		return 1;
}

// set up castling

// f++;

G[0].castling = 0;
if  (strchr(f, 'K')==NULL) G[0].castling |= WSHORT;
if  (strchr(f, 'Q')==NULL) G[0].castling |= WLONG;
if  (strchr(f, 'k')==NULL) G[0].castling |= BSHORT;
if  (strchr(f, 'q')==NULL) G[0].castling |= BLONG;
// #define WSHORT 1 /* if set, white short castling is impossible */

//
// en-passant

// Sometime soon S.A.

initbs();

return 0;

}



/**
*** If an entered string contains seven (or more) slashes
*** it is considered as fen test position.
**/
int sevenslashes(register char *s)
{
	register int n=7;
	while( *s!='\0' && *s!=' ' )
	{
		if(*s=='/') { n--; if(!n) return 1; }
		s++;
	}
	return 0;
}



/**
*** Is the current position terminal?
*** returns: 0 ... not terminal
***          1 ... draw, 50 moves or 3rd repetition; ok to continue play
***          2 ... draw, stalemate
***          3 ... checkmate
**/
int terminal(void)
{

tmove m[256];
int n, c;

if( G[Counter].rule50>=100 || repetition(2) || material_draw() )
{ if(Flag.machine_color==(WHITE|BLACK)) Flag.machine_color=0;  return 1; }

generate_legal_moves( m, &n, c=checktest(Color) );

if( n!=0 ) return 0;

if(c) return 3;
else return 2;

}

void printpositionfen(void)
{

int s, empty_counter=0, Castling=0;

#define EMPTY_COUNTER if (empty_counter) { printf("%d", empty_counter); empty_counter=0; };

for( s=A8; s>=A1; s++ )
{
	switch( B[s] )
	{
	case WK: EMPTY_COUNTER; printf("K"); break;
	case BK: EMPTY_COUNTER; printf("k"); break;
	case WQ: EMPTY_COUNTER; printf("Q"); break;
	case BQ: EMPTY_COUNTER; printf("q"); break;
	case WR: EMPTY_COUNTER; printf("R"); break;
	case BR: EMPTY_COUNTER; printf("r"); break;
	case WB: EMPTY_COUNTER; printf("B"); break;
	case BB: EMPTY_COUNTER; printf("b"); break;
	case WN: EMPTY_COUNTER; printf("N"); break;
	case BN: EMPTY_COUNTER; printf("n"); break;
	case WP: EMPTY_COUNTER; printf("P"); break;
	case BP: EMPTY_COUNTER; printf("p"); break;

        default: empty_counter++; break;
	}

	if( s%10 == 8 )
	{
		EMPTY_COUNTER;
		s-=18;
		if (s!=H1-18) printf("/"); else printf(" ");
	}
}

if( Color == WHITE ) printf("w "); else printf("b ");

	/****** White Short ******/
	if( ( G[Counter].castling & WSHORT ) == 0 )
		{ printf("K"); Castling++;}

	/****** White Long ******/
	if( ( G[Counter].castling & WLONG ) == 0 )
                { printf("Q"); Castling++;}

	/****** Black Short ******/
	if( ( G[Counter].castling & BSHORT ) == 0 )
                { printf("k"); Castling++;}
	/****** Black Long ******/
	if( ( G[Counter].castling & BLONG ) == 0 )
                { printf("q"); Castling++;}

if (!Castling) printf("- "); else printf(" ");

if( Counter != 0 )
{
	int c = Counter-1;
	if( piece(G[c].m.in1) == PAWN  &&  abs(G[c].m.from-G[c].m.to) == 20 )
	/* The last move was a pawn double step */
	{
		int destination;
		int special = G[c].m.to;
		
		destination = Color==WHITE ? special+10 : special-10;
		printf("%c%c ", file[destination%10], row[destination/10]); 
	}
	else
		printf("- ");
}
else printf("- ");

printf("%d %d\n", G[Counter].rule50, Counter/2+1);

}

void about(void)
{

printf(" Phalanx "); puts(VERSION);
puts(" Copyright (C) Dusan Dobes, 1997-2000");

printf(" Level ................. ");
switch( Flag.level )
{
case averagetime:
	printf( "average time %i seconds\n", Flag.centiseconds/100 );
break;
case timecontrol:
	printf( "%i moves in %i minutes\n",
	       Flag.moves, Flag.centiseconds/6000 );
break;
case fixeddepth:
	printf( "%i search depth\n", Flag.depth/100 );
break;
case fixedtime:
	printf( "fixed time %i seconds\n", Flag.centiseconds/100 );
}

printf(" Time .................. ");
if( Flag.cpu ) puts("cpu"); else puts("elapsed");

printf(" Book .................. ");
if( Flag.book ) puts("on"); else puts("off");

printf(" Learning .............. ");
if( Flag.learn ) puts("on"); else puts("off");

printf(" Permanent brain ....... ");
switch( Flag.ponder )
{	case 0: puts("off"); break;
	case 1: puts("on"); break;
	case 2: puts("on (pondering)"); break;
	default:
		break;
		/* 2005-09-14, José de Paula
		 * GCC 3.4 thinks that this "default:" without a
		 * statement is an error, so I put a break here.
		 */
}

printf(" Transposition table ... ");
if( SizeHT == 0 ) puts("not used");
else
{
	printf("hashing %i positions in %lu bytes\n",
	SizeHT, (unsigned long)(sizeof(thashentry)*SizeHT) );
}

if( Flag.easy )
{
printf(" Easy level ............ %i\n",Flag.easy);
}

printf(" Resigning ............. ");
if( Flag.resign ) printf("%i.%02i\n",Flag.resign/100,Flag.resign%100);
else printf("off\n");

printf("\n");

}



/* SIG_INT handler */
void interrupt(int x)
{
	int c;
	signal(SIGINT,SIG_IGN);

	if( Flag.polling )
	{
	/* ignore lines that begin with '.' */
	c=getc(stdin); ungetc(c,stdin);
	if( c=='.' ) { fgets(Inp,255,stdin); goto go_on; }
	}

	if( Flag.ponder < 2 )
	{
		puts("interrupted");
		Abort = 1;
		goto go_on;
	}
	while( command() ) {}

	go_on:;
	if( !Abort && !Flag.polling ) signal(SIGINT,interrupt);
}



int command(void)
{
	static int no_prompt = 0;

	if( Flag.xboard < 2 )
	{
		if( no_prompt )
		{
			no_prompt = 0;
		}
		else
		if( Flag.ponder >= 2 )
		{
			printf("\n[ pondering ]\n");
		}
		else
		{
			printf( "[ %s, %i ]\n", Color==WHITE ? "white" : "black", Counter/2+1 );
		}
	}

	if( Inp[0] == '\0' )
	if( fgets(Inp,255,stdin) == NULL ) strcpy(Inp,"quit\n");

	if( strncmp(Inp,"exit",4) == 0 && Flag.analyze )
	{ Flag.machine_color = Flag.analyze = 0; Inp[0]='\0'; return 1; }

	if( strncmp(Inp,"quit",4) == 0
	 || strncmp(Inp,"exit",4) == 0 )
	{
		if( Flag.ponder < 2 ) return 0;
		else
		{ Abort = 1; return 0; }
	}

/* COMMAND: comment */
	if( Inp[0]=='#' )
	{ printf("%s",Inp); no_prompt = 1; Inp[0]='\0'; return 1; }

/* COMMAND: no op */
	if( Inp[0]=='\n' )
	{	Inp[0]='\0';
		if( Flag.ponder >= 2 ) return 0; /* continue search */
		else return 1;
	}

/* COMMAND: analyze */
	if( strncmp( Inp, "analyze", 7 ) == 0 )
	{
		if( Flag.ponder >= 2 ) { Abort = 1; return 0; }
		puts("analyze mode, type 'exit' to terminate");
		Flag.analyze = 1;
		Flag.machine_color = 3;
		Inp[0]='\0'; return 1;
	}

/* COMMAND: force */
	if( strncmp( Inp, "force", 5 ) == 0 )
	{
		if( Flag.ponder >= 2 ) { Abort = 1; return 0; }
		puts("you play both");
		if( Flag.analyze ) Flag.machine_color = Color;
		else Flag.machine_color = 0;
		Inp[0]='\0'; return 1;
	}

/* COMMANDS: white, black */
	if(  strncmp( Inp, "white", 5 ) == 0
	  || strncmp( Inp, "black", 5 ) == 0 )
	{
		if( Flag.ponder >= 2 ) { Abort = 1; return 0; }
		puts("you do not play both");
		if( Flag.machine_color == 0 )
			Flag.machine_color = enemy(Color);
		Inp[0]='\0'; return 1;
	}

/* COMMAND: both */
	if( strncmp( Inp, "both", 4 ) == 0 )
	{
		if( Flag.ponder >= 2 ) { Abort = 1; return 0; }
		puts("machine plays both");
		Flag.machine_color = 3;
		Inp[0]='\0'; return 1;
	}

/* COMMAND: new */
	if( strncmp( Inp, "new", 3 ) == 0 )
	{
		if( Flag.ponder >= 2 ) { Abort = 1; return 0; }
		// setfen("rnbqkbnr/pppppppp/////PPPPPPPP/RNBQKBNR/w");
		setfen("rnbqkbnr/pppppppp/////PPPPPPPP/RNBQKBNR w KQkq");
		DrawScore = -20;
		if( Flag.analyze ) Flag.machine_color = WHITE;
		else
		{
			Flag.machine_color = BLACK;
			puts("initial position set, machine plays black");
		}
		Inp[0]='\0'; return 1;
	}

/* COMMAND: go */
	if( strncmp( Inp, "go", 2 ) == 0 )
	{
		if( Flag.ponder >= 2 ) { Abort = 1; return 0; }
		Flag.machine_color = Color;
		Inp[0]='\0'; return 1;
	}

/* COMMAND: bd */
	if( strncmp( Inp, "bd", 2 ) == 0 || strncmp( Inp, "d\n", 2 ) == 0 )
	{ printboard(NULL); Inp[0]='\0'; return 1; }

/* COMMAND: bk */
	if( strncmp( Inp, "bk", 2 ) == 0 )
	{
		tmove m[256]; int n;
		extern void bk(tmove *, int );
		if( Flag.ponder >= 2 ) { Abort = 1; return 0; }
		generate_legal_moves( m, &n, checktest(Color) );
		bk(m,n);
/* printf("%08X\n",G[Counter].hashboard); */
		Inp[0]='\0'; return 1;
	}

/* COMMAND: post */
	if( strncmp( Inp, "post", 4 ) == 0 )
	{
		Flag.post = 1;
		puts("post on");
		Inp[0]='\0'; return 1;
	}

/* COMMAND: nopost */
	if( strncmp( Inp, "nopost", 6 ) == 0 )
	{
		Flag.post = 0;
		puts("post off");
		Inp[0]='\0'; return 1;
	}

/* COMMAND: level */
	if( strncmp( Inp, "level ", 6 ) == 0 )
	{
		l_level( Inp+6 );
		Inp[0]='\0'; return 1;
	}

/* COMMAND: time */
	if( strncmp( Inp, "time ", 5 ) == 0 )
	{
		sscanf( Inp+5, "%li", &Time );
		Inp[0]='\0'; return 1;
	}

/* COMMAND: otim */
	if( strncmp( Inp, "otim ", 5 ) == 0 )
	{
		sscanf( Inp+5, "%li", &Otim );
		Inp[0]='\0'; return 1;
	}

/* COMMAND: rating <phalanx> <opponent> */
	if( strncmp( Inp, "rating ", 7 ) == 0 )
	{
		int phal, oppo, diff;
		if( sscanf( Inp+7, "%i %i", &phal, &oppo ) == 2 )
		{
			diff = oppo-phal;
			if( diff > 300 ) DrawScore = 20;
			else if( diff < -300 ) DrawScore = -20;
			else DrawScore = diff/15;
			printf("setting draw score to %i\n",DrawScore);
			Inp[0]='\0'; return 1;
		}
	}


/* COMMAND: hard */
	if( strncmp( Inp, "hard", 4 ) == 0 )
	{
		if( Flag.ponder==0 && Flag.easy==0 )
		Flag.ponder = 1;
		if(Flag.ponder) puts("pondering on");
		Inp[0]='\0'; return 1;
	}

/* COMMAND: easy */
	if( strncmp( Inp, "easy", 4 ) == 0 )
	{
		if( Flag.ponder != 0 )
		{
			if( Flag.ponder >= 2 ) { Abort = 1; return 0; }
			Flag.ponder = 0;
		}
		puts("pondering off");
		Inp[0]='\0'; return 1;
	}

/* COMMAND: beep, draw, ... ignored */
	if(
	    strncmp( Inp, "beep\n", 5 ) == 0
	 || strncmp( Inp, "bogus\n", 6 ) == 0
	 || strncmp( Inp, "draw\n", 5 ) == 0
	 || strncmp( Inp, "name ", 5 ) == 0
	 || strncmp( Inp, "random\n", 7 ) == 0
	 || strncmp( Inp, "noise ", 6 ) == 0
	 || strncmp( Inp, ".\n", 2 ) == 0
	)
	{ /* ignore */ Inp[0]='\0'; return 1; }

/* COMMAND: depth */
	if( strncmp( Inp, "depth", 5 ) == 0 )
	{
		int newdepth;
		if( Inp[5] == '\n' )
		{ printf("depth= "); scanf( "%i", &newdepth ); }
		else
		{ sscanf( &Inp[6],"%i",&newdepth); }
		Flag.depth = 100*newdepth;
		if( Flag.depth < 200 ) Flag.depth = 200;
		else if( Flag.depth > MAXPLY*100 ) Flag.depth = MAXPLY*100;
		Flag.level = fixeddepth;

		printf("search depth %i\n", Flag.depth/100 );
		Inp[0]='\0'; return 1;
	}

/* COMMAND: book */
	if( strncmp( Inp, "book", 4 ) == 0 )
	{
		Flag.book = ! Flag.book;
		if( Flag.book ) puts("book on");
		else puts("book off");
		Inp[0]='\0'; return 1;
	}

/* COMMAND: about */
	if( strncmp( Inp, "about", 5 ) == 0 )
	{ about(); Inp[0]='\0'; return 1; }

/* COMMAND: fen */
	if( strncmp( Inp, "fen", 3 ) == 0 )
	{ printpositionfen(); Inp[0]='\0'; return 1; }

/* COMMAND: help */
	if( strncmp( Inp, "help", 4 ) == 0 )
	{
puts("COMMAND SUMMARY: about (shows settings), bd (displays position - same");
puts("as 'd'), bk (shows book info), book (enables/disables book), both");
puts("(machine plays both), depth <N> (set search depth), fen, force (user");
puts("plays both, go (start computing), help, history (show game moves),");
puts("level <moves> <minutes> <increment in seconds>, level <seconds per");
puts("move>, new (new game), post (show thinking), remove (take back last");
puts("move, two plies), nopost (do not show thinking), quit (same as 'exit'");
puts("or end of file character), score (show static evaluation), time <N>");
puts("(remaining time is N/100 s), undo (undo last ply - same as 'u'),");
puts("<FEN position> (set test position, start search, show result),");
puts("# (comment)");
		Inp[0]='\0'; return 1;
	}

/* COMMAND: score */
	if( strncmp( Inp, "score\n", 6 ) == 0 )
	{
		Scoring = 1; Depth = 100;
		printf("\n (stm) material = %i\n",
			G[Counter].mtrl-G[Counter].xmtrl);
		printf(" TOTAL EVALUATION = %i\n",
			score_position() );
		printf("Wknow.hung=%i; Wknow.khung=%i\n",
		        Wknow.hung, Wknow.khung);
		printf("Bknow.hung=%i; Bknow.khung=%i\n",
		        Bknow.hung, Bknow.khung);
		puts("");
		Scoring = 0;
		Inp[0]='\0'; return 1;
	}

/* COMMAND: xboard */
	if( strncmp( Inp, "xboard", 6 ) == 0 )
	{
		Flag.xboard = 2;
		puts("xboard mode on");
		Inp[0]='\0'; return 1;
	}

/* COMMAND: history */
	if( strncmp( Inp, "history\n", 8 ) == 0 )
	{
		int i;
		int sc = (Counter+Color+1) % 2;
		if( sc ) printf("\n 1. ... ");
		for( i=0; i<Counter; i++ )
		{
			if( (i+sc)%2 == 0 )
			{
				if( (i+sc)%6 == 0 ) puts("");
				printf( "%2i. ", (i+sc+2)/2 );
			}
			printm( G[i].m, NULL );
		}
		puts("");
		Inp[0]='\0'; return 1;
	}

/* COMMAND: undo (xboard) */
	if( strncmp( Inp, "undo", 4 ) == 0 || strncmp( Inp, "u\n", 2 ) == 0 )
	{
		if( Flag.ponder >= 2 ) { Abort = 1; return 0; }
		if( Counter != 0 )
		{
			undo_move( & G[Counter-1].m );
			if(Flag.xboard<2) puts("halfmove back");
			if( Flag.machine_color != 0 )
			Flag.machine_color = enemy(Color);
			if( Flag.analyze ) Flag.machine_color = Color;
		}
		else puts("cannot undo");
		Inp[0]='\0'; return 1;
	}

/* COMMAND: remove (xboard) */
	if( strncmp( Inp, "remove", 6 ) == 0 )
	{
		if( Flag.ponder >= 2 ) { Abort = 1; return 0; }
		if( Counter > 1 )
		{
			undo_move( & G[Counter-1].m );
			undo_move( & G[Counter-1].m );
			if(Flag.xboard<2) puts("move back");
			if( Flag.machine_color != 0 )
			Flag.machine_color = enemy(Color);
		}
		else puts("cannot remove");
		Inp[0]='\0'; return 1;
	}

/* COMMAND: edit (xboard) */
	if( strncmp( Inp, "edit\n", 5 ) == 0 )
	{
		if( Flag.ponder >= 2 ) { Abort = 1; return 0; }
		edit();
		if( Flag.analyze ) Flag.machine_color = 3;
		else Flag.machine_color = 0;
		if( Inp[0]=='e' ) Inp[0]='\0';
		return 1;
	}

/* COMMAND: setboard <FEN> */
/* added by Pascal Georges */
	if( strncmp( Inp, "setboard ", 9 ) == 0 )
	{
		setfen(Inp+9);
		Inp[0] = '\0';
		return 1;
	}

/* COMMAND: test a position */
	if( sevenslashes(Inp) ) /* test */
	{
		if( Flag.ponder >= 2 ) { Abort = 1; return 0; }
		if( setfen(Inp) == 0 )
		{
			static int entered=0;
			static int found=0;
			int f=0;
			tmove m;
			int i;

			printf("\n%s",Inp);
			m = root_search();
			Flag.machine_color = 0;
			if( ! Flag.post ) { Depth += 100; infoline(0,NULL); }

			entered++;
			for( i=0; Inp[i]!='\0'; i++ )
			if( checkmove( Inp+i, &m ) ) { f=1; found++; break; }

			if( f ) printf("[ Solution correct    ");
			else
			{	FILE * nf; /* not found */
				static char filename[16] = "notfound.fin";
				if( entered-found == 1 )
					nf = fopen(filename,"w");
				else	nf = fopen(filename,"a");
				if( nf != NULL )
				{
					fputs( Inp, nf );
					fclose(nf);
				}
				printf("[ Solution incorrect  ");
			}

			printf(" %i.%02i%%, %i/%i ]\n",
				found*100/entered,
				found*10000/entered%100,
				found,
				entered);

			no_prompt = 1;
		}
		Inp[0]='\0'; return 1;
	}

	/* suppose this is an user move */
	if( Flag.ponder >= 2 )
	{
		tmove *pm = sandex(Inp,Pm,Pn);

		if(pm!=NULL)
		if(  pm->from == Pondermove.from
		  && pm->to == Pondermove.to
		  && pm->in1 == Pondermove.in1
		  && pm->in2 == Pondermove.in2
		  && pm->in2a == Pondermove.in2a
		  && pm->special == Pondermove.special )
		{
			Flag.ponder = 1;
			if( ! l_iterate() ) Abort = 2;
			else
			if( !Flag.polling ) signal(SIGINT,interrupt);
			Inp[0] = '\0'; return 0;
		}
		Abort = 1; return 0;
	}

	if( terminal() < 2 )
	{
		tmove m[256]; int n; int i;
		generate_legal_moves( m, &n, checktest(Color) );
		{ tmove *sd = sandex(Inp,m,n); if(sd!=NULL) i=sd-m; else i=n; }
		if( i != n )
		{
			printf("\nyour move is ");
			printm( m[i], NULL ); puts("");
			do_move( m+i );
			switch( terminal() )
			{
				case 1: puts("1/2-1/2 {Drawn game}"); break;
				case 2: puts("1/2-1/2 {Stalemate}"); break;
				case 3:
					if( Color == WHITE )
					puts("0-1 {Black mates}");
					else
					puts("1-0 {White mates}");
			}
			Inp[0]='\0'; return 1;
		}
	}

	/*** UNKNOWN COMMAND / ILLEGAL MOVE ***/

	{ char *c=strchr(Inp,'\n'); if(c!=NULL) *c='\0'; else *Inp='\0'; }
	printf("Illegal move: %s\n",Inp);
	Inp[0] = '\0';

	return 1;
}



void shell(void)
{

while( command() )
{
	while( ( Flag.machine_color & Color ) && terminal() < 2 )
	{
		tmove m;
		m = root_search();

		if( Flag.analyze )
		{ undo_move(&m); Flag.machine_color = enemy(Color); continue; }

		if( Flag.ponder >= 2 )
		{
			Flag.ponder = 1;
			undo_move(&m);
			undo_move(&Pondermove);
			continue;
		}

		if( Flag.machine_color == 3 && Abort == 1 )
			Flag.machine_color = 0;

		{
			int ter;

			if( Flag.resign != 0 )
			{
				static int resign=0;

				if( m.value < -Flag.resign && Counter > 20 )
				resign++; else resign=0;

				if( resign > 3
				 && Otim > 2000  /* oppo must have 20 s */
				 && Otim + Flag.increment*900 > 9000
/*				 && m.value > -CHECKMATE+100	*/
				)
				{
					if( Flag.xboard )
						puts("tellics resign");
					if( Color == WHITE )
						puts("1-0 {Black resigns}");
					else	puts("0-1 {White resigns}");
				}
			}

			printf("my move is "); printm( m, NULL );
			if( Flag.xboard )
			{ printf("\n%i. ... ",(Counter+1)/2); gnuprintm(m); }
			puts("");

			switch( ( ter = terminal() ) )
			{
				case 1: puts("1/2-1/2 {Drawn game}"); continue;
				case 2: puts("1/2-1/2 {Stalemate}"); continue;
				case 3:
					if( Color == WHITE )
					puts("0-1 {Black mates}");
					else
					puts("1-0 {White mates}");
				continue;
			}

			if(   Flag.ponder == 1
			   && Flag.machine_color != 3
			   && ter < 2
			   && ( Abort != 0 || Abort != 4 ) )
			{
				int i;
				Pondermove = PV[0][1];
				generate_legal_moves(Pm,&Pn,checktest(Color));

				for( i=0; i!=Pn; i++ )
				if(  Pondermove.from == Pm[i].from
				  && Pondermove.to == Pm[i].to
				  && Pondermove.in1 == Pm[i].in1
				  && Pondermove.in2a == Pm[i].in2a ) break;

				if( i==Pn )
				{
					if( Flag.post )
					{
					puts("No move to ponder from PV.");
					puts("Looking for a move to ponder.");
					}

					if( Bookout < 4 || Counter < 10 )
						i = bookmove( Pm, Pn );
					else
						i = -1;

					if( i != -1 )
					{
						Pondermove = Pm[i];
						if( Flag.post )
						puts("Found in book.");
					}
					else
					{
						if( Flag.post )
						puts("Trying search.");
					Depth = -100;
					NoAbort = 1;
					Ply = 1;
					search(Pm,Pn,-CHECKMATE,CHECKMATE);
					NoAbort = 0;
					Pondermove = PV[1][1];
					}
					for( i=0; i!=Pn; i++ )
					if(  Pondermove.from == Pm[i].from
					  && Pondermove.to == Pm[i].to
					  && Pondermove.in1 == Pm[i].in1
					  && Pondermove.in2a == Pm[i].in2a )
						break;
					if( i==Pn )
					{ if(Flag.post)
					  puts("No move found.  A bug?");
					  i=0;
					}
					else
					{ if(Flag.post) puts("Done."); }
				}

				if( i != Pn )
				{	/* OK, the move was also found in just
					 * generated list of moves, not only in
					 * the PV (principal variation).  Let's
					 * ponder then!!! ********************/
					do_move(&Pondermove);
					if( terminal() > 1 )
					undo_move(&Pondermove);
					else
					{
						if( Flag.post )
						{
							printf("Hint: ");
							printm(Pondermove,NULL);
							puts("");
						}
						Flag.ponder = 2;
					}
				}
			}
			continue;
		}
	}
}
}

