#pragma once

#include "ThreadUtils.h"

class ThreadPool
{
public:
	typedef void(__stdcall* OverlappedCallback)(DWORD, LPOVERLAPPED);

	struct ThreadPair
	{
		std::stop_source stopSource;
		std::shared_ptr<std::thread> pThread;
	};

	enum class Execute_ReturnType
	{
		OK = 0,
		Empty = 1,
		Terminate = 2
	};

private:
	HANDLE iocp_ = NULL;
	ThreadType threadType_ = ThreadType::NONE;
	std::wstring name_;
	std::vector<ThreadPair> threads_;
	size_t jobCount_ = 0;
	ULONG32 fetchCount_ = 128;

public:
	bool init(std::wstring_view name, ThreadType threadType, size_t concurrent);

public:
	bool isValid() { return iocp_ != NULL; }

public:
	size_t getJobCount();
	size_t getThreadCount() { return threads_.size(); }

public:
	bool queue(const std::function<void()>& callback);
	bool bind(HANDLE handle, const OverlappedCallback& callback);
	Execute_ReturnType execute(DWORD timeout = INFINITE);

private:
	static void threadFunc(ThreadPool* pPool, size_t index);
	void close();

public:
	ThreadPool() = default;
	ThreadPool(std::wstring_view name, ThreadType threadType, size_t concurrent);
	~ThreadPool();
};

