#include "systemc.h"
#ifndef FIFO_BLOCK
#define FIFO_BLOCK


class Block{
	public:
	Block(){}
	~Block(){}
	int data[64];
	
	friend ostream& operator<< (ostream& out, const Block& b){
		out << "" << endl;
		return out;
	}
	
};
#endif//FIFO_BLOCK
