#ifndef __REMEX_CLIENT_H__
#define __REMEX_CLIENT_H__
#include <string>
#include <cMsg.h>
#include <remexLib.h>
#include <cstdlib>
#include <variant>
#include <memory>
#include <utility>
#include <time.h> // struct timespec
#include <iostream>
#include "CMsgMessages.h"
#include "ClientTypes.h"
namespace RemexClient
{

	class Client
	{
		ClientName name;
		void* handle;
	public:
		Client(ClientName name,  void* handle);
		template<::varReturnType T> RemexVal<T> GetValue(ClientCommand const& command);
		template<::varReturnType T> RemexVal<T> Execute(ClientCommand const& command);	
		void PrintStatus(::remexErrCode code);
	};
	template<::varReturnType T>
	RemexVal<T> Client::GetValue(ClientCommand const& command)
	{
		std::cout << "Client::GetValue called with command (" << command.get() << ")\n";
		RemexVal<T> result{std::nullopt};

		timespec timeout = { .tv_sec = 3, .tv_nsec = 0 };
		auto msg = CreateSendMsg(::remexGetType::rGetVariable, name, command, T);
		auto replyMsg = CreateReplyMessage();
  		int status = ::cMsgSendAndGet(handle, msg, &timeout, &replyMsg);
		if(replyMsg && status == ::CMSG_OK) {
			result = replyMsg.GetResult<T>();
		} else if(replyMsg == nullptr) {
			std::cerr << name.get() << " not subscribed to a CMsg Server\n";
		} else {
			std::cerr << "cMsgSendAndGet returned ( " << ::cMsgPerror(status) << " )\n";
		}
		if(int code = replyMsg.GetErrorCode(); code != ::REMEX_ERRCODE_SUCCESS)
			PrintStatus(static_cast<remexErrCode>(code));
		return result;
	}
	template<varReturnType T>
	RemexVal<T> Client::Execute(ClientCommand const& command)
	{
		std::cout << "Client::Execute called with command (" << command.get() << ")\n";
		RemexVal<T> result{std::nullopt};

		timespec timeout = { .tv_sec = 3, .tv_nsec = 0 };
		auto msg = CreateSendMsg(::remexGetType::rExecuteFunction, name, command, T);
		auto replyMsg = CreateReplyMessage();
  		int status = ::cMsgSendAndGet(handle, msg, &timeout, &replyMsg);
		if(replyMsg && status == ::CMSG_OK) {
			result = replyMsg.GetResult<T>();
		} else if(replyMsg == nullptr) {
			std::cerr << name.get() << " not subscribed to a CMsg Server\n";
		} else {
			std::cerr << "cMsgSendAndGet returned ( " << ::cMsgPerror(status) << " )\n";
		}
		replyMsg.RedirectOutput();

		if(int code = replyMsg.GetErrorCode(); code != ::REMEX_ERRCODE_SUCCESS)
			PrintStatus(static_cast<remexErrCode>(code));
		return result;
	}


}; // namespace Remex
#endif
