#pragma once

#include "ServerHandler.h"

class HttpServer
{
private:
	static ServerHandler serverHandler_;
	static TCPListener listener_;
	static std::shared_mutex lock_;
	static std::vector<std::pair<uint64_t, NetSocketPtr>> sockets_;

public:
	static bool init();
	static bool start();
	static bool run();
	static void stop();

private:
	static void onAccept(NetSocketPtr pNetSocket);
	static void onRecv(ProtocolContextPtr pContext);
	static void onClose(NetSocketPtr pNetSocket);

public:
	HttpServer() = delete;
};

