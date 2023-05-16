#pragma once

class RequestProtocol : public std::enable_shared_from_this<RequestProtocol>
{
public:
	virtual ~RequestProtocol() = 0 {}

}; using RequestProtocolPtr = std::shared_ptr<RequestProtocol>;

class ResponseProtocol : public std::enable_shared_from_this<ResponseProtocol>
{

public:
	virtual ~ResponseProtocol() = 0 {}

};  using ResponseProtocolPtr = std::shared_ptr<ResponseProtocol>;


class ProtocolContext : public std::enable_shared_from_this<ProtocolContext>
{
protected:
	NetSocketPtr pSocket_;
	RequestProtocolPtr pRequestProtocol_;
	ResponseProtocolPtr pResponseProtocol_;

public:
	void setSocket(const NetSocketPtr& pSocket) { pSocket_ = pSocket; }

public:
	NetSocketPtr getSocket() { return pSocket_; }
	RequestProtocolPtr getRequest() { return pRequestProtocol_; }
	ResponseProtocolPtr getResponse() { return pResponseProtocol_; }

public:
	ProtocolContext() = delete;
	ProtocolContext(RequestProtocolPtr pRequest, ResponseProtocolPtr pResponse)
		: pRequestProtocol_(pRequest)
		, pResponseProtocol_(pResponse)
	{

	}

	virtual ~ProtocolContext() = 0 {}

}; using ProtocolContextPtr = std::shared_ptr<ProtocolContext>;

class Protocol
{
protected:
	std::vector<char> buffer_;

public:
	virtual ProtocolContextPtr getContext() = 0;
	virtual DeserializeResultType desirealize() = 0;
	void appendBuffer(const char* buffer, DWORD numberOfBytes);

public:
	virtual ~Protocol() = 0;
};

class ProtocolFactory
{
public:
	virtual Protocol* make() const = 0;

public:
	virtual ~ProtocolFactory() = 0;
};

enum class HttpMethod
{
	NONE,
	GET,
	POST,
	PUT,
	DEL,
};

class HttpRequest : public RequestProtocol
{
private:
	HttpMethod method_ = HttpMethod::NONE;
	std::unordered_map<std::string, std::string> header_;
	std::string uri_;
	std::string version_;
	std::string body_;

public:
	bool hasHeader(const std::string& key);

public:
	void setMethod(std::string_view method);
	void setUri(std::string_view uri) { uri_ = uri; }
	void setVersion(std::string_view version) { version_ = version; }
	void setHeader(std::string_view key, std::string_view value);
	void setData(std::string_view body);

public:
	std::string_view getHeader(const std::string& key);
	std::string_view getUri() { return uri_; }

public:
	HttpRequest() = default;
}; using HttpRequestPtr = std::shared_ptr<HttpRequest>;

class HttpResponse : public ResponseProtocol
{
private:
	std::unordered_map<std::string, std::string> header_;
	int32_t errorCode_;
	std::string body_;

};

class HttpContext : public ProtocolContext
{
public:
	std::shared_ptr<HttpRequest> getHttpRequest() { return std::dynamic_pointer_cast<::HttpRequest, RequestProtocol>(pRequestProtocol_); }
	std::shared_ptr<HttpResponse> getHttpResponse() { return std::dynamic_pointer_cast<::HttpResponse, ResponseProtocol>(pResponseProtocol_); }

public:
	HttpContext()
		: ProtocolContext(std::make_shared<HttpRequest>(), std::make_shared<HttpResponse>())
	{

	}
}; using HttpContextPtr = std::shared_ptr<HttpContext>;

class HttpProtocol : public Protocol
{
private:
	DeserializeResultType resultType_ = DeserializeResultType::None;
	size_t readLength_ = 0;
	std::shared_ptr<HttpContext> httpContext_;

public:
	ProtocolContextPtr getContext() override { return httpContext_; }
	DeserializeResultType desirealize() override;

private:
	bool findMethod(std::string_view view);
	int32_t findHeader(std::string_view view);
	int32_t findData(std::string_view view);
};

class HttpFactory
	: public ProtocolFactory
{
public:
	Protocol* make() const override { return new HttpProtocol(); }

public:
	HttpFactory() = default;
	~HttpFactory() {}
};
