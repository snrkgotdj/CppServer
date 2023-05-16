
#include "stdafx.h"
#include "HttpServer.h"
#include "LogManager.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock")

int main()
{
    setlocale(LC_ALL, "");

    LogManager::getInstance()->init("C:\\Users\\djHome\\Documents\\work\\CppServer\\x64\\", "Log", LOG_TYPE::LT_DEBUG);
    MY_LOG(LOG_TYPE::LT_INFO, "%s", "ServerStart");

    setlocale(LC_ALL, "");

    if (HttpServer::init() == false)
        return -1;

    if (HttpServer::start() == false)
        return -1;

    while (HttpServer::run())
    {
        ::Sleep(1);
    }

    HttpServer::stop();

    return 0;
}
