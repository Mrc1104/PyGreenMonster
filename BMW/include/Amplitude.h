#pragma once
#include "Waveforms.h"
#include "QwEpicChannel.h"
#include "QwEpicHandler.h"
#include <string_view>
#include <iostream>
#include <optional>

namespace BMW {
	class Amplitude
	{
	public:
		static constexpr std::string_view EPIC_PV_SUFFIX{":AMPLITUDE"};
		using dataType = double;
		virtual ~Amplitude() = default;
		virtual std::unique_ptr<Amplitude> Clone() const = 0;
		virtual void Attach(std::string_view pv) = 0;
		virtual void Set(dataType input) = 0;
		virtual dataType Get() const = 0;
		virtual bool Broadcast() = 0;
		virtual bool SetAndBroadcast(dataType input) = 0;
		virtual std::optional<dataType> GetRemote() const = 0;

	};

	struct RampBounds
	{
		// Units [V]
		static constexpr double lower_bound_ = 0.0;
		static constexpr double upper_bound_ = 1.0;
	};
	struct SineBounds
	{
		// Units [Amps]
		static constexpr double lower_bound_ = -0.500;
		static constexpr double upper_bound_ = +0.500;
	};
	struct RFVernierBounds
	{
		// Units [MeV]
		static constexpr double lower_bound_ = -0.178;
		static constexpr double upper_bound_ = +0.178;
	};


	template<typename Bounds>
	class WaveAmplitude : public Amplitude
	{
	public:
		using dataType = Amplitude::dataType;
		static constexpr dataType lower_bound_ = Bounds::lower_bound_;
		static constexpr dataType upper_bound_ = Bounds::upper_bound_;
		WaveAmplitude() = default;
		std::unique_ptr<Amplitude> Clone() const override;
		explicit WaveAmplitude(dataType input);
		WaveAmplitude(std::string_view pv, dataType input = lower_bound_);
		void Attach(std::string_view pv) override;
		void Set(dataType input) override;
		bool Broadcast() override;
		bool SetAndBroadcast(dataType input) override;
		dataType Get() const override;
		std::optional<dataType> GetRemote() const override;
	private:
		dataType value_{Bounds::lower_bound_};
		EpicChannel* channel_{nullptr};
	};
	class Ampl : public Amplitude
	{
		// Look into placement new
		std::unique_ptr<Amplitude> ptr_;
	public:
		using dataType = Amplitude::dataType;
		Ampl();
		Ampl(dataType input);
		Ampl(std::string_view pv, dataType input = dataType{});
		template<typename T> Ampl(T&& ampl);
		std::unique_ptr<Amplitude> Clone() const;
		Ampl(Ampl const& other);
		Ampl& operator=(Ampl const& other);
		Ampl(Ampl && other) noexcept = default;
		Ampl& operator=(Ampl && other) noexcept = default;
		void Attach(std::string_view pv) override;
		void Set(dataType input) override;
		dataType Get() const override;
		bool Broadcast() override;
		bool SetAndBroadcast(dataType input) override;
		std::optional<dataType> GetRemote() const override;
	};

	template<typename Bounds>
	std::unique_ptr<Amplitude> WaveAmplitude<Bounds>::Clone() const
	{
		return std::make_unique<WaveAmplitude<Bounds>>( *this );
	}
	
	template<typename Bounds>
	WaveAmplitude<Bounds>::WaveAmplitude(dataType input)
	{
		Set(input);
	}
	template<typename Bounds>
	WaveAmplitude<Bounds>::WaveAmplitude(std::string_view pv, dataType input)
	{
		Set(input);
		Attach(pv);
	}

	template<typename Bounds>
	void WaveAmplitude<Bounds>::Attach(std::string_view pv)
	{
		std::string suffix{};
		if(pv.find(EPIC_PV_SUFFIX) == std::string_view::npos) {
			suffix = std::string(EPIC_PV_SUFFIX);
		}
		std::string null_term_string = std::string(pv) + suffix;
		channel_ = EpicHandler::Instance().ConnectChannel(null_term_string.c_str());
	}
	
	template<typename Bounds>
	void WaveAmplitude<Bounds>::Set(dataType input)
	{
		value_ = std::clamp(input, Bounds::lower_bound_, Bounds::upper_bound_);
	}
	
	template<typename Bounds>
	bool WaveAmplitude<Bounds>::Broadcast()
	{
		if(!channel_) {
			return false;
		}
		auto fut = channel_->PutAsync(value_);
		try {
			fut.get();
		} catch(std::runtime_error const& e) {
			std::cerr << "Error Setting Amplitude: " << e.what() << '\n';
			return false;
		}
		// TODO: Send to Remex
		return true;
	}
	template<typename Bounds>
	bool WaveAmplitude<Bounds>::SetAndBroadcast(dataType input)
	{
		Set(input);
		return Broadcast();
	}

	template<typename Bounds>
	typename WaveAmplitude<Bounds>::dataType WaveAmplitude<Bounds>::Get() const { return value_; }

	template<typename Bounds>
	std::optional<typename WaveAmplitude<Bounds>::dataType> WaveAmplitude<Bounds>::GetRemote() const
	{
		if(!channel_) return std::nullopt;
		try {
			auto fut = channel_->GetAsync<int>();
			return std::optional<int>{fut.get()};
		} catch(std::runtime_error const& e) {
			std::cerr << "Error Setting Cycles: " << e.what() << '\n';
			return std::nullopt;
		}
	}


	template<typename T>
	Ampl::Ampl(T&& ampl) : ptr_(std::make_unique<std::decay_t<T>>(std::forward<T>(ampl))) { }


	using RampAmplitude = WaveAmplitude<RampBounds>;
	using SineAmplitude = WaveAmplitude<SineBounds>;
	using RFVernierAmplitude = WaveAmplitude<RFVernierBounds>;
}; // namespace BMW

