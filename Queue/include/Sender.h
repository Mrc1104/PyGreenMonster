#pragma once

#include "Queue.h"

template<typename T>
class Sender
{
	ThQueue<T>* q;
public:
	Sender();
	explicit Sender(ThQueue<T>* q);
	void send(T const& msg);
	void attach(ThQueue<T>* q);
};

template<typename T>
Sender<T>::Sender() : q{nullptr}{}
template<typename T>
Sender<T>::Sender(ThQueue<T>* src) : q(src) {}
template<typename T>
void Sender<T>::send(T const& msg)
{
	if(q) {
		q->push(msg);
	}
}
template<typename T>
void Sender<T>::attach(ThQueue<T>* src)
{
	q = src;
}

