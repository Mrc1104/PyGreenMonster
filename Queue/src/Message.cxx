#include "Message.h"

void DeleteMessage(Message& msg) noexcept {
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

Message CreateIntMsg(int msg)          { return CreateMessage(msg); }
Message CreateDblMsg(double msg)       { return CreateMessage(msg); }
Message CreateStrMsg(const char* msg)  { return CreateMessage(msg); }
void DeleteMessage(Message* msg)       {     DeleteMessage(*msg);   }
