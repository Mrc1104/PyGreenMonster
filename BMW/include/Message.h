#pragma once
#include "Queue.h"
extern "C" {
	enum BMWMesageType
	{
		Status = 0,
		Cycles,
		SineWave,
		RampWave,
		Ampl,
		Freq,
		Error
	};

	struct BMWMessage
	{
		int id;
		BMWMesageType type;
		void* data;
	};

	typedef void* BMWReceiver;
};
/*
namespace BWM
{
	class Messenger
	{
		Receiver<BMWMessage> receiver_;
	public:
		template<
		SendMessage(int id, BMWMesageType, 
		BMWMessage ReadMessage
	};

};*/
