/* getopt.c  (slightly modified by O.Garcia, 12/4/92) */
/* now it accepts either '-' or '/' as switch character */

/*
   Newsgroups: mod.std.unix
   Subject: public domain AT&T getopt source
   Date: 3 Nov 85 19:34:15 GMT

   Here's something you've all been waiting for:  the AT&T public domain
   source for getopt(3).  It is the code which was given out at the 1985
   UNIFORUM conference in Dallas.  I obtained it by electronic mail
   directly from AT&T.  The people there assure me that it is indeed
   in the public domain.
 */

#if defined(_MSC_VER)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ERR(s, c)               if(opterr){fprintf(stderr,"%s%s%c\n",argv[0],s,c);}

int opterr = 1;
int optind = 1;
int optopt;
char *optarg;

int
getopt (int argc, char **argv, char *opts)
{
  static int sp = 1;
  register int c;
  register char *cp;

  if (sp == 1)
    if (optind >= argc ||
	(argv[optind][0] != '-' && argv[optind][0] != '/')
	|| argv[optind][1] == '\0')
      return (EOF);
    else if (strcmp (argv[optind], "--") == 0 ||
	     strcmp (argv[optind], "//") == 0)
      {
	optind++;
	return (EOF);
      }
  optopt = c = argv[optind][sp];
  if (c == ':' || (cp = strchr (opts, c)) == NULL)
    {
      ERR (": illegal option -- ", c);
      if (argv[optind][++sp] == '\0')
	{
	  optind++;
	  sp = 1;
	}
      return ('?');
    }
  if (*++cp == ':')
    {
      if (argv[optind][sp + 1] != '\0')
	optarg = &argv[optind++][sp + 1];
      else if (++optind >= argc)
	{
	  ERR (": option requires an argument -- ", c);
	  sp = 1;
	  return ('?');
	}
      else
	optarg = argv[optind++];
      sp = 1;
    }
  else
    {
      if (argv[optind][++sp] == '\0')
	{
	  sp = 1;
	  optind++;
	}
      optarg = NULL;
    }
  return (c);
}

#endif
