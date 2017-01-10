//-----Implementation of Quantize hardware module-----------------
//-----You must modify this file as indicated by TODO comments----

#include "systemc.h"
#include "HW_Quant.h"
#include "ReadBmp_aux.h"


void HW_Quant::Quant_thread(){

	Block block;
	int in_block[64], out_block[64];
	
	while (1) // this is a HW task - run forever
	{
		// TODO: if the input buffer has space
		// TODO: send an interrupt to seek more data from the processor
		if (InBuffer.num_available() < FIFO_SIZE) {
			// TODO: create an interrupt pulse
			// TODO: of size INT_PULSE_WIDTH
			int_input_seek.write(true);
			wait(INT_PULSE_WIDTH);
			int_input_seek.write(false);
			cout << sc_time_stamp() << ": Quantize HW seeking input data from CPU: Interrupt sent" << endl;
		}
	 	
		// now do a blocking read 
		InBuffer.read(block);
		
		// TODO: consume time for this iteration = 5711 micro-seconds
		sc_time hw_quant_time(5711, SC_US);
		wait(hw_quant_time);

		for(int i = 0; i < 64; i++){
			in_block[i] = block.data[i];
		}
			
		quantize(in_block,out_block);
		
		for(int i = 0; i < 64; i++){
			block.data[i] = out_block[i];
		}
		
		// do a blocking write
		OutBuffer.write(block);

		// blocking write is successful
		// there is data available in the output buffer
		// let the CPU know by sending an interrupt
		// TODO: create an interrupt pulse
		// TODO: of size INT_PULSE_WIDTH
		int_output_avail.write(true);
		wait(INT_PULSE_WIDTH);
		int_output_avail.write(false);
		cout << sc_time_stamp() << ": Quantize HW has output data for CPU: Interrupt sent" << endl;
	} // keep looping infinitely
}
// end of HW task

