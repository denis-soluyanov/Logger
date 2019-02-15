#pragma once

#include "Utility.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include <sstream>
#include <memory>

namespace dlog
{
    class Logger final : ConsoleLogger, FileLogger
    {
    private:
        using LoggerPtr = std::unique_ptr<Logger>;

        static LoggerPtr instance_;
        Adapter          adapter_;

        template<typename T>
        class FormatOutput final
        {
        private:
            const T& ref_;
        public:
            FormatOutput(const T& obj) : ref_(obj) {}

            friend std::ostringstream& operator<<(std::ostringstream& os, const FormatOutput<T>& arg)
            {
                os << ' ' << arg.ref_;
                return os;
            }
        };

        template<typename... Types>
        std::string formatString(const Types&... args)
        {
            std::ostringstream os;

            if (adapter_.logTime) {
                os << '[' << getCurrentTime(adapter_.time) << "]:";
            }

            (os << ... << FormatOutput(args)) << '\n';
            return os.str();
        }

        Logger(const std::string& filename, const Adapter& adapter);

        static void createInstance(const std::string& filename, const Adapter& adapter);

        bool canWrite() noexcept;

    public:
        /* 
         * These functions initialize the instance of Logger.
         * You can call one of these function only once, second call will throw an exception.
         */
        static void init(const std::string& filename, uint64_t maxFileSize = 0, bool printInConsole = false);

        /* Use this function for custom initialization. */
        static void init(const std::string& filename, const Adapter& adapter);

        template<typename... Types>
        static void log(const Types&... args)
        {
            if (!instance_) throw std::runtime_error("Instance of Logger wasn't create");

            if (instance_->canWrite()) {
                instance_->print_("Maximum file size reached!\n");
                return;
            }

            std::string record = instance_->formatString(args...);

            if (instance_->adapter_.printInConsole)
                instance_->print_(record);

            instance_->writeInFile(record);
        }

    private:
        /* Disabled */
        Logger(const Logger &)            = delete;
        Logger(Logger &&)                 = delete;
        Logger& operator=(const Logger &) = delete;
        Logger& operator=(Logger &&)      = delete;
    };
}