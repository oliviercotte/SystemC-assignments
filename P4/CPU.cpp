//--------Thread function definitions-----------------------------
//-----You must modify this file as indicated by TODO comments----
//-----You can reuse the code developed in Assignment 3-----------
//-----as indicated by the REUSE comments-------------------------

//must include to use the systemc library
#include "systemc.h"
#include "sysc/kernel/sc_dynamic_processes.h"
#include "CPU.h"
#include "Block.h"
#include "ReadBmp_aux.h"

// The main thread that will spawn JPEG tasks

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
	//OS->task_create("Quantize"/*sc_gen_unique_name("Quantize")*/, QUANTIZE_PRIORITY, Quant_h);
	OS->task_create("Zigzag"/*sc_gen_unique_name("Zigzag")*/, ZIGZAG_PRIORITY, Zigzag_h);
	OS->task_create("Huffman"/*sc_gen_unique_name("Huffman")*/, HUFFMAN_PRIORITY, Huff_h);

	// tasks have been created
	// now kickstart the OS
	OS->run();
	return;
} //end top method
	
void CPU::Read_thread(){
	
	// REUSE: initialize the task in the OS context
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

	// REUSE: terminate the task in the OS context
	OS->task_terminate();
}

void CPU::DCT_thread(){

	//local
	Block block;
	int in_block[64], out_block[64];
	
	// REUSE: initialize the task in the OS context
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

		// TODO: the block should be sent to Quantize HW
		// TODO: the corresponding ISR should tell us if its OK to proceed
		// TODO: wait on the appropriate semaphore to simulate blocking
		// TODO: use non-blocking write on the appropriate FIFO port
		// TODO: to send the data
		write_to_Quantize->wait();
		DCT2Quant.nb_write(block);
		cout << sc_time_stamp() << ": CPU sent block to Quantize HW" << endl;
	}

	// REUSE: terminate the task in the OS context
	OS->task_terminate();
}

void CPU::Zigzag_thread(){
	
	Block block;
	int in_block[64], out_block[64];

	// REUSE: initialize the task in the OS context
	OS->task_init();

	for(int iter = 0; iter < 180; iter++)
	{
		// TODO: the block should be received from Quantize HW
		// TODO: the corresponding ISR should tell us if its OK to proceed
		// TODO: wait on the appropriate semaphore to simulate blocking
		// TODO: use non-blocking read on the appropriate FIFO port
		// TODO: to receive the data
		read_from_Quantize->wait();
		Quant2Zigzag.nb_read(block);
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

	// REUSE: terminate the task in the OS context
	OS->task_terminate();
}

void CPU::Huff_thread(){
	
	Block block;
	int in_block[64];
	
	// REUSE: initialize the task in the OS context
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

	// REUSE: terminate the task in the OS context
	OS->task_terminate();
}

//------------ISRs-------------------------

void CPU::ISR_Quantize_input_seek() {
	// TODO: Allow writing of a new block from CPU to Quantize HW
	write_to_Quantize->post();
}

void CPU::ISR_Quantize_output_avail() {
	// TODO: Allow reading of a new block from Quantize HW
	read_from_Quantize->post();
}
