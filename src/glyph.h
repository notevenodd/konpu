#ifndef  KONPU_GLYPH_H
#define  KONPU_GLYPH_H
#include "platform.h"
#include "c.h"
#include "util.h"

// glyph terminology:                                              uint* glyphs:
// -----------------                                               -------------
// .----------------------.--------.----------.
// | GLYPH TYPE           | BITMAP |  C TYPE  |     quadrant     wide half
// |----------------------|--------|----------|     . . . .      . . . . . . . .
// | quadrant             |  4x4   | uint16_t |     . . . .      . . . . . . . .
// |----------------------|--------|----------|     . . . .      . . . . . . . .
// | half:                |        |          |     . . . .      . . . . . . . .
// |  - tall half         |  4x8   | uint32_t |
// |  - wide half         |  8x4   |          |
// |----------------------|--------|----------|     tall half    glyph
// | (standard) glyph     |  8x8   | uint64_t |     . . . .      . . . . . . . .
// |----------------------|--------|----------|     . . . .      . . . . . . . .
// | pair:                |        | (struct) |     . . . .      . . . . . . . .
// |  - tall pair         |  8x16  |   pair   |     . . . .      . . . . . . . .
// |  - wide pair         |  16x8  |          |     . . . .      . . . . . . . .
// |----------------------|--------|----------|     . . . .      . . . . . . . .
// | tetra                | 16x16  | (struct) |     . . . .      . . . . . . . .
// |                      |        |  tetra   |     . . . .      . . . . . . . .
// '----------------------'--------'----------'
//                                                          struct-based glyphs:
//                                                          --------------------
//
// tetra                              tall pair          wide pair
// (.top_left)        (.top_right)           (.first)
// . . . . . . . .|. . . . . . . .    . . . . . . . .    . . . . . . . .|. . . . . . . .
// . . . . . . . .|. . . . . . . .    . . . . . . . .    . . . . . . . .|. . . . . . . .
// . . . . . . . .|. . . . . . . .    . . . . . . . .    . . . . . . . .|. . . . . . . .
// . . . . . . . .|. . . . . . . .    . . . . . . . .    . . . . . . . .|. . . . . . . .
// . . . . . . . .|. . . . . . . .    . . . . . . . .    . . . . . . . .|. . . . . . . .
// . . . . . . . .|. . . . . . . .    . . . . . . . .    . . . . . . . .|. . . . . . . .
// . . . . . . . .|. . . . . . . .    . . . . . . . .    . . . . . . . .|. . . . . . . .
// ._._._._._._._.|._._._._._._._.    ._._._._._._._.    . . . . . . . .|. . . . . . . .
// . . . . . . . .|. . . . . . . .    . . . . . . . .    (.first)        (.second)
// . . . . . . . .|. . . . . . . .    . . . . . . . .
// . . . . . . . .|. . . . . . . .    . . . . . . . .
// . . . . . . . .|. . . . . . . .    . . . . . . . .
// . . . . . . . .|. . . . . . . .    . . . . . . . .
// . . . . . . . .|. . . . . . . .    . . . . . . . .
// . . . . . . . .|. . . . . . . .    . . . . . . . .
// . . . . . . . .|. . . . . . . .    . . . . . . . .
// (.bottom_left)  (.bottom_right)          (.second)

//------------------------------------------------------------------------------
// structures:

// construct non-struct glpyhs from C literals
#define QUADRANT(lit_hex)   UINT16_C(0x##lit_hex)
#define HALF(lit_hex)       UINT32_C(0x##lit_hex)
#define GLYPH(lit_hex)      UINT64_C(0x##lit_hex)

// pair glyphs (tall and wide pairs)
typedef struct pair {                     // anonymous unions are only supported
#if (__STDC_VERSION__ < 201112L)          // starting from C11, so to remain
           uint64_t first;                // portable with C99, only use .first
           uint64_t second;               // and .second as the names of fields,
#else                                     //
   union { uint64_t first,                // ... but ...
                    top, left, x; };      //
   union { uint64_t second,               // if C99 portability isn't needed,
                    bottom, right, y; };  // you may wish to refer to fields of:
#endif                                    // - a wide pair, as .left and .right,
} pair;                                   // - a tall pair, as .top  and .bottom

// tetra glyph:
typedef struct tetra {
   uint64_t top_left;     uint64_t top_right;
   uint64_t bottom_left;  uint64_t bottom_right;
} tetra;

