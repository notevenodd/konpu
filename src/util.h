#ifndef  KONPU_UTIL_H
#define  KONPU_UTIL_H
#include "platform.h"
#include "c.h"

/* - misc. macro or utilities used throughout the project,
 *                            or that we wish to provide to users.
 */


//===< time >===================================================================

// sleep (in milliseconds)
// *tries* to wait a specified number of milliseconds before returning.
//
// depending on the platform, this functionality might not be available
// and thus may return "immediately". On the other hand, if it is implemented,
// it may dlay a bit longer due to OS scheduling. If milliseconds is <= 0, it
// returns immediately.
void sleep_ms(int milliseconds);

//===</ time >==================================================================


//===< FIXED-WIDTH INTEGERS >===================================================
// utilities on fixed-width unsigned ints, including some of those:
// clz     : count leading  zero bits (but: result is undefined if 0)
// ctz     : count trailing zero bits (but: result is undefined if 0)
// popcount: count the number of bits set
// //bswap   : swap bits
// reverse : reverse every bit
// hamming : this is the "hamming distance" of two bit sequences,
//           it counts positions at which the bits differ, ie: the min. number
//           of substitutions required to change one sequence into the other.
//           It provides a "distance" to tell "how similar" two glyphs are.
//------------------------------------------------------------------------------

// bit/byte selection macros for unsigned integer types
// - 'x' *MUST* be of a unsigned integer type (otherwise undefined behaviour)
// - 'n' should be >= 0 and less than the number of bits of the type of x.
// we consider bit/bytes starting as 0 from the the lowest bit/bytes.

// returns the value (0 or 1) of the nth-bit of x (n in 0..7)
#define uint_bit( x, n)             (((x) >> (n)) & 1U)

// returns a non-zero value iff the nth-bit of x is set (n in 0..7)
// note: this can be done with uint_bit(x,n) too,
//       however, if n is a constant, it might take one op less.
//#define uint64_bit_isset( x, n)     ((x) & (UINT64_C(1)<<(n)))


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
        (((byte) * UINT64_C(0x0202020202) & UINT64_C(0x010884422010)) % 1023)
        // using 3 operations and using 64-bit for * and %), see:
        // http://graphics.stanford.edu/~seander/bithacks.html#ReverseByteWith64BitsDiv
        // The method was attributed to Rich Schroeppel in the Programming Hacks
        // section of Beeler, M., Gosper, R. W., and Schroeppel, R. HAKMEM.
        // MIT AI Memo 239, Feb. 29, 1972.
        // (code snippet is in the public domain)
// TODO:remove this line:        (((byte) * 0x0202020202ULL & 0x010884422010ULL) % 1023)


#if defined(__GNUC__) || defined(__clang__)
    // might not be optimal for mere bytes,
    // but if it's almost just one assembly instructions, it's fine.
    #define byte_clz(x)                __builtin_clz((x) << 24)
                                       /*^FIXME*/ static_assert(sizeof(int)==4);
                                       //'------- I'm adding the 24 so the byte
                                       //         is in leading position as we
                                       //         assumes int is 32 bits. OUCH!
    #define byte_ctz(x)                __builtin_ctz(x)
    #define byte_popcount(x)           __builtin_popcount(x)
#elif __STDC_VERSION__ > 201710L       // C23 or later
    #define byte_clz(x)                stdc_leading_zeros((unsigned char)(x))
    #define byte_ctz(x)                stdc_trailing_zeros((unsigned char)(x))
    #define byte_popcount(x)           stdc_count_ones((unsigned char)(x))
#else
    # error "clz/ctz/popcount for bytes rely on GCC/CLANG builtins"
    // SOMEDAY/MAYBE: provide portable C implementation,
    //                (it may take the forms of precomputed tables!)
#endif

// 'int' is garantueed to be at minimum 16-bit
#if defined(__GNUC__) || defined(__clang__)
//  #define uint16_clz(x)              __builtin_clz(x) // would only work if int is exactly 16 bits
    #define uint16_ctz(x)              __builtin_ctz(x)
    #define uint16_popcount(x)         __builtin_popcount(x)
    //#define uint16_bswap(x)          __builtin_bswap16(x)
#elif __STDC_VERSION__ > 201710L       // C23 or later
    #define uint16_clz(x)              stdc_leading_zeros((uint16_t)(x))
    #define uint16_ctz(x)              stdc_trailing_zeros((uint16_t)(x))
    #define uint16_popcount(x)         stdc_count_ones((uint16_t)(x))
#else
    # error "ctz/popcount/bswap for uint16_t rely on GCC/CLANG builtins"
    // TODO: SOMEDAY/MAYBE: provide portable C implementation.
#endif
#define uint16_hamming_distance(x, y)  uint16_popcount((x) ^ (y));

// 'long' is garantueed to be at minimum 32-bit

#if defined(__GNUC__) || defined(__clang__)
//  #define uint32_clz(x)              __builtin_clzl(x)  // would only work if long is exactly 32 bits
    #define uint32_ctz(x)              __builtin_ctzl(x)
    #define uint32_popcount(x)         __builtin_popcountl(x)
    //#define uint32_bswap(x)          __builtin_bswap32(x)
#elif __STDC_VERSION__ > 201710L       // C23 or later
    #define uint32_clz(x)              stdc_leading_zeros((uint32_t)(x))
    #define uint32_ctz(x)              stdc_trailing_zeros((uint32_t)(x))
    #define uint32_popcount(x)         stdc_count_ones((uint32_t)(x))
#else
    # error "clz/ctz/popcount/bswap for uint32_t rely on GCC/CLANG builtins"
    // TODO: SOMEDAY/MAYBE: provide portable C implementation.
#endif
#define uint32_hamming_distance(x, y)  uint32_popcount((x) ^ (y));

// 'long long' is garantueed to be at minimum 64+ bits
// (commonly, on 64bit systems, 'long' might be)
#if defined(__GNUC__) || defined(__clang__)
    // long long must be 64+ bits, so use clzll/ctzll instead of clzl/ctzl
    #define uint64_clz(x)              __builtin_clzll(x)
    // TODO/FIXME  ^^-^^^--- only works if long long is *exactly* 64 bits
    #define uint64_ctz(x)              __builtin_ctzll(x)
    #define uint64_popcount(x)         __builtin_popcountll(x)
    //#define uint64_bswap(x)          __builtin_bswap64(x)
#elif __STDC_VERSION__ > 201710L       // C23 or later
    #define uint64_clz(x)              stdc_leading_zeros((uint64_t)(x))
    #define uint64_ctz(x)              stdc_trailing_zeros((uint64_t)(x))
    #define uint64_popcount(x)         stdc_count_ones((uint64_t)(x))
#else
    // TODO: provide portable C implementation.
    # error "clz/ctz/popcount/bswap for uint64_t rely on GCC/CLANG builtins"
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
#if __STDC_VERSION__ > 201710L
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
           { static_assert(__builtin_types_compatible_p(__typeof__((x)), __typeof__((y)))); \
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
