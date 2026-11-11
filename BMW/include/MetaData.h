#pragma once
#include <array>
#include <ranges>
#include <string>
#include <iostream>
using namespace std::string_view_literals;

enum Modtype { BMWCoil=0, BMWEnergy, BMWEmpty };
namespace BMW {
enum class Hall	{A,	C};
enum class TrimCard { BMOD1, BMOD2 };
enum class TrimChan { CHAN0, CHAN1, CHAN2, CHAN3 };
enum class EPIVS_PV { WAVESTATE, FREQUENCY, AMPLITUDE, SINEWAVE, RAMPWAVE, RCRINPUT, TRIGSTATEBTN, TRIGBTN, STOPBTN };
struct Name {
	const char* name_;
	explicit consteval Name(const char* name) : name_(name) {}
	const char* Get() const { return name_; }
};
struct Descr {
	const char* descr_;
	explicit consteval Descr(const char* descr) : descr_(descr) {}
	const char* Get() const { return descr_; }
};
struct ID {
	int id_;
	constexpr explicit ID(int id) : id_{id} {}
	int Get() const { return id_; }
};
struct Meta
{
	Name name_;
	ID id_;
	TrimCard card_;
	TrimChan chan_;
	Hall hall_;
	Modtype mod_;
	Descr descr_;

	std::string GetPV() const;
};

class MetaData
{
	static constexpr std::size_t N_CHANNELS = 16;
	static const std::array<Meta, N_CHANNELS> meta_;
	MetaData() = default;
	MetaData(MetaData const&) = delete;
	MetaData& operator=(MetaData const&) = delete;
	MetaData(MetaData &&) = delete;
	MetaData& operator=(MetaData &&) = delete;
public:
	static MetaData const& Instance();
	template<typename Predicate>
	[[nodiscard]] auto Get(Predicate&& predicate) const;
	[[nodiscard]] auto GetHallA() const;
	[[nodiscard]] auto GetHallC() const;
};


template<typename Predicate>
auto MetaData::Get(Predicate&& predicate) const
{
	return meta_ | std::ranges::views::filter(std::forward<Predicate>(predicate));
}

inline auto MetaData::GetHallA() const
{
	return meta_ | std::ranges::views::filter([](Meta const& meta) {
		return (meta.hall_ == Hall::A) && (meta.mod_ != Modtype::BMWEmpty);
	});
}

inline auto MetaData::GetHallC() const
{
	return meta_ | std::ranges::views::filter([](Meta const& meta) {
		return (meta.hall_ == Hall::C) && (meta.mod_ != Modtype::BMWEmpty);
	});
}

std::string stringify(TrimCard card);
std::string stringify(TrimChan chan);

}; //namespace BMW
