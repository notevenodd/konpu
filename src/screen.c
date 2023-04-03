#include "screen.h"

// main framebuffer:
static uint64_t konpu_framebuffer[GRID_WIDTH * GRID_HEIGHT];

// global screen:
canvas screen = { .glyphs = konpu_framebuffer,
                  .width  = GRID_WIDTH,
                  .height = GRID_HEIGHT,
                  .stride = GRID_WIDTH };
