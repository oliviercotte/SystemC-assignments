//-----Definition of the hierarchical system module---------------
//-----You must modify this file as indicated by TODO comments----
//-----You may reuse code from Programming Assignment 4-----------
//-----as indicated by REUSE comments-----------------------------

#include "systemc.h"
#include "Block.h"
#include "CPU.h"
#include "HW_Quant.h"

//top-level module
SC_MODULE(System)
{
private:
	// define the SW-HW modules
	CPU *cpu;
	Quant_IP *hw_quant;

	// TODO: define the bus channel
	bus_channel bus;

	// instantiation of the interrupts
	sc_signal<bool> interrupt_Quant_input_seek, interrupt_Quant_output_avail;

public:
	//constructor
	// TODO: delegated constructor for bus channel (name it "Bus")
	SC_CTOR(System):
		bus("Bus"/*sc_gen_unique_name("Bus")*/)
	{
		// RESUE: instantiate the system-level modules
		// REUSE: name them "CPU" and "Quantize_block"
		cpu = new CPU("CPU"/*sc_gen_unique_name("CPU")*/);
		hw_quant = new Quant_IP("Quantize_block"/*sc_gen_unique_name("Quantize_block")*/);

		// TODO: bind the bus ports
		cpu->bus_port = &bus;
		hw_quant->bus_slave_interface = &bus;
		hw_quant->read_if->bus_port = &bus;
		hw_quant->write_if->bus_port = &bus;

		// REUSE: bind the interrupt ports
		cpu->int_Quantize_input_seek(interrupt_Quant_input_seek);
		hw_quant->interrupt_Quant_input_seek(interrupt_Quant_input_seek);

		cpu->int_Quantize_output_avail(interrupt_Quant_output_avail);
		hw_quant->interrupt_Quant_output_avail(interrupt_Quant_output_avail);
	}
};

