//-------------------Bus Parameters and Access Methods-----------
//-----This file defines the bus protocol timing parameters------
//-----and the SystemC interfaces and channels for the bus-------
//-----DO NOT MODIFY THIS FILE-----------------------------------

#ifndef BUS_H
#define BUS_H

#include "systemc.h"

// define the timing parameters, from the master perspective

// time from the instant that bus master writes address, data, RNW to request
#define BUS_WRITE_T1 sc_time(2, SC_US)

// time from master request to slave data read and acknowledgement
#define BUS_WRITE_T2 sc_time(4, SC_US)

// time from slave acknowledgment to request de-assert
#define BUS_WRITE_T3 sc_time(1, SC_US)

// time from request de-assert to acknowledgment de-assert
#define BUS_WRITE_T4 sc_time(1, SC_US)

// time from the instant that bus master writes address, RNW to request
#define BUS_READ_T1 sc_time(1, SC_US)

// time from master request to slave data write and acknowledgement
#define BUS_READ_T2 sc_time(6, SC_US)

// time from slave acknowledgment to request de-assert
#define BUS_READ_T3 sc_time(1, SC_US)

// time from request de-assert to acknowledgment de-assert
#define BUS_READ_T4 sc_time(1, SC_US)

#define BUS_ADDR_SIZE 16
#define BUS_DATA_SIZE 32 
#define BUS_WORD_SIZE BUS_DATA_SIZE/8 

#define BUS_ADDR_TYPE sc_lv<BUS_ADDR_SIZE>
#define BUS_DATA_TYPE sc_lv<BUS_DATA_SIZE>

#define BUS_ADDRESS_DEFAULT_VALUE "XXXXXXXXXXXXXXXX"
#define BUS_DATA_DEFAULT_VALUE "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

//Bus Master Interface

class bus_master_if: virtual public sc_interface {
public:
	// define the pure virtual master access functions in the abstract base class
	virtual void master_write (BUS_ADDR_TYPE Address, BUS_DATA_TYPE *Data) = 0;
	virtual void master_read (BUS_ADDR_TYPE Address, BUS_DATA_TYPE *Data) = 0;
};
 
class bus_slave_if: virtual public sc_interface {
public:
	// define the pure virtual slave access functions in the abstract base class
	virtual void slave_write (BUS_ADDR_TYPE Address, BUS_DATA_TYPE *Data) = 0;
	virtual void slave_read (BUS_ADDR_TYPE Address, BUS_DATA_TYPE *Data) = 0;
};

class bus_channel: public bus_master_if, public bus_slave_if, public sc_channel {
private:
	// elements of the bus
	BUS_ADDR_TYPE BusAddress; 	// Address
	BUS_DATA_TYPE BusData;		// Data
	sc_logic RNW;				// = logic 1 for Read; = logic 0 for Write transaction
	sc_signal<bool> Req, Ack; 	// signals for Master-Slave synchronization
		
public:
	bus_channel(sc_module_name bus_name) : sc_channel(bus_name) {
		BusAddress = BUS_ADDRESS_DEFAULT_VALUE;
		BusData = BUS_DATA_DEFAULT_VALUE;
		RNW = SC_LOGIC_X;
		Req = false;
		Ack = false; 
	}
						 
	void master_write (BUS_ADDR_TYPE Address, BUS_DATA_TYPE *Data);
	void master_read (BUS_ADDR_TYPE Address, BUS_DATA_TYPE *Data);
	void slave_write (BUS_ADDR_TYPE Address, BUS_DATA_TYPE *Data);
	void slave_read (BUS_ADDR_TYPE Address, BUS_DATA_TYPE *Data);
}; // end bus channel definition

#endif
