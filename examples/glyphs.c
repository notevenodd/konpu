// choose one platform for rendering (or control it from your build system)
#define  KONPU_PLATFORM_SDL2       // will use SDL2
// #define  KONPU_PLATFORM_POSIX   // will output on terminal

#define  KONPU_RES_MODE 1      // <-- some default resolution
#define  KONPU_IMPLEMENTATION  // <-- must be defined once to include the code
#include "konpu.h"             // <-- tadaaa!



void waitkey(void) {
   // TODO: HERE, WE CHEAT for now, by using SDL directly !!!
   //       The plan is for konpu to have its own simple input system
   //       to associate with a renderer. (probably covering just basic
   //       keyboard input)
#if KONPU_PLATFORM_SDL2
    SDL_Event event;
    while (true) {
       while(SDL_PollEvent(&event)) {
          switch(event.type) {
             case SDL_QUIT   : goto quit;
             case SDL_KEYDOWN: return;
          }
       }
       sleep_ms(10);
    }
    return;

quit:
    renderer_sdl2_drop();
    exit(0);
#endif
}


// paint the screen
static inline void paint(void)
{
#if KONPU_PLATFORM_SDL2
    if (!renderer_sdl2_render(screen)) {
       renderer_sdl2_drop(); exit(1);
    }
#elif KONPU_PLATFORM_LIBC
    // escape sequence to clear tty ("\x1b[H\x1b[J")
    putchar(0x1b); putchar(0x5b); putchar(0x48);
    putchar(0x1b); putchar(0x5b); putchar(0x4a);
    // one of the pure-C renderer:
    canvas_render_braille(screen);
#else
#   error "no platform"
#endif
#
}


#define EXAMPLE   GLYPH(7C022A0204A8A800)  // soweli glyph

// print message and glyph
void show(const char* msg, uint64_t glyph)
{
   canvas_set(screen, 0);
   print_quadrant(screen, 0,0, msg);
   canvas_glyph(screen, 1,2) = EXAMPLE;

   #define CHECKER  GLYPH(aa55aa55aa55aa55)
   canvas_glyph(screen, 3,1) = CHECKER & GLYPH_BOTTOM_RIGHT; canvas_glyph(screen, 4,1) = CHECKER & GLYPH_BOTTOM; canvas_glyph(screen, 5,1) = CHECKER & GLYPH_BOTTOM_LEFT;
   canvas_glyph(screen, 3,2) = CHECKER & GLYPH_RIGHT       ; canvas_glyph(screen, 4,2) = glyph                 ; canvas_glyph(screen, 5,2) = CHECKER & GLYPH_LEFT;
   canvas_glyph(screen, 3,3) = CHECKER & GLYPH_TOP_RIGHT   ; canvas_glyph(screen, 4,3) = CHECKER & GLYPH_TOP   ; canvas_glyph(screen, 5,3) = CHECKER & GLYPH_TOP_LEFT;

   print_quadrant(screen, 0,10, "PRESS ANY KEY");

   // refresh and wait for a key
   paint();
   waitkey();
}

int main(int argc, char **argv)
{  (void)argc; (void)argv; // not using argc/argv

#if KONPU_PLATFORM_SDL2
    if (!renderer_sdl2_init("Glyphs Operations Demo", 768, 432))  return 1;
#endif

   uint64_t g = EXAMPLE;
   while(true) {
      //fixme: print! eats up the last character
      show("normal "        , g);
      show("~ inverse "     , ~g);
      show("mirror "        , glyph_mirror(g));
      show("flip "          , glyph_flip(g));
      show("rotate 90 "     , glyph_rotate90(g));
      show("rotate 180 "    , glyph_rotate180(g));
      show("rotate 270 "    , glyph_rotate270(g));
      show("transpose "     , glyph_transpose(g));
   }

   return 1;
}
