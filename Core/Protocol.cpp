#include "pch.h"
#include "Protocol.h"

const static std::string CONTENT_LENGTH = "Content-Length";

void Protocol::appendBuffer(const char* buffer, DWORD numberOfBytes)
{
	std::copy(buffer, buffer + numberOfBytes, std::back_inserter(buffer_));
}

DeserializeResultType HttpProtocol::desirealize()
{
	if (buffer_.size() < 8)
		return DeserializeResultType::Wait;

	if (resultType_ == DeserializeResultType::None)
	{
		readLength_ = 0;
		httpContext_.reset();
		httpContext_ = std::make_shared<HttpContext>();
		resultType_ = DeserializeResultType::Wait;
	}

	std::string_view strView(buffer_.data(), buffer_.size());

	if (false == findMethod(strView))
	{
		resultType_ = DeserializeResultType::None;
		return DeserializeResultType::Error;
	}

	while (true)
	{
		int32_t ret = findHeader(strView);
		if (ret == -1)
			return DeserializeResultType::Wait;

		else if (ret == 1)
			break;
	}

	if (httpContext_->getHttpRequest()->hasHeader(CONTENT_LENGTH))
	{
		int32_t errorCode = findData(strView);
		if (errorCode == -1)
		{
			resultType_ = DeserializeResultType::None;
			return DeserializeResultType::Error;
		}
		else if (errorCode == 1)
		{
			return DeserializeResultType::Wait;
		}
	}

	buffer_.erase(buffer_.begin(), buffer_.begin() + readLength_);
	resultType_ = DeserializeResultType::None;
	return DeserializeResultType::Ok;
}

bool HttpProtocol::findMethod(std::string_view view)
{
	if (readLength_ != 0)
		return true;
	
	size_t methodIndex = view.find_first_of(" ");
	if (methodIndex == -1)
		return false;

	size_t uriIndex = view.find(" ", methodIndex + 1);
	if (uriIndex == -1)
		return false;

	size_t httpVersionIndex = view.find("\r\n", uriIndex);
	if (httpVersionIndex == -1)
		return false;

	httpContext_->getHttpRequest()->setMethod(view.substr(0, methodIndex));
	readLength_ = methodIndex + 1;
	httpContext_->getHttpRequest()->setUri(view.substr(readLength_, uriIndex - readLength_));
	readLength_ = uriIndex + 1;
	httpContext_->getHttpRequest()->setVersion(view.substr(readLength_, httpVersionIndex - readLength_));

	readLength_ = httpVersionIndex + 2;

	return true;
}

int32_t HttpProtocol::findHeader(std::string_view view)
{
	size_t keyIndex = view.find(":", readLength_);
	if (keyIndex == -1)
		return -1;

	size_t valueIndex = view.find("\r\n", keyIndex);
	if (valueIndex == -1)
		return -1;

	std::string_view key = view.substr(readLength_, keyIndex - readLength_);
	readLength_ = keyIndex + 2;
	std::string_view value = view.substr(readLength_, valueIndex - readLength_);
	readLength_ = valueIndex + 2;

	httpContext_->getHttpRequest()->setHeader(key, value);
	
	if (readLength_ < view.size())
	{
		char testC = view[readLength_];
		if (testC == '\r')
		{
			readLength_ += 2;
			return 1;
		}
	}

	return 0;
}

int32_t HttpProtocol::findData(std::string_view view)
{
	std::string_view strLength = httpContext_->getHttpRequest()->getHeader(CONTENT_LENGTH);
	if (strLength.empty())
		return -1;

	int& refErrNo = errno;
	const char* pStart = strLength.data();
	char* pEnd;

	refErrNo = 0;
	const uint64_t contentLength = std::strtoull(pStart, &pEnd, 10);

	if (pStart == pEnd)
		return -1;

	if (refErrNo == ERANGE)
		return -1;

	if (view.size() < readLength_ + contentLength)
		return 1;

	httpContext_->getHttpRequest()->setData(view.substr(readLength_, contentLength));
	
	readLength_ += contentLength;

	return 0;
}

bool HttpRequest::hasHeader(const std::string& key)
{
	if (key.empty())
		return false;

	bool ret = header_.end() != header_.find(key);
	return true;
}

void HttpRequest::setMethod(std::string_view method)
{
	if (0 == method.compare("GET"))
	{
		method_ = HttpMethod::GET;
	}
	else if (0 == method.compare("POST"))
	{
		method_ = HttpMethod::POST;
	}
	
}

void HttpRequest::setHeader(std::string_view key, std::string_view value)
{
	if (key.empty() || value.empty())
		return;

	std::string tempKey(key.data(), key.size());
	std::string tempValue(key.data(), key.size());
	if (tempValue[0] == ' ')
	{
		tempValue.erase(0, 1);
	}

	header_.insert({ tempKey, tempValue });
}

void HttpRequest::setData(std::string_view body)
{
	body_ = std::string(body.data(), body.size());
}

std::string_view HttpRequest::getHeader(const std::string& key)
{
	if (key.empty())
		return "";

	auto iter = header_.find(key);
	if (iter == header_.end())
		return "";

	return iter->second;
}
