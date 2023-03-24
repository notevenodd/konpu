#ifndef  KONPU_RECT_H
#define  KONPU_RECT_H
#include "util.h"

//===< RECTANGLE >==============================================================

// konpu uses a struct with four ints to represents rectangles.
//
// the *regular* representation of (and recommended way to manipulate) a
// rectangle is: - a corner (x,y)
//               - a vector (w,h) from the (x,y) corner to the opposite corner
//               notes: w and h can be negative
//                     |w| is the width of the rectangle
//                     |h| is the height of the rectangle
//
// an alternative way to encode a rectangle uses the *absolute* representation,
// which is:     - a corner (x,y)
//               - the opposite corner (x1,y1)
//
// it is trivial to switch between the two representations of the same rectangle
// with the following relations: x1 = x + w and y1 = y + h
//
// we say a rectangle is *normalized* when (x,y) represents its upper-left corner
// notes: - in regular  representation, this is equivalent to w>=0  && h>=0
//        - in absolute representation, this is equivalent to x1>=x && y1>=y
//
// by convention, in function prototypes,
// a rectangle in regular  (x,y, w,h)   notation will be noted r.
// a rectangle in absolute (x,y, x1,y1) notation will be noted a.

typedef struct rect {
#if (__STDC_VERSION__ >= 201112L)
            int x;
            int y;
    union { int w, x1 };
    union { int h, y1 };
#else
/* C99: */         // C99 doesn't allow anonymous unions, thus fields are named
            int x; // only as per the regular representation. It's possible, but
            int y; // but very error-prone to work in absolute notation, where:
            int w; // - field .w also corresponds to .x1
            int h; // - field .h also corresponds to .y1
#endif
} rect;


// helpers to switch representation between regular (r) and absolute (a):
static inline void  rect_rtoa(rect *r);
static inline void  rect_ator(rect *a);

// functions to normalize a rectangle:
static inline void  rect_normalize(rect *r);
static inline void  rect_normalize_a(rect *a);

// normalize and also switch representation in one go:
static inline void  rect_normalize_rtoa(rect *r);
static inline void  rect_normalize_ator(rect *a);


// clipping (and normalizing) a rectangle
//
// rect r = { x,y, w,h }; // (not normalized, and w and h can be negative too)
// if rect_clip(&r, W,H) {
//    // - at least part of r was visible in the area limited by (0,0) and (W,H)
//    // - r is now clipped as the rectangle fully contained in it,
//    // - r is normalized
//    // - the following identity for the new value of the fields are true:
//    //   x >= 0  ,  y >= 0  ,  0 < w <= W  ,  0 < r <= H
//    ...
// } else {
//    // - r was not visible at all in the area limited by (0,0) and (W,H)
//    // - r is unchanged TODO: that's not true!!!!
//                              it's been normalized and its representation changed to absolute!!!
// }
static inline bool
rect_clip(rect *r, int width, int height);


//--- inline implementation ----------------------------------------------------


static inline void rect_rtoa(rect *r)
{  assert(r);
   r->w += r->x;   // ie: a->x1 = r->x + r->w;
   r->h += r->y;   // ie: a->y1 = r->y + r->h;
}

static inline void rect_ator(rect *a)
{  assert(a);
   a->w -= a->x;   // ie: r->w = a->x1 - a->x;
   a->h -= a->y;   // ie: r->h = a->y1 - a->x;
}

static inline void rect_normalize_a(rect *a)
{  assert(a);                 // with "proper" absolute notation:
   if (a->x < a->w)           // if (a->x < a->x1)
      UTIL_SWAP(a->x, a->w);  //    UTIL_SWAP(a->x, a->x1);
   if (a->y < a->h)           // if (a->y < a->y1)
      UTIL_SWAP(a->y, a->h);  //    UTIL_SWAP(a->y, a->y1);
}

static inline void rect_normalize(rect *r)
{  assert(r);
   if (r->w < 0) {
      r->x +=   r->w;
      r->w  = - r->w;
   }
   if (r->h < 0) {
      r->y +=   r->h;
      r->h  = - r->h;
   }
}

static inline void
rect_normalize_rtoa(rect *r)
{  assert(r);
   if (r->w >= 0) {
      r->w += r->x;
   } else {
      int x = r->x;
      r->x += r->w;
      r->w  = x;
   }
   if (r->h >= 0) {
      r->h += r->y;
   } else {
      int y = r->y;
      r->y += r->h;
      r->h  = y;
   }
}

static inline void
rect_normalize_ator(rect *a)
{  assert(a);
   if (a->w < a->x) {
      a->x  = a->w;
      a->w  = a->x - a->w;
   } else {
      a->w -= a->x;
   }
   if (a->h < a->y) {
      a->y = a->h;
      a->h = a->y - a->h;
   } else {
      a->h -= a->y;
   }
}

static inline bool
rect_clip(rect *r, int width, int height)
{  assert(r);

   // test obvious visibility:
   if (width <= 0 || height <= 0)
      return false;

   // normalize and switch to absolute coordinate
   rect_normalize_rtoa(r);                    // with "absolute" notation:

   // test rectangle visibility:
   if ( r->x >= width || r->y >= height ||    // if (a->x  >= width || a->y  >= height ||
        r->w <= 0     || r->h <= 0      )     //     a->x1 <= 0     || a->x1 <= 0      )
      return false;                           //    return false;

   // clamp the rectangle:
   if (r->x < 0)      r->x = 0;   ;           // if (a->x  < 0)      a->x  = 0;
   if (r->y < 0)      r->y = 0;               // if (a->y  < 0)      a->y  = 0;
   if (r->w > width)  r->w = width;           // if (a->x1 > width)  a->x1 = width;
   if (r->h > height) r->h = height;          // if (a->y1 > height) a->y1 = height;

   // move back to regular semantic:
   rect_ator(r);
   return true;
}

//===</ RECTANGLE >=============================================================

#endif //KONPU_RECT_H