// width/height constants
// TODO: we chose a glyph to be a uint64_t, so a 8x8 size is natrual and set in
// stone and there are raw "8" literals floating around in the source code.
// Let's try to use those macros instead in most cases.
#define GLYPH_WIDTH       8
#define GLYPH_HEIGHT      8
//
#define QUADRANT_WIDTH    (GLYPH_WIDTH  / 2)
#define QUADRANT_HEIGHT   (GLYPH_HEIGHT / 2)
#define TALLHALF_WIDTH    (GLYPH_WIDTH  / 2)
#define TALLHALF_HEIGHT   GLYPH_HEIGHT
#define WIDEHALF_WIDTH    GLYPH_WIDTH
#define WIDEHALF_HEIGHT   (GLYPH_HEIGHT / 2)
#define TALLPAIR_WIDTH    GLYPH_WIDTH
#define TALLPAIR_HEIGHT   (GLYPH_HEIGHT * 2)
#define WIDEPAIR_WIDTH    (GLYPH_WIDTH  * 2)
#define WIDEPAIR_HEIGHT   GLYPH_HEIGHT
#define TETRA_WIDTH       (GLYPH_WIDTH  * 2)
#define TETRA_HEIGHT      (GLYPH_HEIGHT * 2)


//------------------------------------------------------------------------------
// glyphs (unchecked) operations on individual pixel(s)
// fromPixel(x,y)     => return a glyph containing just the pixel(x,y)
// pixelValue(g, x,y) => return the value (0 or 1) of pixel(x,y) in the given glyph
// pixelIndex(x,y)    => return the index of the bit representing the pixel(x,y)

#define   glyph_fromPixel(x,y)                 (GLYPH(1) << glyph_pixelIndex((x),(y)))
#define   glyph_pixelValue(glyph, x,y)         uint_bit((glyph), glyph_pixelIndex((x),(y)))
#define   glyph_pixelIndex(x,y)                ((GLYPH_WIDTH * GLYPH_HEIGHT - 1) - (x) - GLYPH_WIDTH * (y))

#define   quadrant_fromPixel(x,y)              (QUADRANT(1) << quadrant_pixelIndex((x),(y)))
#define   quadrant_pixelValue(quadrant, x,y)   uint_bit((quadrant), quadrant_pixelIndex((x),(y)))
#define   quadrant_pixelIndex(x,y)             ((QUADRANT_WIDTH * QUADRANT_HEIGHT - 1) - (x) - QUADRANT_WIDTH * (y))

#define   tallhalf_fromPixel(x,y)              (TALLHALF(1) << tallhalf_pixelIndex((x),(y)))
#define   tallhalf_pixelValue(tallhalf, x,y)   uint_bit((tallhalf), tallhalf_pixelIndex((x),(y)))
#define   tallhalf_pixelIndex(x,y)             ((TALLHALF_WIDTH * TALLHALF_HEIGHT - 1) - (x) - TALLHALF_WIDTH * (y))

#define   widehalf_fromPixel(x,y)              (WIDEHALF(1) << widehalf_pixelIndex((x),(y)))
#define   widehalf_pixelValue(quandrant, x,y)  uint_bit((widehalf), widehalf_pixelIndex((x),(y)))
#define   widehalf_pixelIndex(x,y)             ((WIDEHALF_WIDTH * WIDEHALF_HEIGHT - 1) - (x) - WIDEHALF_WIDTH * (y))

pair      tallpair_fromPixel(int x, int y);
uint64_t  tallpair_pixelValue(pair tallpair, int x, int y);
#define   tallpair_pixelIndex(x,y)             ((TALLPAIR_WIDTH * TALLPAIR_HEIGHT - 1) - (x) - TALLPAIR_WIDTH * (y))

pair      widepair_fromPixel(int x, int y);
uint64_t  widepair_pixelValue(pair widepair, int x, int y);
#define   widepair_pixelIndex(x,y)             ((WIDEPAIR_WIDTH * WIDEPAIR_HEIGHT - 1) - (x) - WIDEPAIR_WIDTH * (y))

tetra     tetra_fromPixel(int x, int y);
uint64_t  tetra_pixelValue(tetra t, int x, int y);
#define   tetra_pixelIndex(x,y)                ((TETRA_WIDTH * TETRA_HEIGHT - 1) - (x) - TETRA_WIDTH * (y))


