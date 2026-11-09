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

		Status GetStatus() const;
	};

};
