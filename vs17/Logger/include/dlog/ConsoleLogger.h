#pragma once

#include <iostream>
#include <mutex>

namespace dlog
{
    class ConsoleLogger
    {
    private:
        std::ostream& ostream_;
        std::mutex    mtx_;

    protected:
        explicit ConsoleLogger(std::ostream& os = std::clog);
        void print_(const std::string& record);

    private:
        /* Disabled */
        ConsoleLogger(const ConsoleLogger &)            = delete;
        ConsoleLogger(ConsoleLogger &&)                 = delete;
        ConsoleLogger& operator=(const ConsoleLogger &) = delete;
        ConsoleLogger& operator=(ConsoleLogger &&)      = delete;
    };
}
