#pragma once

#include "HttpRequest.h"
#include <algorithm>
#include "string.h"

class Buffer;

class HttpContext
{
public:
    enum HttpRequestParseState
    {
        kExpectRequestLine, // 解析请求行状态
        kExpectHeaders,     // 解析请求头部状态
        kExpectBody,        // 解析请求体状态
        kGotAll,            // 解析完毕状态
    };

    HttpContext()
        : state_(kExpectRequestLine)
    {
    }

    bool parseRequest(Buffer* buf, Timestamp receiveTime);

    bool gotAll() const { return state_ == kGotAll; }

    void reset()
    {
        state_ = kExpectRequestLine;
        HttpRequest dummy;
        request_.swap(dummy);
    }

    const HttpRequest& request() const { return request_; }

    HttpRequest& request() { return request_; }

private:
    bool processRequestLine(const char *begin, const char *end);

    HttpRequestParseState state_;
    HttpRequest request_;
};