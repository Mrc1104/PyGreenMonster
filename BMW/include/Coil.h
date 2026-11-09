#pragma once
#include "MetaData.h"
#include "Waveforms.h"
#include "Cycles.h"
#include "Status.h"
#include "Message.h"
#include "Sender.h"

namespace BMW
{
	class Coil
	{
		State state_{};
		WaveformType wave_type_{WaveformType::Sine};
		Ampl ampl_{SineAmplitude{}};
		Cycles cycles_;
		Frequency freq_{};
		Meta const& meta_;

		bool isEnergyModulator() const;
	public:
		Coil(Meta const& meta);
		void SetAmplitude(double const input);
		void SetWave(WaveformType const input);
		void SetFrequency(double const input);
		void SetCycles(std::size_t const input);
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
