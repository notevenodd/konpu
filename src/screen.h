#ifndef  KONPU_SCREEN_H
#define  KONPU_SCREEN_H
#include "platform.h"
#include "c.h"

//===< SCREEN >=================================================================

#include "canvas.h"
extern canvas screen; // global canvas

// default screen size: aspect ratio and resolution mode
// glyph grid   resolution:     (MODE * ASPECT_Y) x (MODE * ASPECT_Y)
// actual pixel resolution: (8 * MODE * ASPECT_Y) x (8 * MODE * ASPECT_Y)
#ifndef KONPU_RES_ASPECT_X
#   define KONPU_RES_ASPECT_X   16
#endif
#ifndef KONPU_RES_ASPECT_Y
#   define KONPU_RES_ASPECT_Y    9
#endif

#ifndef KONPU_RES_MODE
#   define KONPU_RES_MODE        1
#endif

//------------------------------------------------------------------------------

// resolution in glyph
#define GRID_WIDTH    ((KONPU_RES_ASPECT_X) * (KONPU_RES_MODE))
#define GRID_HEIGHT   ((KONPU_RES_ASPECT_Y) * (KONPU_RES_MODE))

// resolution in pixels
#define RES_WIDTH     (GLYPH_WIDTH  * GRID_WIDTH)
#define RES_HEIGHT    (GLYPH_HEIGHT * GRID_HEIGHT)

//===</ SCREEN >================================================================

#endif //KONPU_SCREEN_H
