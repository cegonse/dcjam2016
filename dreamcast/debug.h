#ifndef _DEBUG_H_
#define _DEBUG_H

#include <kos.h>

/*
 * Several debug methods:
 * - debug_log: display a general message (no severity).
 * - debug_warn: display a warning message (average severity).
 * - debug_error: display a potentially game breaking message (high severity).
*/
void debug_log(const char* str);
void debug_warn(const char* str);
void debug_error(const char* str);

#endif // _DEBUG_H_