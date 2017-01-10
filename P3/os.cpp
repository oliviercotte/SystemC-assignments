//--------Function definitions for classes os and os_task---------
//-----You must modify this file as indicated by TODO comments----

#include "systemc.h"
#include "os.h"

using namespace std;

// implementation of os_task methods

//placeholder constructor since we have a pool
os_task::os_task() {
}

os_task::~os_task() {
}

void os_task::instantiate(sc_core::sc_process_handle handle, const char *name,
		unsigned int priority) {
	static unsigned int count = 0;
	task_name = name;
	// All tasks are initially ready when created
	task_state = READY;
	task_id = count;
	task_priority = priority;
	task_handle = handle;
	activation_flag = rd_suspended_flag = wr_suspended_flag = false;
	count++;
}

const char *os_task::get_name() {
	return task_name.data();
}

enum state os_task::get_state() {
	return task_state;
}

unsigned int os_task::get_id() {
	return task_id;
}

unsigned int os_task::get_priority() {
	return task_priority;
}

sc_core::sc_process_handle os_task::get_handle() {
	return task_handle;
}

void os_task::activate() {
	// TODO: set the activation flag and notify the event
	activation_flag = true;
	activation_event.notify();
	task_state = RUNNING;
	return;
}

void os_task::wait_for_activation() {
	// TODO: If the activation flag is not set,
	// TODO: wait for it to be set
	if (activation_flag == false) {
		wait(activation_event);
	}
	// TODO: reset the activation flag
	activation_flag = false;
	// TODO: update the task state
	//task_state = SUSPENDED;
	return;
}

//os class method implementation

os::os() {
	// empty constructor
}

os::~os() {

}

// dynamic task creation function
// this function is called by the main SC thread in the top module
// it may also be called by tasks to dynamically create children

os_task *os::task_create(const char* name, unsigned int priority,
		sc_core::sc_process_handle h) {
	// get a task object from the pool
	static int count = 0;
	os_task *t = &(task_pool[count++]);

	// TODO: instantiate the user task in the OS model
	t->instantiate(h, name, priority);

	// TODO: add the task to the ALL_TASKS list
	ALL_TASKS.push_back(t);

	//newly created task is in the ready state
	// TODO: add it to READY vector list
	READY_TASKS.push_back(t);

	// return the task pointer
	return t;
}

// task initialization function
// this function is called by the SystemC thread
// for the task, at the very beginning

void os::task_init() {
	os_task *t = NULL;

	// TODO: find this task's pointer "t"
	static int count = 0;
	t = &(task_pool[count++]);

	// TODO: wait for the task to be scheduled by the OS
	t->wait_for_activation();

	cout << sc_time_stamp() << ": Task " << t->get_name() << " is initialized" << endl;

	return;
}

// task termination function
// this function is called by the running task
// to terminate itself

void os::task_terminate() {
	// TODO: update the state of the task
	os_task *t = CURRENT;
	t->task_state = TERMINATED;

	// TODO: do a rescheduling on remaining tasks
	schedule();

	// TODO: wait till end of simulation if no tasks remain
	bool all_terminated = true;
	for (int i = 0; i < ALL_TASKS.size(); i++) {
		os_task *temp_t = ALL_TASKS[i];
		if (temp_t->get_state() != TERMINATED) {
			all_terminated = false;
			break;
		}
	}

	if (all_terminated) {
		wait();
	}
}

// this function displays the states of
// all the tasks in the order of their creation 
// It is called by the OS schedule method
// whenever a rescheduling takes place

void os::status() {
	cout << endl << sc_time_stamp() << ": STATUS" << endl;

	for (int i = 0; i < ALL_TASKS.size(); i++) {
		os_task *t = ALL_TASKS[i];
		cout << t->get_name() << ": ";
		if (t->get_state() == RUNNING)
			cout << "RUNNING" << endl;
		else if (t->get_state() == READY)
			cout << "READY" << endl;
		else if (t->get_state() == SUSPENDED)
			cout << "SUSPENDED" << endl;
		else if (t->get_state() == TERMINATED)
			cout << "TERMINATED" << endl;
	}
}

void os::time_wait(sc_time t) {
	// TODO: consume time on the SystemC scheduler
	wait(t);
	return;
} // end time_wait

ERROR os::schedule() {
	// TODO: If there are no tasks or all tasks have terminated
	// TODO: Print the status and return code E_NO_TASKS
	if (READY_TASKS.empty()) {
		status();
		return E_NO_TASKS;
	}

	bool all_terminated = true;
	for (int i = 0; i < ALL_TASKS.size(); i++) {
		os_task *t = ALL_TASKS[i];
		if (t->get_state() != TERMINATED) {
			all_terminated = false;
			break;
		}
	}

	if (all_terminated) {
		status();
		return E_NO_TASKS;
	}

	// TODO: If all tasks are suspended, just display
	// TODO: the states and return code E_OK
	bool not_all_suspended = false;
	for (int i = 0; i < ALL_TASKS.size(); i++) {
		os_task *t = ALL_TASKS[i];
		if (t->get_state() != SUSPENDED) {
			not_all_suspended = true;
			break;
		}
	}

	// otherwise, we have some scheduling to do!
	// TODO: find the highest priority ready task
	if (not_all_suspended) {
		// TODO: remove the highest priority task from the ready list
		// TODO: and activate it!
		unsigned int task_index = 0;
		unsigned int priority = READY_TASKS[0]->get_priority();
		for (int i = 1; i < READY_TASKS.size(); i++) {
			os_task *t = READY_TASKS[i];
			if (t->get_priority() > priority) {
				task_index = i;
				priority = t->get_priority();
			}
		}
		CURRENT = READY_TASKS[task_index];
		READY_TASKS.erase(READY_TASKS.begin() + task_index);
		CURRENT->activate();
	}

	// print out the status of all the tasks
	status();
	return E_OK;
} // end schedule

void os::run() {
	// all the initial tasks have been created
	// kickstart the scheduler
	cout << "Initializing Scheduler... " << endl;
	schedule();
}