//------------------------------------------------------------------------------
// line and bits selection macros for quadrants, halves, and glyphs

// returns a 0x0N or 0xN0 (with the _H suffix) value,
// where N is the nibble representing a quadrant's line (line: 0-3)

#define quadrant_line0(  quadrant)    ((quadrant) >> (QUADRANT_WIDTH*3)       )
#define quadrant_line0_H(quadrant)    ((quadrant) >> (QUADRANT_WIDTH*2) & 0xF0)
#define quadrant_line1(  quadrant)    ((quadrant) >> (QUADRANT_WIDTH*2) & 0x0F)
#define quadrant_line1_H(quadrant)    ((quadrant) >> (QUADRANT_WIDTH*1) & 0xF0)
#define quadrant_line2(  quadrant)    ((quadrant) >> (QUADRANT_WIDTH*1) & 0x0F)
#define quadrant_line2_H(quadrant)    ((quadrant)                       & 0xF0)
#define quadrant_line3(  quadrant)    ((quadrant)                       & 0x0F)
#define quadrant_line3_H(  quadrant)  ((quadrant) << (QUADRANT_WIDTH*1) & 0x0F)

// returns a 0x0N or 0xN0 (with the _H suffix) value,
// where N is the nibble representing a tall half's line (line: 0-7)
#define tallhalf_line0(  tallhalf)    ((tallhalf) >> 28      )
#define tallhalf_line0_H(tallhalf)    ((tallhalf) >> 24 & 0xF0)
#define tallhalf_line1(  tallhalf)    ((tallhalf) >> 24 & 0x0F)
#define tallhalf_line1_H(tallhalf)    ((tallhalf) >> 20 & 0xF0)
#define tallhalf_line2(  tallhalf)    ((tallhalf) >> 20 & 0x0F)
#define tallhalf_line2_H(tallhalf)    ((tallhalf) >> 16 & 0xF0)
#define tallhalf_line3(  tallhalf)    ((tallhalf) >> 16 & 0x0F)
#define tallhalf_line3_H(tallhalf)    ((tallhalf) >> 12 & 0xF0)
#define tallhalf_line4(  tallhalf)    ((tallhalf) >> 12 & 0x0F)
#define tallhalf_line4_H(tallhalf)    ((tallhalf) >>  8 & 0xF0)
#define tallhalf_line5(  tallhalf)    ((tallhalf) >>  8 & 0x0F)
#define tallhalf_line5_H(tallhalf)    ((tallhalf) >>  4 & 0xF0)
#define tallhalf_line6(  tallhalf)    ((tallhalf) >>  4 & 0x0F)
#define tallhalf_line6_H(tallhalf)    ((tallhalf)       & 0xF0)
#define tallhalf_line7(  tallhalf)    ((tallhalf)       & 0x0F)
#define tallhalf_line7_H(  tallhalf)  ((tallhalf) <<  4 & 0x0F)

// returns a 0x00-0xFF value representing a wide half's line (line: 0-3)
#define widehalf_line(widehalf, n)    uint_byte((widehalf), 4-(n))
#define widehalf_line0(widehalf)      ((widehalf) >> 24       )
#define widehalf_line1(widehalf)      ((widehalf) >> 16 & 0xFF)
#define widehalf_line2(widehalf)      ((widehalf) >>  8 & 0xFF)
#define widehalf_line3(widehalf)      ((widehalf)       & 0xFF)

// returns a 0x00-0xFF value representing a glyph line (line: 0-7)
#define glyph_line(glyph, n)          uint_byte((glyph), 7-(n))
#define glyph_line0(glyph)            ((glyph) >>  56        )
#define glyph_line1(glyph)            ((glyph) >>  48  & 0xFF)
#define glyph_line2(glyph)            ((glyph) >>  40  & 0xFF)
#define glyph_line3(glyph)            ((glyph) >>  32  & 0xFF)
#define glyph_line4(glyph)            ((glyph) >>  24  & 0xFF)
#define glyph_line5(glyph)            ((glyph) >>  16  & 0xFF)
#define glyph_line6(glyph)            ((glyph) >>   8  & 0xFF)
#define glyph_line7(glyph)            ((glyph)         & 0xFF)


//------------------------------------------------------------------------------
// constants / placeholders / masks

