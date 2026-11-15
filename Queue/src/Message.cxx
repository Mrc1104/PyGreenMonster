#include "Message.h"
#include <iostream>

void DeleteMessage(Message& msg) noexcept {
	std::cout << "Deleting msg\n";
    switch (msg.type) {
        case MessageType::Int:
            MessageTypeTraits<int>::Destroy(msg.payload);
            break;
        case MessageType::Double:
            MessageTypeTraits<double>::Destroy(msg.payload);
            break;
        case MessageType::String:
            MessageTypeTraits<char*>::Destroy(msg.payload);
            break;
    }
	return;
}

Message CreateIntMsg(int msg)          {std::cout << "Creating int msg\n";  return CreateMessage(msg); }
Message CreateDblMsg(double msg)       {std::cout << "Creating dbl msg\n";  return CreateMessage(msg); }
Message CreateStrMsg(const char* msg)  {std::cout << "Creating str msg\n";  return CreateMessage(msg); }
void DeleteMessage(Message* msg)       {std::cout << "Deleting msg\n";      DeleteMessage(*msg);   }

void PrintMessage(Message* msg)
{
    switch (msg->type) {
        case MessageType::Int:
			std::cout << "Received int msg: " << msg->payload.int_msg << '\n';
            break;
        case MessageType::Double:
			std::cout << "Received dbl msg: " << msg->payload.dbl_msg << '\n';
            break;
        case MessageType::String:
			std::cout << "Received str msg: " << msg->payload.str_msg << '\n';
            break;
    }
}
