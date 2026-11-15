#pragma once
#include "Message.h"

extern "C"
{
	enum BMWMsgtype
	{
		Request = 0,
		Command = 1
	};

	enum BMWMsgSubSys
	{
		Freq = 0, // int
		Ampl = 1, // double
		Cycl = 2, // int
		Wave = 3, // int (enum)
		Trig = 4, // int (enum)
		Stat = 5, // int (enum)
		Erro = 6
	};

	struct BMWMsg
	{
		void* coil;
		BMWMsgtype msg_type;
		BMWMsgSubSys msg_sys;
		Message msg;
	};
	BMWMsg CreateBMWMessage(void* coil, BMWMsgtype type, BMWMsgSubSys sys, Message msg);
	void DeleteBMWMessage(BMWMsg* msg);

	void PrintBMWMsg(BMWMsg* msg);
};
