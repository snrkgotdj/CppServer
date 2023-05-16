#include "pch.h"
#include "TCPListener.h"

#include "Task.h"
#include "NetSocket.h"
#include "Protocol.h"

TCPListener::~TCPListener()
{
	if (pThreadPool_)
	{
		delete pThreadPool_;
		pThreadPool_ = nullptr;
	}
}

bool TCPListener::init(const IPEndPoint& endPoint, const std::function<void(NetSocketPtr)>& acceptCallback)
{
	listenSocket_ = WSASocket((int32_t)AddressFamily::INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket_ == INVALID_SOCKET)
	{
		PrintLastError();
		return false;
	}

	SocketAddress socketAddress = endPoint.serialize();

	int errorCode = bind(listenSocket_, reinterpret_cast<sockaddr*>(socketAddress.getBuffer()), (int)socketAddress.getBufferLength());
	if (errorCode == SOCKET_ERROR)
	{
		PrintLastError();
		closeSocket();
		return false;
	}

	BOOL on = TRUE;
	setsockopt(listenSocket_, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, reinterpret_cast<const char*>(&on), sizeof(on));
	if (errorCode == SOCKET_ERROR)
	{
		PrintLastError();
		closeSocket();
		return false;
	}

	errorCode = listen(listenSocket_, 512);
	if (errorCode == SOCKET_ERROR)
	{
		PrintLastError();
		closeSocket();
		return false;
	}

	listenEndPoint_ = endPoint;

	if (pThreadPool_ == nullptr)
	{
		pThreadPool_ = new ThreadPool(L"TCPListener", ThreadType::NETWORK, std::thread::hardware_concurrency() * 2);
	}

	acceptCallback_ = 

	bool ret = pThreadPool_->bind((HANDLE)listenSocket_, &TCPListener::onAccept);
	if (ret == false)
	{
		closeSocket();
		return false;
	}

	return true;
}

void TCPListener::onAccept(DWORD numberOfBytesTransferred, LPOVERLAPPED pOverlapped)
{
	// 이 코드를 이해했다면 c++ 기본은 되신분
	AcceptOverlapped* pAcceptOverlapped = reinterpret_cast<AcceptOverlapped*>((char*)pOverlapped - (char*)(((AcceptOverlapped*)(nullptr))->getOverlapped()));
	pAcceptOverlapped->excute(numberOfBytesTransferred);

	NetSocketPtr pSocket = pAcceptOverlapped->getSocket();
	SocketOwner* pOwner = pSocket->getSocketOwner();
	if (pOwner == nullptr)
	{
		// 로그
		return;
	}
	
	if (pOwner->isListener() == false)
	{
		//로그
		return;
	}

	TCPListener* pListener = (TCPListener*)pOwner;
	if (pListener->acceptCallback_ == nullptr)
		return;

	pListener->acceptCallback_(pSocket);

	{
		std::unique_lock lock(pListener->connectionMapLock_);
		pListener->connectionMap_.insert(std::make_pair(pSocket->getConnectionId(), pSocket));
	}

	pSocket->setProtocol(pListener->pProtocolFactory_->make());

	if (false == pListener->bindQueue(pSocket))
	{
		// 로그 남김
	}
	
	pSocket->receiveRequest();

	pListener->asyncAccept();
}

bool TCPListener::start()
{
	size_t threadCnt = pThreadPool_->getThreadCount();
	for (size_t i = 0; i < threadCnt; ++i)
	{
		bool ret = asyncAccept();
		if (ret == false)
			return false;
	}

	return true;
}

void TCPListener::disconnect(NetSocketPtr pSocket)
{
	{
		std::unique_lock lock(connectionMapLock_);
		connectionMap_.erase(pSocket->getConnectionId());
	}

	{
		std::unique_lock lock(connectionPoolLock_);
		connectionPool_.push_back(pSocket);
	}
}

bool TCPListener::asyncAccept()
{
	NetSocketPtr pSocket = getOrCreateSocket();
	if (pSocket == nullptr)
		return false;

	std::unique_ptr<AcceptOverlapped> acceptExOverlapped = std::make_unique<AcceptOverlapped>(pSocket);
	
	DWORD dwBytes = 0;
	BOOL ret = AcceptEx(listenSocket_, pSocket->getSocket(), acceptExOverlapped->getBuffer(), 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &dwBytes, acceptExOverlapped->getOverlapped());
	if (ret == FALSE)
	{
		if (ERROR_IO_PENDING != WSAGetLastError())
		{
			PrintLastError();
			return false;
		}
	}

	acceptExOverlapped.release();
	return true;
}

bool TCPListener::bindQueue(NetSocketPtr pSocket)
{
	pThreadPool_->bind((HANDLE)pSocket->getSocket(), &NetSocket::callOverlapped);

	return false;
}

std::shared_ptr<NetSocket> TCPListener::getOrCreateSocket()
{
	std::unique_lock lock(connectionPoolLock_);
	if (connectionPool_.empty())
	{
		NetSocketPtr pSocket = NetSocket::create(this);
		return pSocket;
	}

	NetSocketPtr pSocket = connectionPool_.front();
	connectionPool_.erase(connectionPool_.begin());

	return pSocket;
}

void TCPListener::closeSocket()
{
	if (listenSocket_ == INVALID_SOCKET)
		return;

	::closesocket(listenSocket_);
	listenSocket_ = INVALID_SOCKET;
}

