#pragma once
#include <string_view>
#include "QwEpicHandler.h"
#include "QwEpicChannel.h"

enum WaveformType
{
	Sine = 0,
	Ramp
};

namespace BMW
{
	template<WaveformType WaveType>
	struct WaveformTypeTraits;

	template<>
	struct WaveformTypeTraits<WaveformType::Sine>
	{
		static constexpr std::string_view EPIC_PV_SUFFIX{":SINEWAVE"};
		static constexpr int ENABLE  = 1;
		static constexpr int DISABLE = 0;
	};
	template<>
	struct WaveformTypeTraits<WaveformType::Ramp>
	{
		static constexpr std::string_view EPIC_PV_SUFFIX{":RAMPWAVE"};
		static constexpr int ENABLE  = 1;
		static constexpr int DISABLE = 0;
	};
	
	template<WaveformType Type>
	class ConcreteWaveform
	{
		EpicChannel* channel_{nullptr};
	public:
		ConcreteWaveform() = default;
		ConcreteWaveform(ConcreteWaveform const&) = default;
		ConcreteWaveform& operator=(ConcreteWaveform const&) = default;
		ConcreteWaveform(std::string_view pv);
		void Attach(std::string_view pv);
		bool Enable();
		bool Disable();
		// if input == Type, enable
		// else, disable
		bool Toggle(WaveformType input);
	};

	class Waveform
	{
		template <WaveformType T> using waveType = ConcreteWaveform<T>;
		waveType<WaveformType::Sine> sine_;
		waveType<WaveformType::Ramp> ramp_;
		WaveformType active_;
	public:
		Waveform() = default;
		Waveform(std::string_view pv);
		void Attach(std::string_view pv);
		void Set(WaveformType input);
		bool SetAndBroadcast(WaveformType input);
		bool Broadcast();
		WaveformType Get() const;
	};


	template<WaveformType Type>
	ConcreteWaveform<Type>::ConcreteWaveform(std::string_view pv)
	{
		Attach(pv);	
	}
	template<WaveformType Type>
	void ConcreteWaveform<Type>::Attach(std::string_view pv)
	{
		auto constexpr EPIC_PV_SUFFIX = WaveformTypeTraits<Type>::EPIC_PV_SUFFIX;
		std::string suffix{};
		if(pv.find(EPIC_PV_SUFFIX) == std::string_view::npos) {
			suffix = std::string(EPIC_PV_SUFFIX);
		}
		std::string null_term_string = std::string(pv) + suffix;
		channel_ = EpicHandler::Instance().ConnectChannel(null_term_string.c_str());
	}

	template<WaveformType Type>
	bool ConcreteWaveform<Type>::Enable()
	{
		if(!channel_) {
			return false;
		}
		auto constexpr ENABLE = WaveformTypeTraits<Type>::ENABLE;
		auto fut = channel_->PutAsync(ENABLE);
		try {
			fut.get();
		} catch(std::runtime_error const& e) {
			std::cerr << "Error Waveform Type: " << e.what() << '\n';
			return false;
		}
		// TODO: Send to Remex
		return true;
	}

	template<WaveformType Type>
	bool ConcreteWaveform<Type>::Disable()
	{
		if(!channel_) {
			return false;
		}
		auto constexpr DISABLE = WaveformTypeTraits<Type>::DISABLE;
		auto fut = channel_->PutAsync(DISABLE);
		try {
			fut.get();
		} catch(std::runtime_error const& e) {
			std::cerr << "Error Waveform Type: " << e.what() << '\n';
			return false;
		}
		// TODO: Send to Remex
		return true;
	}
	template<WaveformType Type>
	bool ConcreteWaveform<Type>::Toggle(WaveformType input)
	{
		return (input == Type) ? Enable() : Disable();
	}
};
