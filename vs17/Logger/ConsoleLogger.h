#pragma once

#include <iostream>
#include <mutex>

class ConsoleLogger
{
private:
    std::ostream& ostream_;
    std::mutex    mtx_;

protected:
    explicit ConsoleLogger(std::ostream& os = std::clog) : ostream_(os) {}

    void print_(const std::string& record)
    {
        std::lock_guard<std::mutex> guard(mtx_);

        ostream_.write(record.c_str(), record.size());
    }

private:
    /* Disabled */
    ConsoleLogger(const ConsoleLogger &)            = delete;
    ConsoleLogger(ConsoleLogger &&)                 = delete;
    ConsoleLogger& operator=(const ConsoleLogger &) = delete;
    ConsoleLogger& operator=(ConsoleLogger &&)      = delete;
};