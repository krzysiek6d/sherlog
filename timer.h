#pragma once

#include <iostream>
#include <chrono>

#define MEASURE_FUNCTION() Timer timer{__func__}

class Timer
{
public:
       using ClockType = std::chrono::steady_clock;
    Timer(const char* func)
        : function_name_{func}
        , start_{ClockType::now()}
    {
    }
    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    auto operator=(const Timer&) -> Timer& = delete;
    auto operator=(Timer&&) -> Timer& = delete;
    ~Timer()
    {
        using namespace std::chrono;
        auto stop = ClockType::now();
        auto duration = stop - start_;
        auto ms = duration_cast<milliseconds>(duration).count();
        std::cout << ms << " ms " << function_name_ << std::endl;
    }
private:
    const char* function_name_{};
    const ClockType::time_point start_;
};