// - "placeholder" glyph (a solid block with margin 1)
#define QUADRANT_PLACEHOLDER   QUADRANT(0660)
#define TALLHALF_PLACEHOLDER   HALF(06666660)
#define WIDEHALF_PLACEHOLDER   HALF(007e7e00)
#define GLYPH_PLACEHOLDER      GLYPH(007e7e7e7e7e7e00)  // alt: GLYPH(007e566a566a7e00)
#define TALLPAIR_PLACEHOLDER   ((pair){ .first        = GLYPH(007e7e7e7e7e7e7e) , \
                                        .second       = GLYPH(7e7e7e7e7e7e7e00) } )
#define WIDEPAIR_PLACEHOLDER   ((pair){ .first        = GLYPH(007f7f7f7f7f7f00) , \
                                        .second       = GLYPH(00fefefefefefe00) } )
#define TETRA_PLACEHOLDER      ((tetra){.top_left     = GLYPH(007f7f7f7f7f7f7f) , \
                                        .top_right    = GLYPH(00fefefefefefefe) , \
                                        .bottom_left  = GLYPH(7f7f7f7f7f7f7f00) , \
                                        .bottom_right = GLYPH(fefefefefefefe00) } )
// - a placeholder glyphs that spells the word "TODO"
#define GLYPH_TODO                                      GLYPH(ee4a4e00ceaace00)
#define TALLPAIR_TODO          ((pair){ .top          = GLYPH(0000000000000000) , \
                               /*TODO*/ .bottom       = GLYPH(0000000000000000) } )
#define WIDEPAIR_TODO          ((pair){ .top          = GLYPH(0000000000000000) , \
                               /*TODO*/ .bottom       = GLYPH(0000000000000000) } )
#define TETRA_TODO             ((tetra){.top_left     = GLYPH(7e5a181818183c00) , \
                                        .top_right    = GLYPH(386cc6c6c66c3800) , \
                                        .bottom_left  = GLYPH(f86c6666666cf800) , \
                                        .bottom_right = GLYPH(386cc6c6c66c3800) } )

// masks to that select 4x4 pixel parts of glyph with | ...
#define GLYPH_TOP_LEFT         GLYPH(f0f0f0f000000000)
#define GLYPH_TOP_RIGHT        GLYPH(0f0f0f0f00000000)
#define GLYPH_BOTTOM_LEFT      GLYPH(00000000f0f0f0f0)
#define GLYPH_BOTTOM_RIGHT     GLYPH(000000000f0f0f0f)
#define GLYPH_TOP              (GLYPH_TOP_LEFT|GLYPH_TOP_RIGHT)
#define GLYPH_BOTTOM           (GLYPH_BOTTOM_LEFT|GLYPH_BOTTOM_RIGHT)
#define GLYPH_LEFT             (GLYPH_TOP_LEFT|GLYPH_BOTTOM_LEFT)
#define GLYPH_RIGHT            (GLYPH_TOP_RIGHT|GLYPH_BOTTOM_RIGHT)

// masks to select a part 4x4 pixel parts of a half with | ...
#define TALLHALF_TOP           HALF(ffff0000)
#define TALLHALF_BOTTOM        HALF(0000ffff)
#define WIDEHALF_LEFT          HALF(f0f0f0f0)
#define WIDEHALF_RIGHT         HALF(0f0f0f0f)

// masks to that select 2x2 pixel parts of quadrants | ...
#define QUADRANT_TOP_LEFT      QUADRANT(cc00)
#define QUADRANT_TOP_RIGHT     QUADRANT(3300)
#define QUADRANT_BOTTOM_LEFT   QUADRANT(00cc)
#define QUADRANT_BOTTOM_RIGHT  QUADRANT(0033)
#define QUADRANT_TOP           (QUADRANT_TOP_LEFT|QUADRANT_TOP_RIGHT)
#define QUADRANT_BOTTOM        (QUADRANT_BOTTOM_LEFT|QUADRANT_BOTTOM_RIGHT)
#define QUADRANT_LEFT          (QUADRANT_TOP_LEFT|QUADRANT_BOTTOM_LEFT)
#define QUADRANT_RIGHT         (QUADRANT_TOP_RIGHT|QUADRANT_BOTTOM_RIGHT)

