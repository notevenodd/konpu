/*******************************************************************************
 * @file
 * The PseudoGraphics renderer prints a textual representation of the screen
 * by outputting Unicode pseudographics characters encoded in UTF-8.
 * see: https://en.wikipedia.org/wiki/Semigraphics
 ******************************************************************************/
#ifndef  KONPU_RENDERER_PSEUDOGRAPHICS_H
#define  KONPU_RENDERER_PSEUDOGRAPHICS_H
#include "platform.h"
#include "c.h"

/// @brief indicates to the PseudoGraphics renderer which set of characters
///        (based on their "resolution")
typedef enum rendererPseudoGraphicsMode {
   RENDERER_PSEUDOGRAPHICS_MODE_1x1, ///< full blocks            (space + full block (U+2588) from "Block Elements")
   RENDERER_PSEUDOGRAPHICS_MODE_1x2, ///< horizontal half blocks (same as 1x1 + horizontal half blocks from "Block Elements")
   RENDERER_PSEUDOGRAPHICS_MODE_2x1, ///< vertical half blocks   (same as 1x1 +  vertical  half blocks from "Block Elements")
   RENDERER_PSEUDOGRAPHICS_MODE_2x2, ///< quadrant blocks        (same as 1x2 + 2x1  + quadrant blocks from "Block Elements")
   RENDERER_PSEUDOGRAPHICS_MODE_2x3, ///< sextant characters     (same as 2x1 + sextants from "Symbols for Legacy Computing")
   RENDERER_PSEUDOGRAPHICS_MODE_2x4, ///< braille characters     (all characters from the "Braille Patterns" block)

   RENDERER_PSEUDOGRAPHICS_MODE_ENUMCOUNT  ///< enum counter
} rendererPseudoGraphicsMode;


#if KONPU_PLATFORM_LIBC
/// @brief RENDERER_PSEUDOGRAPHICS is set to a non-zero value if the
///        PseudoGraphics renderer is available.
#   define RENDERER_PSEUDOGRAPHICS            4

/// @brief  initialize a PseudoGraphics renderer
/// @param  mode determines the set of pseudo graphics characters to be used.
///         (if not in range of the enum, a default choice is chosen by Konpu)
/// @return 0 iff initialization if successful
///         (always the case if the renderer if available)
int rendererPseudoGraphics_init(enum rendererPseudoGraphicsMode mode);

#else
#   define RENDERER_PSEUDOGRAPHICS            0
#   define rendererPseudoGraphics_init(mode)  1

#endif //KONPU_PLATFORM_LIBC
#endif //KONPU_RENDERER_PSEUDOGRAPHICS_H
