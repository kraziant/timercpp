#include "timercpp.h"
#include <chrono>
#include <iostream>

using namespace std;

int main()
{

    timer::set_timeout st{[]() { std::cout << "hello\n"; },
                          std::chrono::milliseconds(300)};

    timer::interval    inter{[]() { std::cout << "tick...\n"; },
                          std::chrono::milliseconds(100)};

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
