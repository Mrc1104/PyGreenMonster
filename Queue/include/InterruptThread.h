#pragma once
#include "Interrupt.h"
#include <type_traits>

class InterruptThread
{
	std::thread internal_thread;
	interrupt_flag* flag;
public:
	InterruptThread() noexcept = default;
	template<typename FunctionType,
			 typename... Args,
			 typename = std::enable_if_t<!std::is_same_v<std::decay_t<FunctionType>,InterruptThread>>>
	InterruptThread(FunctionType&& f, Args&&... args);
	InterruptThread(InterruptThread const&) = delete;
	InterruptThread& operator=(InterruptThread const&) = delete;
	InterruptThread(InterruptThread&& other) noexcept;
	InterruptThread& operator=(InterruptThread&& other) noexcept;

	void join() noexcept;
	void detach();
	bool joinable() const;
	void interrupt();

};

template<typename FunctionType, typename... Args, typename>
InterruptThread::InterruptThread(FunctionType&& f, Args&&... args)
{
	std::promise<interrupt_flag*> p;
	internal_thread = std::thread([fn = std::forward<FunctionType>(f), &p, tup = std::make_tuple(std::forward<Args>(args)...)]()mutable{
		p.set_value(&this_thread_interrupt_flag);
		try {
		std::apply(std::move(fn), std::move(tup));
		} catch( thread_interrupted const&) {}
	});
	flag = p.get_future().get();
}
