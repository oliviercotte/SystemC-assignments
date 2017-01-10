#ifndef _ChenDCT_aux_h
#define _ChenDCT_aux_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <math.h>
	
typedef struct BData {
   int bb[64];
}bdata;

#define NO_MULTIPLY
#ifdef NO_MULTIPLY
#define LS(r,s) ((r) << (s))
#define RS(r,s) ((r) >> (s))       /* Caution with rounding... */
#else
#define LS(r,s) ((r) * (1 << (s)))
#define RS(r,s) ((r) / (1 << (s))) /* Correct rounding */
#endif

#define MSCALE(expr)  RS((expr),9)

/* Cos constants */

#define c1d4 362L
#define c1d8 473L
#define c3d8 196L
#define c1d16 502L
#define c3d16 426L
#define c5d16 284L
#define c7d16 100L

#ifdef __cplusplus
}
#endif

#endif

