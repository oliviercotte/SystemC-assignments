#ifndef _ReadBmp_aux_h
#define _ReadBmp_aux_h

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

#ifdef _Schedule
FILE *schefp;
#endif	

extern long BmpFileHeader[5];
extern long BmpInfoHeader[11];
extern char BmpColors[256][3];
extern unsigned char ifp[12215];
extern int MDUWide, MDUHigh;
extern int NumberMDU;
extern int MDUPadWidth, MDUPadHeight;
extern int ImageWidth, ImageHeight;
extern unsigned char ScanBuffer[1024];
extern int BmpScanWidth, BmpScanHeight;
extern int bmpinput[64];
extern int ACFrequency[257];
extern int DCFrequency[257];
extern int iteration;
extern int ifp_ptr;

extern void InitGlobals();
extern void ReadBmpBlock(int);
extern int  IsBmpFile();
extern void ReadBmpHeader();
extern int ReadWord();
extern int ReadRevWord();
extern int ReadByte();
extern long ReadDWord();
extern long ReadRevDWord();

extern void chendct(int*,int*);
extern void quantize(int*,int*);
extern void zigzag(int*,int*);
extern void huffencode(int*);
extern void FileWrite();

#ifdef __cplusplus
}
#endif
#endif
