#pragma once

#include <fstream>
#include <string>
#include <mutex>

template<typename OutStream, typename T>
class FormatOutput final
{
private:
    const T& ref_;
    
public:
    FormatOutput(const T& obj) : ref_(obj) {}

    friend OutStream& operator<<(OutStream& os, const FormatOutput<OutStream, T>& arg)
    {
        os << arg.ref_ << ' ';
        return os;
    }
};

class DefaultLogger
{
private:
    std::ofstream   fstream_;
    std::string     filename_;
    std::mutex      mtx_;

protected:
    using LockGuard = std::lock_guard<std::mutex>;

    DefaultLogger(const std::string& filename);

    template<typename Format, typename... Types>
    void write(Format& format, const Types&... args)
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

    /* Disabled */
    DefaultLogger(const DefaultLogger &)            = delete;
    DefaultLogger(DefaultLogger &&)                 = delete;
    DefaultLogger& operator=(const DefaultLogger &) = delete;
    DefaultLogger& operator=(DefaultLogger &&)      = delete;
};