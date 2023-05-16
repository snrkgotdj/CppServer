#pragma once

#include "IPEndPoint.h"
#include "ThreadPool.h"
#include "SocketOwner.h"

class ProtocolFactory;

class TCPListener final : public SocketOwner
{
private:
	SOCKET listenSocket_ = INVALID_SOCKET;
	IPEndPoint listenEndPoint_;
	ThreadPool* pThreadPool_ = nullptr;
	ProtocolFactory* pProtocolFactory_ = nullptr;
	std::function<void(NetSocketPtr)> acceptCallback_;

private:
	std::mutex connectionPoolLock_;
	std::list<NetSocketPtr> connectionPool_;

private:
	std::mutex connectionMapLock_;
	std::unordered_map<size_t, NetSocketPtr> connectionMap_;
	
public:
	bool init(const IPEndPoint& endPoint, const std::function<void(NetSocketPtr)>& acceptCallback);

public:
	bool start();
	void disconnect(NetSocketPtr pSocket) override;

private:
	static void onAccept(DWORD numberOfBytesTransferred, LPOVERLAPPED pOverlapped);
	void closeSocket();
	bool asyncAccept();
	bool bindQueue(NetSocketPtr pSocket);

public:
	bool isListener() override { return true; }

private:
	NetSocketPtr getOrCreateSocket();

public:
	~TCPListener();
};

