#pragma once
#include "MetaData.h"
#include "Waveforms.h"
#include "Cycles.h"
#include "Status.h"
#include "Frequency.h"
#include "Amplitude.h"
#include "Message.h"
#include "Sender.h"

namespace BMW
{
	class Coil
	{
		State state_{};
		Waveform wave_{};
		Ampl ampl_{};
		Cycles cycles_{};
		Frequency freq_{};
		Meta const& meta_;

		bool isEnergyModulator() const;
	public:
		Coil(Meta const& meta);
		bool SetAmplitude(double const input);
		bool SetWave(WaveformType const input);
		bool SetFrequency(double const input);
		bool SetCycles(std::size_t const input);
	};

};
/*
extern "C"
{
	typedef void* BMWCoil;
	BMWCoil* CreateListOfHallACoils(std::size_t* items);
	DestroyListOfCoil(BMWCoil* coils);
	std::size_t GetCoilMemSize();
	void AttachBMWReceiver(BMWCoil coil, BMWReceiver receiver);
	void SetBMWAmplitude(BMWCoil coil, double input);
	void SetBMWAmplitude(BMWCoil coil, double const input);
	void SetBMWWave(BMWCoil coil, WaveformType const input);
	void SetBMWFrequency(BMWCoil coil, double const input);
	void SetBMWCycles(BMWCoil coil, std::size_t const input);

	void GetCoilName(BMWCoil coil);
	void GetCoilId(BMWCoil coil);
};
*/
