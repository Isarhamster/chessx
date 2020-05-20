#include "phalanx.h"

#undef SHOWREADS
#undef SHOWDUPS

tsbook Sbook;
tpbook Pbook;

int Bookout=0;

/*
 *	myfwrite(), myfread() to fix the endianess problem.
 */

#	define ISI ((int)sizeof(int))

void myfwrite( void *ptr, int size, FILE *stream )
{
	int i,j;
	for( i=0; i<size-ISI; i+=ISI )
	{ for( j=0; j!=ISI; j++ ) fwrite( (char*)ptr+i+j, 1, 1, stream ); }
	for( j=0; i+j<size; j++ ) fwrite( (char*)ptr+i+j, 1, 1, stream );
}

void myfread(  void *ptr, int size, FILE *stream )
{
	int i,j;
	for( i=0; i<size-ISI; i+=ISI )
	{ for( j=0; j!=ISI; j++ ) fread( (char*)ptr+i+j, 1, 1, stream ); }
	for( j=0; i+j<size; j++ ) fread( (char*)ptr+i+j, 1, 1, stream );
}

/*
 *	write position into string s, to be saved in book or
 *	compared with another position string.
 *	Format: pieces as usual, black pieces by capital P,N,B,...
 *	empty squares are stored as numbers, e.g. '24' means
 *	24 empty sq.
 */
void postr(char *s)
{
	int i, counter, lenght;
	const char P[14]=
	{'-','-','p','P','n','N','b','B','r','R','q','Q','k','K'};

	sprintf( s, "%1X%1X", (Color==BLACK), G[Counter].castling & 0xF );

	counter=0; lenght=2;
	for(i=A1;i!=H9;i++) if( B[i] && B[i] != 3 )
	{
		if(counter)
		{
			sprintf(&s[lenght],"%i",counter);
			if(counter>9) lenght++;
			lenght++; counter=0;
		}
		sprintf(&s[lenght],"%c",P[B[i]/8+(color(B[i])==BLACK)]);
		lenght++;
	}
	else if( B[i]==0 ) counter++;

	if(counter) sprintf(&s[lenght],"%i ",counter);
	else sprintf(&s[lenght]," ");
}




/*
 *	Compare position strings p1 and p2.
 *	p1==p2 ... returns 0
 *	p1>p2 ... 1; p1<p2 ... -1
 */
int poscmp(char *p1, char *p2)
{
	register int i;
	for(i=0; i!=80; i++)
	{
		if(p1[i]=='\0') p1[i]=' ';
		if(p2[i]=='\0') p2[i]=' ';

		if(p1[i]<p2[i]) return -1;
		if(p1[i]>p2[i]) return  1;
		if(p1[i]==' ') return 0;
	}
	fprintf(stderr,"Phalanx error: Opening book corrupted.\n");
	exit(2);
}



/*
 *	Now the book searching functions.  If there is less than
 *	512 bytes to search, the book is searched linearly.
 */
int linSB(FILE *f, long from, long to, char *p, char *s)
{
	if( fseek(f,from,SEEK_SET)!=0 ) { return -1; }
	if(!feof(f)) fgets(s,80,f);	/* skip to the first \n */

	while(ftell(f)<=to && !feof(f))
	{
		fgets(s,100,f);
		if( poscmp(p,s)==0 ) return 0;
	}

	return -1;
}



/*
 *	There is more than 512 bytes to search -> use binary search.
 */
int binSB(FILE *f, long from, long to, char *p, char *s)
{
	long mid;

	if( to-from < 512 ) return( linSB(f,from,to,p,s) );

	mid=(to+from)/2;
	if( fseek(f,mid,SEEK_SET)!=0 ) return -1;
	if(!feof(f)) fgets(s,80,f);	/* skip to the first \n */

	if(!feof(f)) fgets(s,100,f);

	switch(poscmp(p,s))
	{
		case  0: return(0);
		case  1: return( binSB(f,mid,to,p,s) );
		case -1: return( binSB(f,from,mid,p,s) );
		default: return -1;
	}
}



