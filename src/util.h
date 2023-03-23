#ifndef  KONPU_UTIL_H
#define  KONPU_UTIL_H
/* minimal general library and misc. utilities
 * - that we used throughout the project,
 * - and/or that we wish to provide to users.
 */

#include "config.h"  // user's config (we always want this)


//===< C STANDARD >=============================================================

#if (! defined(__STDC_VERSION__)) || (__STDC_VERSION__ < 199901L)
#   error "We expect a compiler which conforms to C99 (or later) standard"
#endif

//===</ C STANDARD >============================================================



//===< "FREESTANDING" C >=======================================================
// (minimal) includes from the C standard library: those includes should be
// headers-only files which are safe to include even if we decide not to link
// with libc:

// C99:
#include <float.h>
// #include <iso646.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// C11:
// #include <stdalign.h>
// #include <stdnoreturn.h>

// upcoming C23:
// #include <stdbit.h>

//===</ "FREESTANDING" C >======================================================



//===< VARIADIC OVERLOADING >===================================================
// Utilities using variadic macros.
// The most important one is UTIL_OVERLOAD, which supports macros/functions
// "overloading" based on the number of parameters.  (note: C11 can also do type
//                          overloading with _Generic, but that's another story)

// UTIL_EXPAND is a macro used to trigger macro expansion while concatenating
// its parameters. It can be useful to use to create other macros.
#define UTIL_EXPAND(item, ...)   UTIL_CAT(item, __VA_ARGS__)
#define UTIL_CAT(item, ...)      item##__VA_ARGS__

