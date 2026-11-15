#pragma once
#include "Queue.h"
#include "Sender.h"

template<typename T>
class Receiver
{
	ThQueue<T> q;
public:
	bool try_and_pop(T& val);
	T wait_and_pop();
	operator Sender<T>();
};

template<typename T>
Receiver<T>::operator Sender<T>()
{
	return Sender<T>(&q);
}

template<typename T>
bool Receiver<T>::try_and_pop(T& val)
{
	return q.try_and_pop(val);
}

template<typename T>
T Receiver<T>::wait_and_pop()
{
	return q.wait_and_pop();
}
