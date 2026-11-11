#pragma once
#include <cstddef>
#include "Waveforms.h"
#include "Status.h"
extern "C"
{
	typedef void* BMWCoil;
	BMWCoil CreateListOfHallACoils(std::size_t* items);
	void DestroyListOfCoil(BMWCoil* coils);
	std::size_t GetCoilMemSize();
	bool SetBMWAmplitude(BMWCoil coil, double input);
	bool SetBMWWave(BMWCoil coil, WaveformType const input);
	bool SetBMWFrequency(BMWCoil coil, double const input);
	bool SetBMWCycles(BMWCoil coil, int const input);


	const char* GetCoilName(BMWCoil coil);
	int GetCoilId(BMWCoil coil);
	Modtype GetModType(BMWCoil coil);
	Status GetStatus(BMWCoil coil);
};

