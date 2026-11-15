#pragma once
#include "InterruptThread.h"
#include "BMWMessages.h"
#include "Receiver.h"
#include "Sender.h"
namespace BMW
{

	class Manager
	{
		// How We communicate with Python
		Receiver<BMWMsg> outgoing_; // Python receives msgs with this
		Sender<BMWMsg>   sender_;   // We send Python Msgs with this
		
		// How Python Communicates with us
		Receiver<BMWMsg> incoming_;        // we receve msgs with this
		Sender<BMWMsg>   incoming_sender_; // Python sends us msgs with this

		// Worker thread
		InterruptThread worker_;

		Manager();
		~Manager();
	public:
		static Manager& Instance();
		Manager(Manager const&) = delete;
		Manager& operator=(Manager const&) = delete;
		Manager(Manager&&) = delete;
		Manager& operator=(Manager&&) = delete;

		Receiver<BMWMsg>* GetOutgoing();
		Sender<BMWMsg>*   GetIncoming();
		void CleanUp();
	};
};

extern "C"
{
	void BMWShutdown();
	typedef void* BMWReceiver;
	BMWReceiver	GetBMWReceiver();
	bool CheckForMsg(BMWReceiver receiver, BMWMsg* msg);
		

	typedef void* BMWSender;
	BMWSender GetBMWSender();
	void SendMsg(BMWSender sender, BMWMsg* msg);
};;
