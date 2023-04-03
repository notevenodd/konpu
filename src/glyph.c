#include "glyph.h"

pair
tallpair_fromPixel(int x, int y) {
   if (y < GLYPH_HEIGHT)  return (pair){ .first  = glyph_fromPixel(x, y) };
   else                   return (pair){ .second = glyph_fromPixel(x, y - GLYPH_HEIGHT) };
}

pair
widepair_fromPixel(int x, int y) {
   if (x < GLYPH_WIDTH)   return (pair){ .first  = glyph_fromPixel(x, y) };
   else                   return (pair){ .second = glyph_fromPixel(x - GLYPH_WIDTH, y) };
}

tetra
tetra_fromPixel(int x, int y) {
   tetra t = {0};
   if (x < GLYPH_WIDTH) {
      if (y < GLYPH_HEIGHT)  t.top_left     = glyph_fromPixel(x              , y               );
      else                   t.bottom_left  = glyph_fromPixel(x              , y - GLYPH_HEIGHT);
   } else {
      if (y < GLYPH_HEIGHT)  t.top_right    = glyph_fromPixel(x - GLYPH_WIDTH, y               );
      else                   t.bottom_right = glyph_fromPixel(x - GLYPH_WIDTH, y - GLYPH_HEIGHT);
   }
   return t;
}

uint64_t
tallpair_pixelValue(pair tallpair, int x, int y) {
   if (y < GLYPH_HEIGHT)  return glyph_pixelValue(tallpair.first,  x, y);
   else                   return glyph_pixelValue(tallpair.second, x, y - GLYPH_HEIGHT);
}

uint64_t
widepair_pixelValue(pair widepair, int x, int y) {
   if (x < GLYPH_WIDTH)   return glyph_pixelValue(widepair.first,  x, y);
   else                   return glyph_pixelValue(widepair.second, x - GLYPH_WIDTH, y);
}

uint64_t
tetra_pixelValue(tetra t, int x, int y) {
   if (x < GLYPH_WIDTH) {
      if (y < GLYPH_HEIGHT)  return glyph_pixelValue(t.top_left    , x              , y               );
      else                   return glyph_pixelValue(t.bottom_left , x              , y - GLYPH_HEIGHT);
   } else {
      if (y < GLYPH_HEIGHT)  return glyph_pixelValue(t.top_right   , x - GLYPH_WIDTH, y               );
      else                   return glyph_pixelValue(t.bottom_right, x - GLYPH_WIDTH, y - GLYPH_HEIGHT);
   }
}