int parsemove( char *inp, tmove *m, int n )
{
	int special, from, to, in2a, i;

	special = 0;
	from = inp[0]-'a'+1 + 10*(inp[1]-'1') + 20;
	to =   inp[2]-'a'+1 + 10*(inp[3]-'1') + 20;
	in2a = 0;

	if(    strncmp(inp,"o-o-o",5)==0
	    || strncmp(inp,"0-0-0",5)==0
	    || strncmp(inp,"O-O-O",5)==0
	    || ( B[E1]==WK && strncmp(inp,"e1c1",4)==0 )
	    || ( B[E8]==BK && strncmp(inp,"e8c8",4)==0 ) )
		special = LONG_CASTLING;
	else if(    strncmp(inp,"o-o",3)==0
	         || strncmp(inp,"0-0",3)==0
	         || strncmp(inp,"O-O",3)==0
	         || ( B[E1]==WK && strncmp(inp,"e1g1",4)==0 )
	         || ( B[E8]==BK && strncmp(inp,"e8g8",4)==0 ) )
		special = SHORT_CASTLING;

	switch(inp[4])
	{
	case 'Q': case 'q': in2a=QUEEN+Color; break;
	case 'R': case 'r': in2a=ROOK+Color; break;
	case 'B': case 'b': in2a=BISHOP+Color; break;
	case 'N': case 'n': in2a=KNIGHT+Color; break;
	default : in2a=B[from];
	}

	for(i=0; i!=n; i++)
	{
	if( ( special>0 && special<A1 && m[i].special == special ) ||
	    ( (m[i].from==from) && (m[i].to==to) && (m[i].in2a==in2a) )
	  ) return(i);
	}

	return(-1);
}



/* binary book */
int sbookmoves( int *moves, int *values, tmove *m, int n )
{
	int index, i;
	int64 first, last=0, middle=0;
	unsigned fkey, lkey;
	unsigned pos;
	long booksize; 	/* filesize in bytes */
	FILE *f;
	int hits=0;

	if( (f=Sbook.f) == NULL ) return -1;

	if( G[Counter].hashboard < Sbook.firstkey
	 || G[Counter].hashboard > Sbook.lastkey ) return -1;

	fkey = Sbook.firstkey; lkey = Sbook.lastkey;

	booksize = Sbook.filesize/6;

	first = 0; last = booksize-1;
	if( G[Counter].hashboard == Sbook.firstkey ) middle=0;
	else if( G[Counter].hashboard == Sbook.lastkey ) middle=booksize-1;
	else
	while( first < last )
	{
		/* middle = (first+last) / 2; */

		middle = (G[Counter].hashboard-fkey)*(last-first)
		       / (lkey-fkey) + first;

		if(middle==first) middle++; if(middle==last) middle--;
		if(middle==first)
		{
#ifdef SHOWREADS
			printf("notfoundinbook %i\n", hits );
#endif
			return -1;
		}

		if( fseek(f,middle*6,SEEK_SET)!=0 ) return -1;
		myfread( &pos, sizeof(unsigned), f ); hits++;

		if( pos < G[Counter].hashboard ) { first=middle; fkey=pos; }
		else if( pos == G[Counter].hashboard ) break;
		else { last=middle; lkey=pos; }
	}

/*
printf("[%i %i %i %i] [%08X %08X]\n",
        first,middle,last,booksize,pos,G[Counter].hashboard);
*/

#ifdef SHOWREADS
	printf("yesfoundinbook %i\n",hits);
#endif

	if( middle == 0 ) pos = G[Counter].hashboard;
	else
	do
	{
		middle --;
		if( fseek(f,middle*6,SEEK_SET)!=0 ) return -1;
		myfread( &pos, sizeof(unsigned), f );
	} while( middle>0 && pos==G[Counter].hashboard );

	if(pos!=G[Counter].hashboard)
	{ middle ++; pos = G[Counter].hashboard; }

	/*** Position found in book! ***/
	index=0;
	if( fseek(f,middle*6+4,SEEK_SET)!=0 ) return -1;
	while( middle<booksize && pos==G[Counter].hashboard )
	{
		unsigned short sm;

		myfread( &sm, sizeof(unsigned short), f );
		for( i=0; i!=n; i++ ) if( sm == smove(m+i) )
		{ moves[index]=i; values[index]=100; index++; }

		myfread( &pos, sizeof(unsigned), f );
	}

	return index;
}



