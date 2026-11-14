#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include "Queue.h"
#include "Message.h"

std::atomic<bool> is_done{false};
int constexpr N = 5;
int    ivals[N] = {1, 2, 3, 4, 5};
double dvals[N] = {1.01, 2.02, 3.03, 4.04, 5.05};
char const*  svals[N] = {"Ryan", "Kristin", "Carter", "Bennet", "Russell"};


void print(Message& msg)
{
    switch (msg.type) {
        case MessageType::Int:
			std::cout << "Received int msg: " << msg.payload.int_msg << '\n';
            break;
        case MessageType::Double:
			std::cout << "Received dbl msg: " << msg.payload.dbl_msg << '\n';
            break;
        case MessageType::String:
			std::cout << "Received str msg: " << msg.payload.str_msg << '\n';
            break;
    }
}

void writer(threadsafe_queue<Message>* q)
{
	int counter = 0;
	while(is_done.load() == false) {
		int sel = counter % 3;
		int idx = counter % N;
		if(sel == 0) q->push(CreateMessage(ivals[idx]));
		if(sel == 1) q->push(CreateMessage(dvals[idx]));
		if(sel == 2) q->push(CreateMessage(svals[idx]));
		counter++;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void reader(threadsafe_queue<Message>* q)
{
	while(is_done.load() == false) {
		auto msg = q->wait_and_pop();
		print(msg);
		DeleteMessage(msg);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main()
{
	std::cout << "Starting test\n";
	std::cout << "Enter 'q' to quit...\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));

	threadsafe_queue<Message> q;
	std::thread writer_th(writer, &q);
	std::thread reader_th(reader, &q);

	while(getchar() != 'q') {
		std::cout << "Enter 'q' to quit...\n";
	}
	is_done.store(true);

	writer_th.join();
	reader_th.join();
	return 0;
}

