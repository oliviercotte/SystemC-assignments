#ifndef _Quantize_aux_h
#define _Quantize_aux_h
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

extern int LuminanceQuantization[64];

#ifdef __cplusplus
}
#endif

#endif
