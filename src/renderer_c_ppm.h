#ifndef  KONPU_RENDERER_C_PPM_H
#define  KONPU_RENDERER_C_PPM_H
#include "platform.h"
#ifdef   KONPU_PLATFORM_LIBC

#include "canvas.h"

// print the canvas on C standard output as a "P6" PPM image
// see: https://en.wikipedia.org/wiki/Netpbm
//
// This might be used, for example, to create a simple konpu program in pure C,
// and then from your shell pipe its output into a media player, such as
// mpv(1): `... | mpv ... --no-correct-pts -`
//
void canvas_render_ppm(const_canvas cvas);

#endif //KONPU_PLATFORM_LIBC
#endif //KONPU_RENDERER_C_PPM_H
