#pragma once
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

class FileHandler
{
public:
    FileHandler(const std::string fileName)
    {
        fd_ = ::open(fileName.c_str(), O_CLOEXEC | O_RDONLY);
        if (fd_ >= 0)
        {
            struct stat buf;
            fstat(fd_, &buf);
            fileSize_ = buf.st_size;
        }
        else
            fileSize_ = 0;
    }
    ~FileHandler()
    {
        if (fd_ >= 0)
            ::close(fd_);
    }
    int getFd() const { return fd_; }
    size_t fileSize() const { return fileSize_; }

private:
    int fd_;
    size_t fileSize_;
};
