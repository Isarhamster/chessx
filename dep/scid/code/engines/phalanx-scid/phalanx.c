
/*
 *  The main module:
 *  - Parsing command line parameters
 *  - benchmark
 */

#include "phalanx.h"

/* 2005-09-14, José de Paula
 * GCC 3.0 deprecated multi-line strings, and, as of GCC 3.4, they are no longer supported.
 */
void badoptions(void)
{
printf("\nUsage:    phalanx [options] [<moves> <minutes> [<increment in seconds>]]\n          phalanx [options] [<seconds per move>]\n          phalanx bench\n          phalanx bcreate [options]\nOptions:  -t <transposition table size in kilobytes>\n          -f <fixed search time in seconds>\n          -x <+/->  xboard mode on/off        default: on\n          -p <+/->  permanent brain on/off    default: off\n          -s <+/->  show thinking on/off      default: off\n           -c <+/->  cpu time                  default: off\n          -o <+/->  polling input             default: on\n          -b <+/->  opening book              default: on\n          -r <resign value in centipawns>     default: 0 (no resigning)\n          -e <easy level 0...100>             default: 0 (best play)\n          -l <+/->  learning on/off           default: on\n          -v        print version and exit\n          -P <primary book directory>\n          -S <secondary book directory>\n          -L <learning file directory>\n          -g <log file name>\nExamples: phalanx -c+ -s+ -o - -x- -f 60 -t4000\n          xboard -fcp \"phalanx -l+ -r800\"\n");
exit(0);
}


char *get_book_file(char *bookdir,char *env_variable,
		    char *path,char *name,int mode)
{
  char file[256];
  char *aux;
  
  if(bookdir)			/* Specified in options? */
    path = bookdir;
  else if((aux=getenv(env_variable))) /* In an environment variable? */
    path = aux;
  else if(!access(name,mode))	/* In the current directory? */
    path = ".";
  else				/* Desperacy now, use compile time file. */
    ;

#ifdef GNUFUN
  snprintf(file,255,"%s/%s",path,name);
#else
  sprintf(file,"%s/%s",path,name);
#endif
  return strdup(file);
}


