#include "renderer_c_ppm.h"
#if KONPU_PLATFORM_LIBC
#include <stdio.h>

//=== C PPM renderer ===========================================================

#ifndef KONPU_RENDERER_PPM_ZOOM_X
#define KONPU_RENDERER_PPM_ZOOM_X 3  // default ZOOM_X
#endif
#ifndef KONPU_RENDERER_PPM_ZOOM_Y
#define KONPU_RENDERER_PPM_ZOOM_Y 3  // default ZOOM_Y
#endif

// Assuming a dimension with 200 glyphs in one direction, 20*8*200 is max value
// which wouldn't overflow 15 bits (the max value a int may have if it's 16bits).
// So let's restrict zoom to that, which is more than enough anway.
#if (KONPU_RENDERER_PPM_ZOOM_X < 0) || (KONPU_RENDERER_PPM_ZOOM_X > 20)
#   error "KONPU_RENDERER_PPM_ZOOM_X value is too big"
#endif
#if (KONPU_RENDERER_PPM_ZOOM_Y < 0) || (KONPU_RENDERER_PPM_ZOOM_Y > 20)
#   error "KONPU_RENDERER_PPM_ZOOM_Y value is too big"
#endif


void canvas_render_ppm(const_canvas cvas)
{  CANVAS_ASSERT(cvas);

   printf("P6\n%d %d\n255\n", KONPU_RENDERER_PPM_ZOOM_X * GLYPH_WIDTH  * cvas.width,
                              KONPU_RENDERER_PPM_ZOOM_Y * GLYPH_HEIGHT * cvas.height);

   for (int y = 0; y < GLYPH_HEIGHT * cvas.height; y++) {
       for (int ny = 0; ny < KONPU_RENDERER_PPM_ZOOM_Y; ny++) {
           for (int x = 0; x < cvas.width; x++) {
               uint64_t glyph = canvas_glyph(cvas, x, y/GLYPH_HEIGHT);
               unsigned char line = glyph_line(glyph, y%GLYPH_HEIGHT);

               // TODO: read some color "attributes" for cell(x,y).
               //       fake it for now.
               static int FG_R = 0xFF, FG_G = 0xFF, FG_B = 0x00;
               static int BG_R = 0x00, BG_G = 0x00, BG_B = 0x80;

               for (int i = GLYPH_WIDTH - 1; i >= 0; i--) {
                   if (line & (1 << i)) {
                      for (int nx = 0; nx < KONPU_RENDERER_PPM_ZOOM_X; nx++) {
                          putchar(FG_R); putchar(FG_G); putchar(FG_B);
                      }
                   } else {
                      for (int nx = 0; nx < KONPU_RENDERER_PPM_ZOOM_X; nx++) {
                          putchar(BG_R); putchar(BG_G); putchar(BG_B);
                      }
                   }
               }
           }
       }
   }

   fflush(stdout);
}

//====</ C PPM renderer > ======================================================

#endif //KONPU_PLATFORM_LIBC
