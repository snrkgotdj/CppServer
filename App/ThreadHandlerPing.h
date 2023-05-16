#pragma once

#include "ThreadHandlerBase.h"

class ThreadHandlerPing final
	: public std::enable_shared_from_this<ThreadHandlerPing>
	, public ThreadHandlerBase
{
public:
	void start();

private:
	void execute();

public:
	ThreadHandlerPing(ProtocolContextPtr pContext)
		: ThreadHandlerBase(pContext)
	{

	}
};

