#include "Interrupt.h"

interrupt_flag::interrupt_flag()
: thread_cond{nullptr}
, thread_cond_any{nullptr}
{}

void interrupt_flag::set()
{
	flag.store(true, std::memory_order_relaxed);
	std::lock_guard lk(set_clear_mutex);
	if(thread_cond) {
		thread_cond->notify_all();
	} else if(thread_cond_any) {
		thread_cond_any->notify_all();
	}
}

bool interrupt_flag::is_set() { return flag.load(std::memory_order_relaxed); }

void interrupt_flag::set_condition_variable(std::condition_variable& cv)
{
	std::lock_guard lk(set_clear_mutex);
	thread_cond = &cv;
}
	
void interrupt_flag::clear_condition_variable()
{
	std::lock_guard lk(set_clear_mutex);
	thread_cond = nullptr;
}

interrupt_flag::clear_cv_on_destruct::~clear_cv_on_destruct()
{
	this_thread_interrupt_flag.clear_condition_variable();
}