//------------------------------------------------------------------------------
// make standard glyphes out of smaller components,
// makes halves out of quadrants

// halves from two quadrants:
static inline  uint32_t  tallhalf2(uint16_t top,  uint16_t bottom);
static inline  uint32_t  widehalf2(uint16_t left, uint16_t right);
// glyph from four quadrants
static inline  uint64_t  glyph4(uint16_t top_left   , uint16_t top_right,
                                uint16_t bottom_left, uint16_t bottom_right);
/* TODO: implement
// glyph from one half, and two quadrants
static inline  uint64_t  glyph3_left  (uint32_t tall_left,   uint16_t top_right,   uint16_t bottom_right);
static inline  uint64_t  glyph3_right (uint32_t tall_right,  uint16_t top_left,    uint16_t bottom_left);
static inline  uint64_t  glyph3_top   (uint32_t wide_top,    uint16_t bottom_left, uint16_t bottom_right);
static inline  uint64_t  glyph3_bottom(uint32_t wide_bottom, uint16_t top_left,    uint16_t top_right);
// glyph from two halves:
static inline  uint64_t  glyph2_wide(uint32_t top,  uint32_t bottom);
static inline  uint64_t  glyph2_tall(uint32_t left, uint32_t right);
*/


// implementation

static inline  uint32_t  tallhalf2(uint16_t top,  uint16_t bottom)
{ return (uint32_t)top << 16 | bottom; }

static inline  uint32_t  widehalf2(uint16_t left, uint16_t right)
{ //      v-- just to make the computation happens in at least 32 bits
  //          as we shift by 24 bits ------------------------------------v
  return (uint32_t)( quadrant_line0_H(left) | quadrant_line0(right) ) << 3*WIDEHALF_WIDTH |
         (uint32_t)( quadrant_line1_H(left) | quadrant_line1(right) ) << 2*WIDEHALF_WIDTH |
                   ( quadrant_line2_H(left) | quadrant_line2(right) ) << 1*WIDEHALF_WIDTH |
                   ( quadrant_line3_H(left) | quadrant_line3(right) ) << 0*WIDEHALF_WIDTH ;
}

static inline  uint64_t  glyph4(uint16_t top_left   , uint16_t top_right,
                                uint16_t bottom_left, uint16_t bottom_right)
{
  return (uint64_t)( quadrant_line0_H(top_left)    | quadrant_line0(top_right)    ) << 7*GLYPH_WIDTH |
         (uint64_t)( quadrant_line1_H(top_left)    | quadrant_line1(top_right)    ) << 6*GLYPH_WIDTH |
         (uint64_t)( quadrant_line2_H(top_left)    | quadrant_line2(top_right)    ) << 5*GLYPH_WIDTH |
         (uint64_t)( quadrant_line3_H(top_left)    | quadrant_line3(top_right)    ) << 4*GLYPH_WIDTH |
         (uint64_t)( quadrant_line0_H(bottom_left) | quadrant_line0(bottom_right) ) << 3*GLYPH_WIDTH |
         (uint64_t)( quadrant_line1_H(bottom_left) | quadrant_line1(bottom_right) ) << 2*GLYPH_WIDTH |
                   ( quadrant_line2_H(bottom_left) | quadrant_line2(bottom_right) ) << 1*GLYPH_WIDTH |
                   ( quadrant_line3_H(bottom_left) | quadrant_line3(bottom_right) ) << 0*GLYPH_WIDTH ;
}

// TODO.... all the functions...

//------------------------------------------------------------------------------

// glyph measurement (return a value [0-8])
static inline  int       glyph_marginTop   (uint64_t glyph); // # of upper empty lines
static inline  int       glyph_marginBottom(uint64_t glyph); // # of lower empty lines
static inline  int       glyph_marginLeft  (uint64_t glyph); // # of left empty columns
static inline  int       glyph_marginRight (uint64_t glyph); // # of right empty columns
//TODO: maybe name height and width are confusing,
//      those are not GLYPH_WIDTH and GLYPH_HEIGHT
static inline  int       glyph_height      (uint64_t glyph); // height of a glyph
static inline  int       glyph_width       (uint64_t glyph); // width of a glyph
// notes: - the coordinates of the upper-left non-empty pixel of a glyph is:
//          (glyph_marginLeft(glyph), glyph_marginTop(glyph))
//        - so sometimes, you might be interested in "8 - ...margin..." instead
//          of the true width/height of a glyph.

