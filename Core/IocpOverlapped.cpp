#include "pch.h"
#include "IocpOverlapped.h"
#include "NetSocket.h"
#include "SocketOwner.h"


IocpOverlapped::IocpOverlapped()
{
	memset(&overlapped_, 0, sizeof(overlapped_));
}

void AcceptOverlapped::excute(DWORD numberOfBytes)
{
	SOCKADDR_IN* pLocalAddr = nullptr;
	SOCKADDR_IN* pRemoteAddr = nullptr;

	int localLen = 0;
	int remoteLen = 0;

	GetAcceptExSockaddrs(buffer_, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, (LPSOCKADDR*)&pLocalAddr, &localLen, (LPSOCKADDR*)&pRemoteAddr, &remoteLen);

	pSocket_->setLocalEndPoint(IPEndPoint(pLocalAddr->sin_addr.s_addr, htons(pLocalAddr->sin_port)));
	pSocket_->setRemoteEndPoint(IPEndPoint(pRemoteAddr->sin_addr.s_addr, htons(pRemoteAddr->sin_port)));
}

void ReceiveOverlapped::excute(DWORD numberOfBytes)
{
	if (0 == numberOfBytes)
	{
		pSocket_->close();
		return;
	}

	pSocket_->onRecv(wsaBuffer_.buf, numberOfBytes);

	pSocket_->receiveRequest();
}

void ReceiveOverlapped::clear()
{
	if (nullptr == wsaBuffer_.buf)
	{
		wsaBuffer_.buf = new char[1024];
		wsaBuffer_.len = 1024;
	}

	if (nullptr == wsaBuffer_.buf)
		return;

	memset(wsaBuffer_.buf, 0x00, wsaBuffer_.len);
}

void DisconnectOverlapped::excute(DWORD numberOfBytes)
{
	if (pSocket_ == nullptr)
		return;

	pSocket_->onClose();
	
	SocketOwner* pOwner = pSocket_->getSocketOwner();
	if (pOwner == nullptr)
		return;

	pOwner->disconnect(pSocket_->shared_from_this());
}
