#pragma once
#include <cstddef>
#include "Waveforms.h"
#include "Status.h"
#include "TriggerState.h"
#include "MetaData.h"
extern "C"
{
	typedef void* BMWCoilObj;
	BMWCoilObj CreateListOfHallACoils(std::size_t* items);
	void DestroyListOfCoils(BMWCoilObj coils, std::size_t items);
	std::size_t GetCoilMemSize();
	bool SetBMWAmplitude(BMWCoilObj coil, double input);
	bool SetBMWWave(BMWCoilObj coil, WaveformType const input);
	bool SetBMWFrequency(BMWCoilObj coil, double const input);
	bool SetBMWCycles(BMWCoilObj coil, int const input);
	bool SetBMWTrigger(BMWCoilObj coil, TrigState const input);


	const char* GetCoilName(BMWCoilObj coil);
	int GetCoilId(BMWCoilObj coil);
	Modtype GetModType(BMWCoilObj coil);
	Status GetStatus(BMWCoilObj coil);

	bool EnableBeamMod();
	bool DisableBeamMod();
	bool isBeamModEnabled();

};

