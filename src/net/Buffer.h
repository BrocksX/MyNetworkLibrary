#pragma once
#include <string>
#include "nocopyable.h"

class Buffer
{
public:
    DISALLOW_COPY_AND_MOVE(Buffer);
    Buffer() = default;
    ~Buffer() = default;

    void append(const char* str, const int &size);
    ssize_t size() const;
    const char* c_str() const;
    void clear();
    void getline();
    void setBuf(const char*);
    const std::string &buf() const;

private:
    std::string buf_;

};
