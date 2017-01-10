//-----Implementation of Quantize hardware IP module-----------------
//-----You must modify this file as indicated by TODO comments-------
//-----You may reuse code from Programming Assignment 4--------------
//-----as indicated by REUSE comments--------------------------------

//must include to use the systemc library
#include "Bus.h"
#include "systemc.h"
#include "HW_Quant.h"
#include "ReadBmp_aux.h"


void HW_Quant::Quant_thread(){

	Block block;
	int in_block[64], out_block[64];
	
	while (1) // this is a HW task - run forever
	{
		// do a blocking read on the input buffer
		InBuffer.read(block);
		
		// REUSE: consume time for this iteration = 5711 micro-seconds
		sc_time hw_quant_time(5711, SC_US);
		wait(hw_quant_time);

		for(int i = 0; i < 64; i++){
			in_block[i] = block.data[i];
		}
			
		quantize(in_block,out_block);
		
		for(int i = 0; i < 64; i++){
			block.data[i] = out_block[i];
		}
		
		// do a blocking write on the output buffer
		OutBuffer.write(block);
	} // keep looping infinitely
}
// end of HW task

// define the Read Interface task
void ReadIF::BusReader() {
	// loop infinitely to model HW
	while (1) {
		// TODO: reset the bus address to the starting address for this transaction
		BUS_ADDR_TYPE Addr = BLOCK_DCT2QUANT_START;
		BUS_DATA_TYPE BusWord;

		// TODO: if the buffer to Quantize has room
		// TODO: seek more data from CPU
		if (toQuantize.num_free() > 0) {
			cout << sc_time_stamp() << ": Quantize HW seeking input data from CPU: Sending Interrupt" << endl;
			// REUSE: create the interrupt pulse
			// REUSE: of size INT_PULSE_WIDTH
			int_input_seek.write(true);
			wait(INT_PULSE_WIDTH);
			int_input_seek.write(false);
			
			Block block;

			// TODO: now gather data from the bus
			for(int i = 0; i < BLOCK_SIZE; i++){
				bus_port->slave_read(Addr, &BusWord);
				block.data[i] = BusWord.to_int();
				Addr = BUS_ADDR_TYPE(Addr.to_int() + BUS_WORD_SIZE);
			}

			// TODO: write the assembled block to the Quantize hardware function
			toQuantize.write(block);
		}
		else { // the toQuantize internal buffer is full
			// TODO: wait for buffer to be read
			wait(toQuantize.data_read_event());
		}
	} // end while 1 loop
} // end bus reader	

// define the Write Interface task
void WriteIF::BusWriter() {
	// loop infinitely to model HW
	while (1) {
		// TODO: reset the bus address to the starting address for this transaction
		BUS_ADDR_TYPE Addr = BLOCK_QUANT2ZIGZAG_START;
		BUS_DATA_TYPE BusWord;

		// TODO: if the buffer from Quantize has data
		// TODO: send it to the CPU
		if (fromQuantize.num_available() > 0) {
			Block block;

			// TODO: read the outbound block from the Quantize hardware function
			fromQuantize.read(block);

			cout << sc_time_stamp() << ": Quantize HW has output data for CPU: Sending Interrupt" << endl;

			// REUSE: create the interrupt pulse
			// REUSE: of size INT_PULSE_WIDTH
			int_output_avail.write(true);
			wait(INT_PULSE_WIDTH);
			int_output_avail.write(false);
			
			// TODO:now send data over the bus
			for(int i = 0; i < BLOCK_SIZE; i++) {
				BusWord = BUS_DATA_TYPE(block.data[i]);
				bus_port->slave_write(Addr, &BusWord);
				Addr = BUS_ADDR_TYPE(Addr.to_int() + BUS_WORD_SIZE);
			}
		}
		else { // the fromQuantize internal buffer is empty
			// TODO: wait for buffer to be written
			wait(fromQuantize.data_written_event());
		}
	} // end while 1 loop
} // end bus reader	
