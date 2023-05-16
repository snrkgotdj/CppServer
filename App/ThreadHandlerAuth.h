#pragma once

#include "ThreadHandlerBase.h"

class ThreadHandlerAuth final
	: public std::enable_shared_from_this<ThreadHandlerAuth>
	, ThreadHandlerBase
{
public:
	void start();

private:
	void execute();

public:
	ThreadHandlerAuth(ProtocolContextPtr pContext)
		: ThreadHandlerBase(pContext)
	{

	}
	
	~ThreadHandlerAuth()
	{

	}
};

