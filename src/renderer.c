#include "renderer.h"

struct rendererObject rendererSingleton = {
   .id     = RENDERER_NULL,
   .render = &renderer_null,
   .drop   = &renderer_null
};


int renderer_null(void)
{
   return 0;
}

int renderer_drop(void)
{
   int ret = rendererSingleton.id;
   rendererSingleton.error  = (*rendererSingleton.drop)();
   // ^-- if an error occcurs during uninitialization, we put in the `error`
   //     field of the renderer.
   //
   // v-- As for the other fields, we set the renderer back to the Null renderer
   rendererSingleton.id     = RENDERER_NULL;
   rendererSingleton.render = &renderer_null;
   rendererSingleton.drop   = &renderer_null;

   return (rendererSingleton.error)? ret : 0;
}
