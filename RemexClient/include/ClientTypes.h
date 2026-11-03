#ifndef __CLIENT_TYPES_H__
#define __CLIENT_TYPES_H__
#include <string>
#include <remexLib.h>
#include <optional>
#include <variant>
namespace RemexClient
{
	template<typename ...Ts>
	constexpr bool dependent_false_v = false;

	// Enum-based type traits	
	template<::varReturnType T> struct RemexTypes;
	template<> struct RemexTypes<::varReturnType::rVOID> {
		using RemexReturnType = std::monostate;
	};
	template<> struct RemexTypes<::varReturnType::rFLOAT> {
		using RemexReturnType = float;
	};
	template<> struct RemexTypes<::varReturnType::rDOUBLE> {
		using RemexReturnType = double;
	};
	template<> struct RemexTypes<::varReturnType::rINT32> {
		using RemexReturnType = int;
	};
	template<> struct RemexTypes<::varReturnType::rINT16> {
		using RemexReturnType = short;
	};
	template<> struct RemexTypes<::varReturnType::rUINT32> {
		using RemexReturnType = unsigned;
	};
	template<> struct RemexTypes<::varReturnType::rUINT16> {
		using RemexReturnType = unsigned short;
	};
	// Helper Alias
	template<::varReturnType T>
	using RemexReturnType_t = typename RemexTypes<T>::RemexReturnType;

	template<::varReturnType T>
	using RemexVal = std::optional<RemexClient::RemexReturnType_t<T>>;

	class ClientName
	{
		std::string name;
	public:
		ClientName();
		explicit ClientName(std::string name);
		std::string const& get() const { return name; }
	};
	class ClientCommand
	{
		std::string command;
	public:
		explicit ClientCommand(std::string cmd) : command(std::move(cmd)) {}
		std::string const& get() const { return command; }
	};

}; // namespace RemexClient


#endif
