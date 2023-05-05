#pragma once
class ThreadPool
{
	using OverlappedCallback = const std::function<void(DWORD, LPOVERLAPPED)>;

private:
	HANDLE iocp_;
	std::wstring name_;
	std::vector<std::shared_ptr<std::thread>> threads_;
	size_t jobCount_ = 0;

public:
	size_t getJobCount();

public:
	bool queue(const std::function<void()>& callback);
	bool bind(HANDLE handle, const OverlappedCallback& callback);

private:
	static void threadFunc(ThreadPool* pPool, size_t index);
	void close();

public:
	ThreadPool() = delete;
	ThreadPool(std::wstring_view name, size_t concurrent);
	~ThreadPool();
};

