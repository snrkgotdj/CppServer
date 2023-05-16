#include "stdafx.h"
#include "ThreadHandlerAuth.h"
#include "DBThread.h"
#include "RoomManager.h"

void ThreadHandlerAuth::start()
{
	DBThread::queue(std::bind(&ThreadHandlerAuth::execute, shared_from_this()));
}

void ThreadHandlerAuth::execute()
{
	RoomManager::dispatch(pContext_);
}
