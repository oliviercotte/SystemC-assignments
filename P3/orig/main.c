#include "ReadBmp_aux.h"
/*
#include "ChenDct_aux.h"
#include "Quantize_aux.h"
#include "Zigzag_aux.h"
#include "HuffEncode_aux.h"
*/

int main() {
	int iter;
	int out_block[64];
	int in_block[64];
	int i;

	ReadBmpHeader();
	ImageWidth = BmpInfoHeader[1];
	ImageHeight = BmpInfoHeader[2];
	InitGlobals();
	NumberMDU = MDUWide * MDUHigh;

	for (iter = 0; iter < 180; iter++)
	{
		ReadBmpBlock(iter);
		chendct(bmpinput,out_block);
		quantize(out_block,in_block);
		zigzag(in_block,out_block);
		huffencode(out_block);
	}
	FileWrite();
	return;
}
