#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <type_traits>
#include "Interrupt.h"

template<typename T>
class threadsafe_queue
{
	static_assert(std::is_trivially_copyable_v<T>,
			"This queue is only for trivially copyable objects!");
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable_any data_cond;
public:
	threadsafe_queue() = default;
	void push(T val);
	T wait_and_pop();
	bool try_and_pop(T& val);
	bool empty() const;
};
template<typename T>
void threadsafe_queue<T>::push(T val)
{
	std::lock_guard lk(mut);
	data_queue.push(val);
	data_cond.notify_one();
}

template<typename T>
T threadsafe_queue<T>::wait_and_pop()
{
	std::unique_lock lk(mut);
	interruptible_wait(data_cond, lk, [this]{ return !data_queue.empty(); });
	auto res = data_queue.front();
	data_queue.pop();
	return res;
}

template<typename T>
bool threadsafe_queue<T>::try_and_pop(T& val)
{
	std::lock_guard lk(mut);
	if(data_queue.empty()) return false;
	val = data_queue.front();
	data_queue.pop();
	return true;
}

template<typename T>
bool threadsafe_queue<T>::empty() const
{
	std::lock_guard lk(mut);
	return data_queue.empty();
}

