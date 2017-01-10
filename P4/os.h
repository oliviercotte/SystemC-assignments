//--------Definition of OS primitives------------------------------
//-----You must modify this file by reusing the os_fifo methods----
//-----------from Programming Assignment 3-------------------------

//-------Note the new preemption event in os class-----------------
//-------Note the new class os_sem definition----------------------


#include "systemc.h"
#include "error.h"

using namespace std;

// FIFO definition for IPC communication

#ifndef FIFO_SIZE
#define FIFO_SIZE (int) 16
#endif

#define TASK_POOL_SIZE 20


// forward declarations
class os;
class os_task;
class os_sem;
template <class T> class os_fifo;

// type definition for task state
enum state {RUNNING, READY, SUSPENDED, TERMINATED};

// Task definition
class os_task{
	friend class os;
	friend class os_sem;
	template <class T> friend class os_fifo;
public:
	os_task();
	~os_task();
	void instantiate(sc_core::sc_process_handle handle, const char *name, unsigned int priority);
	const char *get_name();
	enum state get_state();
	unsigned int get_id();
	unsigned int get_priority();
	sc_core::sc_process_handle get_handle();
	//bool operator < (os_task rhs) { return task_name.data() < rhs.get_name(); }

private:
	//each task has a name, state, id and priority
	string task_name;
	enum state task_state;
	unsigned int task_id;
	unsigned int task_priority;

	// the SystemC thread for the task
	sc_core::sc_process_handle task_handle;

	// activation flag and event are used to run the task
	bool activation_flag, rd_suspended_flag, wr_suspended_flag;
	sc_event activation_event;
	void activate();
	void wait_for_activation();

}; // end class os_task

class os{
	friend class os_sem;
	template <class T> friend class os_fifo;
public:
	os();
	~os();

	//dynamically create a new task
	os_task *task_create(const char *name, unsigned int priority, sc_core::sc_process_handle h);

	// initialize a newly created task
	// called at the beginning of the SystemC thread of the task
	void task_init();

	//when a task has finished, it can call task_terminate to take itself off the scheduler
	void task_terminate();

	//consume time on the CPU
	void time_wait(double t, sc_core::sc_time_unit u)
		{ sc_core::sc_time time(t,u); time_wait(time); }

	//list states of all the tasks (RUNNING, READY, SUSPENDED, TERMINATED)
	void status();

	// kickstart the scheduler
	// called after the initial tasks are created
	void run();

private:
	// static pool of tasks
	os_task task_pool[TASK_POOL_SIZE];

	// pointer to the currently running task
	os_task * CURRENT;

	//a vector that keeps all the ready tasks
	std::vector<os_task*> READY_TASKS;

	//a vector that keeps all created tasks
	std::vector<os_task*> ALL_TASKS;
	
	//task can consume time on CPU by calling time_wait
	void time_wait(sc_core::sc_time t);

	// determine the next task to run
	ERROR schedule();

	sc_event preemption;

}; // end class os

// os_sem class
// this class defines the semaphore in the OS context
// it derives from the sc_semaphore class
// note that we assume only one thread to be blocked on semaphore

class os_sem: public sc_semaphore
{
private:
	class os *OS;
	class os_task *blocked_task;

public:
	os_sem (class os *host_os, int init_value = 0): sc_semaphore(init_value) {
		OS = host_os;
		blocked_task = NULL;
	}
	int wait();
	int post();

}; // end os_sem class definition
			

// os_fifo class
// this is a template class that must be completely
// defined in the header file

template <class T>
class os_fifo: public sc_fifo<T>
{
private:
	class os *OS;
        sc_time wr_delay, rd_delay;
	class os_task *blocked_writer, *blocked_reader;

public:
	os_fifo(class os *host_os, const char *name, int size,
                      sc_time wrd = sc_time(0, SC_MS),
                      sc_time rdd = sc_time(0, SC_MS)):sc_fifo<T>(name, size) {
        	wr_delay = wrd;
        	rd_delay = rdd;
        	blocked_writer = NULL;
        	blocked_reader = NULL;
        	OS = host_os;
	}

// The write method is not thread safe
// We assume that there is only one writer task per FIFO

