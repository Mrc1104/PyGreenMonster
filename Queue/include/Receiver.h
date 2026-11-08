#pragma once
#include "Queue.h"
#include "Sender.h"
#include <memory>

template<typename T>
class Receiver
{
	ThQueue<T> q;
public:
	T* try_pop();
	operator Sender<T>();

};

template<typename T>
Receiver<T>::operator Sender<T>()
{
	return Sender<T>(&q);
}

template<typename T>
T* Receiver<T>::try_pop()
{
	return q.try_pop();
}
