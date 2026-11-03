#ifndef __CMSG_MESSAGES_H__
#define __CMSG_MESSAGES_H__
#include <memory>
#include <string>
#include <iostream>
#include <variant>
#include <cMsg.h>
#include <remexLib.h>
#include "ClientTypes.h"
#include "ClientException.h"
namespace RemexClient
{
	class CMsgMessage
	{
		// Opaque ptr
		void* msg;
		enum class MSG_TYPE
		{
			REPLY,
			SEND
		};
		CMsgMessage(MSG_TYPE type = MSG_TYPE::REPLY)
		: msg(nullptr)
		{
  			if(type == MSG_TYPE::SEND) msg = ::cMsgCreateMessage();
		}
	public:
		~CMsgMessage() {
			::cMsgFreeMessage(&msg);
		}
		CMsgMessage(CMsgMessage const&) = delete;
		CMsgMessage& operator=(CMsgMessage const&) = delete;
		CMsgMessage(CMsgMessage&& other)
		: msg( std::exchange(other.msg, nullptr) ) {}
		CMsgMessage& operator=(CMsgMessage&& other) noexcept {
			std::swap(msg, other.msg);
			return *this;
		}
	public:
		// Factory Methods
		friend CMsgMessage
		CreateSendMsg( ::remexGetType const subject
			 , ClientName const &host
			 , ClientCommand const &cmd
  			 , ::varReturnType const rType);
		friend CMsgMessage
		CreateReplyMessage();
	public:
		// QoL Casts
		explicit operator bool() const { return msg == nullptr; }
		void** operator&() { return &msg; }
		operator void*() { return msg; }
	public:
		// Accessors
		std::string GetSubject() const;
		std::string GetType() const;
		void RedirectOutput() const;
		int GetErrorCode() const;
		template<::varReturnType T>
		RemexReturnType_t<T> GetResult() const;
	};
	inline CMsgMessage
	CreateSendMsg(::remexGetType const subject, ClientName const &host, ClientCommand const &cmd, ::varReturnType const rType)
	{
		// Expose this somehow
		int useMutexLock = 1;
		CMsgMessage msg{CMsgMessage::MSG_TYPE::SEND};
		switch(subject) {
			case ::remexGetType::rGetVariable:
				::cMsgSetSubject(msg.msg, "GetVar");
				break;

			case ::remexGetType::rLoadLibrary:
				::cMsgSetSubject(msg.msg, "LoadLibrary");
				break;

			case ::remexGetType::rExecuteFunction:
				::cMsgSetSubject(msg.msg, "ExecuteFunction");
				break;
			default:
				throw ClientException("Unknown remexGetType!");
				break;
		}	
		::cMsgSetType(msg.msg, std::string(std::string("to") + host.get()).c_str());
		::cMsgAddString(msg.msg, "variable", cmd.get().c_str());

		::cMsgAddInt32(msg.msg, "return_type",rType);
		::cMsgAddInt32(msg.msg, "useMutexLock", useMutexLock);

		return msg;	
	}
	inline CMsgMessage
	CreateReplyMessage()
	{
		return CMsgMessage{CMsgMessage::MSG_TYPE::REPLY};
	}

	template<::varReturnType T> RemexReturnType_t<T> CMsgMessage::GetResult() const
	{
		RemexReturnType_t<T> result;
		if constexpr      (T == ::varReturnType::rFLOAT ) cMsgGetFloat( msg, "return_value", &result);
		else if constexpr (T == ::varReturnType::rDOUBLE) cMsgGetDouble(msg, "return_value", &result);
		else if constexpr (T == ::varReturnType::rINT32 ) cMsgGetInt32( msg, "return_value", &result);
		else if constexpr (T == ::varReturnType::rINT16 ) cMsgGetInt16( msg, "return_value", &result);
		else if constexpr (T == ::varReturnType::rUINT32) cMsgGetUint32(msg, "return_value", &result);
		else if constexpr (T == ::varReturnType::rUINT16) cMsgGetUint16(msg, "return_value", &result);
		else if constexpr (T == ::varReturnType::rVOID)   result = std::monostate{};
		return result;
	}

}; // using namespace RemexClient
#endif
