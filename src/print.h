#ifndef  KONPU_PRINT_H
#define  KONPU_PRINT_H
#include "platform.h"

#include "canvas.h"

// printing functions

void print         (canvas win, int x, int y, const char* str);
void print_widehalf(canvas win, int x, int y, const char* str);
void print_tallhalf(canvas win, int x, int y, const char* str);
void print_quadrant(canvas win, int x, int y, const char* str);


#endif  //KONPU_PRINT_H
