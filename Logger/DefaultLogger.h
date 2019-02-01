#pragma once

#include <fstream>
#include <string>
#include <mutex>
#include "Utility.h"

class DefaultLogger
{
private:
    std::ofstream   fstream_;
    std::string     filename_;
    std::mutex      mtx_;

protected:
    using LockGuard = std::lock_guard<std::mutex>;

    DefaultLogger(const std::string& filename) : filename_(filename)
    {
        fstream_.open(filename_);
    }

    template<typename... Types>
    void write_(const Types&... args)
    {
        LockGuard guard(mtx_);

        (fstream_ << ... << FormatOutput<std::ofstream, Types>(args)) << '\n';
    }

public:
    ~DefaultLogger()
    {
        try {
            fstream_.close();
        }
        catch (...) {}
    }

private:
    /* Disabled */
    DefaultLogger(const DefaultLogger &)            = delete;
    DefaultLogger(DefaultLogger &&)                 = delete;
    DefaultLogger& operator=(const DefaultLogger &) = delete;
    DefaultLogger& operator=(DefaultLogger &&)      = delete;
};