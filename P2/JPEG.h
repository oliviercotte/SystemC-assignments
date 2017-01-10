//---------Defines the JPEG module--------------------------------------------
//---------You must edit this file as indicated by TODO comments--------------

#include "systemc.h"
#include "Block.h"

#ifndef FIFO_SIZE
#define FIFO_SIZE 16
#endif

using std::auto_ptr;

//Read Module
SC_MODULE(Read_Module)
{
	public:
	//Constructor
	SC_CTOR(Read_Module){
		SC_THREAD(Read_thread);
	}
	
	//Declaration of the thread
	void Read_thread();
	
	//TODO: declare the FIFO output port, name it outport
	sc_fifo_out<Block> outport;
};

//DCT Module
SC_MODULE(DCT_Module)
{
	public:
	//Constructor
	SC_CTOR(DCT_Module){
		SC_THREAD(DCT_thread);
	}
	
	//Declaration of the thread
	void DCT_thread();
	
	//TODO: declare the FIFO input port, name it inport
	sc_fifo_in<Block> inport;
	//TODO: declare the FIFO output port, name it outport
	sc_fifo_out<Block> outport;
};

//Quant Module
SC_MODULE(Quant_Module)
{
	public:
	//Constructor
	SC_CTOR(Quant_Module){
		SC_THREAD(Quant_thread);
	}
	
	//Declaration of the thread
	void Quant_thread();
	
	//TODO: declare the FIFO input port, name it inport
	sc_fifo_in<Block> inport;
	//TODO: declare the FIFO output port, name it outport
	sc_fifo_out<Block> outport;
};

//Zigzag Module
SC_MODULE(Zigzag_Module)
{
	public:
	//Constructor
	SC_CTOR(Zigzag_Module){
		SC_THREAD(Zigzag_thread);
	}
	
	//Declaration of the thread
	void Zigzag_thread();
	
	//TODO: declare the FIFO input port, name it inport
	sc_fifo_in<Block> inport;
	//TODO: declare the FIFO output port, name it outport
	sc_fifo_out<Block> outport;
};

//Huff Module
SC_MODULE(Huff_Module)
{
	public:
	//Constructor
	SC_CTOR(Huff_Module){
		SC_THREAD(Huff_thread);
	}
	//Declaration of the thread
	void Huff_thread();
	
	//TODO: declare the FIFO input port, name it inport
	sc_fifo_in<Block> inport;
};

//JPEG module
SC_MODULE(JPEG)
{
	public:
	//TODO: define the constructor
	SC_CTOR(JPEG) :
		read_module(new Read_Module (sc_gen_unique_name("read_module"))),
		dct_module(new DCT_Module (sc_gen_unique_name("dct_module"))),
		quant_module(new Quant_Module (sc_gen_unique_name("quant_module"))),
		zigzag_module(new Zigzag_Module (sc_gen_unique_name("zigzag_module"))),
		huff_module(new Huff_Module (sc_gen_unique_name("huff_module"))),
		Read2DCT(new sc_fifo<Block>(sc_gen_unique_name("Read2DCT_fifo"), FIFO_SIZE)),
		DCT2Quant(new sc_fifo<Block>(sc_gen_unique_name("DCT2Quant_fifo"), FIFO_SIZE)),
		Quant2Zigzag(new sc_fifo<Block>(sc_gen_unique_name("Quant2Zigzag_fifo"), FIFO_SIZE)),
		Zigzag2Huff(new sc_fifo<Block>(sc_gen_unique_name("Zigzag2Huff_fifo"), FIFO_SIZE))
	{
		//TODO: Instantiate the sub-modules and assign them to their pointers

		//TODO: Instantiate the FIFOs with the size given by macro FIFO_SIZE
		//      and assign them to their pointers

		//TODO: Bind the modules' FIFO ports
		read_module->outport(*Read2DCT);

		dct_module->inport(*Read2DCT);
		dct_module->outport(*DCT2Quant);

		quant_module->inport(*DCT2Quant);
		quant_module->outport(*Quant2Zigzag);

		zigzag_module->inport(*Quant2Zigzag);
		zigzag_module->outport(*Zigzag2Huff);

		huff_module->inport(*Zigzag2Huff);
	}

	private:
	//TODO: Declare the module pointers as
	//      read_module, dct_module, quant_module, zigzag_module, huff_module
	auto_ptr<Read_Module> read_module;
	auto_ptr<DCT_Module> dct_module;
	auto_ptr<Quant_Module> quant_module;
	auto_ptr<Zigzag_Module> zigzag_module;
	auto_ptr<Huff_Module> huff_module;
	//TODO: Declare the FIFO pointers
	typedef std::auto_ptr<sc_fifo<Block> > fifo_ptr;
	fifo_ptr Read2DCT;
	fifo_ptr DCT2Quant;
	fifo_ptr Quant2Zigzag;
	fifo_ptr Zigzag2Huff;
};
