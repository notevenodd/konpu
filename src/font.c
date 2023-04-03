#include "font.h"

#define Q_TODO  0x6ff6 // some kind of filled "circle" (for now!)
uint16_t chr_quadrant_font[] = {
   //////////////////////////////////
   // range 0-31                   //
   //////////////////////////////////
   /* TODO: find which 32 symbols should be in the range 0-31 */
   [  0] = Q_TODO, [  1] = Q_TODO, [  2] = Q_TODO, [  3] = Q_TODO,
   [  4] = Q_TODO, [  5] = Q_TODO, [  6] = Q_TODO, [  7] = Q_TODO,
   [  8] = Q_TODO, [  9] = Q_TODO, [ 10] = Q_TODO, [ 11] = Q_TODO,
   [ 12] = Q_TODO, [ 13] = Q_TODO, [ 14] = Q_TODO, [ 15] = Q_TODO,
   [ 16] = Q_TODO, [ 17] = Q_TODO, [ 18] = Q_TODO, [ 19] = Q_TODO,
   [ 20] = Q_TODO, [ 21] = Q_TODO, [ 22] = Q_TODO, [ 23] = Q_TODO,
   [ 24] = Q_TODO, [ 25] = Q_TODO, [ 26] = Q_TODO, [ 27] = Q_TODO,
   [ 28] = Q_TODO, [ 29] = Q_TODO, [ 30] = Q_TODO, [ 31] = Q_TODO,

   //////////////////////////////////
   // range 32-96 is same as ASCII //
   //////////////////////////////////
   // letters (TODO: check those alts too)
   //                B:0xcec0 / C:0xe8e0 / J:0x2ae0(!!!)
   //                Q:0xee20 0xeac0 0xeac2 / R:0xe880
   // numbers (note: 0 = O, 2 = Z, 5 = S, 8 = full 3x3, 9 = Q or alt-Q (TODO))
   // punctuation: * = X,  // @ = Q or alt-Q (TODO)
   //              & alts (but they're worse): 0x5ad0 0x4ae0 0x5ae1
   // toki pona "look-alike":
   // +=en, I=pini, K:ken, L:pi, T:pake, U:luka/poki, V:suli, */X:ala, Y:anu

   [' '] = 0x0000, ['!'] = 0x44a0, ['"'] = 0xa000,

   // note: all those have width 4
   ['#'] = 0x6f60, ['$'] = 0x76e0, ['%'] = 0xb6d0, ['&'] = 0x5ad0,

   ['\'']= 0x4400, ['('] = 0xc8c0, [')'] = 0x6260, ['*'] = 0xa4a0,
   ['+'] = 0x4e40, [','] = 0x0240, ['-'] = 0x0e00, ['.'] = 0x0040,
   ['/'] = 0x2480,

   ['0'] = 0xeae0, ['1'] = 0xc4e0, ['2'] = 0xc460, ['3'] = 0xc6c0,
   ['4'] = 0xae20, ['5'] = 0x64c0, ['6'] = 0x8ee0, ['7'] = 0xe220,
   ['8'] = 0xeee0, ['9'] = 0xee20,

   [':'] = 0x4040, [';'] = 0x2060, ['<'] = 0x2420, ['='] = 0xe0e0,
   ['>'] = 0x8480, ['?'] = 0xe640, ['@'] = 0xeac0,

   ['A'] = 0x4ea0, ['B'] = 0xcee0, ['C'] = 0x6860, ['D'] = 0xcac0,
   ['E'] = 0xece0, ['F'] = 0xec80, ['G'] = 0xcae0, ['H'] = 0xaea0,
   ['I'] = 0xe4e0, ['J'] = 0x22c0, ['K'] = 0xaca0, ['L'] = 0x88e0,
   ['M'] = 0xeea0, ['N'] = 0xcaa0, ['O'] = 0xeae0, ['P'] = 0xee80,
   ['Q'] = 0xeac4, ['R'] = 0xcea0, ['S'] = 0x64c0, ['T'] = 0xe440,
   ['U'] = 0xaae0, ['V'] = 0xaac0, ['W'] = 0xaee0, ['X'] = 0xa4a0,
   ['Y'] = 0xa440, ['Z'] = 0xc460,

   ['['] = 0x6460, ['\\']= 0x8420, [']'] = 0xc4c0, ['^'] = 0x4a00,
   ['_'] = 0x00e0, ['`'] = 0x8400,

   ////////////////////////////////////
   // 97-122 is DIFFERENT than ASCII //
   ////////////////////////////////////
   // in this range are the ASCII lowercase letters, but we can't differentiate
   // them in such a tiny font, so we use that range for other symbols.
   // (one should ascii-uppercase a string before printing with this minifont)
   /* TODO: find which 26 tiny symbols would be good for the range 97-122 */
   [ 97] = Q_TODO, [ 98] = Q_TODO, [ 99] = Q_TODO, [100] = Q_TODO,
   [101] = Q_TODO, [102] = Q_TODO, [103] = Q_TODO, [104] = Q_TODO,
   [105] = Q_TODO, [106] = Q_TODO, [107] = Q_TODO, [108] = Q_TODO,
   [109] = Q_TODO, [110] = Q_TODO, [111] = Q_TODO, [112] = Q_TODO,
   [113] = Q_TODO, [114] = Q_TODO, [115] = Q_TODO, [116] = Q_TODO,
   [117] = Q_TODO, [118] = Q_TODO, [119] = Q_TODO, [120] = Q_TODO,
   [121] = Q_TODO, [122] = Q_TODO,

   ////////////////////////////////////
   // range 123-126 is same as ASCII //
   ////////////////////////////////////
   ['{'] = 0x6c60, ['|'] = 0x4440, ['}'] = 0xc6c0, ['~'] = 0x2e80,

   /* TODO: find defaults symbols for the range 127-255
    *       or alternatively, we can make the array stops here.
    */
};
