//---------Defines the CPU module---------------
//---------You must edit this file--------------

#include "systemc.h"
#ifndef FIFO_SIZE
#define FIFO_SIZE 16
#endif
#include "Block.h"

//module
SC_MODULE(CPU)
{
	public:
	//TODO: define the constructor
	// The threads are named: Read_thread, DCT_thread, Quant_thread, Zigzag_thread and Huff_thread
	// The FIFO channels are named: Read2DCT, DCT2Quant, Quant2Zigzag, and Zigzag2Huff
	// Make sure you use the correct names for threads and channels
	SC_CTOR(CPU) :
		Read2DCT(new sc_fifo<Block>(sc_gen_unique_name("Read2DCT_fifo"),FIFO_SIZE)),
		DCT2Quant(new sc_fifo<Block>(sc_gen_unique_name("DCT2Quant_fifo"),FIFO_SIZE)),
		Quant2Zigzag(new sc_fifo<Block>(sc_gen_unique_name("Quant2Zigzag_fifo"),FIFO_SIZE)),
		Zigzag2Huff(new sc_fifo<Block>(sc_gen_unique_name("Zigzag2Huff_fifo"),FIFO_SIZE))
	{
		SC_THREAD(Read_thread);
		SC_THREAD(DCT_thread);
		SC_THREAD(Quant_thread);
		SC_THREAD(Zigzag_thread);
		SC_THREAD(Huff_thread);
	}

	//TODO: declare the thread functions here
	//The thread functions are defined in CPU.cpp
	void Read_thread();
	void DCT_thread();
	void Quant_thread();
	void Zigzag_thread();
	void Huff_thread();

	private:
	//TODO: Instantiate the FIFO channels
	typedef std::auto_ptr<sc_fifo<Block> > fifo_ptr;
	fifo_ptr Read2DCT;
	fifo_ptr DCT2Quant;
	fifo_ptr Quant2Zigzag;
	fifo_ptr Zigzag2Huff;

};