// glyph flips and n*90-degrees rotations (angles are measured counterclockwise)
static inline  uint64_t  glyph_flip         (uint64_t glyph); // flip along -
static inline  uint64_t  glyph_mirror       (uint64_t glyph); // flip along |
static inline  uint64_t  glyph_transpose    (uint64_t glyph); // flip along \.
static inline  uint64_t  glyph_rotate90     (uint64_t glyph); //  90deg rotation
static inline  uint64_t  glyph_rotate180    (uint64_t glyph); // 180deg rotation
static inline  uint64_t  glyph_rotate270    (uint64_t glyph); // 270deg rotation

// transformations: shifts
static inline uint64_t  glyph_shiftLeft     (uint64_t glyph, unsigned n);
static inline uint64_t  glyph_shiftRight    (uint64_t glyph, unsigned n);
static inline uint64_t  glyph_shiftTop      (uint64_t glyph, unsigned n);
static inline uint64_t  glyph_shiftBottom   (uint64_t glyph, unsigned n);

// transformations: cyclic shifts
//static inline uint64_t  glyph_cycleLeft   (uint64_t glyph, unsigned n); // TODO
//static inline uint64_t  glyph_cycleRight  (uint64_t glyph, unsigned n); // TODO
//static inline uint64_t  glyph_cycleTop    (uint64_t glyph, unsigned n); // TODO
//static inline uint64_t  glyph_cycleBottom (uint64_t glyph, unsigned n); // TODO


//--- inline implementation ----------------------------------------------------


/* glyph measurements */

static inline int glyph_marginTop(uint64_t glyph)
{ return (glyph)? (uint64_clz(glyph) / GLYPH_HEIGHT) : GLYPH_HEIGHT; }

static inline int glyph_marginBottom(uint64_t glyph)
{ return (glyph)? (uint64_ctz(glyph) / GLYPH_HEIGHT) : GLYPH_HEIGHT; }

static inline int glyph_height(uint64_t glyph)
{ return (glyph)? (GLYPH_HEIGHT - (uint64_clz(glyph) / GLYPH_HEIGHT) - (uint64_ctz(glyph) / GLYPH_HEIGHT)) : 0; }

// binary-OR combines all the line-bytes of the glyph into a byte
static inline unsigned char
glyph_flatten(uint64_t glyph) {
   return ((glyph  >>  GLYPH_WIDTH * 7) | (glyph  >>  GLYPH_WIDTH * 6) |
           (glyph  >>  GLYPH_WIDTH * 5) | (glyph  >>  GLYPH_WIDTH * 4) |
           (glyph  >>  GLYPH_WIDTH * 3) | (glyph  >>  GLYPH_WIDTH * 2) |
           (glyph  >>  GLYPH_WIDTH * 1) | (glyph  >>  GLYPH_WIDTH * 0) );
}

static inline int glyph_marginLeft(uint64_t glyph)
{ return (glyph)? byte_clz(glyph_flatten(glyph)) : 0; }

static inline int glyph_marginRight(uint64_t glyph)
{ return (glyph)? byte_ctz(glyph_flatten(glyph)) : 0; }

static inline int glyph_width(uint64_t glyph)
{
   if (glyph) {
      unsigned char line = glyph_flatten(glyph);
      return GLYPH_WIDTH - byte_clz(line) - byte_ctz(line);
   } else {
      return 0;
   }
}

/* flips / transposition / rotations */
// inspiration for implementation: Hacker's Delight +
// https://stackoverflow.com/questions/1667591/rotating-a-bitmap-90-degrees

static inline uint64_t glyph_flip(uint64_t glyph)
{  glyph = ((glyph & GLYPH(ffffffff00000000)) >> 32) | ((glyph & GLYPH(00000000ffffffff)) << 32);
   glyph = ((glyph & GLYPH(ffff0000ffff0000)) >> 16) | ((glyph & GLYPH(0000ffff0000ffff)) << 16);
   glyph = ((glyph & GLYPH(ff00ff00ff00ff00)) >>  8) | ((glyph & GLYPH(00ff00ff00ff00ff)) <<  8);
   return glyph;
}

