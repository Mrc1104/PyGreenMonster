#pragma once
#include <cstddef>
#include "Waveforms.h"
#include "Status.h"
#include "MetaData.h"
extern "C"
{
	typedef void* BMWCoil;
	BMWCoil CreateListOfHallACoils(std::size_t* items);
	void DestroyListOfCoils(BMWCoil coils, std::size_t items);
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

