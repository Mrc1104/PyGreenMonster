#include "Queue.h"
#include "Sender.h"
#include "Receiver.h"
#include <iostream>

int main()
{
	ThQueue<int> queue;
	for(int i = 0; i < 5; i++) queue.push(new int(i*i));
	do {
		std::cout << "Press 'q' to quit\n";
		if(int* p = queue.try_pop()) {
			std::cout << "Received value: " << *p << '\n';
			delete p;
		} else {
			std::cout << "No value available...\n";
		}
	}while(getchar() != 'q');
	Receiver<int> rec;
	Sender<int> send(rec);

	send.send(new int(5));
	if(auto ptr = rec.try_pop()) {
		std::cout << "Receiver received: " << *ptr << '\n';
		delete ptr;
	}
	
	return 0;
}
