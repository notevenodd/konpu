#ifndef  KONPU_C_H
#define  KONPU_C_H
#include "platform.h"

/* - include "platform.h"
 * - checks for C
 * - includes most free-standing (libc independent) C headers,
 * - a few "polyfills" to reach C23 level (when possible)
 * - a few c utilities but possibly without libc (assert, ...)
 * - variadic macro utilities
 */

/*===< basic C standard >=====================================================*/

/* we REQUIRE C99 or later: */
#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)
#   error "Compiler doesn't conform to C99 (or later) standard."
#endif

// FREE-STANDING headers: those are includes from the C standard library which
// should be headers-only files, and thus are safe to include on any platform,
// even if when we do not link to libc.

/* C89 free-standing headers */

#  include  <float.h>        // limits of floating-point types
#           if __STDC_VERSION__ == 199901L // C99
               // "polyfill" def. (for C99)
               // Starting from C11, *DECIMAL_DIG exists for float, double, and
               // long double. The "unqualified" DECIMAL_DIG is specifically for
               // long double and gets deprecated in C23, so use this instead:
#              define LDBL_DECIMAL_DIG    DECIMAL_DIG
#           endif

#  include  <limits.h>       // range of integer types
#           if CHAR_BIT != 8 // <-- we REQUIRE 8-bits bytes
#              error "bytes do not have an 8-bits width"
#           endif
#           if __STDC_VERSION__ <= 201710L // "polyfill" def. (for prior to C23)
#              define CHAR_WIDTH   CHAR_BIT
#              define SCHAR_WIDTH  CHAR_BIT
#              define UCHAR_WIDTH  CHAR_BIT
#           endif

#  include  <stdarg.h>       // variable arguments

#  include  <stddef.h>       // common macro definitions

/* C95 free-standing headers */
//#include  <iso646.h>       // alt-spelling of operators
            // we do not include this.

/* C99 free-standing headers */

//#include  <stdbool.h>      // "convenience" macros for bool/true/false
#           if __STDC_VERSION__ <= 201710L   // only needed prior to C23
#              include <stdbool.h>
#           endif

#  include  <stdint.h>       // fixed-width integer types  (extra support below)
            // we REQUIRE (u)int8_t, (u)int16_t, (u)int32_t, (u)int64_t:
            // (we don't have static_assert for sure yet, so let's do it portably):
            /*[[maybe_unused]]*/ typedef char UTIL_static_assert_has_int8  [(sizeof( int_least8_t ) == 1)? 1 : -1];
            /*[[maybe_unused]]*/ typedef char UTIL_static_assert_has_uint8 [(sizeof(uint_least8_t ) == 1)? 1 : -1];
            /*[[maybe_unused]]*/ typedef char UTIL_static_assert_has_int16 [(sizeof( int_least16_t) == 2)? 1 : -1];
            /*[[maybe_unused]]*/ typedef char UTIL_static_assert_has_uint16[(sizeof(uint_least16_t) == 2)? 1 : -1];
            /*[[maybe_unused]]*/ typedef char UTIL_static_assert_has_int32 [(sizeof( int_least32_t) == 4)? 1 : -1];
            /*[[maybe_unused]]*/ typedef char UTIL_static_assert_has_uint32[(sizeof(uint_least32_t) == 4)? 1 : -1];
            /*[[maybe_unused]]*/ typedef char UTIL_static_assert_has_int64 [(sizeof( int_least64_t) == 8)? 1 : -1];
            /*[[maybe_unused]]*/ typedef char UTIL_static_assert_has_uint64[(sizeof(uint_least64_t) == 8)? 1 : -1];
#           if __STDC_VERSION__ <= 201710L // "polyfill" def. (for prior to C23)
#              define INT8_WIDTH            8
#              define INT16_WIDTH          16
#              define INT32_WIDTH          32
#              define INT64_WIDTH          64
#              define INT_LEAST8_WIDTH      8
#              define INT_LEAST16_WIDTH    16
#              define INT_LEAST32_WIDTH    32
#              define INT_LEAST64_WIDTH    64
#              define UINT8_WIDTH           8
#              define UINT16_WIDTH         16
#              define UINT32_WIDTH         32
#              define UINT64_WIDTH         64
#              define UINT_LEAST8_WIDTH     8
#              define UINT_LEAST16_WIDTH   16
#              define UINT_LEAST32_WIDTH   32
#              define UINT_LEAST64_WIDTH   64
#           endif

