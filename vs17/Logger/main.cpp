#include <iostream>
#include "Logger.h"

int main()
{
    dlog::Logger::init("test.log");
    dlog::Logger::log("Hello world!");

    return 0;
}