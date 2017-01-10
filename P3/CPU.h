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

#ifndef QUANTIZE_PRIORITY
#define QUANTIZE_PRIORITY 1
#endif

#ifndef ZIGZAG_PRIORITY
#define ZIGZAG_PRIORITY 1
#endif

#ifndef HUFFMAN_PRIORITY
#define HUFFMAN_PRIORITY 1
#endif

//module
SC_MODULE(CPU) {
public:
	//constructor
	SC_CTOR(CPU) {
		// TODO: Instantiate the OS and the os_fifo channels here
		// note that write delay is 6 MS and read delay is 4 MS
		// sizes of the FIFO channels is given by macro FIFO_SIZE
		OS = new os;

		sc_time wrd = sc_time(6, SC_MS);
		sc_time rdd = sc_time(4, SC_MS);

		Read2DCT = new os_fifo<Block>(OS, "Read2DCT"/*sc_gen_unique_name("Read2DCT")*/, FIFO_SIZE, wrd, rdd);
		DCT2Quant = new os_fifo<Block>(OS, "DCT2Quant"/*sc_gen_unique_name("DCT2Quant")*/, FIFO_SIZE, wrd, rdd);
		Quant2Zigzag = new os_fifo<Block>(OS, "Quant2Zigzag"/*sc_gen_unique_name("Quant2Zigzag")*/, FIFO_SIZE, wrd, rdd);
		Zigzag2Huff = new os_fifo<Block>(OS, "Zigzag2Huff"/*sc_gen_unique_name("Zigzag2Huff")*/, FIFO_SIZE, wrd, rdd);

		//define the top level thread
		SC_THREAD(top);
	}

	// define the thread functions here
	// but the actual threads will be spawned dynamically
	void top();
	void Read_thread();
	void DCT_thread();
	void Quant_thread();
	void Zigzag_thread();
	void Huff_thread();

	//define the FIFO pointers
	os_fifo<Block> *Read2DCT;
	os_fifo<Block> *DCT2Quant;
	os_fifo<Block> *Quant2Zigzag;
	os_fifo<Block> *Zigzag2Huff;

	// pointer to the operating system model
	os *OS;
};
