#include "HuffEncode_aux.h"

int LastDC=0;
int ofp_ptr=0;
int imagewidth, imageheight;
int ACXhuff_bits[36];
int ACXhuff_huffval[257];
int DCXhuff_bits[36];
int DCXhuff_huffval[257];
int ACEhuff_ehufco[257];
int ACEhuff_ehufsi[257];
int DCEhuff_ehufco[257];
int DCEhuff_ehufsi[257];
int huffsize[257];
int huffcode[257];
unsigned char ofp[2000];
int input[64];
int lastp;

int LumiQuant[] = {
16, 11, 10, 16, 24, 40, 51, 61,
12, 12, 14, 19, 26, 58, 60, 55,
14, 13, 16, 24, 40, 57, 69, 56,
14, 17, 22, 29, 51, 87, 80, 62,
18, 22, 37, 56, 68, 109, 103, 77,
24, 35, 55, 64, 81, 104, 113, 92,
49, 64, 78, 87, 103, 121, 120, 101,
72, 92, 95, 98, 112, 100, 103, 99};
int IZgzgIndex[] =
	{0,  1,  8, 16,  9,  2,  3, 10,
	17, 24, 32, 25, 18, 11,  4,  5,
	12, 19, 26, 33, 40, 48, 41, 34,
	27, 20, 13,  6,  7, 14, 21, 28,
	35, 42, 49, 56, 57, 50, 43, 36,
	29, 22, 15, 23, 30, 37, 44, 51,
	58, 59, 52, 45, 38, 31, 39, 46,
	53, 60, 61, 54, 47, 55, 62, 63};

int csize[] = {
0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 
5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};