/*
 *	bookmoves() returns number of moves found in book, or 0,
 *	if no move is found.  moves[0],...,moves[index-1] are indexes
 *	to the bookmoves in m[], n is numer of moves.  Moves must
 *	be generated before.
 */
int bookmoves( int *moves, int *values, tmove *m, int n )
{
	char s[256], p[256];
	int index, i;
	long booksize; 	/* filesize in bytes */
	FILE *f;

	if( (f=Pbook.f) == NULL ) return -1;

	booksize = Pbook.filesize;

	postr(p);	/* Current position to string */
	if( binSB(f,0,booksize,p,s) == -1 )
	{ return -1; }

	/*** Position found in book! ***/
	index=0;
	for(i=0; s[i]!='\n'; i++)
	{
		int pm;
		while(s[i]!=' ' && s[i]!='\n')
		{
			if(s[i]=='!' && index) values[index-1]+=100;
			i++;
		}
		if(s[i]=='\n') break; i++; if(s[i]=='\n') break;
		if( (pm=parsemove(&s[i],m,n)) != -1 )
		{ moves[index]=pm; values[index]=100; index++; }
	}

	return index;
}



/*
 *	This function returns index of a random move found in book
 *	or -1, if no move was found.  Moves must be generated.
 */
int bookmove( tmove *m, int n )
{
	int moves[80], values[80], index;
	int foundtxt=0;

	index = bookmoves(moves,values,m,n);
	if( index <= 0 ) index = sbookmoves(moves,values,m,n);
	else
	{
#ifdef SHOWDUPS
		int moves2[80]; int values2[80], index2;
		index2 = sbookmoves(moves2,values,m,n);
		if( index2>=1 && index>=1 && Counter>8 )
		{
			char p[256]; int i;
			postr(p);
			puts("found in both sbook and pbook");
			printboard(NULL);
			puts(p);
			printf("   0      0       0      0  book2 ");
			for( i=0; i!=index2; i++ )
			if( i==0 || moves2[i-1]!=moves2[i] )
			{ printm( m[moves2[i]], NULL ); }
			puts("");
		}
#endif
		foundtxt=1;
	}

	if( index > 0 )
	{
		int ii, sumvalues=0, rn;
		for( ii=0; ii!=index; ii++ )
		{ sumvalues += values[ii]; }

		rn = rand()%sumvalues;

		sumvalues = 0;
		for( ii=0; ii!=index; ii++ )
		{ sumvalues += values[ii]; if( sumvalues >= rn ) break; }

		if( Flag.post )
		{	int i;
			char s[128];
			if( Flag.xboard )
			sprintf(s,"   0      0       0      0  book");
			else
			sprintf(s,"Book moves ");
			if( foundtxt ) sprintf(s+strlen(s),"1 ");
			else sprintf(s+strlen(s),"2 ");
			for( i=0; i!=index; i++ )
			{ printm( m[moves[i]], s+strlen(s) ); }
			sprintf(s+strlen(s),"\n");
			printf(s);
			if( Flag.log!=NULL && Flag.ponder<2 )
			{
				char sm[64];
				if(Flag.xboard) fprintf(Flag.log,s+26);
				else            fprintf(Flag.log,s);
				fprintf(Flag.log,"  selected move ");
				printm( m[moves[ii]], sm );
				fprintf(Flag.log,sm);
				fprintf(Flag.log,"\n");
			}
		}
		return ( moves[ii] );
	}

	return( -1 );
}



/*
 *	Print all book moves (command bk)
 */
