#pragma once
#include <array>
#include <ranges>
#include <iostream>
using namespace std::string_view_literals;
namespace BMW {

enum class Hall	{A,	C};
enum class TrimCard { BMOD1, BMOD2 };
enum class TrimChan { CHAN0, CHAN1, CHAN2, CHAN3 };
enum class EPIVS_PV { WAVESTATE, FREQUENCY, AMPLITUDE, SINEWAVE, RAMPWAVE, RCRINPUT, TRIGSTATEBTN, TRIGBTN, STOPBTN };
enum class Modtype { Coil, Energy, Empty };
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
};

class MetaData
{
	static constexpr std::size_t N_CHANNELS = 16;
	static const std::array<Meta, N_CHANNELS> meta_;
public:
	template<typename Predicate>
	[[nodiscard]] auto Get(Predicate&& predicate);
};


template<typename Predicate>
[[nodiscard]] auto MetaData::Get(Predicate&& predicate)
{
	return meta_ | std::ranges::views::filter(std::forward<Predicate>(predicate));
}


}; //namespace BMW
