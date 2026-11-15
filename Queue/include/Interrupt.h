#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <future>
#include <stdexcept>

struct thread_interrupted : std::exception {};

class interrupt_flag
{
	std::atomic<bool> flag;
	std::condition_variable* thread_cond;
	std::condition_variable_any* thread_cond_any;
	std::mutex set_clear_mutex;
public:
	interrupt_flag();
	void set();
	bool is_set();
	void set_condition_variable(std::condition_variable& cv);
	void clear_condition_variable();

	template<typename Lockable>
	void wait(std::condition_variable_any& cv, Lockable& lk);

	struct clear_cv_on_destruct
	{
		~clear_cv_on_destruct();
	};
};

inline thread_local interrupt_flag this_thread_interrupt_flag;

inline void interruption_point()
{
	if(this_thread_interrupt_flag.is_set())
	{
		throw thread_interrupted();
	}
};

template<typename Lockable>
void interruptible_wait(std::condition_variable_any& cv, Lockable& lk)
{
	this_thread_interrupt_flag.wait(cv, lk);
}

template<typename Lockable, typename Predicate>
void interruptible_wait(std::condition_variable_any& cv, Lockable& lk, Predicate pred)
{
	interruption_point();
	while(!pred()) {
		interruptible_wait(cv, lk);
	}
}




template<typename Lockable>
void interrupt_flag::wait(std::condition_variable_any& cv, Lockable& lk)
{
	struct custom_lock {
		interrupt_flag* self;
		Lockable& lk;
		custom_lock(interrupt_flag* self_,
				    std::condition_variable_any& cond,
					Lockable& lk_) : self(self_), lk(lk_)
		{
			self->set_clear_mutex.lock();
			self->thread_cond_any=&cond;
		}
		void unlock()
		{
			lk.unlock();
			self->set_clear_mutex.unlock();
		}
		void lock()
		{
			std::lock(self->set_clear_mutex, lk);
		}
		~custom_lock()
		{
			self->thread_cond_any = nullptr;
			self->set_clear_mutex.unlock();
		}
	};
	custom_lock cl(this, cv, lk);
	interruption_point();
	cv.wait(cl);
	interruption_point();
}

