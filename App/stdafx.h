#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <WS2tcpip.h>

#include <iostream>

#include <vector>
#include <list>
#include <mutex>
#include <thread>
#include <functional>
#include <string>

#include <TypeDefine.h>

#include <ThreadPool.h>
#include <SocketHelper.h>
#include <DNSEndPoint.h>
#include <TCPListener.h>
#include <NetSocket.h>