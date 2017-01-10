#ifndef _HuffEncode_aux_h
#define _HuffEncode_aux_h
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _Schedule
extern FILE *schefp;
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

extern int LastDC;
extern int csize[256];
extern int input[64];
extern unsigned char ofp[2000];
extern int imageheight,imagewidth;
extern int LumiQuant[64];
extern int IZgzgIndex[64];
extern int huffsize[257];
extern int huffcode[257];
extern int ACXhuff_bits[36];
extern int ACXhuff_huffval[257];
extern int DCXhuff_bits[36];
extern int DCXhuff_huffval[257];
extern int ACEhuff_ehufco[257];
extern int ACEhuff_ehufsi[257];
extern int DCEhuff_ehufco[257];
extern int DCEhuff_ehufsi[257];
extern int lastp;
extern int ofp_ptr;
extern int LuminanceDCBits[16];
extern int LuminanceDCValues[12];
extern int LuminanceACBits[16];
extern int LuminanceACValues[162];
extern unsigned int lmask[17];

extern int WriteBits(int, int);
extern int WriteWord(int);
extern int WriteByte(int);
extern void WriteMarker(int);
extern void WriteAPP0();
extern void WriteSOF();
extern void WriteDQT();
extern void WriteDHT();
extern void WriteSOS();
extern void SizeTable_AC();				 /*From Xhuff to Ehuff */
extern void OrderCodes_AC();
extern void SizeTable_DC();				 /*From Xhuff to Ehuff */
extern void OrderCodes_DC();
extern void CodeTable();
extern void JpegDefaultHuffman();
extern void SpecifiedHuffman_AC();
extern void SpecifiedHuffman_DC();
extern void EncodeAC();
extern void EncodeDC();
extern void EncodeHuffman_AC(int);
extern void EncodeHuffman_DC(int);
extern void WriteHuffman_AC();
extern void WriteHuffman_DC();

#ifdef __cplusplus
}
#endif
#endif
