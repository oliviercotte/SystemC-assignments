#include "HuffEncode_aux.h"

void huffencode(int *in_block) {
	int i,  j,  num_mdu;
	static int iter=0;

	if(iter==0){
		imageheight=96;
		imagewidth=116;
		JpegDefaultHuffman();
		WriteMarker(0xd8);
		WriteAPP0();
		WriteSOF();
		WriteDQT();
		WriteDHT();
		WriteSOS();
		num_mdu = ((imageheight + 7) >> 3) * ((imagewidth + 7 >> 3));
	}
	for(j = 0; j < 64 ; j++)
	{
		input[j] = in_block[j];
	}
	EncodeDC();
	EncodeAC();
	if(iter==179){
		WriteBits(-1,  0);
		WriteMarker(0xd9);
	}
	iter++;
}
