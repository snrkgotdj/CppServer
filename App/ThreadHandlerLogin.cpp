#include "stdafx.h"
#include "ThreadHandlerLogin.h"

#include "DBThread.h"

void ThreadHandlerLogin::start()
{
	DBThread::queue(std::bind(&ThreadHandlerLogin::execute, shared_from_this()));
}

void ThreadHandlerLogin::execute()
{

	DBThread::queue(std::bind(&ThreadHandlerLogin::postExecute, shared_from_this()));
}

void ThreadHandlerLogin::postExecute()
{
}

