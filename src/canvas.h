#ifndef  KONPU_CANVAS_H
#define  KONPU_CANVAS_H
#include "platform.h"
#include "glyph.h"
#include "rect.h"

//===< CANVAS >=================================================================

typedef struct canvas {
  uint64_t   *glyphs;
  int         width;
  int         height;
  int         stride;  // https://en.wikipedia.org/wiki/Stride_of_an_array
} canvas;

typedef canvas const_canvas;
// we use this synonym when indicating that we don't INTEND to modify the glyphs
// on the canvas. Of course, it means nothing, the compiler will not check.


// macros for *direct* access to glyphs (or their numerical index):
// GLYPH and GLYPH_PTR macros can serve as either a rvalue or lvalue
// they do no bound checking, so use responsibly.
#define CANVAS_GLYPH(canvas, x,y)       (*CANVAS_GLYPH_PTR((canvas), (x),(y)))
#define CANVAS_GLYPH_IDX(canvas, x,y)   ((x) + (y) * (canvas).stride)
#define CANVAS_GLYPH_PTR(canvas, x,y)   ((canvas).glyphs + CANVAS_GLYPH_IDX((canvas), (x),(y)))


// SET or BLEND a GLYPH at position (x,y)
// --------------------------------------
//
// * Set whole glyph as-is:
//   copy   : CANVAS_GLYPH(canvas, x,y)  =  glyph;
//
// * Blend using the set pixels of the glyph:
//   with the following effects on the canvas pixels:
//   - set    : CANVAS_GLYPH(canvas, x,y) |=  glyph;
//   - unset  : CANVAS_GLYPH(canvas, x,y) &= ~glyph;
//   - toggle : CANVAS_GLYPH(canvas, x,y) ^= ~glyph;
//
// * Blend using a mask:
//   uint64_t mask; // contains 1 where bits from the glyph are selected
//   uint64_t *bg = CANVAS_PTR(canvas, x,y);
//   *bg = UINT_MERGE(*bg, glyph, mask);
//
//
// MANIPULATE PIXELS at (x,y)
// --------------------------
//
//   - Query the value:
//     bool lit = uint_bit(CANVAS_GLYPH(canvas, x/8, y/8),
//                         glyph_pixel_idx(x%8, y%8));
//
//   - Set/Unset/Toggle pixel(x,y)
//     - set    : CANVAS_GLYPH(canvas, x/8, y/8) |=  glyph_pixel(x%8, y%8);
//     - unset  : CANVAS_GLYPH(canvas, x/8, y/8) &= ~glyph_pixel(x%8, y%8);
//     - toggle : CANVAS_GLYPH(canvas, x/8, y/8) ^= ~glyph_pixel(x%8, y%8);



/////////////////////
// canvas creation //
/////////////////////

// a literal of one canvas which is a null state
#define CANVAS_NULL             ((canvas){0})

// for a given canvas, returns a canvas defined by the crop area.
// the returned canvas will *share* the same underlying memory.
// if the crop area is not visible, the null canvas will be returned.
static inline  canvas  canvas_crop(canvas cvas, rect crop_area);


/////////////////
// safe access //
/////////////////

// return true iff canvas is in a *null state*
// null state means glyph pointer is null or width or height is 0
static inline  bool  canvas_isnull(canvas c);

// assert canvas isn't in an *invalid state*
// a canvas is not in an invalid state iff all those conditions are true:
// - its dimensions and stride are positive
// - (TODO: stride >= width ???)
// - its glyphs pointer may be NULL, but then its dimensions must be 0
#define CANVAS_ASSERT(canvas) do            \
        { assert((canvas).height >= 0);     \
          assert((canvas).width  >= 0);     \
          assert((canvas).stride >= 0);     \
          assert((canvas).glyphs || ((canvas).height == 0 && (canvas).width == 0)); \
        } while(0)

// clamp (signed integer) coordinate variables to a canvas dimensions
#define CANVAS_CLAMP(canvas, x,y) do        \
        { assert((canvas));                 \
          UTIL_CLAMP((x), (canvas).width);  \
          UTIL_CLAMP((y), (canvas).height); \
        } while (0)

// returns the glyph at position (x,y)
//         or 0 if position is invalid or if canvas is null
// (note: 0 may also be returned as a glyph, not just
//        when canvas is null or position is invalid)
static inline uint64_t canvas_at(canvas c, int x, int y);

// set a glyph at position (x,y)
// will return true iff glyph is set (valid position, canvas not null)
static inline bool canvas_putglyph(canvas c, uint64_t glyph, int x, int y);

/* TODO: use a function pointer or what to have multiple renderer ???
// specify a platform-agnostic interface for rendering a canvas
// the .c implementations of this header will be platform dependent
// (be it C, SDL, HTML <canvas>, X11, linux framebuffer, ...)
void canvas_render(const_canvas cvas);
*/



//--- inline implementation ----------------------------------------------------

static inline bool canvas_isnull(canvas cvas)
{  CANVAS_ASSERT(cvas);
   // return (cvas.glyphs == NULL || cvas.height == 0 || cvas.width == 0);
   //        ^-- let's use <= 0 (shouldn't be more "expensive" than == 0),
   //            in case we get an invalid canvas and `assert` is disabled.
   return (cvas.glyphs == NULL || cvas.height <= 0 || cvas.width <= 0);
}

static inline uint64_t
canvas_at(canvas cvas, int x, int y)
{ return (canvas_isnull(cvas) || x >= cvas.width || y >= cvas.height) ?
         CANVAS_GLYPH(cvas, x,y) : 0; }

static inline bool
canvas_putglyph(canvas cvas, uint64_t glyph, int x, int y)
{  if (canvas_isnull(cvas) || x >= cvas.width || y >= cvas.height)
      return false;
   CANVAS_GLYPH(cvas, x,y) = glyph;
   return true;
}

static inline canvas
canvas_crop(canvas cvas, rect crop_area)
{  CANVAS_ASSERT(cvas);

   if (rect_clip(&crop_area, cvas.width, cvas.height)) {
      cvas.glyphs += CANVAS_GLYPH_IDX(cvas, crop_area.x, crop_area.y),
      cvas.width   = crop_area.w;
      cvas.height  = crop_area.h;
      return cvas;
   } else {
      return CANVAS_NULL;
   }
}


////////////////////////////////////////////////////////////////////////////////

// fill the canvas' grid with one glyph
static inline void
canvas_set(canvas cvas, uint64_t glyph)
{  CANVAS_ASSERT(cvas);
   for (int y = 0; y < cvas.height; y++)
       for (int x = 0; x < cvas.width; x++)
           CANVAS_GLYPH(cvas, x,y) = glyph;
}


// draw a line on the given canvas between the points (x0,y0) and (x1,y1)
void canvas_line(canvas cvas, int x0, int y0, int x1, int y1);

#endif //KONPU_CANVAS_H
