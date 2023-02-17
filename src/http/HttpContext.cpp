#include "HttpContext.h"
#include "Buffer.h"

// 解析请求行
bool HttpContext::processRequestLine(const char *begin, const char *end)
{
    bool succeed = false;
    const char *start = begin;
    const char *space = std::find(start, end, ' ');

    if (space != end && request_.setMethod(start, space))
    {
        start = space + 1;
        space = std::find(start, end, ' ');
        if (space != end)
        {
            // 查看是否有请求参数
            const char *question = std::find(start, space, '?');
            if (question != space)
            {
                request_.setPath(start, question);
                request_.setQuery(question, space);
            }
            else
            {
                request_.setPath(start, space);
            }
            start = space + 1;
            succeed = (end - start == 8 && std::equal(start, end - 1, "HTTP/1."));
            if (succeed)
            {
                if (*(end - 1) == '1')
                {
                    request_.setVersion(HttpRequest::kHttp11);
                }
                else if (*(end - 1) == '0')
                {
                    request_.setVersion(HttpRequest::kHttp10);
                }
                else
                {
                    succeed = false;
                }
            }
        }
    }
    return succeed;
}

bool HttpContext::parseRequest(Buffer *buf, Timestamp receiveTime)
{
    bool ok = false;
    bool hasMore = true;
    const char *head = buf->c_str();
    while (hasMore)
    {
        // 请求行状态
        if (state_ == kExpectRequestLine)
        {
            const char *crlf = strstr(head, "\r\n");
            if (crlf)
            {
                ok = processRequestLine(head, crlf);
                if (ok)
                {
                    request_.setReceiveTime(receiveTime);
                    // 向后移动位置直到 crlf + 2
                    head = crlf + 2;
                    state_ = kExpectHeaders;
                }
                else
                {
                    hasMore = false;
                }
            }
            else
            {
                hasMore = false;
            }
        }
        // 解析请求头
        else if (state_ == kExpectHeaders)
        {
            const char *crlf = strstr(head, "\r\n");
            if (crlf)
            {
                const char *colon = strchr(head, ':');
                if (colon != nullptr)
                {
                    // 添加状态首部
                    request_.addHeader(head, colon, crlf);
                }
                else
                {
                    state_ = kGotAll;
                    hasMore = false;
                }
                head = crlf + 2;
            }
            else
            {
                hasMore = false;
            }
        }
        // 解析请求体，只支持GET请求
        else if (state_ == kExpectBody)
        {
            hasMore = false;
        }
    }
    return ok;
}
