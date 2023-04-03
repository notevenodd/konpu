#include "print.h"
#include "font.h"

void print(canvas cvas, int x, int y, const char* str)
{ /*TODO:not implemented*/ print_quadrant(cvas, 2*x,2*y, str); }

void print_widehalf(canvas cvas, int x, int y, const char* str)
{ /*TODO:not implemented*/ print_quadrant(cvas, 2*x,y, str); }

void print_tallhalf(canvas cvas, int x, int y, const char* str)
{ /*TODO:not implemented*/ print_quadrant(cvas, x,2*y, str); }


void print_quadrant(canvas cvas, int x, int y, const char* str)
{  CANVAS_ASSERT(cvas);
   assert(str);
   //TODO: CANVAS_CLAMP(....

   // half-grid coordinates to grid coordinates + quandrant location indicator
   bool left = x % 2; // print quadrant of the left side of a glyph?
   bool top  = y % 2; // print quadrant of the top  side of a glyph?
   x /= 2;
   y /= 2;

   uint16_t  q_left = 0;
   uint64_t *glyph = CANVAS_GLYPH_PTR(cvas, x,y);

   for (; *str != '\0'; str++) {
       unsigned char c = *str;
       uint16_t q = chr_quadrant(c);

       if (left) {
          q_left = q;
          glyph  = CANVAS_GLYPH_PTR(cvas, x,y);
          left   = false;
       } else {
          // before writing the quadrant, check if cursor is at a correct
          // position, then scroll up, or move if needed.
          if (x >= cvas.width) {
             x = 0;
             if (top) {
                top = false;
             } else {
                top = true;
                y++;
             }
          }
          if (y >= cvas.height) {
             // we scroll up everything one line: TODO
             // for now, let's loop back to the origin
             y = 0;
          }

          // write the quadrant in the glyph
          if (top) {
             *glyph = (uint64_t)widehalf2(q_left, q) << 32 |
                      (*glyph & GLYPH_BOTTOM) >> 32        ;
          } else {
             *glyph = (*glyph & GLYPH_TOP) |
                      widehalf2(q_left, q) ;
          }
          left=true;
          x++;
       }
   }


   // if we finished one the bottom-right corner, we need to make sure to clamp the
   // cursor there
   if (x >= cvas.width)  x = cvas.width;
}
