/*******************************************************************************
 * @file
 * The PPM renderer prints the canvas on C standard output as a "P6" PPM image
 * see: https://en.wikipedia.org/wiki/Netpbm
 *
 * This might be used, for example, to create a simple konpu program in pure C,
 * and then from your shell pipe its output into a media player, such as
 * mpv(1): `... | mpv ... --no-correct-pts -`
 ******************************************************************************/
#ifndef  KONPU_RENDERER_PPM_H
#define  KONPU_RENDERER_PPM_H
#include "platform.h"
#include "c.h"

#if KONPU_PLATFORM_LIBC
#   define RENDERER_PPM        3

/// @brief  initialize the "PPM" renderer
/// @param  zoomx how much to zoom (in the x direction).
///         if value is not in [1-20], a default sensible value is chosen.
/// @param  zoomy how much to zoom (in the y direction).
///         if value is not in [1-20], a default sensible value is chosen.
/// @return 0 iff initialization if successful
///         (always the case if the renderer if available)
int rendererPPM_init(int zoomx, int zoomy);

#else
#   define RENDERER_PPM                    0
#   define rendererPPM_init(zoomx, zoomy)  1

#endif //KONPU_PLATFORM_LIBC
#endif //KONPU_RENDERER_PPM_H
