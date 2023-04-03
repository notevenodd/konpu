#include "renderer_c_pseudographics.h"
#if KONPU_PLATFORM_LIBC
#include <stdio.h>


// print the two first utf-8 bytes for a U+25xx character
#define block() do { putchar(0xe2); putchar(0x96); } while(0)

// TODO: user could decide to clean a terminal or not
//       put this somewhere ...
// escape sequence for clearing a terminal
#define tty_clear() do { /* "\x1B[H\x1B[J" */           \
          putchar(0x1b); putchar(0x5b); putchar(0x48);  \
          putchar(0x1b); putchar(0x5b); putchar(0x4a);  \
        } while (0)


void canvas_render_fullblocks(const_canvas cvas)
{  CANVAS_ASSERT(cvas);
   for (int y = 0; y < GLYPH_WIDTH * cvas.height; y++) {
       for (int x = 0; x < GLYPH_HEIGHT * cvas.width; x++) {

           // get a position for pixel (x,y):
           uint64_t glyph = CANVAS_GLYPH(cvas, x / GLYPH_WIDTH, y / GLYPH_HEIGHT);
           int n = glyph_pixelIndex(x % GLYPH_WIDTH, y % GLYPH_HEIGHT);

           // read pixel value and print space or block accordingly:
           if (uint_bit(glyph, n)) {
              block(); putchar(0x88); //U+2588 (full block)
           } else {
              putchar(0x20); // space
           }

       }
       putchar(0x0a); // '\n'
   }
   fflush(stdout);
}


void canvas_render_halfblocks_h(const_canvas cvas)
{  CANVAS_ASSERT(cvas);
   for (int y = 0; y < GLYPH_WIDTH * cvas.height; y += 2) {
       for (int x = 0; x < GLYPH_HEIGHT * cvas.width; x++) {

           // get a position for pixel (x,y):
           uint64_t glyph = CANVAS_GLYPH(cvas, x / GLYPH_WIDTH, y / GLYPH_HEIGHT);
           int n = glyph_pixelIndex(x % GLYPH_WIDTH, y % GLYPH_HEIGHT);

           // print half block
           unsigned char c = uint_bit(glyph, n)               << 1 |
                             uint_bit(glyph, n - GLYPH_WIDTH)      ;
           switch(c) {
              case 0 : putchar(0x20);          break; // space
              case 1 : block(); putchar(0x84); break; // U+2584 (lower half)
              case 2 : block(); putchar(0x80); break; // U+2580 (upper half)
              case 3 : block(); putchar(0x88); break; // U+2588 (full block)
              default: unreachable();
           }

       }
       putchar(0x0a); // '\n'
   }
   fflush(stdout);
}


void canvas_render_halfblocks_v(const_canvas cvas)
{  CANVAS_ASSERT(cvas);
   for (int y = 0; y < GLYPH_WIDTH * cvas.height; y++) {
       for (int x = 0; x < GLYPH_HEIGHT * cvas.width; x += 2) {

           // get a position for pixel (x,y):
           uint64_t glyph = CANVAS_GLYPH(cvas, x / GLYPH_WIDTH, y / GLYPH_HEIGHT);
           int n = glyph_pixelIndex(x % GLYPH_WIDTH, y % GLYPH_HEIGHT);

           // print vertical half block
           unsigned char c = uint_bit(glyph, n)     << 1 |
                             uint_bit(glyph, n - 1)      ;
           switch(c) {
              case 0 : putchar(0x20);          break; // space
              case 1 : block(); putchar(0x90); break; // U+2590 (right half)
              case 2 : block(); putchar(0x8c); break; // U+258C (left half)
              case 3 : block(); putchar(0x88); break; // U+2588 (full block)
              default: unreachable();
           }

       }
       putchar(0x0a); // '\n'
   }
   fflush(stdout);
}


void canvas_render_quadblocks(const_canvas cvas)
{  CANVAS_ASSERT(cvas);
   for (int y = 0; y < GLYPH_WIDTH * cvas.height; y += 2) {
       for (int x = 0; x < GLYPH_HEIGHT * cvas.width; x += 2) {

           // get a position for pixel (x,y):
           uint64_t glyph = CANVAS_GLYPH(cvas, x / GLYPH_WIDTH, y / GLYPH_HEIGHT);
           int n = glyph_pixelIndex(x % GLYPH_WIDTH, y % GLYPH_HEIGHT);

           // read a half block character and print it out:
           unsigned char c = uint_bit(glyph, n)                   << 3 |
                             uint_bit(glyph, n - 1)               << 2 |
                             uint_bit(glyph, n - GLYPH_WIDTH)     << 1 |
                             uint_bit(glyph, n - GLYPH_WIDTH - 1)      ;

           // print it out as UTF-8
           static unsigned char quad_lastbyte[] = {
              0x20, 0x97, 0x96, 0x84, 0x9d, 0x90, 0x9e, 0x9f,
              0x98, 0x9a, 0x8c, 0x99, 0x80, 0x9c, 0x9b, 0x88  };
           if (c)  block();
           putchar(quad_lastbyte[c]);

       }
       putchar(0x0a); // '\n'
   }
   fflush(stdout);
}


