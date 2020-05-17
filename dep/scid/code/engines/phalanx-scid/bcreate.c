#include "phalanx.h"

int WIN=5;             /* highest value for every win of this move */
int DRA=2;             /* much smaller value for drawing moves */
int LOO=1;             /* lowest value for losing moves */
int UNK=1;             /* the result is unknown. there is often [Result "*"]
                          in lots of PGN files */
int Asize=500000;      /* default buffer size.  one cell is 12 bytes.
                          qsort() temporarily needs about twice as much
                          memory to sort the array. */
int MaxBookPly=70;     /* we want only opening to be in the book ... stop
                          the game parsing at this ply. */
int MaxComment=4096;   /* maximum size of comment in bytes.  if a comment
                          exceeds this limit, we search for another game. */
int MinValue=15;       /* move bonus must be >= MinValue, otherwise the move
                          is not added into the opening book. */
int MinPercentage=80;  /* alternate moves must be at least at MinPercentage %
                          of the value of the most valuable move */

FILE * tb1; FILE * tb2;

typedef struct
{
	unsigned hashboard;
	unsigned n;
	unsigned short move;
} tb;          /* 10 bytes */

tb *A;
int I=0;

int bsortkey( const void *a, const void *b )
{
	if( ((tb*)a)->hashboard > ((tb*)b)->hashboard )
		return 1;
	else
	if( ((tb*)a)->hashboard < ((tb*)b)->hashboard )
		return -1;
	else
	if( ((tb*)a)->move > ((tb*)b)->move )
		return 1;
	else
	if( ((tb*)a)->move < ((tb*)b)->move )
		return -1;
	else
		return 0;
}

int usortkey( const void *a, const void *b )
{
	if( ((tb*)a)->n < ((tb*)b)->n )
		return 1;
	else	return -1;
}

int games=0; int positions=0;

void printit(int x)
{
  printf("%8i%10i%7i%%",
          games,positions,I*100/Asize);
  fflush(stdout);
  setTimeout(printit, 5);
}

void compress(void)
{
	int i1, i2;
	printit(0);
	clearTimeout(void);
	printf("\nsorting buffer ............ ");

	qsort( A, I, sizeof(tb), bsortkey );

	puts("done");
	printf("compressing buffer ........ ");
	i1=0; i2=1;
	while( i2 != I )
	{
		while( i2 != I
		    && A[i1].hashboard == A[i2].hashboard
		    && A[i1].move == A[i2].move )
		{
			A[i1].n += A[i2].n;
			i2++;
		}
		if( i2 != I )
		{ i1++; A[i1]=A[i2]; i2++; }
	}
	I=i1+1;
	A[I].n = 0;
	printf("done, buffer usage=%i (%i%%)\n",I,100*I/Asize);

	/** Now, take tb1 file and the memory buffer and join them to
        *** the tb2 (rbook.phalanx.tmp).  Rename tb2 to tb1.
	**/
	{
		int i=0;
		int done_m, done_f;
		tb mtb, ftb;

		printf("adding to file ............ ");
		tb1=fopen("rbook.phalanx","rb");
		tb2=fopen("rbook.phalanx.tmp","wb");
		rewind(tb1);

		mtb=A[0]; i=1; done_m=(i==I);
		myfread( &ftb, sizeof(tb), tb1 ); done_f=feof(tb1);

		while( !done_f || !done_m )
		{
			if( !done_m && !done_f && bsortkey(&mtb,&ftb)==0 )
			{
				mtb.n += ftb.n;
				myfwrite(&mtb,sizeof(tb),tb2);
				mtb=A[i]; i++; done_m=(i==I);
				myfread(&ftb,sizeof(tb),tb1); done_f=feof(tb1);
			}
			else
			if( !done_m && ( done_f || bsortkey(&mtb,&ftb)==-1 ) )
			{
				myfwrite(&mtb,sizeof(tb),tb2);
				mtb=A[i]; i++; done_m=(i==I);
			}
			else
			if( !done_f && ( done_m || bsortkey(&mtb,&ftb)==1 ) )
			{
				myfwrite(&ftb,sizeof(tb),tb2);
				myfread(&ftb,sizeof(tb),tb1); done_f=feof(tb1);
			}
		}

		/* good, it's there, close the two files, move newly
		 * joined to the first one and reopen */

		fclose(tb1); fclose(tb2);
		rename("rbook.phalanx.tmp","rbook.phalanx");
		printf("done\n");
	}

	I=0;

	setTimeout(printit, 5);
}

