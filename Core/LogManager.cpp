#include "pch.h"
#include "LogManager.h"

LogManager* LogManager::g_pInst = nullptr;

LogManager* LogManager::getInstance()
{
	if (g_pInst == nullptr)
	{
		g_pInst = new LogManager;
	}

	return g_pInst;
}

bool LogManager::init(std::string_view path, std::string_view fileName, LOG_TYPE logType)
{
	if (isInit_.exchange(true))
		return true;

	path_ = path;
	fileName_ = fileName;
	logType_ = logType;

	write(LOG_TYPE::LT_INFO, "Log Initialize");

	return false;
}

void LogManager::write(LOG_TYPE logType, const char* format, ...)
{
	if (logType_ < logType)
		return;

	std::shared_ptr<std::string> logString = std::make_shared<std::string>();
	
	va_list marker;
	va_start(marker, format);
	int strLen = _vscprintf(format, marker);
	logString->resize(strLen + 1);

	std::string& tempString = *logString;

	_vsnprintf_s(tempString.data(), tempString.size(), strLen, format, marker);
	
}
