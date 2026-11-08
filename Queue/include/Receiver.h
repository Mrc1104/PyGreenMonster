#pragma once
#include "Queue.h"
#include "Sender.h"
#include <memory>

template<typename T>
class Receiver
{
	ThQueue<T> q;
public:
	bool try_pop(T& value);
	std::shared_ptr<T> try_pop();
	operator Sender<T>();

};

template<typename T>
Receiver<T>::operator Sender<T>()
{
	return Sender<T>(&q);
}

template<typename T>
bool Receiver<T>::try_pop(T& value)
{
	return q.try_pop(value);
}
template<typename T>
std::shared_ptr<T> Receiver<T>::try_pop()
{
	return q.try_pop();
}
