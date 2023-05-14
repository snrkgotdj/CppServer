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
	threadPool_.init(L"LogManager", ThreadType::LOG, 1);

	write(LOG_TYPE::LT_INFO, "Log Initialize");

	return true;
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
	threadPool_.queue(std::bind(&LogManager::writeInternal, this, logType_, logString));
}

void LogManager::writeInternal(LOG_TYPE logType, std::shared_ptr<std::string> logString)
{
	if (ThreadUtils::isEqualThread(ThreadType::LOG) == false)
	{
		assert(0);
		return;
	}

	if (file_ == INVALID_HANDLE_VALUE)
	{
		if (createLogFile() == false)
			return;
	}

	if (file_ == INVALID_HANDLE_VALUE)
		return;

	DateTime now = DateTime::now();

	std::stringstream ss;
	ss << "[" << now.toString("%y-%m-%d %H:%M:%S.%f]");

	switch (logType)
	{
	case LOG_TYPE::LT_SYSTEM:
		ss << "[SYSTEM]";
		break;
	case LOG_TYPE::LT_FATAL:
		ss << "[FATAL]";
		break;
	case LOG_TYPE::LT_ERROR:
		ss << "[ERROR]";
		break;
	case LOG_TYPE::LT_WARN:
		ss << "[WARN]";
		break;
	case LOG_TYPE::LT_INFO:
		ss << "[INFO]";
		break;
	case LOG_TYPE::LT_TRACE:
		ss << "[TRACE]";
		break;
	case LOG_TYPE::LT_DEBUG:
		ss << "[DEBUG]";
		break;
	default:
		ss << "[INVALID]";
		break;
	}

	ss << logString->c_str() << "\n" << (char)0;

	setTextColor(logType);
	printf_s(ss.view().data());
	resetTextColor();

	DWORD dwWrite = 0;
	WriteFile(file_, ss.view().data(), static_cast<DWORD>(ss.view().length()), &dwWrite, NULL);
}

bool LogManager::createLogFile()
{
	createFileTime_ = DateTime::now();
	std::stringstream ss;

	ss << path_ << fileName_ << createFileTime_.toString("%y_%m_%d_%H_%M_%S.txt");
	ss << char(0);

	file_ = CreateFileA(ss.view().data(),
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (file_ == INVALID_HANDLE_VALUE)
	{
		PrintLastError();
		return false;
	}

	return true;
}

void LogManager::setTextColor(LOG_TYPE logLevel)
{
	int color = 0;

	switch (logLevel)
	{
	case LOG_TYPE::LT_SYSTEM:
		color = BLUE + BLACK * 16;
		break;
	case LOG_TYPE::LT_FATAL:
		color = MAGENTA + BLACK * 16;
		break;
	case LOG_TYPE::LT_ERROR:
		color = RED + BLACK * 16;
		break;
	case LOG_TYPE::LT_WARN:
		color = BROWN + BLACK * 16;
		break;
	case LOG_TYPE::LT_INFO:
		color = LIGHTGREEN + BLACK * 16;
		break;
	case LOG_TYPE::LT_TRACE:
		color = WHITE + BLACK * 16;
		break;
	case LOG_TYPE::LT_DEBUG:
		color = CYAN + BLACK * 16;
		break;
	default:
		return;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void LogManager::resetTextColor()
{
	int color = LIGHTGRAY + BLACK * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
