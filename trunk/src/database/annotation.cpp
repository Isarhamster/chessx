/****************************************************************************
*   Copyright (C) 2019 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "annotation.h"

const char* s_tan = "\\[%(egt|emt|clk)\\s*(\\d?\\d:\\d?\\d:\\d\\d)\\]";
const char* s_emt = "\\[%(emt)\\s*(\\d?\\d:\\d?\\d:\\d\\d)\\]"; //
const char* s_clk = "\\[%(clk)\\s*(\\d?\\d:\\d?\\d:\\d\\d)\\]"; // Clock
const char* s_egt = "\\[%(egt)\\s*(\\d?\\d:\\d?\\d:\\d\\d)\\]"; // Elapsed game time
const char* s_can = "\\[%(csl|cal)\\s*([^\\]]*)\\]";
const char* s_csl = "\\[%(csl)\\s*([^\\]]*)\\]";
const char* s_cal = "\\[%(cal)\\s*([^\\]]*)\\]"; // Arrow
