//-----Module definition for Quantize hardware IP block -------------
//-----You must modify this file as indicated by TODO comments-------
//-----You may reuse code from Programming Assignment 4 -------------
//-----as indicated by the REUSE comments----------------------- ----

#include "systemc.h"
#include "Block.h"
#include "Bus.h"
#include "AddressMap.h"

#define BLOCK_SIZE 64

#define INT_PULSE_WIDTH sc_time(1, SC_US)

//module
SC_MODULE(HW_Quant) {
public:
	//constructor
	SC_CTOR(HW_Quant) {
		//instantiate the Quantize thread
		SC_THREAD(Quant_thread);
	}

	//define the Quantize thread method
	void Quant_thread();

	//REUSE: define the FIFO buffer interfaces
	//REUSE: name them InBuffer and OutBuffer
	sc_fifo_in<Block> InBuffer;
	sc_fifo_out<Block> OutBuffer;
};

// Read interface module
SC_MODULE(ReadIF) {
public:
	// constructor
	SC_CTOR(ReadIF) :
		bus_port(NULL)
	{
		// instantiate the reader thread
		SC_THREAD(BusReader);
	}

	// define the bus reader method
	void BusReader();

	// TODO: define the FIFO output interface to hardware block
	// TODO: name it toQuantize
	sc_fifo_out<Block> toQuantize;

	// TODO: define the bus slave interface
	// TODO: name it bus_port
	bus_slave_if *bus_port;

	// TODO: define the interrupt port for seeking data
	// TODO: name it int_input_seek
	sc_out<bool> int_input_seek;
};
//end read interface

SC_MODULE(WriteIF) {
public:
	// constructor
	SC_CTOR(WriteIF) :
		bus_port(NULL)
	{
		// instantiate the write thread
		SC_THREAD(BusWriter);
	}

	// define the bus writer method
	void BusWriter();

	// TODO: define the FIFO input interface from hardware block
	// TODO: name it fromQuantize
	sc_fifo_in<Block> fromQuantize;

	// TODO: define the bus slave interface
	// TODO: name it bus_port
	bus_slave_if *bus_port;

	// TODO: define the interrupt port for seeking data
	// TODO: name it int_output_avail
	sc_out<bool> int_output_avail;
};
//end read interface

// Hierarchical HW IP module
SC_MODULE(Quant_IP) {
public:
	// TODO: define the sub-modules
	HW_Quant *hw_quant;
	ReadIF *read_if;
	WriteIF *write_if;

	// TODO: define the local channels between interface modules and HW_Quant;
	sc_fifo<Block> FIFO_DCT2Quant, FIFO_Quant2Zigzag;

	// TODO: define the top-level interrupt ports
	sc_out<bool> interrupt_Quant_input_seek, interrupt_Quant_output_avail;

	// TODO: define the bus slave interface
	bus_slave_if *bus_slave_interface;

	// constructor
	SC_CTOR(Quant_IP): // construct the local FIFOs in HW
		FIFO_DCT2Quant(FIFO_SIZE),
		FIFO_Quant2Zigzag(FIFO_SIZE)
	{
		// TODO: instantiate the sub-modules
		hw_quant = new HW_Quant("HW_Quant"/*sc_gen_unique_name("HW_Quant")*/);
		read_if = new ReadIF("ReadIF"/*sc_gen_unique_name("ReadIF")*/);
		write_if = new WriteIF("WriteIF"/*sc_gen_unique_name("WriteIF")*/);

		// TODO: bind the FIFO ports
		read_if->toQuantize(FIFO_DCT2Quant);
		hw_quant->InBuffer(FIFO_DCT2Quant);

		hw_quant->OutBuffer(FIFO_Quant2Zigzag);
		write_if->fromQuantize(FIFO_Quant2Zigzag);

		// TODO: bind the interrupt ports
		read_if->int_input_seek(interrupt_Quant_input_seek);
		write_if->int_output_avail(interrupt_Quant_output_avail);

		// TODO: bind the bus ports
		bus_slave_interface = NULL;
	}
	// end constructor
};
// end Quantize IP definition
