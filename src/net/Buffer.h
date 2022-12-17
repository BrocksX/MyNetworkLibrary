#pragma once
#include <string>
#include "common.h"

class Buffer
{
private:
    std::string buf_;
public:
    DISALLOW_COPY_AND_MOVE(Buffer);
    Buffer() = default;
    ~Buffer() = default;

    void append(const char* _str, int _size);
    ssize_t size() const;
    const char* c_str() const;
    void clear();
    void getline();
    void setBuf(const char*);
    const std::string &buf() const;
};
