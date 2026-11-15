#include "BMWMessages.h"
#include "Coil.h"
#include <iostream>


BMWMsg CreateBMWMessage(void* coil, BMWMsgtype type, BMWMsgSubSys sys, Message msg)
{
	return BMWMsg{coil, type, sys, msg};
}

void DeleteBMWMessage(BMWMsg* msg)
{
	DeleteMessage(msg->msg);
}

void PrintBMWMsg(BMWMsg* msg)
{
	using namespace BMW;
	std::cout << "coil: " << static_cast<Coil*>( msg->coil )->GetName() << '\n';
	std::cout << "msg type: " << (msg->msg_type == Request ? "Request" : "Command") << '\n';
	std::cout << "msg sys:  " << msg->msg_sys << '\n';
	PrintMessage(&msg->msg);
}
