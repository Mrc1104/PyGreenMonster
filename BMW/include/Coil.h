#pragma once
#include "MetaData.h"
#include "Waveforms.h"
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
		Cycles cycles_{};
		Frequency freq_{};
		Meta const& meta_;

		bool isEnergyModulator() const;
	public:
		Coil(Meta const& meta, Sender<Message> sender);
		void SetAmplitude(double const input);
		void SetWave(WaveformType const input);
		void SetFrequency(double const input);
		void SetCycles(std::size_t const input);
	};

};
