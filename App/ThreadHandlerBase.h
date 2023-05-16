#pragma once

class ThreadHandlerBase
{
protected:
	HttpContextPtr pContext_;

public:
	ThreadHandlerBase(ProtocolContextPtr pContext)
		: pContext_(std::dynamic_pointer_cast<HttpContext, ProtocolContext>(pContext))
	{

	}

	virtual ~ThreadHandlerBase() {}
};