void canvas_render_braille(const_canvas cvas)
{  CANVAS_ASSERT(cvas);
   for (int y = 0; y < GLYPH_WIDTH * cvas.height; y += 4) {
       for (int x = 0; x < GLYPH_HEIGHT * cvas.width; x += 2) {

           // get a position for pixel (x,y):
           uint64_t glyph = CANVAS_GLYPH(cvas, x / GLYPH_WIDTH, y / GLYPH_HEIGHT);
           int n = glyph_pixelIndex(x % GLYPH_WIDTH, y % GLYPH_HEIGHT);

           // Read the 2x4 bit pixels of the braille cell.        0x01 0x08
           // The 'Braille Patterns' Unicode block assigns  ___\  0x02 0x10
           // the codepoint 0x28XX, with XX being computed     /  0x04 0x20
           // with those weights for the raised bits,             0x40 0x80
           unsigned char braille =
              uint_bit(glyph, n                      ) * 0x01 |
              uint_bit(glyph, n -                   1) * 0x08 |
              uint_bit(glyph, n -     GLYPH_WIDTH    ) * 0x02 |
              uint_bit(glyph, n -     GLYPH_WIDTH - 1) * 0x10 |
              uint_bit(glyph, n - 2 * GLYPH_WIDTH    ) * 0x04 |
              uint_bit(glyph, n - 2 * GLYPH_WIDTH - 1) * 0x20 |
              uint_bit(glyph, n - 3 * GLYPH_WIDTH    ) * 0x40 |
              uint_bit(glyph, n - 3 * GLYPH_WIDTH - 1) * 0x80 ;

           // conversion to UTF-8 representation (3 bytes) reduces to:
           putchar(0xe2);
           putchar(0xa0 | (braille >> 6));
           putchar( 128 | (braille & 63));

       }
       putchar(0x0a); // '\n'
   }
   fflush(stdout);
}



// FIXME: this rendered is broken!
//        especially, when the sextant must read "pixel" from two glyphs,
//        this is not working properly.
void canvas_render_sextants(const_canvas cvas)
{  CANVAS_ASSERT(cvas);
   // The main issue with sextants is their height (3) isn't a divisor of the
   // glyph's height. The info to put in a sextant might come from TWO glyphs.

   for (int y = 0; y < GLYPH_WIDTH * cvas.height; y += 3) {
       for (int x = 0; x < GLYPH_HEIGHT * cvas.width; x += 2) {

           // glyph from which the sextant info comes from ...
           uint64_t glyph = CANVAS_GLYPH(cvas, x / GLYPH_WIDTH, y / GLYPH_HEIGHT);
           int ypos = y % (GLYPH_WIDTH);
           int n = glyph_pixelIndex(x % GLYPH_WIDTH, ypos);

           // read the sextant pixels
           unsigned sext = uint_bit(glyph, n                      ) << 5 |
                           uint_bit(glyph, n -                   1) << 4 ;
                           // we started by reading the first line
           switch (ypos) {
              default:     // we can read the two following line from same glyph
                  sext |=  uint_bit(glyph, n -     GLYPH_WIDTH    ) << 3 |
                           uint_bit(glyph, n -     GLYPH_WIDTH - 1) << 2 |
                           uint_bit(glyph, n - 2 * GLYPH_WIDTH    ) << 1 |
                           uint_bit(glyph, n - 2 * GLYPH_WIDTH - 1)      ;
                  break;

              case  6:     // we can read one more line from same glyph
                  sext |=  uint_bit(glyph, n -     GLYPH_WIDTH    ) << 3 |
                           uint_bit(glyph, n -     GLYPH_WIDTH - 1) << 2 ;
                  if (y > GLYPH_WIDTH * cvas.height)
                     break;
                  glyph = *(&glyph + cvas.stride); // then we go to the glyph
                  n = glyph_pixelIndex(x % GLYPH_WIDTH, 0);  //    underneath
                  sext |=  uint_bit(glyph, n                      ) << 2 |
                           uint_bit(glyph, n                   - 1) << 1 ;
                  break;

              case  7:     // we need to read all the rest from underneath glyph
                  if (y > GLYPH_WIDTH * cvas.height)
                     break;
                  glyph = *(&glyph + cvas.stride);
                  n = glyph_pixelIndex(x % GLYPH_WIDTH, 0);
                  sext |=  uint_bit(glyph, n                      ) << 3 |
                           uint_bit(glyph, n                   - 1) << 2 |
                           uint_bit(glyph, n -     GLYPH_WIDTH    ) << 2 |
                           uint_bit(glyph, n -     GLYPH_WIDTH - 1) << 1 ;
                  break;
           }


//TODO/FIXME ASAP NOW: quick ugly temporary hack 'cause we       ^
//                     built the sextant wrongly!!! FIX ABOVE ---' and remove:
sext = byte_reverse(sext) >> 2;


           // print out the sextant characters in UTF-8

           // codepoints for sextants in the 'Symbols for Legacy Computing'
           // Unicode blocks are logically arranged as per the bit pixels, but
           // as sextant can be vertical half blocks, they sometimes have other
           // codepoints. I first thought to just have a "switch" or lookup for
           // all 64 cases, but I figured out this logic which isn't so
           // expensive to compute... do an integer division by 21:
           // sext = 21 * q + r,
           //  if r != 0  => codepoint is: (0x1FB00 - 1) + 20*q + r
           //             => else sextant is a half block.
           // in terms of UTF-8, codepoitn 0x1FBxx is easy to compute.

           unsigned q = sext / 21;
           if (sext % 21) {
              // the UTF-8 conversion of codepoint 0x1FB00 - 1 + sext - q
              // is four bytes and reduces to this:
              putchar(0xf0); putchar(0x9f); putchar(0xac);
              putchar(128|(sext - q - 1));
           } else {
              switch(q) { // sext is vertical halves/empy/full
                 case 0 : putchar(0x20);          break; // space
                 case 1 : block(); putchar(0x8c); break; // U+258C (left half)
                 case 2 : block(); putchar(0x90); break; // U+2590 (right half)
                 case 3 : block(); putchar(0x88); break; // U+2588 (full block)
                 default: unreachable();
              }
           }

       }
       putchar(0x0a); // '\n'
   }
   fflush(stdout);
}



#undef block
#undef tty_clear
#endif //KONPU_PLATFORM_LIBC
