// choose one platform for rendering (or control it from your build system)
#define  KONPU_PLATFORM_SDL2       // will use SDL2
// #define  KONPU_PLATFORM_POSIX   // will output on terminal

#define  KONPU_RES_MODE 2      // <-- some default resolution
#define  KONPU_IMPLEMENTATION  // <-- must be defined once to include the code
#include "konpu.h"             // <-- tadaaa!


// paint the screen
static inline void paint(int delay)
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

   // sleep some n * 60th of a seconds
   sleep_ms(delay * 1000/60);
}


int main(int argc, char **argv)
{  (void)argc; (void)argv; // not using argc/argv

#if KONPU_PLATFORM_SDL2
    if (!renderer_sdl2_init("hello, world!", 768, 432))  return 1;
#endif

   random_init(0xCAFE);

   // some toki pona glyphs
#  define GLYPH_LI         GLYPH(6030180C18306000)
#  define GLYPH_SONA       GLYPH(9254007C44447C00)
#  define GLYPH_SOWELI     GLYPH(7C022A0204A8A800)
#  define GLYPH_SUNO       GLYPH(101028C628101000)
#  define GLYPH_JAN        GLYPH(386C6C6C386CC600) // bold

   // we'll wait a bit before rendering a frame:
#  define DELAY            15 //milliseconds

   while(true) {

      // TODO: HERE, WE CHEAT for now, by using SDL directly !!!
      //       The plan is for konpu to have its own simple input system
      //       to associate with a renderer. (probably covering just basic
      //       keyboard input)
      #if KONPU_PLATFORM_SDL2
          SDL_Event event;
          while(SDL_PollEvent(&event)) {
             switch(event.type) {
                case SDL_QUIT: goto quit;
             }
          }
      #endif

      canvas_set(screen, 0); paint(DELAY);

      CANVAS_GLYPH(screen, 0,0) = GLYPH_SOWELI;  paint(DELAY);

      CANVAS_GLYPH(screen, 1,0) = glyph4(chr_quadrant('I'), chr_quadrant('L'),
                                         chr_quadrant('K'), chr_quadrant('O')) | GLYPH(ff);
      paint(DELAY);
      CANVAS_GLYPH(screen, 2,0) = glyph4(chr_quadrant('O'), 0                ,
                                         chr_quadrant('N'), chr_quadrant('P'));
      paint(DELAY);
      CANVAS_GLYPH(screen, 3,0) = glyph4(0                , chr_quadrant('&'),
                                         chr_quadrant('U'), 0);
      paint(DELAY);

      CANVAS_GLYPH(screen,  5,0) = chr('H');  paint(DELAY);
      CANVAS_GLYPH(screen,  6,0) = chr('E');  paint(DELAY);
      CANVAS_GLYPH(screen,  7,0) = chr('L');  paint(DELAY);
      CANVAS_GLYPH(screen,  8,0) = chr('L');  paint(DELAY);
      CANVAS_GLYPH(screen,  9,0) = chr('O');  paint(DELAY);

      CANVAS_GLYPH(screen, 0,2) |= glyph_rotate180(GLYPH_SOWELI);
      CANVAS_GLYPH(screen, 1,2) |= GLYPH_LI;
      CANVAS_GLYPH(screen, 2,2) |= glyph_flip(GLYPH_SONA);
      CANVAS_GLYPH(screen, 0,3) |= GLYPH_JAN;
      CANVAS_GLYPH(screen, 1,3) |= GLYPH_SONA;
      CANVAS_GLYPH(screen, 2,3) |= GLYPH_LI;
      CANVAS_GLYPH(screen, 3,3) |= GLYPH_SUNO;
      paint(DELAY);

      canvas_line(screen, -8,8, 100,64);
      paint(DELAY);

      // create another canvas in the screen:
      rect crop = {12,4, -20,80};
      canvas area = canvas_crop(screen, crop);
      canvas_set(area, GLYPH(5fbe5fbe5fbe5fbe));
      paint(DELAY);

      print_quadrant(area, 6,1, "ilo Konpu says this: Hello, World! Hello there, how Are you today? Has it been a fantastic day, or what?");
      paint(DELAY);

      for(int i = 0; i < 75; i++) {
         canvas_line(screen, random()%256,random()%144, random()%256,random()%144);
         paint(3);
      }

   }

#if KONPU_PLATFORM_SDL2
quit:
   renderer_sdl2_drop();
#endif
   return 0;
}
