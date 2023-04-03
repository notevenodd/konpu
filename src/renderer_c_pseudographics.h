#ifndef  KONPU_RENDERER_C_PSEUDOGRAPHICS_H
#define  KONPU_RENDERER_C_PSEUDOGRAPHICS_H
#include "platform.h"
#include "c.h"
#ifdef   KONPU_PLATFORM_LIBC
#include "canvas.h"

// rendering a canvas using "pseudo graphics" characters:
// .------------------------------------------------------------.------------.
// | Unicode pseudographic characters used                      | character  |
// |                                                            |"resolution"|
// |------------------------------------------------------------|------------|
// | space + full block (U+2588) from "Block Elements"          |    1 x 1   |
// | same as 1x1 + horizontal half blocks from "Block Elements" |    1 x 2   |
// | same as 1x1 +  vertical  half blocks from "Block Elements" |    2 x 1   |
// | same as 1x2 + 2x1  + quadrant blocks from "Block Elements" |    2 x 2   |
// | same as 2x1 + sextants from "Symbols for Legacy Computing" |    2 x 3   |
// | all characters from the "Braille Patterns" block           |    2 x 4   |
// '------------------------------------------------------------'------------'

// print the canvas on stdout, using "1x1" blocks
void canvas_render_fullblocks(const_canvas cvas);

// print the canvas on stdout, using "1x2" half blocks
void canvas_render_halfblocks_h(const_canvas cvas);

// print the canvas on stdout, using "2x1" half blocks
void canvas_render_halfblocks_v(const_canvas cvas);

// print the canvas on stdout, using "2x2" quadrant blocks
void canvas_render_quadblocks(const_canvas cvas);

// print the canvas on stdout, using "2x3" sextants blocks
void canvas_render_sextants(const_canvas cvas);

// print the canvas on stdout, using "2x4" braille dots
void canvas_render_braille(const_canvas cvas);


#endif //KONPU_PLATFORM_LIBC
#endif //KONPU_RENDERER_C_PSEUDOGRAPHICS_H
