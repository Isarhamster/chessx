/////////////////////////////////////////////////////////////////////
//
//  FILE:       tkscid.h
//              Scid extensions to Tcl/Tk interpreter
//
//  Part of:    Scid (Shane's Chess Information Database)
//
//  Notice:     Copyright (c) 1999-2004 Shane Hudson.  All rights reserved.
//              Copyright (c) 2006-2007 Pascal Georges
//              Copyright (c) 2013 Benini Fulvio
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////


#include "tcl.h"

class Progress;
struct scidBaseT;


// Macro TCL_ARGS expands to the argument-type list that any
// Tcl command function takes.
#define TCL_ARGS  ClientData cd, Tcl_Interp * ti, int argc, const char ** argv


int sc_eco_base       (TCL_ARGS);
int sc_eco_game       (TCL_ARGS);
int sc_eco_read       (TCL_ARGS);
int sc_eco_summary    (TCL_ARGS);
int sc_eco_translate  (TCL_ARGS);

int sc_filter_first   (TCL_ARGS);
int sc_filter_freq    (scidBaseT* dbase, const HFilter& filter, Tcl_Interp * ti, int argc, const char ** argv);
int sc_filter_last    (TCL_ARGS);
int sc_filter_next    (TCL_ARGS);
int sc_filter_prev    (TCL_ARGS);
int sc_filter_stats   (TCL_ARGS);

int sc_game_crosstable (TCL_ARGS);
int sc_game_find      (TCL_ARGS);
int sc_game_firstMoves (TCL_ARGS);
int sc_game_import    (TCL_ARGS);
int sc_game_info      (TCL_ARGS);
int sc_game_load      (TCL_ARGS);
int sc_game_merge     (TCL_ARGS);
int sc_game_moves     (TCL_ARGS);
int sc_game_novelty   (TCL_ARGS);
int sc_game_new       (TCL_ARGS);
int sc_game_pgn       (TCL_ARGS);
int sc_game_pop       (TCL_ARGS);
int sc_game_push      (TCL_ARGS);
int sc_game_save      (TCL_ARGS);
int sc_game_startBoard (TCL_ARGS);
int sc_game_strip     (TCL_ARGS);
int sc_game_tags      (TCL_ARGS);
int sc_game_tags_get  (TCL_ARGS);
int sc_game_tags_set  (TCL_ARGS);
int sc_game_tags_reload (TCL_ARGS);
int sc_game_tags_share (TCL_ARGS);

int sc_info_limit     (TCL_ARGS);
int sc_info_suffix    (TCL_ARGS);
int sc_info_tb        (TCL_ARGS);
int sc_info_priority  (TCL_ARGS);

int sc_move_add       (TCL_ARGS);
int sc_move_addSan    (TCL_ARGS);
int sc_move_addUCI    (TCL_ARGS);
int sc_move_back      (TCL_ARGS);
int sc_move_forward   (TCL_ARGS);
int sc_move_pgn       (TCL_ARGS);

int sc_report_create  (TCL_ARGS);
int sc_report_select  (TCL_ARGS);

int sc_pos_addNag     (TCL_ARGS);
int sc_pos_analyze    (TCL_ARGS);
int sc_pos_bestSquare (TCL_ARGS);
int sc_pos_getNags    (TCL_ARGS);
int sc_pos_hash       (TCL_ARGS);
int sc_pos_html       (TCL_ARGS);
int sc_pos_isAt       (TCL_ARGS);
int sc_pos_isLegal    (TCL_ARGS);
int sc_pos_isPromo    (TCL_ARGS);
int sc_pos_matchMoves (TCL_ARGS);
int sc_pos_moves      (TCL_ARGS);
int sc_pos_probe      (TCL_ARGS);
int sc_pos_probe_board (TCL_ARGS);
int sc_pos_setComment (TCL_ARGS);

int sc_tree_stats    (TCL_ARGS);
int sc_tree_cachesize (TCL_ARGS);
int sc_tree_cacheinfo (TCL_ARGS);

int sc_var_delete     (TCL_ARGS);
int sc_var_delete_free(TCL_ARGS);
int sc_var_enter      (TCL_ARGS);
int sc_var_first      (TCL_ARGS);
int sc_var_list       (TCL_ARGS);

errorT search_index(const scidBaseT* base, HFilter& filter, int argc, const char ** argv, const Progress& progress);
int sc_search_board   (Tcl_Interp* ti, const scidBaseT* dbase, HFilter filter, int argc, const char** argv);
int sc_search_material (TCL_ARGS);
int sc_search_header  (ClientData cd, Tcl_Interp * ti, scidBaseT* base, HFilter& filter, int argc, const char ** argv);
int sc_search_rep_add (TCL_ARGS);
int sc_search_rep_go  (TCL_ARGS);

int sc_book_load      (TCL_ARGS);
int sc_book_close     (TCL_ARGS);
int sc_book_moves     (TCL_ARGS);
int sc_book_positions (TCL_ARGS);
int sc_book_update    (TCL_ARGS);
int sc_book_movesupdate (TCL_ARGS);
//////////////////////////////////////////////////////////////////////
/// END of tkscid.h
//////////////////////////////////////////////////////////////////////
