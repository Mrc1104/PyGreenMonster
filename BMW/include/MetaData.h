#pragma once
#include <string>
#include <string_view>
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
	std::string_view name_;
	constexpr explicit Name(std::string_view name) : name_(name) {}
	std::string_view Get() const { return name_; }
};
struct Descr {
	std::string_view descr_;
	constexpr explicit Descr(std::string_view descr) : descr_(descr) {}
	operator std::string_view() const { return descr_; }
	std::string_view Get() const { return descr_; }
};
struct ID {
	std::size_t id_;
	constexpr explicit ID(std::size_t id) : id_{id} {}
	operator std::size_t() const { return id_; }
	std::size_t Get() const { return id_; }
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
