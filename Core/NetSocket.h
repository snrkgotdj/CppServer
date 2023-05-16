#pragma once

#include "IPEndPoint.h"
#include "Task.h"
#include "IocpOverlapped.h"

class SocketOwner;
class Protocol;
class ProtocolContext;

class NetSocket : public std::enable_shared_from_this<NetSocket>
{
	friend class TCPListener;
	friend class ReceiveOverlapped;
	friend class DisconnectOverlapped;

private:
	static std::atomic_uint64_t netSocketCount;

private:
	SOCKET socket_;
	IPEndPoint localEndPoint_;
	IPEndPoint remoteEndPoint_;
	SocketOwner* pOwner_ = nullptr;
	size_t connectionId_ = -1;
	std::function<void(NetSocketPtr)> closeHandler_;
	std::function<void(std::shared_ptr<ProtocolContext>)> recvHandler_;

private:
	Protocol* pProtocol_ = nullptr;
	ReceiveOverlapped receiveOverlapped_ = this;
	DisconnectOverlapped disconnectOverlapped_ = this;
	DateTime lastRecvTime_;

public:
	static NetSocketPtr create(SocketOwner* pOwner);

private:
	bool init(SocketOwner* pOwner);

public:
	void close();
	void receiveRequest();

private:
	void onClose();
	void onRecv(const char* buffer, DWORD numberOfBytes);
	static void callOverlapped(DWORD numberOfBytesTransferred, LPOVERLAPPED pOverlapped);

public:
	void setLocalEndPoint(IPEndPoint endPoint) { localEndPoint_ = endPoint; }
	void setRemoteEndPoint(IPEndPoint endPoint) { remoteEndPoint_ = endPoint; }
	void setProtocol(Protocol* pProtocol);
	void setHandler(const std::function<void(std::shared_ptr<ProtocolContext>)>& recvHandler, const std::function<void(NetSocketPtr)>& closeHandler);
	void setLastRecvTime() { lastRecvTime_ = DateTime::kstNow(); }

public:
	SOCKET getSocket() { return socket_; }
	IPEndPoint getLocalEndPoint() { return localEndPoint_; }
	IPEndPoint getRemoteEndPoint() { return remoteEndPoint_; }
	SocketOwner* getSocketOwner() { return pOwner_; }
	size_t getConnectionId() { return connectionId_; }

private:
	NetSocket& operator=(const NetSocket& _Other) = delete;
	NetSocket& operator=(NetSocket&& _Other) noexcept = delete;

private:
	NetSocket() = default;
	NetSocket(const NetSocket& other) = delete;
	NetSocket(NetSocket&& other) = delete;

public:
	~NetSocket() {}
};

