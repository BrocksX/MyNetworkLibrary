#pragma once
#include <string>
#include "common.h"

class Buffer
{
private:
    std::string buf;
public:
    DISALLOW_COPY_AND_MOVE(Buffer);
    Buffer();
    ~Buffer();

    void append(const char* _str, int _size);
    ssize_t size();
    const char* c_str();
    void clear();
    void getline();
    void setBuf(const char*);
};
