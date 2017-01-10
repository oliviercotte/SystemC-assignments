#include "Zigzag_aux.h"

void zigzag(int *in_block, int *out_block) {
	int i,  z;

	for (i = 0; i < 64; i++)
	{
		z = ZigzagIndex[i];
		out_block[z] = in_block[i];
	}
}
