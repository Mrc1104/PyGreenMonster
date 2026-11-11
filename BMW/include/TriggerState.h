#pragma once
#include <string_view>
#include <iostream>
#include "QwEpicChannel.h"
#include "QwEpicHandler.h"

enum TrigState
{
	BMWEnable,
	BMWDisable
};
namespace BMW
{
	template<TrigState State>
	struct TrigStateTraits;

	template<>
	struct TrigStateTraits<TrigState::BMWEnable>
	{
		static constexpr std::string_view EPIC_PV_SUFFIX{":TRIGSTATEBTN"};
		static constexpr int ENABLE  = 1;
		static constexpr int DISABLE = 0;
	};
	template<>
	struct TrigStateTraits<TrigState::BMWDisable>
	{
		static constexpr std::string_view EPIC_PV_SUFFIX{":STOPBTN"};
		static constexpr int ENABLE  = 1;
		static constexpr int DISABLE = 0;
	};
	template<TrigState State>
	class ConcreteTrigState
	{
		EpicChannel* channel_{nullptr};
	public:
		ConcreteTrigState() = default;
		ConcreteTrigState(ConcreteTrigState const&) = default;
		ConcreteTrigState& operator=(ConcreteTrigState const&) = default;
		ConcreteTrigState(std::string_view pv);
		void Attach(std::string_view pv);
		bool Enable();
		bool Disable();
		// if input == State, enable
		// else, disable
		bool Toggle(TrigState input);
	};


	class TriggerState
	{
		template <TrigState T> using state = ConcreteTrigState<T>;
		state<TrigState::BMWEnable>  enable_;
		state<TrigState::BMWDisable> disable_;
		TrigState active_;
	public:
		TriggerState() = default;
		TriggerState(std::string_view pv);
		void Attach(std::string_view pv);
		void Set(TrigState input);
		bool SetAndBroadcast(TrigState input);
		bool Broadcast();
		TrigState Get() const;
	};


	template<TrigState State>
	ConcreteTrigState<State>::ConcreteTrigState(std::string_view pv)
	{
		Attach(pv);	
	}
	template<TrigState State>
	void ConcreteTrigState<State>::Attach(std::string_view pv)
	{
		auto constexpr EPIC_PV_SUFFIX = TrigStateTraits<State>::EPIC_PV_SUFFIX;
		std::string suffix{};
		if(pv.find(EPIC_PV_SUFFIX) == std::string_view::npos) {
			suffix = std::string(EPIC_PV_SUFFIX);
		}
		std::string null_term_string = std::string(pv) + suffix;
		channel_ = EpicHandler::Instance().ConnectChannel(null_term_string.c_str());
	}

	template<TrigState State>
	bool ConcreteTrigState<State>::Enable()
	{
		if(!channel_) {
			return false;
		}
		auto constexpr ENABLE = TrigStateTraits<State>::ENABLE;
		auto fut = channel_->PutAsync(ENABLE);
		try {
			fut.get();
		} catch(std::runtime_error const& e) {
			std::cerr << "Error Trigger Enable: " << e.what() << '\n';
			return false;
		}
		// TODO: Send to Remex
		return true;
	}

	template<TrigState State>
	bool ConcreteTrigState<State>::Disable()
	{
		if(!channel_) {
			return false;
		}
		auto constexpr DISABLE = TrigStateTraits<State>::DISABLE;
		auto fut = channel_->PutAsync(DISABLE);
		try {
			fut.get();
		} catch(std::runtime_error const& e) {
			std::cerr << "Error Trigger Enable: " << e.what() << '\n';
			return false;
		}
		// TODO: Send to Remex
		return true;
	}
	template<TrigState State>
	bool ConcreteTrigState<State>::Toggle(TrigState input)
	{
		return (input == State) ? Enable() : Disable();
	}

};