/* Default huffman table */
int LuminanceDCBits[] = {
0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

int LuminanceDCValues[] = {
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b};

int LuminanceACBits[] = {
0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03,
0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7d};

int LuminanceACValues[] = {
0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
0xf9, 0xfa};

unsigned int lmask[] = {
		0x0000, 
		0x0001, 0x0003, 0x0007, 0x000f,
		0x001f, 0x003f, 0x007f, 0x00ff,
		0x01ff, 0x03ff, 0x07ff, 0x0fff,
		0x1fff, 0x3fff, 0x7fff, 0xffff
};

void JpegDefaultHuffman()
{
#ifdef _Schedule
	//fprintf(schefp, "JpegDefautHuffman()\n");
#endif	
	SpecifiedHuffman_DC();
	SpecifiedHuffman_AC();
}

void EncodeDC()
{
	int s, diff, cofac;
#ifdef _Schedule
//	fprintf(schefp, "EncodeDC()\n");
#endif	

	diff = input[0] - LastDC;
	LastDC = input[0];
	if(diff<0)
		cofac = -diff;
	else
		cofac = diff;
	if (cofac < 256) {
		s = csize[cofac];
	}
	else {
		cofac = cofac >> 8;
		s = csize[cofac] + 8;
	}

	EncodeHuffman_DC(s);
	if (diff < 0) {
		diff--;
	}
	WriteBits(s, diff);
}

void EncodeAC()
{
	int i, k, r, ssss, cofac;
#ifdef _Schedule
	//fprintf(schefp, "EncodeAC()\n");
#endif	
	r=0;
	for (k=1; k<64; k++) {
		if(input[k]<0)
			cofac = -input[k];
		else
			cofac = input[k];
		if (cofac < 256)	
			ssss = csize[cofac];
		else {
			cofac = cofac >> 8;
			ssss = csize[cofac] + 8;
		}
		if (input[k] == 0) {
			if (k == 63) {
				EncodeHuffman_AC(0);
				k = 64;
			}
			else
				r++;
		}
		else {
			// ## smsoc:loop(0,4)
			while (r > 15) {
				EncodeHuffman_AC(240);
				r -= 16;
			}
			i = 16 * r + ssss;
			r = 0;
			EncodeHuffman_AC(i);
			if (input[k] < 0)		
				WriteBits(ssss, input[k]-1);
			else					
				WriteBits(ssss, input[k]);
		}
	}
}

void SpecifiedHuffman_DC()
{
	int i;
	int accum=0;
	int tmp;
#ifdef _Schedule
	//fprintf(schefp, "SpecifiedHuffman_DC()\n");
#endif	
 
	for(i=0;i<16;i++)
	{
		tmp = LuminanceDCBits[i];
		accum+= tmp;
		DCXhuff_bits[i+1] = tmp;
	}

	// ## smsoc:loop(12,12)
	for(i=0;i<accum;i++)
	{
		DCXhuff_huffval[i] = LuminanceDCValues[i];
	}
	SizeTable_DC();
	CodeTable();
	OrderCodes_DC();
}

void SpecifiedHuffman_AC()
{
	int i;
	int accum=0;
	int tmp;
#ifdef _Schedule
	//fprintf(schefp, "SpecifiedHuffman_AC()\n");
#endif	
 
	for(i=0;i<16;i++)
	{
		tmp = LuminanceACBits[i];
		accum+= tmp;
		ACXhuff_bits[i+1] = tmp;
	}
	// ## smsoc:loop(162,162)
	for(i=0;i<accum;i++)
	{
		ACXhuff_huffval[i] = LuminanceACValues[i];
	}
	SizeTable_AC();
	CodeTable();
	OrderCodes_AC();
}

void SizeTable_DC()
{
    int i,j,p;
    p=0;
#ifdef _Schedule
	//fprintf(schefp, "SizeTable_DC()\n");
#endif	

    for(i=1;i<17;i++)
    {
	// ## smsoc:loop(1,8)
        for(j=1;j<=DCXhuff_bits[i];j++)
        {
            huffsize[p++] = i;
        }
    }
    huffsize[p] = 0;
    lastp = p;
}

void SizeTable_AC()
{
    int i,j,p;
    p=0;
#ifdef _Schedule
	//fprintf(schefp, "SizeTable_AC()\n");
#endif	

    for(i=1;i<17;i++)
    {
	// ## smsoc:loop(1,125)
        for(j=1;j<=ACXhuff_bits[i];j++)
        {
            huffsize[p++] = i;
        }
    }
    huffsize[p] = 0;
    lastp = p;
}

void CodeTable()
{
    int p,code,size;
    int skip=0;
#ifdef _Schedule
	//fprintf(schefp, "CodeTable()\n");
#endif	

    p=0;
    code=0;
    size = huffsize[0];
    // ## smsoc:loop(1,32)
    while(0==skip)
    {
	// ## smsoc:loop(1,16)
        do
        {
          huffcode[p++] = code++;
        }
        while((huffsize[p]==size)&&(p<257));
        if (0== huffsize[p])
        {
			skip = 1;
        }
		if(!skip)
		{

		// ## smsoc:loop(1,8)
	        do
 	        {
	            code <<= 1;
	            size++;
	        }
	        while(huffsize[p] != size);
	    }
	}
}

void OrderCodes_AC()
{
    int index,p;
#ifdef _Schedule
	//fprintf(schefp, "OrderCodes_AC()\n");
#endif	
    // ## smsoc:loop(1,162)
    for(p=0;p<lastp;p++)
    {
        index = ACXhuff_huffval[p];
        ACEhuff_ehufco[index] = huffcode[p];
        ACEhuff_ehufsi[index] = huffsize[p];
    }
}

void OrderCodes_DC()
{
    int index,p;
#ifdef _Schedule
	//fprintf(schefp, "OrderCodes_DC()\n");
#endif	

    // ## smsoc:loop(1,8)
    for(p=0;p<lastp;p++)
    {
        index = DCXhuff_huffval[p];
        DCEhuff_ehufco[index] = huffcode[p];
        DCEhuff_ehufsi[index] = huffsize[p];
    }
}

void EncodeHuffman_AC(int value)
{
#ifdef _Schedule
	//fprintf(schefp, "EncodeHuffman()_AC\n");
#endif	
	if (ACEhuff_ehufsi[value])
	{
		WriteBits(ACEhuff_ehufsi[value],ACEhuff_ehufco[value]);
	}
}

void EncodeHuffman_DC(int value)
{
#ifdef _Schedule
	//fprintf(schefp, "EncodeHuffman()_DC\n");
#endif	
	if (DCEhuff_ehufsi[value])
	{
		WriteBits(DCEhuff_ehufsi[value],DCEhuff_ehufco[value]);
	}
}

int WriteWord(int code)
{
	int temp;
#ifdef _Schedule
	//fprintf(schefp, "WriteWord()\n");
#endif	
	temp = ofp_ptr;
	ofp[temp]=(code>>8);
	temp++;
	ofp[temp]=(code & 0xff);
	temp++;
	ofp_ptr = temp;
	return 2;
}

int WriteByte(int code)
{
	int temp;
#ifdef _Schedule
	//fprintf(schefp, "WriteByte()\n");
#endif
	temp = ofp_ptr;
	ofp[temp++]=code;	
	ofp_ptr = temp;
	return code;
}


int WriteBits(int n, int code)
{
	static unsigned char write_byte = 0;
	static int left_bits = 8;
	int p;
	int temp;

	

	temp = ofp_ptr;

#ifdef _Schedule
	//fprintf(schefp, "WriteBits()\n");
#endif	
	if (n < 0) {
		if (left_bits < 8) {
			n = left_bits;
			ofp[temp++]=write_byte;
			if (write_byte == 0xff)		
				ofp[temp++]=0;
			write_byte = 0;
			left_bits = 8;
		}
		else	n = 0;

	}
	else
	{
		code &= lmask[n];
		p = n - left_bits;

		if (n == left_bits) {
			write_byte |= code;
			ofp[temp++]=write_byte;
			if (write_byte == 0xff)		
				ofp[temp++]=0;
	
			write_byte = 0;
			left_bits = 8;
		}
		else if (n > left_bits) {
	  	    write_byte |= (code >> p);
			ofp[temp++]=write_byte;
	        if (write_byte == 0xff)
				ofp[temp++]=0;

	        if (p > 8) {
	            write_byte = (0xff & (code >> (p - 8)));
				ofp[temp++]=write_byte;
	            if (write_byte == 0xff)     
					ofp[temp++]=0;
		        p -= 8;
	        }

	        write_byte = (code & lmask[p]) << (8 - p);
	        left_bits = 8 - p;
	    }

		else {
			write_byte |= (code << -p);
			left_bits -= n;
		}
	}
	ofp_ptr = temp;

	return n;
}

void WriteMarker(int m)
{
#ifdef _Schedule
	//fprintf(schefp, "WriteMarker()\n");
#endif	
	ofp[ofp_ptr++]=0xff;
	ofp[ofp_ptr++]=m;
}

void WriteAPP0()
{
#ifdef _Schedule
	//fprintf(schefp, "WriteAPP0()\n");
#endif	
	WriteMarker(0xe0);
	WriteWord(16);
	ofp[ofp_ptr++]='J';
	ofp[ofp_ptr++]='F';
	ofp[ofp_ptr++]='I';
	ofp[ofp_ptr++]='F';
	ofp[ofp_ptr++]='\0';
	WriteWord(0x0102);
	WriteByte(2);
	WriteWord(0x001d);
	WriteWord(0x001d);
	WriteWord(0x0000);
}

void WriteDQT()
{
	int i, Pq=0;
	int start_p, end_p;
#ifdef _Schedule
	//fprintf(schefp, "WriteDQT()\n");
#endif	
	WriteMarker(0xdb);
	start_p = ofp_ptr;
	WriteWord(67);
	for (i=0; i<64; i++) {
		if (LumiQuant[i] > 255)	
			Pq = 0x10;
	}
	WriteByte(Pq|0);
	for (i=0; i<64; i++) {
		if (Pq)		
			WriteWord(LumiQuant[IZgzgIndex[i]]);
		else		
			WriteByte(LumiQuant[IZgzgIndex[i]]);
	}

	end_p = ofp_ptr;
	ofp_ptr=start_p;
	WriteWord(end_p-start_p);
	ofp_ptr=end_p;
}

void WriteDHT()
{
	int start_p, end_p;
#ifdef _Schedule
	//fprintf(schefp, "WriteDHT()\n");
#endif	

	WriteMarker(0xc4);

	start_p = ofp_ptr;
	WriteWord(0);
	WriteByte(0);
	WriteHuffman_DC();
	WriteByte(0x10);
	WriteHuffman_AC();
	end_p = ofp_ptr;
	ofp_ptr=start_p;
	WriteWord(end_p-start_p);
	ofp_ptr=end_p;
}

void WriteSOF()
{
	int end_p, start_p;
#ifdef _Schedule
	//fprintf(schefp, "WriteSOF()\n");
#endif	

	WriteMarker(0xc0);

	start_p = ofp_ptr;
	WriteWord(0);
	WriteByte(8);
	WriteWord(imageheight);
	WriteWord(imagewidth);
	WriteByte(1);
	WriteByte(1);
	WriteByte(0x11);
	WriteByte(0);
	end_p = ofp_ptr;
	ofp_ptr=start_p;
	WriteWord(end_p-start_p);
	ofp_ptr=end_p;
}

void WriteSOS()
{
	int start_p, end_p;
#ifdef _Schedule
	//fprintf(schefp, "WriteSOS()\n");
#endif	

	WriteMarker(0xda);

	start_p = ofp_ptr;
	WriteWord(0);
	WriteByte(1);
	WriteByte(1);
	WriteByte(0);
	WriteByte(0);
	WriteByte(63);
	WriteByte(0);
	end_p = ofp_ptr;
	ofp_ptr=start_p;
	WriteWord(end_p-start_p);
	ofp_ptr=end_p;
}

void WriteHuffman_AC()
{
    int i,accum=0;
#ifdef _Schedule
	//fprintf(schefp, "WriteHuffman_AC()\n");
#endif	

    for(i=1;i<=16;i++)
    {
        WriteByte(ACXhuff_bits[i]);
        accum += ACXhuff_bits[i];
    }
    // ## smsoc:loop(1,163)
    for(i=0;i<accum;i++)
    {
        WriteByte(ACXhuff_huffval[i]);
    }
}

void WriteHuffman_DC()
{
    int i,accum=0;
#ifdef _Schedule
	//fprintf(schefp, "WriteHuffman_DC()\n");
#endif	

    for(i=1;i<=16;i++)
    {
        WriteByte(DCXhuff_bits[i]);
        accum += DCXhuff_bits[i];
    }
    // ## smsoc:loop(1,8)
    for(i=0;i<accum;i++)
    {
        WriteByte(DCXhuff_huffval[i]);
    }
}

void FileWrite()
{
	FILE *f;
	f=fopen("test.jpg","wb");
	fwrite(ofp,sizeof(char),2000,f);
	fclose(f);
	return;
}
