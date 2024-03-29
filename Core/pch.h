﻿
#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <WS2tcpip.h>
#include <iostream>
#include <MSWSock.h>

#include <string>
#include <sstream>
#include <thread>
#include <functional>
#include <vector>
#include <list>
#include <mutex>
#include <chrono>
#include <time.h>
#include <assert.h>

#include "Enum.h"
#include "TypeDefine.h"

static void PrintLastError()
{
	DWORD example_error = WSAGetLastError();
	TCHAR* message = nullptr;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		nullptr,
		example_error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(TCHAR*)&message,
		0,
		nullptr);

	if (message == nullptr)
	{
		wprintf(L"[ErrorCode:%u] %s", example_error, message);
		LocalFree(message);
	}
}

#endif //PCH_H
