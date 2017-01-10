#include "System.h"

int sc_main(int argc, char *argv[]) 
{
	//to remove a warning
	sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

	// create instances
	System system("SYSTEM");

	//run example infinitely or until an exit function is called
	sc_core::sc_start(100, sc_core::SC_SEC);

}


