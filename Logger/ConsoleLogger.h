#pragma once

#include <iostream>
#include <mutex>
#include "Utility.h"

class ConsoleLogger
{
private:
    std::ostream& ostream_;
    std::mutex    mtx_;

protected:
    ConsoleLogger(std::ostream& os) : ostream_(os) {}

    template<typename... Types>
    void print_(const Types&... args)
    {
        std::lock_guard<std::mutex> guard(mtx_);

        (ostream_ << ... << FormatOutput<std::ostream, Types>(args)) << '\n';
    }

public:
    /* Disabled */
    ConsoleLogger(const ConsoleLogger &)            = delete;
    ConsoleLogger(ConsoleLogger &&)                 = delete;
    ConsoleLogger& operator=(const ConsoleLogger &) = delete;
    ConsoleLogger& operator=(ConsoleLogger &&)      = delete;
};