/* C11 free-standing headers: ("convenience" macros) */

//#include  <stdalign.h>     // alignas/alignof
#           if __STDC_VERSION__ < 201112L    // C99
               // In C99, we canNOT have alignas/alignof :(
               // but about alignement, at this point in the header, we have the
               // types needed to TRY to "polyfill" <stddef.h> up to C11-level,
               // and provide max_align_t for C99: max_align_t would usually be
               // the largest scalar type, let's try to provide that, but that
               // could fail. Frankly you should not rely on this if you use C99
               typedef union max_align_t {
                  intmax_t      big_int;
#                 ifdef __SIZEOF_INT128__
                     __int128   128bits_int;
#                 endif
                  long double   big_double;
                  void         *pointer;
                  void        (*function_pointer)();
               } max_align_t; // hopefully, that works
#           elif __STDC_VERSION__ <= 201710L // C11 or C17: includes the header
#              include <stdalign.h>
#           endif                            // C23: has native alignas/alignof

//#include  <stdnoreturn.h>  // noreturn macro for _Noreturn keyword
//          Going forward, C23 *deprecates* the <stdnoreturn.h> header/noreturn
//          macro and the _Noreturn keyword too, because noreturn becomes a C
//          attribute: [[noreturn]].
//
//          1. at least provide _Noreturn keyword for C99:
#           if (__STDC_VERSION__ < 201112L) && !defined(_Noreturn)
#              if defined(__GNUC__)   // gcc:
#                 define _Noreturn    __attribute__((noreturn))
#              elif defined(_MSC_VER) // microsoft:
#                 define _Noreturn    __declspec(noreturn)
#              else                   // otherwise:
#                 define _Noreturn    //nothing
#              endif
#           endif
//          2. because C starts using noreturn as an attribute name, it may be
//             *better to just use `_Noreturn` rather than `noreturn`, ...
//             but let's anyway alias noreturn to _Noreturn for C prior to C23
#           if __STDC_VERSION__ <= 201710L
#              ifndef noreturn
#                 define noreturn _Noreturn // deprecated
#              endif
#           endif

/* C23 free-standing headers */

#if __STDC_VERSION__ > 201710L
//TODO: seems those are just macros, no functions.
//      are those free standing headers or not? if so, we could include them.
//#   include <stdbit.h>       // bit-manipulation utilities
//#   include <stdckdint.h>    // checked integer arithmetic
#endif

//===</ basic C standard >======================================================



//===< variadic macros and overloading >========================================
// Utilities using variadic macros.
// The most important one is UTIL_OVERLOAD, which supports macros/functions
// "overloading" based on the number of parameters.  (note: C11 can also do type
//                          overloading with _Generic, but that's another story)


// UTIL_STRINGIFY: triggers macro expansion and turns into a string constant.
#define UTIL_STRINGIFY(x)               UTIL_STRINGIFY_(x)
        #define UTIL_STRINGIFY_(x)      #x

// UTIL_CAT: trigger macro expansion and concatenate its parameters.
// It can be useful to use to create other macros.
#define UTIL_CAT(item, ...)             UTIL_CAT_(item, __VA_ARGS__)
        #define UTIL_CAT_(item, ...)    item##__VA_ARGS__

// UTIL_COUNT expands to the number of its arguments (works for 1-50 parameters)
#define UTIL_COUNT(...)                         \
        UTIL_51TH_ARG_(__VA_ARGS__,             \
        50, 49, 48, 47, 46, 45, 44, 43, 42, 41, \
        40, 39, 38, 37, 36, 35, 34, 33, 32, 31, \
        30, 29, 28, 27, 26, 25, 24, 23, 22, 21, \
        20, 19, 18, 17, 16, 15, 14, 13, 12, 11, \
        10,  9,  8,  7,  6,  5,  4,  3,  2,  1, 0, UTIL_COUNT__TOO_MANY_ARGS)
        // macro that expands to its 51th argument
        // (this is meant to be called from UTIL_COUNT)
        #define UTIL_51TH_ARG_(                                   \
                N01, N02, N03, N04, N05, N06, N07, N08, N09, N10, \
                N11, N12, N13, N14, N15, N16, N17, N18, N19, N20, \
                N21, N22, N23, N24, N25, N26, N27, N28, N29, N30, \
                N31, N32, N33, N34, N35, N36, N37, N38, N39, N40, \
                N41, N42, N43, N44, N45, N46, N47, N48, N49, N50, \
                N, ...)  N

