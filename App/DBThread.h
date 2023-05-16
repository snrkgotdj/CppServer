#pragma once

class DBThread
{
private:
	static ThreadPool threadPool_;

public:
	static bool init(std::wstring_view name, size_t concurrent);
	static bool queue(const std::function<void()>& callback);

public:
	DBThread() = delete;
};

