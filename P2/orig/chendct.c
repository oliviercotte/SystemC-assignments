#include "ChenDCT_aux.h"

void chendct(int *in_block, int *out_block) {
	int i, aptr, tval;
	int a0,  a1,  a2,  a3;
	int b0,  b1,  b2,  b3;
	int c0,  c1,  c2,  c3;
	int v0,  v1,  v2,  v3,  v4,  v5,  v6,  v7;

	for (i = 0; i < 64; i++)
	{
		tval = in_block[i];
		in_block[i] = tval - 128;
	}
	for (i = 0; i < 8; i++)
	{
		aptr = i;
		v0 = in_block[aptr]; aptr += 8;
		v1 = in_block[aptr]; aptr += 8;
		v2 = in_block[aptr]; aptr += 8;
		v3 = in_block[aptr]; aptr += 8;
		v4 = in_block[aptr]; aptr += 8;
		v5 = in_block[aptr]; aptr += 8;
		v6 = in_block[aptr]; aptr += 8;
		v7 = in_block[aptr];
		a0 = LS((v0 + v7),  2); c3 = LS((v0 - v7),  2);
		a1 = LS((v1 + v6),  2); c2 = LS((v1 - v6),  2);
		a2 = LS((v2 + v5),  2); c1 = LS((v2 - v5),  2);
		a3 = LS((v3 + v4),  2); c0 = LS((v3 - v4),  2);
		b0 = a0 + a3; b1 = a1 + a2; b2 = a1 - a2; b3 = a0 - a3;
		out_block[i] = MSCALE(c1d4 * (b0 + b1));
		out_block[i + 32] = MSCALE(c1d4 * (b0 - b1));
		out_block[i + 16] = MSCALE((c3d8 * b2) + (c1d8 * b3));
		out_block[i + 48] = MSCALE((c3d8 * b3) - (c1d8 * b2));
		b0 = MSCALE(c1d4 * (c2 - c1)); b1 = MSCALE(c1d4 * (c2 + c1));
		a0 = c0 + b0; a1 = c0 - b0; a2 = c3 - b1; a3 = c3 + b1;
		out_block[i + 8] = MSCALE((c7d16 * a0) + (c1d16 * a3));
		out_block[i + 24] = MSCALE((c3d16 * a2) - (c5d16 * a1));
		out_block[i + 40] = MSCALE((c3d16 * a1) + (c5d16 * a2));
		out_block[i + 56] = MSCALE((c7d16 * a3) - (c1d16 * a0));
	}
	for (i = 0; i < 8; i++)
	{
		aptr = LS(i,  3);
		v0 = out_block[aptr]; aptr++;
		v1 = out_block[aptr]; aptr++;
		v2 = out_block[aptr]; aptr++;
		v3 = out_block[aptr]; aptr++;
		v4 = out_block[aptr]; aptr++;
		v5 = out_block[aptr]; aptr++;
		v6 = out_block[aptr]; aptr++;
		v7 = out_block[aptr];
		c3 = RS((v0 - v7),  1); a0 = RS((v0 + v7),  1);
		c2 = RS((v1 - v6),  1); a1 = RS((v1 + v6),  1);
		c1 = RS((v2 - v5),  1); a2 = RS((v2 + v5),  1);
		c0 = RS((v3 - v4),  1); a3 = RS((v3 + v4),  1);
		b0 = a0 + a3; b1 = a1 + a2; b2 = a1 - a2; b3 = a0 - a3;
		aptr = LS(i,  3);
		out_block[aptr] = MSCALE(c1d4 * (b0 + b1));
		out_block[aptr + 4] = MSCALE(c1d4 * (b0 - b1));
		out_block[aptr + 2] = MSCALE((c3d8 * b2) + (c1d8 * b3));
		out_block[aptr + 6] = MSCALE((c3d8 * b3) - (c1d8 * b2));
		b0 = MSCALE(c1d4 * (c2 - c1)); b1 = MSCALE(c1d4 * (c2 + c1));
		a0 = c0 + b0; a1 = c0 - b0; a2 = c3 - b1; a3 = c3 + b1;
		out_block[aptr + 1] = MSCALE((c7d16 * a0) + (c1d16 * a3));
		out_block[aptr + 3] = MSCALE((c3d16 * a2) - (c5d16 * a1));
		out_block[aptr + 5] = MSCALE((c3d16 * a1) + (c5d16 * a2));
		out_block[aptr + 7] = MSCALE((c7d16 * a3) - (c1d16 * a0));
	}
	for (i = 0; i < 64; i++)
	{
		tval = out_block[i];
		tval = (((tval < 0) ? (tval - 4) : (tval + 4)) / 8);
		if (tval < -1023)
		{
			tval = -1023;
		}
		else if (tval > 1023)
		{
			tval = 1023;
		}
		out_block[i] = tval;
	}
}