int bpoints, wpoints;

int findgame(void)
{ register int c;
  register int found=0;
  char s[64];
  bpoints=0; wpoints=0;
  while( ! found )
  { while( (c=getchar()) != EOF && c!='R' ) {}
    if( c==EOF ) return 0;
    if( fgets(s,6,stdin)==NULL ) return 0;
    if( strncmp(s,"esult",5) == 0 )
    {
      int nlns=0;
      while( (c=getchar()) == ' ' ) {}
      if( fgets(s,4,stdin)==NULL ) return 0;
      if( strncmp(s,"1-0",3) == 0 ) { bpoints=LOO; wpoints=WIN; }
      else if( strncmp(s,"0-1",3) == 0 ) { bpoints=WIN; wpoints=LOO; }
      else if( strncmp(s,"1/2",3) == 0 ) { bpoints=DRA; wpoints=DRA; }
      else { bpoints=UNK; wpoints=UNK; }

/*** find two newlines in a row -> end of the header ***/
while( (c=getchar()) != EOF && nlns < 2 )
{ if(c=='\n') nlns++; else nlns=0; }

      if(c!=EOF) return 1;
    }
  }
  return 0;
}

extern char piece[7];
extern char file[10];
extern char row[12];

int addmove(char *move)
{ static tmove m[256];
  tmove * mf;
  static int n;
  int i;
  int p=PAWN+Color;
  for(i=2;i!=7;i++) if(piece[i]==move[0]) p=16*i+Color;
  if( move[0]=='O' )
  { p=KING+Color;
    if(move[3]=='-') { if(Color==WHITE){} }
  }

  generate_legal_moves( m, &n, checktest(Color) );
  if( (mf=sandex(move,m,n)) != NULL )
  {
    if( Color==WHITE )
    { if(wpoints)
      { A[I].hashboard=G[Counter].hashboard;
        A[I].move = smove(mf);
        A[I].n = wpoints; positions++;
        I++; if( I>=Asize ) compress();
      }
    }
    else
    { if(bpoints)
      { A[I].hashboard=G[Counter].hashboard;
        A[I].move = smove(mf);
        A[I].n = bpoints; positions++;
        I++; if( I>=Asize ) compress();
      }
    }
    do_move(mf);
    return 1;
  } else { return 0; }
}

void parsegame(void)
{ register int c='*';
  unsigned char m[128];
  int i;
  setfen("rnbqkbnr/pppppppp/////PPPPPPPP/RNBQKBNR/w");
  while( c!='[' && c!=EOF && Counter < MaxBookPly )
  { while( (c=getchar()) != '.' && c!=EOF )
    if(c=='{')
    {
	int csize=0;
	while( (c=getchar())!='}' && c!=EOF && csize<MaxComment )
	csize++;
    }
    while( (c=getchar())==' ' || c=='\n' ) {}
    m[0]=c; i=1;
    while( (c=getchar())!=' ' && c!='\n' && c!=EOF ) { m[i]=c; i++; }
    m[i]='\0';
    if( ! addmove(m) ) break;

    /* find black's move */
    blackmove:;
    while( (c=getchar())==' ' || c=='\n' || c=='.' || isdigit(c) ) {}
    if( c=='{' )  /*** skip comment ***/
    {
	int csize=0;
	while( (c=getchar())!='}' && c!=EOF && csize<MaxComment )
	csize++;
	goto blackmove;
    }
    if( c=='[' || c==EOF ) return;
    m[0]=c; i=1;
    while( (c=getchar())!=' ' && c!='\n' && c!=EOF ) { m[i]=c; i++; }
    m[i]='\0';
    if( ! addmove(m) ) break;
  }
}

void wusage(void)
{
printf("Usage:   phalanx bcreate <buffer size in cells>\n");
printf("Options: -b <buffer size in cells>     one cell is %lu bytes\n",
	(unsigned long) sizeof(tb) );
printf("         -p <maxply>\n");
printf("         -c <max length of comment>\n");
printf("         -v <min value of move to add>\n");
printf("         -g <min value percentage of best move to add others>\n");
printf("         -w <winning move value>\n");
printf("         -d <drawing move value>\n");
printf("         -l <losing move value>\n");
printf("         -u <unknown result move value>\n");

  exit(0);
}

