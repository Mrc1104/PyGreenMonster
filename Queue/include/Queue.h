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
	std::queue<T*> data_queue;
	std::condition_variable data_cond;
public:
	~ThQueue();
	void push(T* value);
	T* wait_and_pop();
	T* try_pop();
	bool empty() const;
};

template<typename T>
void ThQueue<T>::push(T* value)
{
	std::lock_guard lk(mut);
	data_queue.push(value);
	data_cond.notify_one();
}

template<typename T>
T* ThQueue<T>::wait_and_pop()
{
	std::unique_lock lk(mut);
	data_cond.wait(lk, [this]{ return !data_queue.empty(); });
	auto res = data_queue.front();
	data_queue.pop();
	return res;
}

template<typename T>
T* ThQueue<T>::try_pop()
{
	std::lock_guard lk(mut);
	if(data_queue.empty()) return nullptr;
	auto res = data_queue.front();
	data_queue.pop();
	return res;
}

template<typename T>
bool ThQueue<T>::empty() const
{
	std::lock_guard lk(mut);
	return data_queue.empty();

}

template<typename T>
ThQueue<T>::~ThQueue()
{
	std::lock_guard lk(mut);
	while(!data_queue.empty()) {
		auto res = data_queue.front();
		delete res;
		data_queue.pop();
	}
}

