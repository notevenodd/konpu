#include "renderer_SDL2.h"
#include "screen.h"
#if KONPU_PLATFORM_SDL2

// globals variables for a SDL2 renderer:
static SDL_Window   *renderer_sdl2_win  = NULL;
static SDL_Renderer *renderer_sdl2_rndr = NULL;
static SDL_Texture  *renderer_sdl2_tex  = NULL;


// TODO: check how to do some basic error reporting
//       there's SDL_ShowSimpleMessageBox and more that might be used...
// in any case, it would be desirable to a log_error() function,
// which would work with same signature on all platforms.
#if KONPU_PLATFORM_LIBC
#   include <stdio.h>
    // not good. depending on how SDL2 is built, stdin/stdout may redirect to
    // a file or be closed, so stderr would be only last option.
    static inline int
    renderer_sdl2_error(const char* fmt, ...)
    { return fprintf(stderr, "[renderer]: %s\n", SDL_GetError()); }
#else
#   define renderer_sdl2_error(whatever, ...)
#endif



bool renderer_sdl2_init(const char* title, int win_width, int win_height)
{
   if ( (renderer_sdl2_win  != NULL) ||
        (renderer_sdl2_rndr != NULL) ||
        (renderer_sdl2_tex  != NULL) ){
      renderer_sdl2_error("%s", "already initialized?\n");
      return false;
   }

   // init video
   if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
      renderer_sdl2_error("%s", SDL_GetError());
      goto error_init;
   }

   // create window

   // TODO: SDL_WINDOW_RESIZABLE <-- nice, maybe, but should be configurable,
   //                         and we'd need to handle events too for resizing
   Uint32 win_flags = 0;
   // TODO: check win_width, win_height
   // they should be positive, ...
   // if they're both 0, maybe we could go full screen or something

   renderer_sdl2_win = SDL_CreateWindow(
                          (title)? title : "ilo Konpu",
                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                          win_width, win_height, win_flags);
   if (!renderer_sdl2_win) {
      renderer_sdl2_error("%s", SDL_GetError());
      goto error_window;
   }

   // create SDL renderer
   renderer_sdl2_rndr = SDL_CreateRenderer(renderer_sdl2_win, -1,
                            0); // TODO:   SDL_RENDERER_ACCELERATED???
                                //       | SDL_RENDERER_PRESENTVSYNC
   if (!renderer_sdl2_rndr) {
      renderer_sdl2_error("%s", SDL_GetError());
      goto error_renderer;
   }

   // create a single SDL_Texture for painting the whole screen onto.
   renderer_sdl2_tex = SDL_CreateTexture(renderer_sdl2_rndr,
                           SDL_PIXELFORMAT_ARGB8888,
                           SDL_TEXTUREACCESS_STREAMING,
                           screen.width  * GLYPH_WIDTH,
                           screen.height * GLYPH_HEIGHT);
   if (!renderer_sdl2_tex) {
      renderer_sdl2_error("%s", SDL_GetError());
      goto error_texture;
   }
   // TODO: maybe we should setup the texture with a single color
   //       (or something) and render it in the window.


return true;
error_texture:
      SDL_DestroyRenderer(renderer_sdl2_rndr);
error_renderer:
      SDL_DestroyWindow(renderer_sdl2_win);
error_window:
      SDL_QuitSubSystem(SDL_INIT_VIDEO);
error_init:
      renderer_sdl2_error("%s", SDL_GetError());
      return false;
}


void renderer_sdl2_drop()
{
   if (renderer_sdl2_tex) {
      SDL_DestroyTexture(renderer_sdl2_tex);
      renderer_sdl2_tex  = NULL;
   }
   if (renderer_sdl2_rndr) {
      SDL_DestroyRenderer(renderer_sdl2_rndr);
      renderer_sdl2_rndr = NULL;
   }
   if (renderer_sdl2_rndr) {
      SDL_DestroyWindow(renderer_sdl2_win);
      renderer_sdl2_win  = NULL;
   }
   SDL_QuitSubSystem(SDL_INIT_VIDEO);
}


// TODO: cvas should be screen!
bool renderer_sdl2_render(const_canvas cvas)
{  CANVAS_ASSERT(cvas);

   // lock our texture to gain **write-only** access to its pixels
   // all pixels *should* be written before unlocking the texture,
   // otherwise they may have uninitialized value.
   int       pitch;
   void     *pixel_data;
   SDL_LockTexture(renderer_sdl2_tex, NULL, &pixel_data, &pitch);
   uint32_t *pixels = pixel_data;
   assert(pixels != NULL);
   assert(pitch == (sizeof(*pixels) * cvas.width * GLYPH_WIDTH));

   // paint the canvas onto the texture's pixels
   for (int y = 0; y < GLYPH_HEIGHT * cvas.height; y++) {
       for (int x = 0; x < cvas.width; x++) {
           uint64_t glyph = CANVAS_GLYPH(cvas, x, y / GLYPH_HEIGHT);
           unsigned char line = glyph_line(glyph, y % GLYPH_HEIGHT);

           for (int i = (GLYPH_WIDTH - 1); i >= 0; i--) {
               // TODO/FIXME: maybe endianess could cause us issues
               //             also pushing uint32_t ???, naaah prob. fine
               //             or should it be bytes (unsigned char) instead
               //             or we should use SDL_MapRGB or something?
               // TODO:
               //       also in future, we'll have "attributes" here to
               //       determine the color
               if (line & (1 << i)) {
                  *pixels = UINT32_C(0x00cccc00); // pixel on
               } else {
                  *pixels = UINT32_C(0x00000080); // pixel off
               }
               pixels++;
           }
       }
   }

   // now release (unlock) the texture and render it
   SDL_UnlockTexture(renderer_sdl2_tex);
   if (SDL_RenderCopy(renderer_sdl2_rndr, renderer_sdl2_tex, NULL, NULL) < 0) {
      renderer_sdl2_error("%s", SDL_GetError());
      return false;
   }
   SDL_RenderPresent(renderer_sdl2_rndr);
   return true;
}


#endif //KONPU_PLATFORM_SDL2
