#ifndef  KONPU_H
#define  KONPU_H

#include "platform.h"
#include "c.h"

#include "util.h"
#include "glyph.h"
#include "rect.h"
#include "canvas.h"
#include "screen.h"
#include "font.h"
#include "print.h"

//===< renderers >==============================================================
#include "renderer.h"
#include "renderer_SDL2.h"
#include "renderer_ppm.h"
#include "renderer_pseudographics.h"

/// @brief Konpu tries to initialize a renderer with sensible defaults
/// @return 0 iff success
/// note: even if it fails, you can still call `render()` function,
///       although nothing will be shown.
static inline int renderer_initSimple()
{  // TODO: this is more complex than this.
   if (!rendererSDL2_init(NULL, 768, 432))
      return 0;
   if (!rendererPseudoGraphics_init(RENDERER_PSEUDOGRAPHICS_MODE_ENUMCOUNT))
      return 0;
   if (!rendererPPM_init(0, 0))
      return 0;
   return 1;
}
//===< /renderers >=============================================================

#endif //KONPU_H
////////////////////////////////////////////////////////////////////////////////


//===< includes the implementation >============================================
#ifdef   KONPU_IMPLEMENTATION
#   include "util.c"
#   include "canvas.c"
#   include "screen.c"
#   include "font.c"
#   include "print.c"
#   include "renderer.c"
#   include "renderer_SDL2.c"
#   include "renderer_ppm.c"
#   include "renderer_pseudographics.c"
#endif //KONPU_IMPLEMENTATION
//===</ includes the implementation >===========================================
