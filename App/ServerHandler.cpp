#include "stdafx.h"
#include "ServerHandler.h"

#include "ThreadHandlerLogin.h"

bool ServerHandler::init()
{
    dispatcher_.addHandler("/Login", std::bind(&ServerHandler::onTemplateExecute<ThreadHandlerLogin>, this, std::placeholders::_1));

    return false;
}

bool ServerHandler::dispatch(ProtocolContextPtr pContext)
{
    std::shared_ptr<HttpContext> pHttpContext = std::dynamic_pointer_cast<HttpContext, ProtocolContext>(pContext);
    bool ret = dispatcher_.dispatch(pHttpContext->getHttpRequest()->getUri(), pContext);

    return ret;
}
