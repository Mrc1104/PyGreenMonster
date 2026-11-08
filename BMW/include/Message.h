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
		MesageType type;
		void* data;
	};

	typedef void* BMWReceiver;
};

