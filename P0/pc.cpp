//This is simple Producer/Consumer example
//For more examples check
//http://www.asic-world.com/systemc/


//must include to use the systemc library
#include "systemc.h"
#include "pc.h"
#include "headers.h"

void Example::producerThread(){
	cout << sc_time_stamp() << ": Initializing producerThread\n";

	int x;

	//a time variable
	sc_time time = sc_time(3, SC_MS);

	//loop that runs 10 times
	for(int i = 0; i < 5; i++){

		wait(time);
		
		x = producer();

		cout << sc_time_stamp() << ": producerThread writing " << x << " to the fifo\n";
		fifo.write(x);

		//note that write on fifo blocks once the fifo is full
	}
}

void Example::consumerThread(){
	cout  << "\t \t \t \t \t " << sc_time_stamp() << ": Initializing consumerThread\n";

	int x;

	//a time variable
	sc_time time = sc_time(5, SC_MS);

	//loop that runs 10 times
	for(int i = 0; i < 5; i++){
		
		fifo.read(x);
		
		consumer(x);
		
		cout << "\t \t \t \t \t " << sc_time_stamp() << ": consumerThread has read " << x << " from the fifo\n";

		//take time to consume
		wait(time);
	}
	exit(0);
}





