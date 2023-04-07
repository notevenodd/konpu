/*******************************************************************************
 * @file
 * The SDL2 renderer uses Simple DirectMedia Layer 2.0 (aka SDL2),
 * a popular cross-platform library to render the screen.
 * see: https://libsdl.org/
 * 
 * Rendering is done via a single SDL2 Texture with "STREAMING" access.
 * see: https://wiki.libsdl.org/SDL2/SDL_TextureAccess
 ******************************************************************************/
#ifndef  KONPU_RENDERER_SDL2_H
#define  KONPU_RENDERER_SDL2_H
#include "platform.h"
#include "c.h"

#if KONPU_PLATFORM_SDL2
#   define RENDERER_SDL2  2

/// @brief initialize a graphical renderer using SDL2
/// @param title      the title of the window, in UTF-8 encoding
/// @param win_width  the width of the window, in (physical) screen coordinates
/// @param win_height the height of the window, in (phyiscal) screen coordinates
/// @return  0 iff initialization if successful
/// @details In case of an error: 
/// * the returned error value should be a negative error code, and you can 
///   use SDL2's SDL_GetError() to get more information.
/// * of it error value is >0, then its is an internal error.
int rendererSDL2_init(const char* title, int win_width, int win_height);

#else
#   define RENDERER_SDL2                   0
#   define rendererSDL2_init(title, w, h)  1

#endif //KONPU_PLATFORM_SDL2
#endif //KONPU_RENDERER_SDL2_H
