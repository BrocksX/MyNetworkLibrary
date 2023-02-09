#include "HttpContext.h"
#include "Buffer.h"

// 解析请求行
bool HttpContext::processRequestLine(const char *begin, const char *end)
{
    bool succeed = false;
    const char *start = begin;
    const char *space = std::find(start, end, ' ');

    // 不是最后一个空格，并且成功获取了method并设置到request_
    if (space != end && request_.setMethod(start, space))
    {
        // 跳过空格
        start = space+1;
        // 继续寻找下一个空格
        space = std::find(start, end, ' ');
        if (space != end)
        {
            // 查看是否有请求参数
            const char* question = std::find(start, space, '?');
            if (question != space)
            {
                // 设置访问路径
                request_.setPath(start, question);
                // 设置访问变量
                request_.setQuery(question, space);
            }
            else
            {
                request_.setPath(start, space);
            }
            start = space+1;
            // 获取最后的http版本
            succeed = (end-start == 8 && std::equal(start, end-1, "HTTP/1."));
            if (succeed)
            {
                if (*(end-1) == '1')
                {
                    request_.setVersion(HttpRequest::kHttp11);
                }
                else if (*(end-1) == '0')
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

// return false if any error
bool HttpContext::parseRequest(Buffer* buf, Timestamp receiveTime)
{
    bool ok = false;
    bool hasMore = true;
    const char* head = buf->c_str();
    while (hasMore)
    {
        // 请求行状态
        if (state_ == kExpectRequestLine)
        {
            const char* crlf = strstr(head, "\r\n");
            if (crlf)
            {
                ok = processRequestLine(head, crlf);
                if (ok)
                {
                    request_.setReceiveTime(receiveTime);
                    // 向后移动位置直到 crlf + 2
                    head = crlf + 2;
                    // 状态转移，接下来解析请求头
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
            const char* crlf = strstr(head, "\r\n");
            if (crlf)
            {
                // 找到 : 位置
                const char* colon = strchr(head, ':');
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
            //TODE
            hasMore = false;
        }
    }
    return ok;
}
