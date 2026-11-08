#include "Queue.h"
#include "Sender.h"
#include "Receiver.h"
#include <iostream>

int main()
{
	ThQueue<int> queue;
	for(int i = 0; i < 5; i++) queue.push(i*i);
	int val = 0;
	do {
		std::cout << "Press 'q' to quit\n";
		if(queue.try_pop(val)) {
			std::cout << "Received value: " << val << '\n';
		} else {
			std::cout << "No value available...\n";
		}
	}while(getchar() != 'q');
	Receiver<int> rec;
	Sender<int> send(rec);

	send.send(5);
	if(auto sptr = rec.try_pop()) {
		std::cout << "Receiver received: " << *sptr << '\n';
	}
	
	return 0;
}
