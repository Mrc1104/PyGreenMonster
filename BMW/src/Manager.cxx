#include "Manager.h"

using namespace BMW;

Manager::Manager() : outgoing_{}, sender_(outgoing_), incoming_{}, incoming_sender_(incoming_)
{
	worker_  = InterruptThread([this]{
		while(true) {
			auto msg = this->incoming_.wait_and_pop();
			PrintBMWMsg(&msg);
			DeleteBMWMessage(&msg);
		}
	});
 
}

// How to prevent leaking memory from the queues...?
Manager::~Manager() {
	CleanUp();
}

Manager& Manager::Instance()
{
	static Manager instance;
	return instance;
}

Receiver<BMWMsg>* Manager::GetOutgoing() { return &outgoing_; }
Sender<BMWMsg>*    Manager::GetIncoming() { return &incoming_sender_; }

void Manager::CleanUp()
{
	worker_.interrupt();
	if(worker_.joinable()) worker_.join();
}

//////////////////////////////////////
//// C API
//////////////////////////////////////
void BMWShutdown() { Manager::Instance().CleanUp(); }
BMWReceiver GetBMWReceiver() { return Manager::Instance().GetOutgoing(); }
BMWSender GetBMWSender()     { return Manager::Instance().GetIncoming(); }

bool CheckForMsg(BMWReceiver receiver, BMWMsg* msg)
{
	auto* receiv = static_cast<Receiver<BMWMsg>*>( receiver );
	return receiv->try_and_pop(*msg);
}

void SendMsg(BMWSender sender, BMWMsg* msg)
{
	auto* send = static_cast<Sender<BMWMsg>*>( sender );
	send->send(*msg);
}
