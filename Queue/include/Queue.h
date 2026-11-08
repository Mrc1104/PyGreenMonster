#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>



template<typename T>
class ThQueue
{
private:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	void push(T value);
	void wait_and_pop(T& value);
	std::shared_ptr<T> wait_and_pop();
	bool try_pop(T& value);
	std::shared_ptr<T> try_pop();
	bool empty() const;
};

template<typename T>
void ThQueue<T>::push(T value)
{
	std::lock_guard lk(mut);
	data_queue.push(std::move(value));
	data_cond.notify_one();
}

template<typename T>
void ThQueue<T>::wait_and_pop(T& value)
{
	std::unique_lock lk(mut);
	data_cond.wait(lk, [this]{ return !data_queue.empty(); });
	value = std::move(data_queue.front());
	data_queue.pop();
}

template<typename T>
std::shared_ptr<T> ThQueue<T>::wait_and_pop()
{
	std::unique_lock lk(mut);
	data_cond.wait(lk, [this]{ return !data_queue.empty(); });
	try {
		auto res = std::make_shared<T>(std::move(data_queue.front()));
		data_queue.pop();
		return res;
	} catch(...) {
		data_cond.notify_one();
	}
}

template<typename T>
bool ThQueue<T>::try_pop(T& value)
{
	std::lock_guard lk(mut);
	if(data_queue.empty()) return false;
	value = std::move(data_queue.front());
	data_queue.pop();
	return true;
}

template<typename T>
std::shared_ptr<T> ThQueue<T>::try_pop()
{
	std::lock_guard lk(mut);
	if(data_queue.empty()) return std::shared_ptr<T>();
	try {
		auto res = std::make_shared<T>(std::move(data_queue.front()));
		data_queue.pop();
		return res;
	} catch(...) {
		return std::shared_ptr<T>();
	}
}

template<typename T>
bool ThQueue<T>::empty() const
{
	std::lock_guard lk(mut);
	return data_queue.empty();

}
