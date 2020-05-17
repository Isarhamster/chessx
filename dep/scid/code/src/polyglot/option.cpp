
// option.cpp

// includes

#ifdef WINCE
#include <stdlib.h>
#else
#include <cstdlib>
#endif
#include <cstring>

#include "option.h"
#include "util.h"

// constants

static const bool UseDebug = false;

// types

struct option_t {
   const char * var;
   const char * val;
};

// variables

static option_t Option[] = {

   { "OptionFile",    NULL, }, // string

   // options

   { "EngineName",    NULL, }, // string
   { "EngineDir",     NULL, }, // string
   { "EngineCommand", NULL, }, // string

   { "Log",           NULL, }, // true/false
   { "LogFile",       NULL, }, // string

   { "Chess960",      NULL, }, // true/false

   { "Resign",        NULL, }, // true/false
   { "ResignMoves",   NULL, }, // move number
   { "ResignScore",   NULL, }, // centipawns

   { "MateScore",     NULL, }, // centipawns

   { "Book",          NULL, }, // true/false
   { "BookFile",      NULL, }, // string

   { "BookRandom",    NULL, }, // true/false
   { "BookLearn",     NULL, }, // true/false

   { "KibitzMove",    NULL, }, // true/false
   { "KibitzPV",      NULL, }, // true/false

   { "KibitzCommand", NULL, }, // string
   { "KibitzDelay",   NULL, }, // seconds

   { "ShowPonder",    NULL, }, // true/false

   // work-arounds

   { "UCIVersion",    NULL, }, // 1-
   { "CanPonder",     NULL, }, // true/false
   { "SyncStop",      NULL, }, // true/false
   { "PromoteWorkAround", NULL, }, // true/false

   // { "",              NULL, },

   { NULL,            NULL, },
};

// prototypes

static option_t * option_find (const char var[]);

// functions

// option_init()

void option_init() {

   option_set("OptionFile","polyglot.ini");

   // options

   option_set("EngineName","<empty>");
   option_set("EngineDir",".");
   option_set("EngineCommand","<empty>");

   option_set("Log","false");
   option_set("LogFile","polyglot.log");

   option_set("Chess960","false");

   option_set("Resign","false");
   option_set("ResignMoves","3");
   option_set("ResignScore","600");

   option_set("MateScore","10000");

   option_set("Book","false");
   option_set("BookFile","book.bin");

   option_set("BookRandom","true");
   option_set("BookLearn","false");

   option_set("KibitzMove","false");
   option_set("KibitzPV","false");

   option_set("KibitzCommand","tellall");
   option_set("KibitzDelay","5");

   option_set("ShowPonder","true");

   // work-arounds

   option_set("UCIVersion","2");
   option_set("CanPonder","false");
   option_set("SyncStop","false");
   option_set("PromoteWorkAround","false");

   // option_set("","");
}

// option_set()

bool option_set(const char var[], const char val[]) {

   option_t * opt;

   ASSERT(var!=NULL);
   ASSERT(val!=NULL);

   opt = option_find(var);
   if (opt == NULL) return false;

   my_string_set(&opt->val,val);

   if (UseDebug) my_log("POLYGLOT OPTION SET \"%s\" -> \"%s\"\n",opt->var,opt->val);

   return true;
}

// option_find()

static option_t * option_find(const char var[]) {

   option_t * opt;

   ASSERT(var!=NULL);

   for (opt = &Option[0]; opt->var != NULL; opt++) {
      if (my_string_case_equal(opt->var,var)) return opt;
   }

   return NULL;
}

// end of option.cpp

