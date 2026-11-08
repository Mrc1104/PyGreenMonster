#pragma once
#include <algorithm>
#include <memory>
namespace BMW
{
	enum class WaveformType
	{
		Sine,
		Ramp
	};

	class Cycles
	{
	public:
		Cycles();
		Cycles(std::size_t input);
		void Set(std::size_t input);
		std::size_t Get() const;
		// Finite: [1,510]
		// Infinite: 511
		static constexpr std::size_t lower_bound_{1};
		static constexpr std::size_t upper_bound_{511};
	private:
		std::size_t cycles_{lower_bound_};
	};

	class Frequency
	{
	public:
		// Can the frequency be floating-point?
		// Or do they have to be integral values?
		using dataType = unsigned;
		Frequency();
		explicit Frequency(dataType input);
		void Set(dataType input);
		dataType Get() const;
		static constexpr dataType lower_bound_{10};
		static constexpr dataType upper_bound_{250};
	private:
		dataType value_{lower_bound_}; // Units: [Hz]
	};

	class Amplitude
	{
	public:
		using dataType = double;
		virtual ~Amplitude() = default;
		virtual std::unique_ptr<Amplitude> Clone() const = 0;
		virtual void Set(dataType input) = 0;
		virtual dataType Get() const = 0;
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
		static constexpr double lower_bound_ = -0.300;
		static constexpr double upper_bound_ = +0.300;
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
		WaveAmplitude() = default;
		std::unique_ptr<Amplitude> Clone() const override;
		explicit WaveAmplitude(dataType input);
		void Set(dataType input) override;
		dataType Get() const override;
		static constexpr double lower_bound_ = Bounds::lower_bound_;
		static constexpr double upper_bound_ = Bounds::upper_bound_;
	private:
		dataType value_{Bounds::lower_bound_};
	};
	class Ampl : public Amplitude
	{
		// Look into placement new
		std::unique_ptr<Amplitude> ptr_;
	public:
		using dataType = Amplitude::dataType;
		Ampl();
		template<typename T> Ampl(T&& ampl);
		std::unique_ptr<Amplitude> Clone() const;
		Ampl(Ampl const& other);
		Ampl& operator=(Ampl const& other);
		Ampl(Ampl && other) noexcept = default;
		Ampl& operator=(Ampl && other) noexcept = default;
		void Set(dataType input) override;
		dataType Get() const override;
	};
}; // namespace BMW


template<typename Bounds>
std::unique_ptr<BMW::Amplitude> BMW::WaveAmplitude<Bounds>::Clone() const
{
	return std::make_unique<WaveAmplitude<Bounds>>( *this );
}

template<typename Bounds>
BMW::WaveAmplitude<Bounds>::WaveAmplitude(dataType input)
{
	Set(input);
}

template<typename Bounds>
void BMW::WaveAmplitude<Bounds>::Set(dataType input)
{
	value_ = std::clamp(input, Bounds::lower_bound_, Bounds::upper_bound_);
}
template<typename Bounds>
typename BMW::WaveAmplitude<Bounds>::dataType BMW::WaveAmplitude<Bounds>::Get() const { return value_; }

template<typename T>
BMW::Ampl::Ampl(T&& ampl) : ptr_(std::make_unique<std::decay_t<T>>(std::forward<T>(ampl))) { }


using RampAmplitude = BMW::WaveAmplitude<BMW::RampBounds>;
using SineAmplitude = BMW::WaveAmplitude<BMW::SineBounds>;
using RFVernierAmplitude = BMW::WaveAmplitude<BMW::RFVernierBounds>;
