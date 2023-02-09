#include "Buffer.h"
#include <string.h>
#include <iostream>

const std::string &Buffer::buf() const { return buf_; }

void Buffer::append(const char *str, const int &size)
{
    for (int i = 0; i < size; ++i)
    {
        if (str[i] == '\0')
            break;
        buf_.push_back(str[i]);
    }
}

void Buffer::append(const std::string &str)
{
    buf_ += str;
}

ssize_t Buffer::size() const
{
    return buf_.size();
}

const char *Buffer::c_str() const
{
    return buf_.c_str();
}

void Buffer::clear()
{
    buf_.clear();
}

void Buffer::getline()
{
    buf_.clear();
    std::getline(std::cin, buf_);
}

void Buffer::setBuf(const char* _buf)
{
    buf_.clear();
    buf_.append(_buf);
}