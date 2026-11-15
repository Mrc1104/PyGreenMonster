#pragma once
#include <type_traits>
#include <string.h> // ::strdup
#include <cstdlib>  // std::free
extern "C" {
	enum MessageType
	{
		Int    = 0,
		Double = 1,
		String = 3
	};
	union MessagePayload {
		int    int_msg;
		double dbl_msg;
		char*  str_msg;
	};
	struct Message
	{
		MessageType    type;
		MessagePayload payload;
	};
};


template<typename T>
struct MessageTypeTraits;

template<>
struct MessageTypeTraits<int> {
	static auto constexpr MsgType = MessageType::Int;
	using PayloadType = int;
	static constexpr MessagePayload Create(PayloadType input) noexcept { return MessagePayload{.int_msg = input}; }
	static constexpr void   Destroy(MessagePayload&) noexcept {  }

};
template<>
struct MessageTypeTraits<double> {
	static auto constexpr MsgType = MessageType::Double;
	using PayloadType = double;
	static constexpr MessagePayload Create(PayloadType input) noexcept { return MessagePayload{.dbl_msg = input}; }
	static constexpr void   Destroy(MessagePayload&) noexcept {  }
};

template<>
struct MessageTypeTraits<const char*> {
	static auto constexpr MsgType = MessageType::String;
	using PayloadType = char*;
	static MessagePayload Create(const char* input) {
		return MessagePayload{ .str_msg = input ? ::strdup(input) : nullptr};
	}
	static void   Destroy(MessagePayload& input) noexcept {
		std::free(input.str_msg);
		input.str_msg = nullptr;
   	}
};
template<> struct MessageTypeTraits<char*> : MessageTypeTraits<const char*> {};
template<std::size_t N> struct MessageTypeTraits<char[N]> : MessageTypeTraits<const char*> {};
template<std::size_t N> struct MessageTypeTraits<const char[N]> : MessageTypeTraits<const char*> {};


template<typename T, typename = void>
struct IsMessageType : std::false_type{};
template<typename T>
struct IsMessageType<T, std::void_t<typename MessageTypeTraits<T>::PayloadType>> : std::true_type{};


template<typename T>
Message CreateMessage(T val) {
	static_assert(IsMessageType<T>::value, "Not a supported messge type!");
	using Traits        = MessageTypeTraits<T>;
	auto constexpr type = Traits::MsgType;
	return Message{ type, Traits::Create(val)};
}

void DeleteMessage(Message& msg) noexcept;

extern "C"
{
	Message CreateIntMsg(int msg);
	Message CreateDblMsg(double msg);
	Message CreateStrMsg(const char* msg);
	void DeleteMessage(Message* msg);

	void PrintMessage(Message* msg);

};

