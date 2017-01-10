//------------Bus Channel Implementation------------------
//--------Modify this file based on the TODO comments-----

//must include to use the systemc library
#include "systemc.h"
#include "Bus.h"

void bus_channel::master_write(BUS_ADDR_TYPE Address, BUS_DATA_TYPE *Data)
{
	// TODO: put in the appropriate values of bus Address, data and RNW
	BusAddress = Address;
	BusData = *Data;
	RNW = SC_LOGIC_0;

	// TODO: wait for request time and assert the request
	wait(BUS_WRITE_T1);
	Req = true;

	// TODO: wait for acknowledgment from slave
	wait(Ack.posedge_event());

	// TODO: writing is successful - return all signals to default
	BusAddress = BUS_ADDRESS_DEFAULT_VALUE;
	RNW = SC_LOGIC_X;

	// TODO: wait for BUS_WRITE_T3, de-assert the request
	wait(BUS_WRITE_T3);
	Req = false;

	// TODO: wait for acknowledgment signal to be deasserted
	wait(Ack.negedge_event());

	// TODO: bus cycle is complete - return;
	return;
}
// end of master write method

void bus_channel::master_read(BUS_ADDR_TYPE Address, BUS_DATA_TYPE *Data)
{
	// TODO: put the address that the master wants to read from
	BusAddress = Address;
	RNW = SC_LOGIC_1;

	// TODO: wait for request time, BUS_READ_T1, and assert the request
	wait(BUS_READ_T1);
	Req = true;

	// TODO: wait for acknowledgment from slave
	wait(Ack.posedge_event());

	// TODO: bus data is valid, copy it over
	*Data = BusData;
	BusAddress = BUS_ADDRESS_DEFAULT_VALUE;
	RNW = SC_LOGIC_X;

	// TODO: wait for copying time (BUS_READ_T3),
	// TODO: de-assert request to indicate completion of read 
	// TODO: and return address and RNW to default values
	wait(BUS_READ_T3);
	Req = false;

	// TODO: wait for acknowledgment signal to be deasserted
	wait(Ack.negedge_event());

	// bus cycle is complete - return;
	return;
}
// end of master read method


void bus_channel::slave_write(BUS_ADDR_TYPE Address, BUS_DATA_TYPE *Data)
{
	// TODO: wait for the read request from master
	// TODO: for the given address
	do {
		wait(Req.posedge_event());
	} while(RNW != SC_LOGIC_1);

	// TODO: write over the data into the data bus
	BusData = *Data;

	// TODO: wait for slave write time (BUS_READ_T2) and assert the acknowledge
	wait(BUS_READ_T2);
	Ack = true;

	// TODO: wait for request to be de-asserted
	wait(Req.negedge_event());

	// TODO: wait for BUS_READ_T4, de-assert the acknowledge signal
	wait(BUS_READ_T4);
	Ack = false;

	// bus cycle is complete - return;
	return;
}
// end of slave write method

void bus_channel::slave_read(BUS_ADDR_TYPE Address, BUS_DATA_TYPE *Data)
{
	// TODO: wait for the write request from master
	// TODO: for the given address
	do {
		wait(Req.posedge_event());
	} while(RNW != SC_LOGIC_0);

	// TODO: read over the data from the data bus
	*Data = BusData;

	// TODO: wait for slave read time (BUS_WRITE_T2) and assert the acknowledge
	wait(BUS_WRITE_T2);
	Ack = true;

	// TODO: wait for request to be de-asserted
	wait(Req.negedge_event());

	// TODO: wait for BUS_WRITE_T4, de-assert the acknowledge signal
	wait(BUS_WRITE_T4);
	Ack = false;

	// bus cycle is complete - return;
	return;
}
// end of slave read method
