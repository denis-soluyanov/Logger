#include <iostream>
#include "dlog/Logger.h"

int main()
{
    dlog::init("Hello_World.log");

    dlog::Logger::log("*****************************");
    for (int i = 0; i < 50; ++i) {
        dlog::Logger::log("Hello World!", i);
    }
    dlog::Logger::log("*****************************");

    return 0;
}