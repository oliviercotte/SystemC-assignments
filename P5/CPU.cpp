//--------Thread function definitions-----------------------------
//-----You must modify this file as indicated by TODO comments----
//-----You can reuse the code developed in Assignment 4-----------
//-----as indicated by the REUSE comments-------------------------

//must include to use the systemc library
#include "systemc.h"
#include "sysc/kernel/sc_dynamic_processes.h"
#include "CPU.h"
#include "Block.h"
#include "ReadBmp_aux.h"
#include <stdlib.h>

void CPU::top() {

	// define the SystemC process handles
	sc_core::sc_process_handle Read_h, DCT_h, Zigzag_h, Huff_h;

	// spawn the SystemC threads
	Read_h = sc_spawn(sc_bind(&CPU::Read_thread, this));
	DCT_h = sc_spawn(sc_bind(&CPU::DCT_thread, this));
	Zigzag_h = sc_spawn(sc_bind(&CPU::Zigzag_thread, this));
	Huff_h = sc_spawn(sc_bind(&CPU::Huff_thread, this));

	// REUSE: create the tasks in the OS context
	OS->task_create("Read"/*sc_gen_unique_name("Read")*/, READ_PRIORITY,Read_h);
	OS->task_create("DCT"/*sc_gen_unique_name("DCT")*/, DCT_PRIORITY, DCT_h);
	OS->task_create("Zigzag"/*sc_gen_unique_name("Zigzag")*/, ZIGZAG_PRIORITY, Zigzag_h);
	OS->task_create("Huffman"/*sc_gen_unique_name("Huffman")*/, HUFFMAN_PRIORITY, Huff_h);

	// tasks have been created
	// now kickstart the OS
	OS->run();
	return;
} //end top method
	
void CPU::Read_thread(){
	
	// initialize the task in the OS context
	OS->task_init();

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
		// consume time for this iteration = 1119 micro-seconds
		OS->time_wait (1119, SC_US);
		ReadBmpBlock(iter);
		for(int i = 0; i < 64; i++){
			block.data[i] = bmpinput[i];
		}
		
		Read2DCT->write(block);
	}

	// terminate the task in the OS context
	OS->task_terminate();
}

void CPU::DCT_thread(){

	//local
	Block block;
	int in_block[64], out_block[64];
	
	// initialize the task in the OS context
	OS->task_init();

	for(int iter = 0; iter < 180; iter++)
	{
		Read2DCT->read(block);

		// consume time for this iteration = 4321 micro-seconds
		OS->time_wait (4321, SC_US);

		for(int i = 0; i < 64; i++){
			in_block[i] = block.data[i];
		}
			
		chendct(in_block,out_block);
		
		for(int i = 0; i < 64; i++){
			block.data[i] = out_block[i];
		}

		// the block should be sent to Quantize HW
		// REUSE: the corresponding ISR should tell us if its OK to proceed
		write_to_Quantize->wait();

		// TODO: Use the BusWrite_DCT2Quantize function to send the block
		BusWrite_DCT2Quantize(&block);

		cout << sc_time_stamp() << ": CPU sent block to Quantize HW" << endl;
	}

	// terminate the task in the OS context
	OS->task_terminate();
}

void CPU::Zigzag_thread(){
	
	Block block;
	int in_block[64], out_block[64];

	// initialize the task in the OS context
	OS->task_init();

	for(int iter = 0; iter < 180; iter++)
	{
		// the block should be received from Quantize HW
		// REUSE: the corresponding ISR should tell us when to proceed
		read_from_Quantize->wait();

		// TODO: Use the BusRead_Quantize2Zigzag function to receive the block
		BusRead_Quantize2Zigzag(&block);

		cout << sc_time_stamp() << ": CPU received block from Quantize HW" << endl;
		
		// consume time for this iteration = 587 micro-seconds
		OS->time_wait (587, SC_US);

		for(int i = 0; i < 64; i++){
			in_block[i] = block.data[i];
		}
			
		zigzag(in_block,out_block);
		
		for(int i = 0; i < 64; i++){
			block.data[i] = out_block[i];
		}
		
		Zigzag2Huff->write(block);
	}

	// terminate the task in the OS context
	OS->task_terminate();
}

void CPU::Huff_thread(){
	
	Block block;
	int in_block[64];
	
	// initialize the task in the OS context
	OS->task_init();

	for(int iter = 0; iter < 180; iter++)
	{
		Zigzag2Huff->read(block);

		// consume time for this iteration = 10162 micro-seconds
		OS->time_wait (10162, SC_US);

		for(int i = 0; i < 64; i++){
			in_block[i] = block.data[i];
		}
			
		huffencode(in_block);
	}
	cout << sc_time_stamp() << ": Thread Huffman is writing encoded JPEG to file\n";
	FileWrite();
	cout << sc_time_stamp() << ": JPEG encoding completed\n";

	// terminate the task in the OS context
	OS->task_terminate();
}

//---------------ISRs------------------------

void CPU::ISR_Quantize_input_seek() {
	// TODO: Allow writing of a new block from CPU to Quantize HW
	write_to_Quantize->post();
}

void CPU::ISR_Quantize_output_avail() {
	// TODO: Allow reading of a new block from Quantize HW
	read_from_Quantize->post();
}

//---------------Bus Access Methods------------------------

// Bus write implementation
// for data of type Block

void CPU::BusWrite_DCT2Quantize(Block *outBlock) {
	BUS_ADDR_TYPE Addr = BLOCK_DCT2QUANT_START;
	BUS_DATA_TYPE BusWord;

	// TODO: make sure the caller has exclusive access
	BusAccess->wait();

	// TODO: iterate over all the Block elements
	for (int i = 0; i < BLOCK_SIZE; i++) {
		// TODO: data word size is 32 bits: just right to fit an integer
		// TODO: convert the block data item to a bus word
		BusWord = BUS_DATA_TYPE(outBlock->data[i]);

		// TODO: write the word to the bus
		bus_port->master_write(Addr, &BusWord);

		// TODO: compute the next address to write to 
		Addr = BUS_ADDR_TYPE(Addr.to_int() + BUS_WORD_SIZE);
	} // end for

	// TODO: release the bus access
	BusAccess->post();

	return;
}

// Bus read implementation
// for data of type Block

void CPU::BusRead_Quantize2Zigzag(Block *inBlock) {
	BUS_ADDR_TYPE Addr = BLOCK_QUANT2ZIGZAG_START;
	BUS_DATA_TYPE BusWord;

	// TODO: make sure the caller has exclusive access
	BusAccess->wait();

	// TODO: iterate over all the Block elements
	for (int i = 0; i < BLOCK_SIZE; i++) {
		// TODO: read a word from the bus
		bus_port->master_read(Addr, &BusWord);

		// TODO: convert the bus word to a block data item
		inBlock->data[i] = BusWord.to_int();

		// TODO: compute the next address to read from
		Addr = BUS_ADDR_TYPE(Addr.to_int() + BUS_WORD_SIZE);
	} // end for

	// TODO: release the bus access
	BusAccess->post();

	return;
}
