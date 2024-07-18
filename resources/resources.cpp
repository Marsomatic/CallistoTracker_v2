#include <iostream>
#include <chrono>
#include <thread>

using namespace std::this_thread;
using namespace std::chrono;

int state = 0;

void moveStepper(int dirPin, int stepPin, bool dir, int steps)
{
    std::cout << dirPin;
    std::cout << stepPin;
    std::cout << dir;
    std::cout << steps;
}

