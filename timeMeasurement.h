#ifndef TIMEMEASUREMENT_H
#define TIMEMEASUREMENT_H

#include <chrono>

struct TimeMeasurement
{
private:
    std::chrono::steady_clock::time_point start;;

public:
    void updateTime() {
        this->start = std::chrono::steady_clock::now();
    }

    long long getTime() {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    } 
};

#endif