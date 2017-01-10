#include "Quantize_aux.h"

void quantize(int *in_block, int *out_block) {
	int i,  m,  q,  o;

	q = LuminanceQuantization[0];

	for (i=0; i<64; i++)
	{
		m = in_block[i];
		if (m > 0)
		{
			o = (m + q/2) / q;
		}
		else
		{
			o = (m - q/2) / q;
		}
		if (i < 63)
		{
			q = LuminanceQuantization[i+1];
		}
		out_block[i] = o;
	}
}

