#pragma once

class RoomThread
{
private:
	ThreadPool threadPool_;
	std::thread thread_;

public:
	bool queue(const std::function<void()>& callback);

private:
	void execute();
	static void run(RoomThread* _this);

public:
	RoomThread()
		: threadPool_(L"RoomThread", ThreadType::LOGIC, 0)
		, thread_(&RoomThread::run, this)
	{

	}
};