// UTIL_COUNT expands to the number of its arguments (works for 1-50 parameters)
#define UTIL_COUNT(...)                         \
        UTIL_51TH_ARG(__VA_ARGS__,              \
        50, 49, 48, 47, 46, 45, 44, 43, 42, 41, \
        40, 39, 38, 37, 36, 35, 34, 33, 32, 31, \
        30, 29, 28, 27, 26, 25, 24, 23, 22, 21, \
        20, 19, 18, 17, 16, 15, 14, 13, 12, 11, \
        10,  9,  8,  7,  6,  5,  4,  3,  2,  1, 0, UTIL_COUNT__TOO_MANY_ARGS)
        // macro that expands to its 51th argument
        // (this is meant to be called from UTIL_COUNT)
        #define UTIL_51TH_ARG(                                    \
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
        UTIL_EXPAND(UTIL_EXPAND(function##_, UTIL_COUNT(__VA_ARGS__)), _)(__VA_ARGS__)

//===< VARIADIC OVERLOADING >===================================================



//===< unreachable() >==========================================================
//
// The C23+ function-like macro unreachable(). It results in undefined behaviour
// if executed. So, you should ensure your code doesn't execute unreachable().
// It may be used as an optimization as the compiler can use this to optimize
// impossible code branches away.
//
// For example, if we know *for sure*, 3 < val <= 0, we can do:
// switch (val) {
//   case 0: ... break;
//   case 1: ... break;
//   case 2: ... break;
//   default: unreachable();
// }
// and compiler may do a simple lookup with no other testing

#if (__STDC_VERSION__ > 201710L)
    // ^^-- >C17, ie C23 or later. unreachable() was defined in <stddef.h>
#
#elif defined(__GNUC__)
#   define unreachable()   (__builtin_unreachable())
#elif defined(_MSC_VER) // MSVC
#   define unreachable()   (__assume(false))
#else
#   define unreachable()   do {} while(0)
#endif
//===</ unreachable() >=========================================================


//===< ASSERT and STATIC ASSERT >===============================================
//
// assert(condition)          the regular C assert macro which evaluates the
//                            condition (if NDEBUG isn't defined) and terminates
//                            the program if it's zero.
//
// STATIC_ASSERT(condition)   condition is evaluated at compile time and iff
//                            it's zero, a compile-time error occurs.

// assert(condition);
#ifndef NDEBUG
#   include <assert.h>
    // only includes <assert.h> if we have to, because it might not be a
    // "freestanding" header as it could need to link to libc code which
    // will output diagnostic information on the standard error output.
#else
#   define  assert(condition) ((void)0)
#endif

//TODO: we could just use `static_assert` instead of STATIC_ASSERT in UPPER_CASE
// STATIC_ASSERT(condition);
// ('static_assert' now exists in C but isn't (fully) defined until C23)
#if (__STDC_VERSION__ > 201710L)
    // ^^-- >C17, ie upcoming C23 or later: we already have it as keyword
#   define STATIC_ASSERT(condition)  static_assert(condition)
#elif (__STDC_VERSION__ > 199901L) || defined(__GNUC__)
    // ^^-- strictly superior to C99, thus C11 and C17
    //      has _Static_assert(expression, message) as a keyword
    //      (and <assert.h> give it the name static_assert)
    //      this is defined in GCC too.
#   define STATIC_ASSERT(condition)  _Static_assert(condition, __LINE__)
#else
    // pre-C11 pure C solution
    // this macro doesn't work inside structs/unions
#   define STATIC_ASSERT(condition)  \
           typedef char UTIL_EXPAND(static_assertion_line_, __LINE__) [(condition)? 1 : -1]
#endif

//===</ ASSERT and STATIC ASSERT >==============================================


//===< FIXED-WIDTH INTEGERS >===================================================
// utilities on fixed-width unsigned ints, including some of those:
// clz     : count leading  zero bits (but: result is undefined if 0)
// ctz     : count trailing zero bits (but: result is undefined if 0)
// popcount: count the number of bits set
// bswap   : swap bits
// reverse : reverse every bit
// hamming : this is the "hamming distance" of two bit sequences,
//           it counts positions at which the bits differ, ie: the min. number
//           of substitutions required to change one sequence into the other.
//           It provides a "distance" to tell "how similar" two glyphs are.
//------------------------------------------------------------------------------

// we assume a byte is 8 bits (which is virtually ture everywhere nowadays)
//TODO: is this really important?
#if (CHAR_BIT != 8)
#   error "byte doesn't have 8 bits (CHAR_BIT != 8)"
#endif

// bit/byte selection macros:
// - 'x' *MUST* be of a unsigned integer type (otherwise undefined behaviour)
// - 'n' should be >= 0 and less than the number of bits of the 'uint' type
#define uint_bit( x, n)             (((x) >>   (n)) & 1U   )
#define uint_byte(x, n)             (((x) >> 8*(n)) & 0xFFU)

// this macro merges bits from two unsigned integers according to a mask.
// the mask should contains 0 where bits from x are selected, 1 where from y.
#define UINT_MERGE(x, y, mask)      ((x) ^ (((x) ^ (y)) & (mask)))
        // the obvious way would be (x & ~mask) | (y & mask), but this may
        // achieve the same with one operation less.
        // see: https://graphics.stanford.edu/~seander/bithacks.html#MaskedMerge
        // (sent there by Ron Jeffery)  (code snippet is in the public domain)

// reverse bits in a byte
#define byte_reverse(byte) \
        (((byte) * 0x0202020202ULL & 0x010884422010ULL) % 1023)
        // using 3 operations and using 64-bit for * and %), see:
        // http://graphics.stanford.edu/~seander/bithacks.html#ReverseByteWith64BitsDiv
        // The method was attributed to Rich Schroeppel in the Programming Hacks
        // section of Beeler, M., Gosper, R. W., and Schroeppel, R. HAKMEM.
        // MIT AI Memo 239, Feb. 29, 1972.
        // (code snippet is in the public domain)

#if (defined(__GNUC__) || defined(__clang__))
    // might not be optimal for mere bytes,
    // but if it's almost just one assembly instructions, it's fine.
    #define byte_clz(x)                __builtin_clz((x) << 24)
                                       /*^FIXME*/ STATIC_ASSERT(sizeof(int)==4);
                                       //'------- I'm adding the 24 so the byte
                                       //         is in leading position as we
                                       //         assumes int is 32 bits. OUCH!
    #define byte_ctz(x)                __builtin_ctz(x)
    #define byte_popcount(x)           __builtin_popcount(x)
#else
    # error "clz/ctz/popcount for bytes rely on GCC/CLANG builtins"
    // SOMEDAY/MAYBE: provide portable C implementation,
    //                (it may take the forms of precomputed tables!)
#endif

// 'int' is garantueed to be at minimum 16-bit
#define UINT16(literal)                ((uint16_t)##literal)
#if (defined(__GNUC__) || defined(__clang__))
//  #define uint16_clz(x)              __builtin_clz(x) // would only work if int is exactly 16 bits
    #define uint16_ctz(x)              __builtin_ctz(x)
    #define uint16_popcount(x)         __builtin_popcount(x)
    #define uint16_bswap(x)            __builtin_bswap16(x)
#else
    # error "ctz/popcount/bswap for uint16_t rely on GCC/CLANG builtins"
    // SOMEDAY/MAYBE: provide portable C implementation.
#endif
#define uint16_hamming_distance(x, y)  uint16_popcount((x) ^ (y));

// 'long' is garantueed to be at minimum 32-bit
#define UINT32(literal)                ((uint32_t)##literal##UL)
#if (defined(__GNUC__) || defined(__clang__))
//  #define uint32_clz(x)              __builtin_clzl(x)  // would only work if long is exactly 32 bits
    #define uint32_ctz(x)              __builtin_ctzl(x)
    #define uint32_popcount(x)         __builtin_popcountl(x)
    #define uint32_bswap(x)            __builtin_bswap32(x)
#else
    # error "clz/ctz/popcount/bswap for uint32_t rely on GCC/CLANG builtins"
    // SOMEDAY/MAYBE: provide portable C implementation.
#endif
#define uint32_hamming_distance(x, y)  uint32_popcount((x) ^ (y));

// 'long long' is garantueed to be at minimum 64+ bits
// (commonly, on 64bit systems, 'long' might be)
#define UINT64(literal)                ((uint64_t)##literal##ULL)
#if (defined(__GNUC__) || defined(__clang__))
    // long long must be 64+ bits, so use clzll/ctzll instead of clzl/ctzl
    #define uint64_clz(x)              __builtin_clzll(x)
    // TODO     ^^-^^^--- only works if long long is *exactly* 64 bits
    #define uint64_ctz(x)              __builtin_ctzll(x)
    #define uint64_popcount(x)         __builtin_popcountll(x)
    #define uint64_bswap(x)            __builtin_bswap64(x)
#else
    # error "clz/ctz/popcount/bswap for uint64_t rely on GCC/CLANG builtins"
    // SOMEDAY/MAYBE: provide portable C implementation.
    // (note: under MSVC, _BitScanReverse can be used like clz)
#endif
#define uint64_hamming_distance(x, y)  uint64_popcount((x) ^ (y));

//===</ FIXED-WIDTH INTEGERS >==================================================



//===< PSEUDO RANDOM 64-bits NUMBER GENERATOR (PRNG) >==========================
// 64 bits PRNG (STC64):
// (we want to have a 64 bits,so can use it to generate random glyph)
//
// Usage:
//
// 1) Use the global wrapper:
//    uint64_t seed = 123456789;
//    random_init(seed);                   // init with the seed
//    uint64_t i = random();               // get a random number
//    double   f = randomf();              // get a random double [0,1)
//
// 2) Or alternatively, pass the state object around:
//    uint64_t  seed = 123456789;
//    stc64_rng rng  = stc64_init(seed);    // init with the seed
//    uint64_t  i    = stc64_random(&rng);  // get a random number
//    double    f    = stc64_randomf(&rng); // get a random double
//

// I extracted this PRNG from the STC library (https://github.com/tylov/STC),
// so that part of code is under MIT license. I changed a bit the naming, etc.
//
/* MIT License
 *
 * Copyright (c) 2022 Tyge Løvset, NORCE, www.norceresearch.no
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// (hidden) global object:
static inline  void       random_init(uint64_t seed);
static inline  uint64_t   random(void);
static inline  double     randomf(void);

// or explicitely, use by passing around the state:
typedef struct stc64_rng  { uint64_t state[5]; } stc64_rng; // PRNG state
// functions:
static inline  stc64_rng  stc64_init(uint64_t seed);
static inline  uint64_t   stc64_random (stc64_rng* rng);
static inline  double     stc64_randomf(stc64_rng* rng);


//--- inline implementation ----------------------------------------------------

static inline void random_init(uint64_t seed) {
   extern stc64_rng stc64_global; // global stc64 state
   stc64_global = stc64_init(seed);
}

static inline uint64_t random(void) {
   extern stc64_rng stc64_global; // global stc64 state
   return stc64_random(&stc64_global);
}

static inline double randomf(void) {
   extern stc64_rng stc64_global; // global stc64 state
   return stc64_randomf(&stc64_global);
}

/* Init stc64 prng with and without sequence number */
stc64_rng stc64_with_seq(uint64_t seed, uint64_t seq);
static inline stc64_rng stc64_init(uint64_t seed)
   { return stc64_with_seq(seed, seed + 0x3504f333d3aa0b37); }

// Main stc64 prng
static inline uint64_t
stc64_random(stc64_rng* rng) {
    uint64_t *s = rng->state; enum {LR=24, RS=11, LS=3};
    const uint64_t result = (s[0] ^ (s[3] += s[4])) + s[1];
    s[0] = s[1] ^ (s[1] >> RS);
    s[1] = s[2] + (s[2] << LS);
    s[2] = ((s[2] << LR) | (s[2] >> (64 - LR))) + result;
    return result;
}

// Float64 random number in range [0.0, 1.0).
static inline double stc64_randomf(stc64_rng* rng) {
    union {uint64_t i; double f;} u = {0x3FF0000000000000ull | (stc64_random(rng) >> 12)};
    return u.f - 1.0;
}

//===</ PSEUDO RANDOM NUMBER GENERATOR (PRNG) >=================================



//===< MISC. >==================================================================

// Hints to give the compiler to favour the "likely" side of a branch.
// see: https://stackoverflow.com/questions/109710/how-do-the-likely-unlikely-macros-in-the-linux-kernel-work-and-what-is-their-ben
#if (defined(__GNUC__) || defined(__clang__))
#   define likely(condition)    __builtin_expect((condition), 1)
#   define unlikely(condition)  __builtin_expect((condition), 0)
#else
#   define likely(condition)    (condition)
#   define unlikely(condition)  (condition)
#endif

// compile-time constant of type size_t representing the number of elements of
// the given array. ONLY USE ON ARRAYS with size known at compile time (no VLAs)
// DON'T USE ON POINTERS (hint: arrays received as parameters decay to pointers)
#define ARRAY_SIZE(array)       (sizeof((array)) / sizeof((array)[0]))

// `strlen` as a compile time constant for C string literals (BE CAREFUL)
#define CSTR_LENGTH(cstr_lit)   (ARRAY_SIZE(cstr_lit) - 1)


// clamp an *integer* variable of a *signed* type between 0 and a max value
#define UTIL_CLAMP(x, max) do               \
        { if ((x) < 0)        (x) = 0;      \
          if ((x) >= (max))   (x) = (max);  \
        } while (0)

// The typical SWAP macro that swaps two values.
// Classically in C, it also passes the type as parameter, like this:
//    #define UTIL_SWAP(T, x, y) do { T tmp = (x); (x) = (y); (y) = tmp; } while (0)
// but we can remove this redundant parameter.
//
#if (__STDC_VERSION__ > 201710L)
    // ^^-- strictly superior to C17, ie the (as-of-today) upcoming C23 and
    //      later versions which would have "native" typeof (and static_assert)
#   define UTIL_SWAP(x, y) do                            \
           { static_assert(sizeof((x)) == sizeof((y)));  \
             typeof((x)) tmp = (x);                      \
             (x) = (y);                                  \
             (y) = tmp;                                  \
           } while (0)
#elif (defined(__GNUC__) || defined(__clang__))
#   define UTIL_SWAP(x, y) do                            \
           { STATIC_ASSERT(__builtin_types_compatible_p(__typeof__((x)), __typeof__((y)))); \
             __typeof__((x)) tmp = (x);                  \
             (x) = (y);                                  \
             (y) = tmp;                                  \
           } while (0)
#else
    // inspired by: https://stackoverflow.com/questions/3982348/implement-generic-swap-macro-in-c
    // memcopy would hopefully be optimized by the compiler as just the swap.
#   define UTIL_SWAP(x, y) do                            \
           { unsigned char tmp[sizeof((x)) == sizeof((y)) ? (signed)sizeof((x)) : -1]; \
             memcpy(tmp,  &(y), sizeof((x)));            \
             memcpy(&(y), &(x), sizeof((x)));            \
             memcpy(&(x), tmp,  sizeof((x)));            \
           } while(0)
#endif

//===</ MISC. >=================================================================


#endif //KONPU_UTIL_H
