#include "InterruptThread.h"
#include <utility>

InterruptThread::InterruptThread(InterruptThread&& other) noexcept
: internal_thread(std::move(other.internal_thread))
, flag(std::exchange(other.flag, nullptr))
{ }

InterruptThread& InterruptThread::operator=(InterruptThread&& other) noexcept
{
	if(this == &other) return *this;
	if(joinable()) std::terminate();
	internal_thread = std::move(other.internal_thread);
	flag = std::exchange(other.flag, nullptr);
	return *this;
}


void InterruptThread::interrupt()
{
	if(flag) {
		flag->set();
	}
}
void InterruptThread::join() noexcept {
	internal_thread.join();
	flag = nullptr;
}
void InterruptThread::detach() {
	internal_thread.detach();
	flag = nullptr;
}
bool InterruptThread::joinable() const
{
	return internal_thread.joinable();
}
