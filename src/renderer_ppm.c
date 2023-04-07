#include "renderer_ppm.h"
#if RENDERER_PPM
#include "screen.h"
#include <stdio.h>

/// @brief KONPU_RENDERER_PPM_STREAM settings:
/// By default, the PPM renderer will output to `stdout`, however you
/// may define another stream by defining this macro constant.
#ifndef    KONPU_RENDERER_PPM_STREAM
#   define KONPU_RENDERER_PPM_STREAM   stdout
#endif

/// @brief KONPU_RENDERER_PPM_UNLOCKED settings:
/// If this macro constant is defined and Konpu uses the POSIX platform, the PPM
/// renderer will use the "_unlocked" version of `putc` to output characters.
/// This may be faster, but is thread-UNSAFE.
#if defined(KONPU_RENDERER_PPM_UNLOCKED) && KONPU_PLATFORM_POSIX
#   define KONPU_RENDERER_PPM_PUTC(character, stream) \
           putc_unlocked((character), (stream))
#else
#   define KONPU_RENDERER_PPM_PUTC(character, stream)  \
           putc((character), (stream))
#endif


/// @brief KONPU_RENDERER_PPM_ZOOM_X and KONPU_RENDERER_PPM_ZOOM_Y settings:
/// By default, the PPM renderer will zoom the screen 3x. You can change this
/// by defining those macro variables.
#ifndef    KONPU_RENDERER_PPM_ZOOM_X
#   define KONPU_RENDERER_PPM_ZOOM_X 3  // default
#endif
#ifndef    KONPU_RENDERER_PPM_ZOOM_Y
#   define KONPU_RENDERER_PPM_ZOOM_Y 3  // default
#endif
// Assuming a dimension with 200 glyphs in one direction, 20*8*200 is max value
// which wouldn't overflow 15 bits (the max value a int may have if it's 16bits)
// So let's restrict zoom to that, which is more than enough anway.
#if (KONPU_RENDERER_PPM_ZOOM_X < 0) || (KONPU_RENDERER_PPM_ZOOM_X > 20)
#   error "KONPU_RENDERER_PPM_ZOOM_X value is too big"
#endif
#if (KONPU_RENDERER_PPM_ZOOM_Y < 0) || (KONPU_RENDERER_PPM_ZOOM_Y > 20)
#   error "KONPU_RENDERER_PPM_ZOOM_Y value is too big"
#endif


static int
canvas_renderToPPM(const_canvas cvas, FILE* stream, int zoomx, int zoomy)
{  CANVAS_ASSERT(cvas);
   // Assuming a dimension with 200 glyphs in one direction, 20*8*200 is max
   // value which wouldn't overflow 15 bits (the max value a int may have if
   // it's 16bits). So let's restrict the zoom values.
   assert(zoomx >= 0);  assert(zoomx <= 20);
   assert(zoomy >= 0);  assert(zoomy <= 20);

   printf("P6\n%d %d\n255\n", zoomx * GLYPH_WIDTH  * cvas.width,
                              zoomy * GLYPH_HEIGHT * cvas.height);

   for (int y = 0; y < GLYPH_HEIGHT * cvas.height; y++) {
       for (int ny = 0; ny < zoomy; ny++) {
           for (int x = 0; x < cvas.width; x++) {
               uint64_t glyph = canvas_glyph(cvas, x, y/GLYPH_HEIGHT);
               unsigned char line = glyph_line(glyph, y%GLYPH_HEIGHT);

               // TODO: read some color "attributes" for cell(x,y).
               //       fake it for now.
               static int FG_R = 0xFF, FG_G = 0xFF, FG_B = 0x00;
               static int BG_R = 0x00, BG_G = 0x00, BG_B = 0x80;

               for (int i = GLYPH_WIDTH - 1; i >= 0; i--) {
                   if (line & (1 << i)) {
                      for (int nx = 0; nx < zoomx; nx++) {
                        KONPU_RENDERER_PPM_PUTC(FG_R, stream);
                        KONPU_RENDERER_PPM_PUTC(FG_G, stream);
                        KONPU_RENDERER_PPM_PUTC(FG_B, stream);
                      }
                   } else {
                      for (int nx = 0; nx < zoomx; nx++) {
                        KONPU_RENDERER_PPM_PUTC(BG_R, stream);
                        KONPU_RENDERER_PPM_PUTC(BG_G, stream);
                        KONPU_RENDERER_PPM_PUTC(BG_B, stream);
                      }
                   }
               }
           }
       }
   }

   fflush(stream);
   return 0; // TODO: we haven't checked stdio's errors
}

// `render` function for the PPM renderer
static int rendererPPM_render(void)
{
   // TODO: So, we're just forwarding ...
   //       Maybe a `canvas_renderToPPM` function would make sense it we handle
   //       PPM images somewhere else in the code. But will we?...
   return canvas_renderToPPM(screen,
                             KONPU_RENDERER_PPM_STREAM,
                             KONPU_RENDERER_PPM_ZOOM_X,
                             KONPU_RENDERER_PPM_ZOOM_Y);
   // TODO: by the way, KONPU_RENDERER_PPM_ZOOM_X/Y really ought to be
   //       parameters to the `renderPPM_init` functions, and *NOT* some
   //       macro constants. Let's fix that.
}

int rendererPPM_init(void)
{
   // drop the active renderer (we can't do error checking)
   renderer_drop();
   rendererSingleton.error  = 0;

   // set the active render
   rendererSingleton.id     = RENDERER_PPM;
   rendererSingleton.render = &rendererPPM_render;
   render();
   return 0;
}

#endif //RENDERER_PPM