int main( int argc, char **argv )
{

int c;
clock_t t;
char *PbookDir,*SbookDir,*LbookDir,*EcoDir;

#ifdef GNUFUN
int indexptr;
struct option longopts[] = {
  { "help",                no_argument,       NULL, 	      '?' },
  { "version",             no_argument,       NULL, 	      'v' },
  { "trans-table-cells",   required_argument, NULL, 	      'T' },
  { "trans-table-bytes",   required_argument, NULL, 	      't' },
  { "fixed-search-time",   required_argument, NULL, 	      'f' },
  { "xboard-mode",         no_argument, 	&Flag.xboard,  1  },
  { "no-xboard-mode",      no_argument, 	&Flag.xboard,  0  },
  { "permanent-brain",     no_argument, 	&Flag.ponder,  1  },
  { "no-permanent-brain",  no_argument, 	&Flag.ponder,  0  },
  { "show-thinking",       no_argument, 	&Flag.post,    0  },
  { "no-show-thinking",    no_argument, 	&Flag.post,    1  },
  { "cpu-time",            no_argument, 	&Flag.cpu,     1  },
  { "no-cpu-time",         no_argument, 	&Flag.cpu,     0  },
  { "poll-input",          no_argument, 	&Flag.polling, 1  },
  { "no-poll-input",       no_argument, 	&Flag.polling, 0  },
  { "use-opening-book",    no_argument, 	&Flag.book,    1  },
  { "no-use-opening-book", no_argument, 	&Flag.book,    0  },
  { "easy-level",          required_argument, NULL,         'e' },
  { "resign-value",        required_argument, NULL,         'r' },
  { "learning",            no_argument, 	&Flag.learn,   1  },
  { "no-learning",         no_argument, 	&Flag.learn,   0  },
  { "primary-book-dir",    required_argument, NULL, 	      'P' },
  { "secondary-book-dir",  required_argument, NULL, 	      'S' },
  { "learning-file-dir",   required_argument, NULL, 	      'L' },
  { "log-file-name",       required_argument, NULL, 	      'g' },
  { 0,0,0,0 }
};
#endif

t = clock();
 
/* Do NOT buffer I/O - needed for communication with xboard */
setvbuf(stdout, (char*)NULL, _IONBF, 0);

printf("Phalanx "); puts(VERSION);

// if(argc>1) if( strncmp("bcreate\0",argv[1],8) == 0 )
// { return bcreate(argc-1,argv+1); }

/* Initialize the random number generator. */
srand( ((unsigned)time(NULL)) + ((unsigned)getpid()) );

/* SIG_INT */
signal(SIGINT,SIG_IGN);

setfen("rnbqkbnr/pppppppp/////PPPPPPPP/RNBQKBNR/w");

#undef debugsee
#ifdef debugsee
{
	int x; int j;
	setfen("q2r2k1/1b3ppp/2prpn2/3P4/2PRP3/4NB2/5PPP/3R2K1 w");
	printboard(NULL);
	for(j=0;j!=1000000;j++) x=see(C6,D5);
	printf(" [%i]\n",see(C6,D5));
	return 0;
}
#endif

Flag.machine_color = BLACK;
Flag.post = 0;
Flag.analyze = 0;
Flag.xboard = 1;
Flag.book = 1;
Flag.centiseconds = 1000;
Flag.level = averagetime;
Flag.ponder = 0;
Flag.cpu = 0;
Flag.increment = 0;
Flag.polling = 1;
Flag.resign = 0;
Flag.easy = 0;
Flag.noise = 50;         /* 0.5 s */
Flag.learn = 0;
Flag.bench = 0;
Flag.log = NULL;
Scoring = 0;

SbookDir = NULL;
PbookDir = NULL;
EcoDir = NULL;
LbookDir = NULL; 
 
opterr = 0;

#ifdef GNUFUN
while( ( c = getopt_long( argc, argv, "vf:T:t:p:s:x:c:o:r:b:e:l:S:P:L:g:",
			  longopts, &indexptr) ) != -1 )
#else
while( ( c = getopt(argc,argv,"vf:T:t:p:s:x:c:o:r:b:e:l:S:P:L:g:") ) != -1 )
#endif
switch(c)
{
	case 'T':
		if( sscanf( optarg, "%i", &SizeHT ) == 0 ) badoptions();
	break;
	case 't':
		if( sscanf( optarg, "%i", &SizeHT ) == 0 ) badoptions();
		SizeHT = 1024 * SizeHT / sizeof(thashentry);
	break;
	case 'r': { int i;
		if( sscanf( optarg, "%i", &i ) == 0 ) badoptions();
		Flag.resign = abs(i); }
	break;
	case 'f':
		{
		static int t;
		if( sscanf( optarg, "%i", &t ) == 0 ) badoptions();
		Flag.centiseconds = t*100;
		Flag.level = fixedtime;
		} break;
	case 'e':
		{
		static int e;
		if( sscanf( optarg, "%i", &e ) == 0 ) badoptions();
		if( e > 100 ) badoptions();
		Flag.easy = e;
		} break;
	case 'p': switch(*optarg)
	{	case '+': Flag.ponder = 1; break;
		case '-': Flag.ponder = 0; break;
		default: badoptions();
	} break;
	case 's': switch(*optarg)
	{	case '+': Flag.post = 1; break;
		case '-': Flag.post = 0; break;
		default: badoptions();
	} break;
	case 'x': switch(*optarg)
	{	case '+': Flag.xboard = 1; break;
		case '-': Flag.xboard = 0; break;
		default: badoptions();
	} break;
	case 'c': switch(*optarg)
	{	case '+': Flag.cpu = 1; break;
		case '-': Flag.cpu = 0; break;
		default: badoptions();
	} break;
	case 'o': switch(*optarg)
	{	case '+': Flag.polling = 1; break;
		case '-': Flag.polling = 0; break;
		default: badoptions();
	} break;
	case 'b': switch(*optarg)
	{	case '+': Flag.book = 1; break;
		case '-': Flag.book = 0; break;
		default: badoptions();
	} break;
	case 'l': switch(*optarg)
	{	case '+': Flag.learn = 1; break;
		case '-': Flag.learn = 0; break;
		default: badoptions();
	} break;
	case 'g':
	{
		if( ( Flag.log = fopen(optarg,"a") ) != NULL )
		{
			setvbuf(Flag.log, (char*)NULL, _IONBF, 0);
			printf("log file %s\n",optarg);
		}
		else
		printf("telluser Phalanx: cannot open log file\n");
	} break;
        case 'v': /* fprintf(stderr,"%s\n",VERSION); */ exit(0);
	case 'P': PbookDir = strdup(optarg); break;
	case 'E': EcoDir = strdup(optarg); break;
	case 'S': SbookDir = strdup(optarg); break;
	case 'L': LbookDir = strdup(optarg); break;
	case '?':
		badoptions();
	break;
}

{
int m;
switch( argc - optind )
{
case 0: break;
case 1:
	if( argc==2 && strncmp("bench\0",argv[1],6)==0 )
		{ Flag.bench = 1; break; }
	if( sscanf(argv[optind],"%i",&m) == 0 ) badoptions();
	Flag.centiseconds = 100*m;
	Flag.level = averagetime;
break;
case 2:
	if( sscanf(argv[optind],"%i",&m) == 0 ) badoptions();
	Flag.moves = m;
	if( sscanf(argv[optind+1],"%i",&m) == 0 ) badoptions();
	Flag.centiseconds = m*6000;
	Flag.level = timecontrol;
break;
case 3:
	if( sscanf(argv[optind],"%i",&m) == 0 ) badoptions();
	Flag.moves = m;
	if( sscanf(argv[optind+1],"%i",&m) == 0 ) badoptions();
	Flag.centiseconds = m*6000;
	if( sscanf(argv[optind+2],"%i",&m) == 0 ) badoptions();
	Flag.increment = m;
	Flag.level = timecontrol;
break;
default: badoptions();
}
}

if( Flag.easy )
{ SizeHT = 0; Flag.ponder = 0; Flag.learn = 0; }
else
if( SizeHT != 0 )
{
	HT = calloc( SizeHT, sizeof(thashentry) );
	if( HT == NULL )
	{
		puts("cannot alloc hashtable"); SizeHT = 0;
	}
}

/* alloc static eval cache */
{ extern void initcache(void); initcache(); }

/* init distance tables */
{ extern void initdist(void); initdist(); }

if( Flag.bench )
{
#	define NPOS 10 /* number of positions */
	int i;
	long tim;
	int64 allnodes = 0;
	char * positions[NPOS] =
	{ "rnbqkbnr/pppppppp/////PPPPPPPP/RNBQKBNR/w",
	  "2kr3r/pp3Npp/2pbbn2/6B1/2BPp3/6Pq/PPP1Q2P/2KR3R b",
	  "r1bq1r1k/2pn2bp/1p1p1np1/pN1Pp3/1PP1Pp2/P2B1P2/1BQN2PP/1R3RK1 b",
	  "r4rk1/2p1p1b1/p1n3pp/1p1qP3/P1pP4/B1P2PPN/4Q1K1/R6R b",
	  "8/1p3rpp/p2nk3/P3p3/2R5/2N2PP1/1P2K2P/8 w",
	  "rnbr2k1/ppq1ppbp/6p1/2p5/3P4/2PBPN2/P4PPP/1RBQ1RK1 w",
	  "3r1rk1/1q2b1pp/pn3p2/1pp5/4PB2/5N2/PP1RQPPP/3R2K1 w",
	  "8/2p5/5pK1/5R2/4k2P/p4P2/1r6/8 b",
	  "3r1rk1/p3qp1p/2bb2p1/2pp4/8/1P2P3/PBQN1PPP/2R2RK1 b",
	  "k7/ppp5/8/8/8/8/P1P5/K7/w" };

	Flag.cpu = 1;
	Flag.level = fixedtime;
	Flag.centiseconds = 3000;
	Flag.post = Flag.book = Flag.learn = Flag.ponder = Flag.polling = 0;
	printf("Running benchmark, this will take 5 minutes of CPU time.\n");
	printf("----------");
	tim = ptime();

	for( i=0; i!=NPOS; i++ )
	{
		setfen(positions[i]);
		if( i!=0 ) printf("+");
		fflush(stdout);
		root_search();
		if( Abort == 1 ) return 0;
		allnodes += Nodes;
	}
	printf("+\n%10i nodes per second\n",
		(int)(100*allnodes / (ptime()-tim) ));

	return 0;
}

/*** Opening book init ***/
/* Try to open book files.  Give a message if not successful. */
SbookDir = get_book_file(SbookDir,ENV_SBOOK,SBOOK_DIR,SBOOK_FILE,R_OK);
Sbook.f = fopen(SbookDir,"rb");
PbookDir = get_book_file(PbookDir,ENV_PBOOK,PBOOK_DIR,PBOOK_FILE,R_OK);
Pbook.f = fopen(PbookDir,"rb");
EcoDir = get_book_file(EcoDir,ENV_ECO,ECO_DIR,ECO_FILE,R_OK);
Eco = fopen(EcoDir,"rb");
if( Flag.learn )
{
	LbookDir
	= get_book_file(LbookDir,ENV_LEARN,LEARN_DIR,LEARN_FILE,R_OK|W_OK);
	Learn.f = fopen(LbookDir,"r+");
	if( Learn.f == NULL )
	{
#		define LFSZ 65536
		int b[LFSZ];
		char filename[256];
		memset( b, 0, LFSZ*sizeof(int) );
  		sprintf(filename,"./%s",LEARN_FILE);
		free( LbookDir );
		LbookDir = strdup( filename );
		Learn.f = fopen(LbookDir,"w+");
		if( fwrite( b, sizeof(int), LFSZ, Learn.f ) == LFSZ )
		printf("telluser Phalanx: created learn file %s\n",LbookDir);
	}
} else Learn.f = NULL;

if( Pbook.f != NULL )
{
	struct stat fs;
	stat(PbookDir,&fs); Pbook.filesize = fs.st_size;
	printf("primary book %s, %d bytes\n",PbookDir,Pbook.filesize);
}

if( Sbook.f != NULL )
{
	struct stat fs;
	unsigned pos;
	stat(SbookDir,&fs); Sbook.filesize = fs.st_size;
	myfread( &pos, sizeof(unsigned), Sbook.f ); Sbook.firstkey=pos;
	fseek( Sbook.f, Sbook.filesize-6, SEEK_SET );
	myfread( &pos, sizeof(unsigned), Sbook.f ); Sbook.lastkey=pos;
	printf("secondary book %s, %d bytes\n",SbookDir,Sbook.filesize);
}

if( Learn.f != NULL )
{
	struct stat fs;
	stat(LbookDir,&fs); Learn.filesize = fs.st_size;
	printf("learning file %s, %d bytes\n",LbookDir,Learn.filesize);
}

if( Eco != NULL )
{
	struct stat fs;
	stat(EcoDir,&fs);
	printf("eco file %s, %i bytes\n",EcoDir,(int) fs.st_size);
}

if( Sbook.f==NULL || Pbook.f==NULL )
{ if( Flag.xboard ) printf("telluser Phalanx: ");
  printf("Cannot open ");
}

if( Sbook.f == NULL )
  if( Pbook.f == NULL )
    printf("both [%s] and [%s]\n", SBOOK_FILE, PBOOK_FILE );
  else
    printf("[%s]\n", SBOOK_FILE );
else
  if( Pbook.f == NULL )
    printf("[%s]\n", PBOOK_FILE );

if( Learn.f == NULL && Flag.learn )
{ Flag.learn=0; puts("telluser Phalanx: cannot open learn file"); }

printf("tellics set 1 Phalanx "); printf(VERSION);
if( Flag.easy )
	printf(", easy level %i\n", Flag.easy );
else
	printf(", %i kB hashtable, %i/%i kB P/S opening book", (int)(((1+SizeHT)*sizeof(thashentry)-1)/1024),(int)(Pbook.filesize/1024), (int) (Sbook.filesize/1024));
printf("\n");

/************************************/
/************************************/
              shell();
/************************************/
/************************************/

t = clock()-t;
printf("Processor time ......... %i:%02i:%02i.%02i\n",
	(int) (t/CLOCKS_PER_SEC/3600),
	(int) (t/CLOCKS_PER_SEC%3600/60),
	(int) (t/CLOCKS_PER_SEC%60),
	(int) (t*100/CLOCKS_PER_SEC%100) );

if( Age != 0 )
{
printf("Average search depth ... %g\n",
	((float)AllDepth) / ((float)(Age*100)) );
printf("Average NPS report ..... %g\n",
	((float)AllNPS) / ((float)Age) );
}

printf("Hash table age ......... %i\n", Age);

puts("Good bye!");

return 0;

}

