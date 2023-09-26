#pragma once

#include <chrono>

class Timestep
{
    public:
    Timestep() : isReadBefore(false),
                 lastReadTime(std::chrono::high_resolution_clock::now())
    {

    }

    template<typename T>
    float getDelta(bool update = true)
    {
        auto temp = lastReadTime;
        auto timeNow = std::chrono::high_resolution_clock::now();
        if (update)
        {
            lastReadTime = timeNow;
        }

        return std::chrono::duration<float, T>(timeNow - temp).count();
    }


    private:
        bool isReadBefore;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastReadTime;
};