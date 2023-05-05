#pragma once

class Task
{
public:
	virtual void process() = 0;

public:
	Task() = default;
	virtual ~Task() {};
};

class CallbackTask final : public Task
{
private:
	std::function<void()> callback_;

public:
	void setCallback(const std::function<void()>& callback) { callback_ = callback; }

public:
	void process() override;
};

class OverlappedTask final : public Task
{
private:
	DWORD numberOfBytesTransferred = 0;
	LPOVERLAPPED pOverlapped = nullptr;
	std::function<void(DWORD, LPOVERLAPPED)> callback_;

public:
	void process() override;
};