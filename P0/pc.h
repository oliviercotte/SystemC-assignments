#include "systemc.h"
#ifndef FIFO_SIZE
#define FIFO_SIZE 16
#endif

//module
SC_MODULE(Example)
{
	public:
	//constructor
	SC_CTOR(Example): fifo(FIFO_SIZE){
		//make it have threads
		SC_THREAD(producerThread);
		SC_THREAD(consumerThread);
	}

	void producerThread();
	void consumerThread();

	private:
	//have fifo
	sc_fifo<int> fifo;

};
