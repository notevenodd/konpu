#ifndef  KONPU_RENDERER_SDL2_H
#define  KONPU_RENDERER_SDL2_H
#include "platform.h"
#include "c.h"
#ifdef   KONPU_PLATFORM_SDL2

#include "canvas.h"

bool renderer_sdl2_init(const char* title, int zoomx, int zoomy);
void renderer_sdl2_drop();
bool renderer_sdl2_render(const_canvas cvas);

#endif //KONPU_PLATFORM_SDL2
#endif //KONPU_RENDERER_SDL2_H
