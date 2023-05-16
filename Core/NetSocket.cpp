#include "pch.h"
#include "NetSocket.h"

#include "Protocol.h"

std::atomic_uint64_t NetSocket::netSocketCount = 0;

NetSocketPtr NetSocket::create(SocketOwner* pOwner)
{
	NetSocketPtr pSocket(new NetSocket);
	if (pSocket->init(pOwner) == false)
		return nullptr;

	return pSocket;
}

bool NetSocket::init(SocketOwner* pOwner)
{
	pOwner_ = pOwner;
	socket_ = WSASocket((int)AddressFamily::INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	connectionId_ = netSocketCount.fetch_add(1);

	if (socket_ == INVALID_SOCKET)
	{
		PrintLastError();
		return false;
	}

	BOOL noDelayOpt = TRUE;
	int errorCode = setsockopt(socket_, IPPROTO::IPPROTO_TCP, TCP_NODELAY, (const char*)(&noDelayOpt), sizeof(noDelayOpt));
	if (errorCode == SOCKET_ERROR)
	{
		PrintLastError();
		return false;
	}

	BOOL reuseOpt = TRUE;
	errorCode = setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseOpt, sizeof(reuseOpt));
	if (errorCode == SOCKET_ERROR)
	{
		PrintLastError();
		return false;
	}

	LINGER linger = {};
	linger.l_onoff = 1;
	linger.l_linger = 0;
	errorCode = setsockopt(socket_, SOL_SOCKET, SO_LINGER, (const char*)&linger, sizeof(linger));
	if (errorCode == SOCKET_ERROR)
	{
		PrintLastError();
		return false;
	}

	return true;
}

void NetSocket::receiveRequest()
{
	receiveOverlapped_.clear();
	
	DWORD dwRecvBytes = 0;
	DWORD dwFlags = 0;

	int errorCode = WSARecv(socket_, receiveOverlapped_.getWsaBuf(), 1, &dwRecvBytes, &dwFlags, receiveOverlapped_.getOverlapped(), nullptr);
	if (errorCode == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			PrintLastError();
			return;
		}
	}
}

void NetSocket::close()
{
	if (INVALID_SOCKET == socket_)
	{
		return;
	}

	// close callback

	if (FALSE == TransmitFile(socket_, NULL, 0, 0, disconnectOverlapped_.getOverlapped(), NULL, TF_DISCONNECT | TF_REUSE_SOCKET))
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			return;
		}


		PrintLastError();
	}
}

void NetSocket::onClose()
{
	if (closeHandler_ == nullptr)
		return;

	closeHandler_(shared_from_this());
}

void NetSocket::onRecv(const char* buffer, DWORD numberOfBytes)
{
	pProtocol_->appendBuffer(buffer, numberOfBytes);

	while (true)
	{
		DeserializeResultType type = pProtocol_->desirealize();
		switch (type)
		{
		case DeserializeResultType::Ok:
			break;
		case DeserializeResultType::Wait:
			return;
		case DeserializeResultType::Error:
		default:
			// TODO 소켓 종료 루틴
			return;
		}

		if (recvHandler_ == nullptr)
			return;

		ProtocolContextPtr pContext = pProtocol_->getContext();
		if (pContext == nullptr)
			return;

		pContext->setSocket(shared_from_this());
		recvHandler_(pContext);
	}
}

void NetSocket::callOverlapped(DWORD numberOfBytesTransferred, LPOVERLAPPED pOverlapped)
{
	if (pOverlapped == nullptr)
		return;

	IocpOverlapped* pIocpOverlapped = reinterpret_cast<IocpOverlapped*>((char*)pOverlapped - (char*)(((IocpOverlapped*)(nullptr))->getOverlapped()));
	pIocpOverlapped->excute(numberOfBytesTransferred);
}

void NetSocket::setProtocol(Protocol* pProtocol)
{
	if (pProtocol == pProtocol_)
		return;

	if (pProtocol_)
	{
		delete pProtocol_;
	}

	pProtocol_ = pProtocol;
}

void NetSocket::setHandler(const std::function<void(std::shared_ptr<ProtocolContext>)>& recvHandler, const std::function<void(NetSocketPtr)>& closeHandler)
{
	recvHandler_ = recvHandler;
	closeHandler_ = closeHandler;
}