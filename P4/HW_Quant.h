//-----Module definition for Quantize hardware block -------------
//-----You must modify this file as indicated by TODO comments----

#include "systemc.h"
#include "Block.h"

#define INT_PULSE_WIDTH sc_time(1, SC_US)

//module
SC_MODULE(HW_Quant)
{
	public:
	//constructor
	SC_CTOR(HW_Quant) {
		//instantiate the Quantize thread
		SC_THREAD(Quant_thread);
	}

	//define the Quantize thread method
	void Quant_thread();

	//TODO: define the FIFO buffer interfaces
	//TODO: name them InBuffer and OutBuffer
	sc_fifo_in<Block> InBuffer;
	sc_fifo_out<Block> OutBuffer;

	//TODO: define the interrupt ports 
	//TODO: name them int_input_seek, int_output_avail
	sc_out<bool> int_input_seek, int_output_avail;
};
