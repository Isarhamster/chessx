/*
 * Copyright (c) 1999-2004 Shane Hudson
 * Copyright (c) 2018 Fulvio Benini
 *
 * This file is part of Scid (Shane's Chess Information Database).
 *
 * Scid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 *
 * Scid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Scid. If not, see <http://www.gnu.org/licenses/>.
 */

#if defined(WIN32) || defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#undef ERROR
#else
#include <sys/resource.h>
#include <sys/time.h>
#endif

#include "ui.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_info_priority:
//   This gets or sets the priority class of a process.
UI_res_t sc_info_priority(UI_extra_t, UI_handle_t ti, int argc,
                          const char** argv) {
	const char* usage = "Usage: sc_info priority <pid> [normal|idle]";
	if (argc < 3 || argc > 4)
		UI_Result(ti, ERROR_BadArg, usage);

	int pid = strGetInteger(argv[2]);

	bool idlePriority = false;
	if (argc == 4) {
		switch (argv[3][0]) {
		case 'i':
			idlePriority = true;
			break;
		case 'n':
			idlePriority = false;
			break;
		default:
			return UI_Result(ti, ERROR_BadArg, usage);
		}
	}

#ifdef WIN32
	// Try to obtain a process handle for setting the priority class:
	HANDLE hProcess = OpenProcess(
	    PROCESS_SET_INFORMATION | PROCESS_QUERY_INFORMATION, false, pid);
	if (hProcess == NULL)
		return UI_Result(ti, ERROR, "Unable to set process priority.");

	if (argc == 4)
		SetPriorityClass(hProcess, idlePriority ? IDLE_PRIORITY_CLASS
		                                        : NORMAL_PRIORITY_CLASS);

	uint priorityClass = GetPriorityClass(hProcess);
	CloseHandle(hProcess);

	return UI_Result(ti, OK, priorityClass == NORMAL_PRIORITY_CLASS ? 0 : 15);

#else  // #ifdef WIN32
	if (argc == 4) {
		// Try to assign a new priority:
		if (setpriority(PRIO_PROCESS, pid, idlePriority ? 15 : 0) != 0)
			return UI_Result(ti, ERROR, "Unable to set process priority.");
	}
	// Now return the process priority:
	int priority = getpriority(PRIO_PROCESS, pid);
	return UI_Result(ti, OK, priority);
#endif // #ifdef WIN32
}
