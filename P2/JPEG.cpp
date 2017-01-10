//--------Thread function definitions-----------------------------
//-----You must modify this file as indicated by TODO comments----

//must include to use the systemc library
#include "systemc.h"
#include "JPEG.h"
#include "Block.h"
#include "ReadBmp_aux.h"

void Read_Module::Read_thread(){
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
		// TODO: consume time for this iteration = 1119 micro-seconds
		sc_time TimeForIteration(1119, SC_US);
		wait(TimeForIteration);

		ReadBmpBlock(iter);
		for(int i = 0; i < 64; i++){
			block.data[i] = bmpinput[i];
		}
		
		cout << sc_time_stamp() << ": Thread Read BMP attempting to send frame" << iter << " to DCT\n";
		
		// TODO: Insert appropriate port call here
		outport.write(block);

		cout << sc_time_stamp() << ": Thread Read BMP has sent frame" << iter << " to DCT\n";

	}
	wait();
}

void DCT_Module::DCT_thread(){

	//local
	Block block;
	int in_block[64], out_block[64];
	
	cout << sc_time_stamp() << ": Thread DCT is initialized\n";
	for(int iter = 0; iter < 180; iter++)
	{
		cout << sc_time_stamp() << ": Thread DCT is attempting to receive frame" << iter << " from Read BMP\n";
		
		// TODO: Insert appropriate port call here
		inport.read(block);
		
		cout << sc_time_stamp() << ": Thread DCT has received frame" << iter << " from Read BMP\n";

		// TODO: consume time for this iteration = 4321 micro-seconds
		sc_time TimeForIteration(4321, SC_US);
		wait(TimeForIteration);

		memcpy(in_block, block.data, sizeof(in_block));
			
		chendct(in_block,out_block);
		
		memcpy(block.data, out_block, sizeof(block.data));

		cout << sc_time_stamp() << ": Thread DCT is attempting to send frame" << iter << " to Quantize\n";
		
		// TODO: Insert appropriate port call here
		outport.write(block);
		
		cout << sc_time_stamp() << ": Thread DCT has sent frame" << iter << " to Quantize\n";
	}
	wait();
}

void Quant_Module::Quant_thread(){

	Block block;
	int in_block[64], out_block[64];
	cout << sc_time_stamp() << ": Thread Quantize is initialized\n";
	
	for(int iter = 0; iter < 180; iter++)
	{
		cout << sc_time_stamp() << ": Thread Quantize is attempting to receive frame" << iter << " from DCT\n";
		
		// TODO: Insert appropriate port call here
		inport.read(block);

		cout << sc_time_stamp() << ": Thread Quantize has received frame" << iter << " from DCT\n";

		// TODO: consume time for this iteration = 5711 micro-seconds
		sc_time TimeForIteration(5711, SC_US);
		wait(TimeForIteration);
		
		// TODO: Copy over block received from channel into local in_block
		memcpy(in_block, block.data, sizeof(in_block));

		quantize(in_block,out_block);
		
		// TODO: Copy over out_block to block for sending over the channel below
		memcpy(block.data, out_block, sizeof(block.data));
		
		cout << sc_time_stamp() << ": Thread Quantize is attempting to send frame" << iter << " to ZigZag\n";
		
		// TODO: Insert appropriate port call here
		outport.write(block);
		
		cout << sc_time_stamp() << ": Thread Quantize has sent frame" << iter << " to ZigZag\n";
	}
	wait();
}

void Zigzag_Module::Zigzag_thread(){
	
	Block block;
	int in_block[64], out_block[64];
	cout << sc_time_stamp() << ": Thread ZigZag is initialized\n";
	
	for(int iter = 0; iter < 180; iter++)
	{
		cout << sc_time_stamp() << ": Thread ZigZag is attempting to receive frame" << iter << " from Quantize\n";

		// TODO: Insert appropriate port call here
		inport.read(block);
		
		cout << sc_time_stamp() << ": Thread ZigZag has received frame" << iter << " from Quantize\n";

		// TODO: consume time for this iteration = 587 micro-seconds
		sc_time TimeForIteration(587, SC_US);
		wait(TimeForIteration);
		
		// TODO: Copy over block received from channel into local in_block
		memcpy(in_block, block.data, sizeof(in_block));

		zigzag(in_block,out_block);
		
		// TODO: Copy over out_block to block for sending over the channel below
		memcpy(block.data, out_block, sizeof(block.data));

		cout << sc_time_stamp() << ": Thread ZigZag is attempting to send frame" << iter << " to Huffman\n";
		
		// TODO: Insert appropriate port call here
		outport.write(block);
		
		cout << sc_time_stamp() << ": Thread ZigZag has sent frame" << iter << " to Huffman\n";
	}
	wait();
}

void Huff_Module::Huff_thread(){
	
	Block block;
	int in_block[64];
	
	cout << sc_time_stamp() << ": Thread Huffman is initialized\n";
	for(int iter = 0; iter < 180; iter++)
	{
		cout << sc_time_stamp() << ": Thread Huffman is attempting to receive frame" << iter << " from ZigZag\n";
		
		// TODO: Insert appropriate port call here
		inport.read(block);
		
		cout << sc_time_stamp() << ": Thread Huffman has received frame" << iter << " from ZigZag\n";

		// TODO: consume time for this iteration = 10162 micro-seconds
		sc_time TimeForIteration(10162, SC_US);
		wait(TimeForIteration);

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
