#include "stdafx.h"
#include "DBThread.h"

ThreadPool DBThread::threadPool_;

bool DBThread::init(std::wstring_view name, size_t concurrent)
{
	bool ret = threadPool_.init(name, ThreadType::DB, concurrent);
	return ret;
}

bool DBThread::queue(const std::function<void()>& callback)
{
	bool ret = threadPool_.queue(callback);
	return ret;
}