FILE * Eco = NULL;
void bk( tmove *m, int n )
{
int moves[80], values[80], idx;
int pass;
int sumvalues=0;

#define SHOWECO
#ifdef SHOWECO
static int att=1;
static int seco=0;
if( att != 0 && Eco!=NULL )
{
	typedef struct teco { unsigned hashboard; unsigned point; } teco;
	static struct teco * peco = NULL;
	if( att==1 && Counter==0 && G[Counter].hashboard==0x39512910 /*init*/)
	{
		int c; tmove * move;
		peco = malloc(2048*sizeof(teco));
		if( peco == NULL )
		{ puts(" telluser cannot alloc memory for ECO");
		  att=0; goto abort; }

		printf("telluser creating ECO index, please wait\n");

		while( (c=fgetc(Eco))!='[' ) if( c==EOF ) goto doneinit;
		for(;;)
		{
		  tmove m[128];
		  char ms[32]; int msi; int n;
		  (peco+seco)->point=ftell(Eco)-1;
		  while( (c=getc(Eco))!=']' ) if( c==EOF ) goto doneinit;
		  if( c==EOF ) goto doneinit;
		  setfen("rnbqkbnr/pppppppp/////PPPPPPPP/RNBQKBNR/w");
		  for(;;)
		  {
		 	msi=0;
		  	while( (c=getc(Eco))==' ' || c=='\n' ) {}
			ungetc(c,Eco);
			while( (c=getc(Eco))!=' ' && c!='\n' && msi<=30 )
			{ ms[msi]=c; msi++;
			  if( c==EOF || c=='[' )
			  { ungetc(c,Eco); goto nextgame; }
			}
			ms[msi]='\0'; /* printf("%s ",ms); */
			generate_legal_moves( m, &n, checktest(Color) );
			move=sandex(ms,m,n);
			if( move==NULL ) goto nextgame;
			do_move(move);
		  	if( c==EOF ) goto doneinit;
		  }
		  nextgame:;
		  peco[seco].hashboard=G[Counter].hashboard;
		  seco++;
		  while( (c=fgetc(Eco))!='[' ) if( c==EOF ) goto doneinit;
		}

		doneinit:;
		printf(" parsed %i ECO records\n",seco);
		att=2;
		setfen("rnbqkbnr/pppppppp/////PPPPPPPP/RNBQKBNR/w");
	}

	if( Counter>0 && att>1 )
	{
		int counter=Counter;
		int text=-1;
		while( Counter > 0 )
		{
			int i;
			for(i=0;i!=seco;i++)
			if(peco[i].hashboard==G[Counter].hashboard)
			{ text=peco[i].point; goto foundeco; }
			undo_move( & G[Counter-1].m );
		}
		foundeco:;
		while( counter > Counter ) do_move( & G[Counter].m );
		if( text==-1 ) printf(" no eco found\n");
		else
		{ char t[128], * c;
		  fseek(Eco,text,SEEK_SET);
		  fgets(t,126,Eco);
		  t[127]='\0';
		  c=strchr(t,'['); if(c!=NULL) *c = ' ';
		  c=strrchr(t,']'); if(c!=NULL) *c = '\0';
		  puts(t);
		}
	}
	abort:;
}
#endif

for( pass=0; pass!=2; pass++ )
{
	if( pass==0 )
	{
	int i;
	idx = bookmoves(moves,values,m,n);
	if( idx != -1 ) for(i=0;i!=idx;i++) sumvalues += values[i];
	printf(" primary book moves\n");
	}
	else
	{
	idx = sbookmoves(moves,values,m,n);
	printf(" secondary book moves\n");
	}

	if( idx > 0 )
	{	int i;
		for( i=0; i!=idx; i++ )
		if( i==0 || moves[i-1]!=moves[i] )
		{
			printf("   ");
			printm( m[moves[i]], NULL );
			if(pass==0)
				printf("%3d%%\n",values[i]*100/sumvalues);
			else	printf("\n");
		}
	}
	else
	printf("   no move found\n");

}

if( Flag.xboard>1 ) puts("");

{ char c[128]; postr(c); printf("%s\n",c); }

}


