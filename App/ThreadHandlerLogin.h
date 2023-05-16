#pragma once

#include "ThreadHandlerBase.h"

class ThreadHandlerLogin : public ThreadHandlerBase, public std::enable_shared_from_this<ThreadHandlerLogin>
{
public:
	void start();

private:
	void execute();
	void postExecute();

public:
	ThreadHandlerLogin(ProtocolContextPtr pContext)
		: ThreadHandlerBase(pContext)
	{ 
	}
	
};

