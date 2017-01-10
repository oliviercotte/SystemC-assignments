#include "pc.h"

int sc_main(int argc, char *argv[]) 
{
	//to remove a warning
	sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

	/* initialize random seed: */
	srand ( time(NULL) );

	// create instances
	Example example("Example_instance");

	//run example infinitely or until an exit function is called
	sc_core::sc_start(-1, sc_core::SC_NS);

}