// UTIL_OVERLOAD is a macro that performs "variadic overloading"
// [ it expands to the name of a given function prefixed by _N_
//   (where N is the number of arguments) and applies its argument to it ]
//
// Here's an example of variadic overloading:
//    int myfunction_2_(int a, int b);         // function with two parameters
//    int myfunction_3_(int a, int b, int c);  // function with three parameters
// define variadic overload:
//    #define myfunction(...)  UTIL_OVERLOAD(myfunction, __VA_ARGS__)
// so now I can call all myfunction_*_ using a unique name:
//    myfunction(10,20);    // will call myfunction_2_
//    myfunction(10,20,30); // will call myfunction_3_
//
// hint: it's powerful, but don't abuse it, and you don't have to use it at all.
//
#define UTIL_OVERLOAD(function, ...) \
        UTIL_CAT(UTIL_CAT(function##_, UTIL_COUNT(__VA_ARGS__)), _)(__VA_ARGS__)

//===</ variadic macros and overloading >=======================================



//===< nullptr >================================================================
#if __STDC_VERSION__ <= 201710L
    // prior to C23, this doesn't exist
    // **BEWARE**, this is an attempt at forward compatibility compiling newer
    // code which would use nullptr. But there's one crucial difference, the
    // type of nullptr is void* and NOT nullptr_t. There are only few places it
    // would matter, but for exmaple _Generic could be one of them.
    #define nullptr  ((void*)0)
#endif
//==============================================================================



//===< char8_t, char16_t, char32_t (<uchar.h>) >================================

// char8_t is defined in C23 in <uchar.h> as a UTF-8 character type and is
// basically an alias for unsigned char, so let's typedef it as such for older
// C versions.
#if __STDC_VERSION__ <= 201710L // prior to C23
    typedef  unsigned char   char8_t;
#endif

// char16_t, char32_t are defined in C11 in <uchar.h> as respectively 16-bits
// and 32-bits character types. For C99, let's typedef those.
// C23 mentions
#if __STDC_VERSION__ == 199901L // C99
    typedef  uint_least16_t  char16_t;
    typedef  uint_least32_t  char32_t;
#endif

//===</ char8_t, char16_t, char32_t (<uchar.h>) >===============================



//===< uint128_t >==============================================================
// an (u)int128_t type may be defined if such type is available,
// So let's bring it in case it's available to the compiler.
// To test the type availability, one can use #ifdef INT128_WIDTH

#if __STDC_VERSION__ <= 201710L // prior to C23
#   ifdef __SIZEOF_INT128__
       // ^-- when this is defined, we assume compiler has a __int128 type
       //     available (this is the case in gcc)
       typedef  signed   __int128    int128_t;
       typedef  unsigned __int128    uint128_t;
       typedef  signed   __int128    int_fast128_t;
       typedef  unsigned __int128    uint_fast128_t;
       typedef  signed   __int128    int_least128_t;
       typedef  unsigned __int128    uint_least128_t;
#      define   UINT128_MAX          ((uint128_t)-1)
#      define   INT128_MAX           ((int128_t)+(UINT128_MAX/2))
#      define   INT128_MIN           (-INT128_MAX-1) //TODO: hmmm, looks UB-ish
#      define   UINT_LEAST128_MAX    UINT128_MAX
#      define   INT_LEAST128_MAX     INT128_MAX
#      define   INT_LEAST128_MIN     INT128_MIN
#      define   UINT_FAST128_MAX     UINT128_MAX
#      define   INT_FAST128_MAX      INT128_MAX
#      define   INT_FAST128_MIN      INT128_MIN
#      define   INT128_WIDTH         128
#      define   UINT128_WIDTH        128
#      define   INT_LEAST128_WIDTH   128
#      define   UINT_LEAST128_WIDTH  128
#      define   INT_FAST128_WIDTH    128
#      define   UINT_FAST128_WIDTH   128
#   endif
#endif
//===</ uint128_t >=============================================================


//===< unreachable() >==========================================================
// The C23+ function-like macro unreachable().
// calling unreachable() results in undefined behaviour. It should never be
// executed and its purpose is to be used as a hint the compiler could use to
// optimize "impossible" branches away.
//
// For example, if we know *for sure*, 3 < val <= 0, we can do:
// switch (val) {
//   case  0: ...  break;
//   case  1: ...  break;
//   case  2: ...  break;
//   default: unreachable();
// }
// and compiler may do a simple lookup with no other testing

#if __STDC_VERSION__ > 201710L
    // ^^-- >C17, ie C23 or later. unreachable() was defined in <stddef.h>
#
#elif defined(__GNUC__)
#   define unreachable()   (__builtin_unreachable())
#elif defined(_MSC_VER) // MSVC
#   define unreachable()   (__assume(false))
#else
    // triggers UB if code gets executed:
    static inline _Noreturn void unreachable(void) { *((int*)0) = 1/0; } //volontary UB
#endif
//===</ unreachable() >=========================================================



//===< assert(condition) >======================================================
//
// macro which (may) evaluate a condition and terminates the program if
// evaluation is zero.
// - if NDEBUG is defined, expands to nothing
// - otherwise if platform is SDL, uses SDL_assert,
// - otherwise if C's library platform is enabled, then takes it from <assert.h>,
// - otherwise expands to nothing.

#ifdef NDEBUG
#   define assert(condition)   ((void)0)
#elif KONPU_PLATFORM_SDL2
#   define assert(condition)   SDL_assert(condition)
#elif KONPU_PLATFORM_LIBC
#   include <assert.h>
#else // with no platform, no can do
      // or TODO could we conceivably write an error message on the main screen?
#   define assert(condition)   ((void)0)
#endif

//===</ assert(condition) >=====================================================



//===< static_assert(condition, [message]) >====================================
// 'static_assert' as exists now in C, but "pollyfill"-ed for all versions of C
//
// condition is evaluated at compile time and iff it's zero, then compile-time
// error occurs.

#if __STDC_VERSION__ > 201710L // C23 or later
    // nothing (static_assert is a keyword)
#elif (__STDC_VERSION__ > 199901L) || defined(__GNUC__)
    // ^^-- C11 and C17 (and gcc) have _Static_assert(expression, message)
#   define STATIC_ASSERT_2_(condition, msg)  _Static_assert(condition, msg)
#   define STATIC_ASSERT_1_(condition)       _Static_assert(condition, UTIL_STRINGIFY(__LINE__))
#   undef  static_assert // <-- this may have been defined as a macro from <assert.h>
#   define static_assert(...)                UTIL_OVERLOAD(STATIC_ASSERT, __VA_ARGS__)
#else
    // pre-C11 pure C solution
    // caveat: this macro does NOT work inside structs/unions
#   define STATIC_ASSERT_2_(condition, msg)  \
           /*[[maybe_unused]]*/ typedef char UTIL_static_assertion_##msg [(condition)? 1 : -1]
#   define STATIC_ASSERT_1_(condition)  \
           /*[[maybe_unused]]*/ typedef char UTIL_CAT(UTIL_static_assertion_line_, __LINE__) [(condition)? 1 : -1]
#   undef  static_assert  // just in case
#   define static_assert(...)                UTIL_OVERLOAD(STATIC_ASSERT, __VA_ARGS__)
#endif

//===</ static_assert(condition, [message]) >===================================



//===< thread_local >===========================================================
// konpu doesn't use real multithreading, but anyway let's support the keyword
// thread_local from C (properly introduced in C23)
// C11 and C17: even if compiler defines __STDC_NO_THREADS__, it must define a
//              _Thread_local keyword

#if __STDC_VERSION__ < 201112L // prior to C11
#   if defined(__GNUC__)
#      define thread_local  __thread
#   elif defined(_MSC_VER)
#      define thread_local  __declspec(thread)
#   endif
#elif __STDC_VERSION__ <= 201710L // C11 and C17
#   ifndef thread_local // <-- might already be defined from <threads.h>
#      define thread_local  _Thread_local
#   endif
#endif
//===</ thread_local >==========================================================



//===< likely(condition) / unlikely(condition) >================================

// likely and unlikely is not defined in C standard, but is still widely used as
// optimization hint to tell the compiler to favour the "likely" side of a
// branch.

// SOMEDAY/MAYBE: Note that [[likely]] and [[unlikely]] are defined as
//                attributes in C++ (C++20), so syntax can clash, thus I may
//                need to change the names if I seek C++ compatibility.

#if defined(__GNUC__)
#   define likely(condition)    __builtin_expect((condition), 1)
#   define unlikely(condition)  __builtin_expect((condition), 0)
#else
#   define likely(condition)    (condition)
#   define unlikely(condition)  (condition)
#endif

//===</ likely(condition) / unlikely(condition) >================================


#endif //KONPU_C_H
