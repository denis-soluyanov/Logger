#include <iostream>
#include "Logger.h"
#include <future>

std::atomic_bool done = false;

void task(const std::string& msg)
{
    while (!done)
        dlog::Logger::log(msg);
}


// total time = 1'225'195'400
int main()
{
    using namespace dlog::size_literals;

    dlog::Adapter adpter;
    adpter.printInConsole = false;
    adpter.maxFileSize = 0;
    adpter.logTime = true;
    adpter.time.showTimezone = false;

    dlog::init("test10.log");


    auto start_time = std::chrono::steady_clock::now();

    for (auto i = 0; i < 50'000; ++i) {
        dlog::Logger::log("Hello World! i = ", i);
    }

    auto total_time = std::chrono::steady_clock::now() - start_time;
    std::cout << "total time = " << total_time.count() << '\n';
    //auto future1 = std::async(std::launch::async, task, "**************");
    //auto future2 = std::async(std::launch::async, task, "--------------");


    //std::this_thread::sleep_for(std::chrono::seconds(3));
    //done = true;
    //future1.get();
    //future2.get();

    return 0;
}