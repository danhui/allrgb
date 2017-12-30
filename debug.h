#pragma once

#include <cstdio>
#include <cassert>

// debug() is like printf, except it takes an additional level for debug level,
// where a higher number results in more verbose output.
// The difference between debug and debug_v is that debug_v includes file name,
// line number, and function name.
// Source: http://stackoverflow.com/a/1644898

// A gotcha: additional arguments (i.e. variables for debug output) are
// mandatory, debug_l serves to sidestep that.

#ifdef DEBUG

// Normal debug.
#define debug(level, fmt, ...) \
  do { if (DEBUG && DEBUG >= level) \
    fprintf(stderr, fmt, __VA_ARGS__); } while (0)

// Debug with line number, file name, etc.
#define debug_v(level, fmt, ...) \
  do { if (DEBUG && DEBUG >= level) { \
    fprintf(stderr, "%s:%d:%s(): ", __FILE__, __LINE__, __func__); \
    fprintf(stderr, fmt, __VA_ARGS__); } } while (0)

// Debug with no variables to output.
#define debug_l(level) \
  do { if (DEBUG && DEBUG >= level) \
    fprintf(stderr, "%s:%d:%s()", __FILE__, __LINE__, __func__); } while (0)

// If debug level >= level, return true.
#define d_check(level) \
  (DEBUG && DEBUG >= level ? 1 : 0)

#else

#define debug(level, fmt, ...) 0
#define debug_v(level, fmt, ...) 0
#define debug_l(level) 0
#define d_check(level) 0

#endif
