#include "stdafx.h"
#include "RoomHandler.h"

#include "ThreadHandlerPing.h"

bool RoomHandler::init()
{
	dispatcher_.addHandler("/Ping", std::bind(&RoomHandler::onTemplateExecute<ThreadHandlerPing>, this, std::placeholders::_1));
	
	return true;
}

bool RoomHandler::isHandler(std::string_view uri)
{
	MessageHandler handler;
	bool ret = dispatcher_.getHandler(uri, handler);

	return ret;
}

bool RoomHandler::dispatch(ProtocolContextPtr pContext)
{
	std::shared_ptr<HttpContext> pHttpContext = std::dynamic_pointer_cast<HttpContext, ProtocolContext>(pContext); 
	bool ret = dispatcher_.dispatch(pHttpContext->getHttpRequest()->getUri(), pContext);

	return ret;
}
