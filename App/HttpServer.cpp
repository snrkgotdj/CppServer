#include "stdafx.h"
#include "HttpServer.h"

#include "DBThread.h"
#include "RoomManager.h"
#include "IPAddressUtils.h"

ServerHandler HttpServer::serverHandler_;

bool HttpServer::init()
{
	SocketHelper::initSockets();
	
	serverHandler_.init();

	DBThread::init(L"DBThread", 8);

	RoomManager::init(8);

	bool ret = listener_.init(IPEndPoint(IPAddressUtils::parse("0.0.0.0"), 30000), std::bind(&HttpServer::onAccept, std::placeholders::_1));
	return ret;
}

bool HttpServer::start()
{
	bool ret = listener_.start();
	return ret;
}

bool HttpServer::run()
{
	return true;
}

void HttpServer::stop()
{
	SocketHelper::closeSockets();
}

void HttpServer::onAccept(NetSocketPtr pNetSocket)
{
	MY_LOG(LOG_TYPE::LT_DEBUG, "connect!! [%I64u]", pNetSocket->getConnectionId());
	
	pNetSocket->setHandler(std::bind(&HttpServer::onRecv, std::placeholders::_1), std::bind(&HttpServer::onClose, std::placeholders::_1));

	pNetSocket->setLastRecvTime();

	std::unique_lock lock(lock_);
	sockets_.push_back(std::make_pair(pNetSocket->getConnectionId(), pNetSocket));
}

void HttpServer::onRecv(ProtocolContextPtr pContext)
{
	std::shared_ptr<HttpContext> pHttpContext = std::dynamic_pointer_cast<HttpContext, ProtocolContext>(pContext);
	if (pHttpContext == nullptr)
		return;

	HttpRequestPtr pRequest = pHttpContext->getHttpRequest();
	if (serverHandler_.dispatch(pContext) == false)
	{
		if (RoomManager::isHandler(pRequest->getUri()) == false)
		{
			// 에러
		}

		if (pHttpContext->getHttpRequest()->hasHeader("Auth") == false)
		{
			// 에러
		}

		std::shared_ptr<ThreadHandlerAuth>
	}
	

}

void HttpServer::onClose(NetSocketPtr pNetSocket)
{
}