static inline uint64_t glyph_mirror(uint64_t glyph)
{  glyph = ((glyph & GLYPH(f0f0f0f0f0f0f0f0)) >> 4) | ((glyph & GLYPH(0f0f0f0f0f0f0f0f)) << 4);
   glyph = ((glyph & GLYPH(cccccccccccccccc)) >> 2) | ((glyph & GLYPH(3333333333333333)) << 2);
   glyph = ((glyph & GLYPH(aaaaaaaaaaaaaaaa)) >> 1) | ((glyph & GLYPH(5555555555555555)) << 1);
   return glyph;
}

static inline uint64_t  glyph_transpose(uint64_t glyph) {
   // Based on "Hacker's Delight" (second edition) by Henry S. Warren, Jr.
   // https://web.archive.org/web/20190915025154/http://www.hackersdelight.org/
   // https://en.wikipedia.org/wiki/Hacker%27s_Delight
   glyph = ( glyph        & GLYPH(aa55aa55aa55aa55)       ) |
           ((glyph        & GLYPH(00aa00aa00aa00aa)) << 7 ) |
           ((glyph >> 7)  & GLYPH(00aa00aa00aa00aa)       ) ;
   glyph = ( glyph        & GLYPH(cccc3333cccc3333)       ) |
           ((glyph        & GLYPH(0000cccc0000cccc)) << 14) |
           ((glyph >> 14) & GLYPH(0000cccc0000cccc)       ) ;
   glyph = ( glyph        & GLYPH(f0f0f0f00f0f0f0f)       ) |
           ((glyph        & GLYPH(00000000f0f0f0f0)) << 28) |
           ((glyph >> 28) & GLYPH(00000000f0f0f0f0))        ;
   return glyph;
}

static inline uint64_t glyph_rotate90(uint64_t glyph)
{ return glyph_transpose(glyph_mirror(glyph)); }

static inline uint64_t glyph_rotate180(uint64_t glyph)
{ return glyph_mirror(glyph_flip(glyph)); }

static inline uint64_t glyph_rotate270(uint64_t glyph)
{ return glyph_transpose(glyph_flip(glyph)); }


/* shifts */

static inline uint64_t
glyph_shiftLeft(uint64_t glyph, unsigned n)
{ return (glyph << n) & (~GLYPH(0) - n * GLYPH(0101010101010101)); }
// works only for 1


// glyph_shiftLeft(uint64_t glyph, unsigned n)
// { return (glyph >>n) & n * GLYPH(0101010101010101); }
//
static inline uint64_t
glyph_shiftRight(uint64_t glyph, unsigned n)
{ return (glyph >> n) & (~GLYPH(0) - n * GLYPH(1010101010101010)); }

static inline uint64_t
glyph_shiftTop(uint64_t glyph, unsigned n)
{ return (glyph << GLYPH_WIDTH * n); }

static inline uint64_t
glyph_shiftBottom(uint64_t glyph, unsigned n)
{ return (glyph >> GLYPH_WIDTH * n); }



/* swaps */

static inline uint64_t  glyph_swapWidehalves(uint64_t glyph) // swap top/bottom halves
{ return (glyph << 32) | (glyph >> 32); }

static inline uint64_t  glyph_swapTallhalves(uint64_t glyph) // swap left/right halves
{ return glyph & GLYPH(f0f0f0f0f0f0f0f0); }
//{ return (glyph & GLYPH(f0f0f0f0f0f0f0f0)) | (glyph & GLYPH(0f0f0f0f0f0f0f0f)); }

// (use n=4 to swap top and bottom halves)
static inline uint64_t  glyph_cycleTop(uint64_t glyph, unsigned n)
{  assert(n < 8);
   n*=8; return (glyph << n) | (glyph >> (64 - n)); }

// (use n=4 to swap top and bottom halves)
static inline uint64_t  glyph_cycleBottom(uint64_t glyph, unsigned n)
{  assert(n < 8);
   n*=8; return (glyph >> n) | (glyph << (64 - n)); }


// merge glyph a and b, according to a mask
// mask bits contain 1 where bits from b should be selected; 0 where from a.
static inline uint64_t
glyph_merge(uint64_t a, uint64_t b, uint64_t mask) {
   // return (a & ~mask) | (b & mask);
   //
   // same but, this saves one operation:
   // (see: https://graphics.stanford.edu/~seander/bithacks.html#MaskedMerge)
   return a ^ ((a ^ b) & mask);
}


#endif //KONPU_GLYPH_H
