//--------Thread function definitions-----------------------------
//-----You must modify this file as indicated by TODO comments----

//must include to use the systemc library
#include "systemc.h"
#include "CPU.h"
#include "Block.h"
#include "ReadBmp_aux.h"
#include <string.h>

void CPU::Read_thread(){
	cout << sc_time_stamp() << ": Thread Read BMP is initialized\n";
	
	//initial part before the loop
	ReadBmpHeader();
	ImageWidth = BmpInfoHeader[1];
	ImageHeight = BmpInfoHeader[2];
	InitGlobals();
	NumberMDU = MDUWide * MDUHigh;
	
	//local
	Block block;

	for(int iter = 0; iter < 180; iter++)
	{
		ReadBmpBlock(iter);
		memcpy(block.data, bmpinput, sizeof(block.data));
		cout << sc_time_stamp() << ": Thread Read BMP attempting to send frame" << iter << " to DCT\n";
		// TODO: Insert appropriate channel call here
		Read2DCT->write(block);
		cout << sc_time_stamp() << ": Thread Read BMP has sent frame" << iter << " to DCT\n";
	}
	wait();
}

void CPU::DCT_thread(){

	//local
	Block block;
	int in_block[64], out_block[64];
	
	cout << sc_time_stamp() << ": Thread DCT is initialized\n";
	for(int iter = 0; iter < 180; iter++)
	{
		cout << sc_time_stamp() << ": Thread DCT is attempting to receive frame" << iter << " from Read BMP\n";
		// TODO: Insert appropriate channel call here
		Read2DCT->read(block);
		cout << sc_time_stamp() << ": Thread DCT has received frame" << iter << " from Read BMP\n";
		memcpy(in_block, block.data, sizeof(in_block));
		chendct(in_block,out_block);
		memcpy(block.data, out_block, sizeof(block.data));
		cout << sc_time_stamp() << ": Thread DCT is attempting to send frame" << iter << " to Quantize\n";
		// TODO: Insert appropriate channel call here
		DCT2Quant->write(block);
		cout << sc_time_stamp() << ": Thread DCT has sent frame" << iter << " to Quantize\n";
	}
	wait();
}

void CPU::Quant_thread(){

	Block block;
	int in_block[64], out_block[64];
	cout << sc_time_stamp() << ": Thread Quantize is initialized\n";
	
	for(int iter = 0; iter < 180; iter++)
	{
		cout << sc_time_stamp() << ": Thread Quantize is attempting to receive frame" << iter << " from DCT\n";
		// TODO: Insert appropriate channel call here
		DCT2Quant->read(block);
		cout << sc_time_stamp() << ": Thread Quantize has received frame" << iter << " from DCT\n";
		// TODO: Copy over block received from channel into local in_block
		memcpy(in_block, block.data, sizeof(in_block));
		quantize(in_block,out_block);
		// TODO: Copy over out_block to block for sending over the channel below
		memcpy(block.data, out_block, sizeof(block.data));
		cout << sc_time_stamp() << ": Thread Quantize is attempting to send frame" << iter << " to ZigZag\n";
		// TODO: Insert appropriate channel call here
		Quant2Zigzag->write(block);
		cout << sc_time_stamp() << ": Thread Quantize has sent frame" << iter << " to ZigZag\n";
	}
	wait();
}

void CPU::Zigzag_thread(){
	
	Block block;
	int in_block[64], out_block[64];
	cout << sc_time_stamp() << ": Thread ZigZag is initialized\n";
	
	for(int iter = 0; iter < 180; iter++)
	{
		cout << sc_time_stamp() << ": Thread ZigZag is attempting to receive frame" << iter << " from Quantize\n";
		// TODO: Insert appropriate channel call here
		Quant2Zigzag->read(block);
		cout << sc_time_stamp() << ": Thread ZigZag has received frame" << iter << " from Quantize\n";
		// TODO: Copy over block received from channel into local in_block
		memcpy(in_block, block.data, sizeof(in_block));
		zigzag(in_block,out_block);
		// TODO: Copy over out_block to block for sending over the channel below
		memcpy(block.data, out_block, sizeof(block.data));
		cout << sc_time_stamp() << ": Thread ZigZag is attempting to send frame" << iter << " to Huffman\n";
		// TODO: Insert appropriate channel call here
		Zigzag2Huff->write(block);
		cout << sc_time_stamp() << ": Thread ZigZag has sent frame" << iter << " to Huffman\n";
	}
	wait();
}

void CPU::Huff_thread(){
	
	Block block;
	int in_block[64];
	
	cout << sc_time_stamp() << ": Thread Huffman is initialized\n";
	for(int iter = 0; iter < 180; iter++)
	{
		cout << sc_time_stamp() << ": Thread Huffman is attempting to receive frame" << iter << " from ZigZag\n";
		// TODO: Insert appropriate channel call here
		Zigzag2Huff->read(block);
		cout << sc_time_stamp() << ": Thread Huffman has received frame" << iter << " from ZigZag\n";
		// TODO: Copy over block received from channel into local in_block
		memcpy(in_block, block.data, sizeof(in_block));
		huffencode(in_block);
	}
	cout << sc_time_stamp() << ": Thread Huffman is writing encoded JPEG to file\n";
	FileWrite();
	cout << sc_time_stamp() << ": JPEG encoding completed\n";
	wait();
}

//----------------End of thread function definitions--------------
