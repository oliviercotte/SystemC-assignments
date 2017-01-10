//---------Defines the CPU module---------------
//---------You must edit this file--------------
//---------based on the TODO comments-----------


#include "systemc.h"
#include "Block.h"
#include "os.h"

// define the task priorities 

#ifndef READ_PRIORITY
#define READ_PRIORITY 1
#endif

#ifndef DCT_PRIORITY
#define DCT_PRIORITY 1
#endif

#ifndef ZIGZAG_PRIORITY
#define ZIGZAG_PRIORITY 1
#endif

#ifndef HUFFMAN_PRIORITY
#define HUFFMAN_PRIORITY 1
#endif


//module
SC_MODULE(CPU)
{
	public:
	//constructor
	SC_CTOR(CPU)
	{
		// TODO: Instantiate the OS and the os_fifo channels here
		// note that write delay is 6 MS and read delay is 4 MS
		// sizes of the FIFO channels is given by macro FIFO_SIZE
		OS = new os;

		write_to_Quantize = new os_sem(OS);
		read_from_Quantize = new os_sem(OS);

		sc_time wrd = sc_time(6, SC_MS);
		sc_time rdd = sc_time(4, SC_MS);

		Read2DCT = new os_fifo<Block>(OS, "Read2DCT"/*sc_gen_unique_name("Read2DCT")*/, FIFO_SIZE, wrd, rdd);
		//DCT2Quant = new os_fifo<Block>(OS, "DCT2Quant"/*sc_gen_unique_name("DCT2Quant")*/, FIFO_SIZE, wrd, rdd);
		//Quant2Zigzag = new os_fifo<Block>(OS, "Quant2Zigzag"/*sc_gen_unique_name("Quant2Zigzag")*/, FIFO_SIZE, wrd, rdd);
		Zigzag2Huff = new os_fifo<Block>(OS, "Zigzag2Huff"/*sc_gen_unique_name("Zigzag2Huff")*/, FIFO_SIZE, wrd, rdd);

		//define the top level thread
		SC_THREAD(top);

		// TODO: register the ISRs using SC_METHOD
		// TODO: assign the sensitivity list to trigger on interrupt
		// TODO: but make sure you don't run the ISRs at model initialization
		SC_METHOD(ISR_Quantize_input_seek);
			dont_initialize(); // don't call ISR_Quantize_input_seek at time 0
			sensitive << int_Quantize_input_seek.pos();

		SC_METHOD(ISR_Quantize_output_avail);
			dont_initialize(); // don't call ISR_Quantize_output_avail at time 0
			sensitive << int_Quantize_output_avail.pos();
	}

	// define the thread functions here
	// but the actual threads will be spawned dynamically
	void top();
	void Read_thread();
	void DCT_thread();
	void Zigzag_thread();
	void Huff_thread();

	// define the ISR methods
	// these are defined statically
	void ISR_Quantize_input_seek();
	void ISR_Quantize_output_avail();

	//define the FIFO pointers
	os_fifo<Block> *Read2DCT;
	os_fifo<Block> *Zigzag2Huff;

	// define the semaphore pointers
	os_sem *write_to_Quantize, *read_from_Quantize;

	// pointer to the operating system model
	os *OS;

	// CPU hardware port definitions
	//------------------------------------------------------------------
	// TODO: define the FIFO ports DCT2Quant and Quant2Zigzag
	// TODO: these ports will be used to access the system-level FIFOs
	sc_fifo_in<Block> Quant2Zigzag;
	sc_fifo_out<Block> DCT2Quant;

	// TODO: define the interrupt ports
	// TODO: name them int_Quantize_input_seek and int_Quantize_output_avail
	sc_in<bool> int_Quantize_input_seek, int_Quantize_output_avail;
};
