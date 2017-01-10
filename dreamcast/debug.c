#include "debug.h"

void debug_log(const char* str)
{
	dbglog(DBG_DEBUG, str);
}

void debug_warn(const char* str)
{
	dbglog(DBG_WARNING, str);
}

void debug_error(const char* str)
{
	dbglog(DBG_ERROR, str);
}