int bcreate( int argc, char ** argv )
{
  FILE * f;
  int c;

  while( (c=getopt(argc,argv,"b:p:c:v:g:w:d:l:u:")) != -1 )
  switch(c)
  {
	case 'b':
		if( sscanf( optarg, "%i", &Asize ) == 0 ) wusage();
		if( Asize < 1000 ) wusage();
	break;
	case 'p':
		if( sscanf( optarg, "%i", &MaxBookPly ) == 0 ) wusage();
		if( MaxBookPly < 2 ) wusage();
	break;
	case 'c':
		if( sscanf( optarg, "%i", &MaxComment ) == 0 ) wusage();
		if( MaxComment < 2 ) wusage();
	break;
	case 'v':
		if( sscanf( optarg, "%i", &MinValue ) == 0 ) wusage();
		if( MinValue < 0 ) wusage();
	break;
	case 'g':
		if( sscanf( optarg, "%i", &MinPercentage ) == 0 ) wusage();
		if( MinPercentage < 50 || MinPercentage > 100 ) wusage();
	break;
	case 'w':
		if( sscanf( optarg, "%i", &WIN ) == 0 ) wusage();
		if( WIN < 1 || WIN > 10 ) wusage();
	break;
	case 'd':
		if( sscanf( optarg, "%i", &DRA ) == 0 ) wusage();
		if( DRA < 0 || DRA > 10 ) wusage();
	break;
	case 'l':
		if( sscanf( optarg, "%i", &LOO ) == 0 ) wusage();
		if( LOO < 0 || LOO > 10 ) wusage();
	break;
	case 'u':
		if( sscanf( optarg, "%i", &UNK ) == 0 ) wusage();
		if( UNK < 0 || UNK > 10 ) wusage();
	break;
	default: wusage();
  }

  A=malloc(Asize*sizeof(tb));
  if( A == NULL )
  { printf("cannot alloc %lu bytes of memory\n",
    (unsigned long)(Asize*sizeof(tb))); exit(0);
  }

  {
    struct stat fs;
    if( ! stat("rbook.phalanx",&fs) )
	{ puts("rbook.phalanx exists, skipping stage 1"); goto stage_2; }
    else
	{ tb1 = fopen("rbook.phalanx", "w+"); }
  }

  setvbuf(stdout, (char*)NULL, _IONBF, 0);
  puts(  "creating opening book");
  puts(  "-----------------------------");
  puts(  "   games positions buffer%");
  puts(  "-----------------------------");
  setTimeout(printit, 5);
  while( findgame() )
  { parsegame(); games++; }
  printf("end of file, parsed %i positions in %i games\n",positions,games);
  if( games == 0 )
  { puts("opening book not created"); return 0; }
  compress();
  clearTimeout(void);

 stage_2:;

  if( ( f = fopen( SBOOK_FILE, "wb" ) ) == NULL )
  { printf("cannot create book file %s\n", SBOOK_FILE); return 1; }

 {
  int dots=0;
  struct stat fs;
  int fsize;
  tb M;
  tb1=fopen("rbook.phalanx","rb");
  myfread( &M, sizeof(tb), tb1 );

  stat("rbook.phalanx",&fs); fsize = fs.st_size/sizeof(tb);

  printf("writing book ");

  while(!feof(tb1))
  {
	int i;
	int minn;
	A[0]=M;
	I=0;

	do
	{
		int fpos=ftell(tb1)/sizeof(tb);
		if( fpos % (fsize/30) == 1 )
		{
		  if( dots==3 )
		  { printf("%i%%", (int)(((float)fpos)*100/fsize + 1) ); dots=0; }
		  else printf(".");
		  dots++;
		}
		I++; myfread( A+I, sizeof(tb), tb1 );
	}
	while( !feof(tb1) && M.hashboard==A[I].hashboard );

	M=A[I];

	/* sort them, better first */
	qsort( A, I, sizeof(tb), usortkey );

	/* more variability in frequently played lines of early openings */
	{ unsigned i=100, j=10, p=MinPercentage;
	  while( A[0].n > i && p>10 ) { p-=j; j--; i*=10; }
	  minn=A[0].n*p/100;
	}

	if( minn > A[0].n ) minn = A[0].n;

	if( minn < MinValue ) minn=MinValue;

	for( i=0; i!=I && A[i].n>=minn; i++ ) {}

/*
	printf("got %i of %i moves at hasboard of %08X, next HB %08X\n",
		i, I, A[0].hashboard, M.hashboard );
*/

	I=i;

	for(i=0;i!=I;i++)
	{
		unsigned short m = A[i].move;
		myfwrite( A+i, sizeof(unsigned), f );
		myfwrite( &m, sizeof(unsigned short), f );
	}
  }
  printf(" done\n");
 }

  return 0;
}

