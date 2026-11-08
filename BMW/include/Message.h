#pragma once
#include "Queue.h"
namespace BMW
{
	enum MesageType
	{
		Status,
		Cycles,
		SineWave,
		RampWave,
		Ampl,
		Freq,
		Error
	};

	struct Message
	{
		int id;
		MesageType type;
		void* data;
	};
};
