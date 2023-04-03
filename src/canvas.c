#include "canvas.h"

// TODO/FIXME: __builtin_abs is a gcc/clang builtin
//             `int abs(int z)` is otherwise from <stdlib.h>
//             so, TODO => macro wrap those properly somewhere
#define abs(x)  __builtin_abs((x))

void canvas_line(canvas cvas, int x0, int y0, int x1, int y1)
{  CANVAS_ASSERT(cvas);

   // TODO: the special cases of drawing a vertical or an horizontal line
   //       could be handled separately and optimized a lot.
   // if (y0 == y1) { // horizontal line
   //   ...
   // } else if (x0 == x1) { // vertical line
   //   ...
   // } else {
   //   bresenham as below ...


   // This is the classic Bresenham's Algorithm which draws straight lines
   // using only simple integer arithmetic.
   // see: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
   //
   // This particular way to handle both directions x and y simultaneously is
   // based on: https://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C
   //           Content in Rosetta code is (CC BY-SA 4.0)
   //           https://creativecommons.org/licenses/by-sa/4.0/

   int dx  = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
   int dy  = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
   int err = (dx>dy ? dx : -dy)/2, e2;

   for(;;) {

      // setPixel(x0,y0)
      if (x0 >= 0  &&  x0 < GLYPH_WIDTH  * cvas.width  &&
          y0 >= 0  &&  y0 < GLYPH_HEIGHT * cvas.height) {
         // TODO: after implementing canvas_pixel* macros/inline funcs
         //       remove this ugly expression
         CANVAS_GLYPH(cvas, x0 / 8, y0 / 8) |= glyph_fromPixel(x0 % 8, y0 % 8);
      }

      if (x0 == x1 && y0 == y1) break;
      e2 = err;
      if (e2 >-dx) { err -= dy; x0 += sx; }
      if (e2 < dy) { err += dx; y0 += sy; }
   }
}
