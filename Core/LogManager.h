#pragma once

#include "ThreadPool.h"
#include "DateTime.h"

enum class LOG_TYPE
{
	LT_SYSTEM,
	LT_FATAL,
	LT_ERROR,
	LT_WARN,
	LT_INFO,
	LT_TRACE,
	LT_DEBUG
};

constexpr int BLACK = 0;
constexpr int BLUE = 1;
constexpr int GREEN = 2;
constexpr int CYAN = 3;
constexpr int RED = 4;
constexpr int MAGENTA = 5;
constexpr int BROWN = 6;
//	콘솔 기본 색깔
constexpr int LIGHTGRAY = 7;
constexpr int DARKGRAY = 8;
constexpr int LIGHTBLUE = 9;
constexpr int LIGHTGREEN = 10;
constexpr int LIGHTCYAN = 11;
constexpr int LIGHTRED = 12;
constexpr int LIGHTMAGENTA = 13;
constexpr int YELLOW = 14;
constexpr int WHITE = 15;

#define MY_LOG LogManager::getInstance()->write

class LogManager
{
private:
	static LogManager* g_pInst;

private:
	std::atomic_bool isInit_ = false;
	LOG_TYPE logType_ = LOG_TYPE::LT_DEBUG;
	ThreadPool threadPool_;

private:
	HANDLE file_ = INVALID_HANDLE_VALUE;
	std::string path_;
	std::string fileName_;
	DateTime createFileTime_;

public:
	static LogManager* getInstance();
	
public:
	bool init(std::string_view path, std::string_view fileName, LOG_TYPE logType);
	void write(LOG_TYPE logType, const char* format, ...);

private:
	void writeInternal(LOG_TYPE logType, std::shared_ptr<std::string> logString);
	bool createLogFile();
	void setTextColor(LOG_TYPE logLevel);
	void resetTextColor();
};