	void write(const T& data) {
		//-----REUSE CODE FROM PROG. ASSIGNMENT 3--------
		// TODO immediately after calling the base write function
		//      call the base update() function
		os_task *temp_t = OS->CURRENT;
		// TODO: check for blocking condition
		if (sc_fifo<T>::num_free() == 0) {
			// TODO: change the state of this writer and set the CURRENT task to NULL
			temp_t->task_state = SUSPENDED;
			OS->CURRENT = NULL;

			cout << sc_time_stamp() << ": Task " << temp_t->get_name() << " blocked on writing to fifo " << sc_fifo<T>::name() << "\n";

			// TODO: set the blocked_writer pointer
			blocked_writer = temp_t;
			blocked_writer->wr_suspended_flag = true;

			// TODO: find the next active task and run it
			OS->schedule();

			// TODO: wait for this writer to be activated
			blocked_writer->wait_for_activation();
		}

		// TODO: consume time for the writing
		OS->time_wait(wr_delay);

		// TODO: do the actual writing by calling the base class method
		sc_fifo<T>::write(data);
		sc_fifo<T>::update();

		// display the new state of this writer
		cout << sc_time_stamp() << ": Task " << temp_t->get_name() << " wrote to fifo " << sc_fifo<T>::name() << "\n";

		// TODO: the writing might unblock the reader
		// TODO: if so, unblock the reader and reschedule
		if (blocked_reader) {
			blocked_reader->rd_suspended_flag = false;

			if (blocked_reader->wr_suspended_flag == false) {
				os_task *blocked_t = blocked_reader;
				blocked_reader->task_state = READY;
				OS->READY_TASKS.push_back(blocked_reader);
				blocked_reader = 0;

				if (blocked_t->get_priority() > temp_t->get_priority()) {
					temp_t->task_state = READY;
					OS->READY_TASKS.push_back(temp_t);
					OS->schedule();
					temp_t->wait_for_activation();
				}
			}
		}
	} // end write

// The read method is not thread safe
// We assume that there is only one reader task per FIFO
	void read(T& data) {
		//-----REUSE CODE FROM PROG. ASSIGNMENT 3--------
		// TODO immediately after calling the base read function
		//      call the base update() function
		os_task *temp_t = OS->CURRENT;
		// TODO: check for blocking condition
		if (sc_fifo<T>::num_available() == 0) {
			// TODO: change the state of this reader and set the CURRENT task to NULL
			temp_t->task_state = SUSPENDED;
			OS->CURRENT = NULL;

			cout << sc_time_stamp() << ": Task " << temp_t->get_name() << " blocked on reading from fifo " << sc_fifo<T>::name() << "\n";

			// TODO: set the blocked_reader pointer
			blocked_reader = temp_t;
			blocked_reader->rd_suspended_flag = true;

			// TODO: find the next active task and run it
			OS->schedule();

			// TODO: wait for this reader to be activated
			blocked_reader->wait_for_activation();
		}

		// TODO: consume time for the reading
		OS->time_wait(rd_delay);

		// TODO: do the actual reading by calling the base class method
		sc_fifo<T>::read(data);
		sc_fifo<T>::update();

		// display the new state of this reader
		cout << sc_time_stamp() << ": Task " << temp_t->get_name() << " read from fifo " << sc_fifo<T>::name() << "\n";

		// TODO: the reading might unblock the writer
		// TODO: if so, unblock the writer and reschedule
		if (blocked_writer) {
			blocked_writer->wr_suspended_flag = false;

			if (blocked_writer->rd_suspended_flag == false) {
				os_task *blocked_t = blocked_writer;
				blocked_writer->task_state = READY;
				OS->READY_TASKS.push_back(blocked_writer);
				blocked_writer = 0;

				if (blocked_t->get_priority() > temp_t->get_priority()) {
					temp_t->task_state = READY;
					OS->READY_TASKS.push_back(temp_t);
					OS->schedule();
					temp_t->wait_for_activation();
				}
			}
		}
	} // end read
};
	
