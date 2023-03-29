#ifndef  KONPU_PLATFORM_H
#define  KONPU_PLATFORM_H
// platform checks (and potential includes)


//===< PLATFORM SDL2 >==========================================================

// normalize: set to 1 if symbol is defined but has no value
#if defined(KONPU_PLATFORM_SDL2) && ~(~KONPU_PLATFORM_SDL2 + 0) == 0 && ~(~KONPU_PLATFORM_SDL2 + 1) == 1
#   undef  KONPU_PLATFORM_SDL2
#   define KONPU_PLATFORM_SDL2        1
#endif
// input value checks:
#if defined(KONPU_PLATFORM_SDL2) && (KONPU_PLATFORM_SDL2 != 0) && (KONPU_PLATFORM_SDL2 != 1)
#   error("KONPU_PLATFORM_SDL2 defined but not set to 0 or 1")
#endif
// default value:
#ifndef KONPU_PLATFORM_SDL2
#   define KONPU_PLATFORM_SDL2        0
#endif
// action if enabled: include "SDL.h"
#if KONPU_PLATFORM_SDL2
#   include "SDL.h"
#endif
//===</ PLATFORM SDL2 >=========================================================



//===< PLATFORM POSIX >=========================================================
// (only checked a little bit on Linux with GNU's libc)

// normalize: set to 1 if symbol is defined but has no value
#if defined(KONPU_PLATFORM_POSIX) && ~(~KONPU_PLATFORM_POSIX + 0) == 0 && ~(~KONPU_PLATFORM_POSIX + 1) == 1
#   undef  KONPU_PLATFORM_POSIX
#   define KONPU_PLATFORM_POSIX       1
#endif
// input value checks:
#if defined(KONPU_PLATFORM_POSIX) && (KONPU_PLATFORM_POSIX < 0)
#   error("invalid KONPU_PLATFORM_POSIX value (is < 0)")
#endif
#if defined(_POSIX_C_SOURCE) && ~(~_POSIX_C_SOURCE + 0) == 0 && ~(~_POSIX_C_SOURCE + 1) == 1
#   error("_POSIX_C_SOURCE is defined but has not value")
#endif
#if defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE < 1)
#   error("invalid _POSIX_C_SOURCE value (is < 1)")
#endif
// default value set:
#ifndef KONPU_PLATFORM_POSIX
#   if KONPU_PLATFORM_SDL2
#      define KONPU_PLATFORM_POSIX    0
#   elif defined(_POSIX_C_SOURCE)
#      define KONPU_PLATFORM_POSIX    _POSIX_C_SOURCE
#   else
#      define KONPU_PLATFORM_POSIX    0
#   endif
#endif
// action if enabled: setup some macro variables to enable C POSIX functions
//                    (such as _POSIX_C_SOURCE, _X_OPEN_SOURCE, ...)
#if KONPU_PLATFORM_POSIX
    // _POSIX_SOURCE macro: was sometimes used to specify/enable POSIX.1
    // It seems unused nowadays if _POSIX_C_SOURCE is defined > 0.
    // (Let's define this anyway just in case)
#   ifndef _POSIX_SOURCE
#      define _POSIX_SOURCE        1
#   endif

    // _POSIX_C_SOURCE macro: may specify/force conformance to POSIX.
    // Format is a date corresponding to a revision of the standard.
#   ifndef _POSIX_C_SOURCE
#      if KONPU_PLATFORM_POSIX  >= 200809L
#         define _POSIX_C_SOURCE   200809L  // POSIX 2008
#      else
#         define _POSIX_C_SOURCE   200112L
#      endif
#   endif
    // ensure minimum value:
    // (200112L would be the min baseline for POSIX that can include C99)
#   if _POSIX_C_SOURCE < 200112L
#      define _POSIX_C_SOURCE      200112L
#   endif

    // _X_OPEN_SOURCE macro: may specify/force conformance to XSI (X/Open System
    // Interfaces Extension), which includes POSIX.
#   ifndef _X_OPEN_SOURCE
#      if KONPU_PLATFORM_POSIX  >= 200809L
#         define _X_OPEN_SOURCE    700  // X/Open 7 (incl. POSIX 2008)
#      else
#         define _X_OPEN_SOURCE    600  // X/Open 5 (incl. POSIX 2004, ie: a
#      endif                            //           minor update of POSIX 2001)
#   endif
    // ensure minimum value:
#   if _X_OPEN_SOURCE < 500
#      define _POSIX_C_SOURCE      600
#   endif

#endif
//===</ PLATFORM POSIX >========================================================



//===< PLATFORM WINDOWS >=======================================================
// (not tested and not supported / MAYBE in future)

// normalize: set to 1 if symbol is defined but has no value
#if defined(KONPU_PLATFORM_WINDOWS) && ~(~KONPU_PLATFORM_WINDOWS + 0) == 0 && ~(~KONPU_PLATFORM_WINDOWS + 1) == 1
#   undef  KONPU_PLATFORM_WINDOWS
#   define KONPU_PLATFORM_WINDOWS     1
#endif
// input value checks:
#if defined(KONPU_PLATFORM_WINDOWS) && (KONPU_PLATFORM_WINDOWS != 0) && (KONPU_PLATFORM_WINDOWS != 1)
#   error("KONPU_PLATFORM_WINDOWS defined but not set to 0 or 1")
#endif
// default value:
#ifndef KONPU_PLATFORM_WINDOWS
#   if KONPU_PLATFORM_SDL2
#      define KONPU_PLATFORM_WINDOWS  0
#   elif defined(_WIN32) && !defined(__CYGWIN__)
#      define KONPU_PLATFORM_WINDOWS  1
#   else
#      define KONPU_PLATFORM_WINDOWS  0
#   endif
#endif
// action if enabled: include <windows.h>
#if KONPU_PLATFORM_WINDOWS
#   define  WIN32_LEAN_AND_MEAN // <--  exclude inclusion of many APIs
#   include <windows.h>
#endif
//===</ PLATFORM WINDOWS >======================================================



//===< PLATFORM LIBC >==========================================================

// normalize: set to 1 if symbol is defined but has no value
#if defined(KONPU_PLATFORM_LIBC) && ~(~KONPU_PLATFORM_LIBC + 0) == 0 && ~(~KONPU_PLATFORM_LIBC + 1) == 1
#   undef  KONPU_PLATFORM_LIBC
#   define KONPU_PLATFORM_LIBC        1
#endif
// input value checks:
#if defined(KONPU_PLATFORM_LIBC) && (KONPU_PLATFORM_LIBC != 0) && (KONPU_PLATFORM_LIBC != 1)
#   error("KONPU_PLATFORM_LIBC defined but not set to 0 or 1")
#endif
// default value:
#ifndef KONPU_PLATFORM_LIBC
#   if KONPU_PLATFORM_SDL2
#      define KONPU_PLATFORM_LIBC     0
#   elif KONPU_PLATFORM_POSIX || KONPU_PLATFORM_WINDOWS
#      define KONPU_PLATFORM_LIBC     1
#   else
#      define KONPU_PLATFORM_LIBC     0
#   endif
#endif
// action if enabled: none
//===</ PLATFORM LIBC >=========================================================


#endif //KONPU_PLATFORM_H
