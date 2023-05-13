#pragma once

#include "ThreadPool.h"

enum class LOG_TYPE
{
	LT_SYSTEM,
	LT_FATAL,
	LT_ERROR,
	LT_WARN,
	LT_INFO,
	LT_TRACE,
	LT_DEBUG,
};

class LogManager
{
private:
	static LogManager* g_pInst;

private:
	std::atomic_bool isInit_;
	std::string path_;
	std::string fileName_;
	LOG_TYPE logType_;
	ThreadPool threadPool_;

public:
	static LogManager* getInstance();
	
public:
	bool init(std::string_view path, std::string_view fileName, LOG_TYPE logType);
	void write(LOG_TYPE logType, const char* format, ...);
};

