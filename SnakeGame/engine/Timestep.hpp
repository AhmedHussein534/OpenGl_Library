#pragma once

#include <chrono>

class Timestep
{
    public:
    Timestep() : isReadBefore(false)
    {

    }

    template<typename T>
    float getDelta(bool update = true)
    {
        if (!isReadBefore)
        {
            if (update)
            {
                lastReadTime = std::chrono::high_resolution_clock::now();
                isReadBefore = true;
            }

            return 0.0f;
        }
        else
        {
            auto temp = lastReadTime;
            auto timeNow = std::chrono::high_resolution_clock::now();
            if (update)
            {
                lastReadTime = timeNow;
            }

            return std::chrono::duration<float, T>(timeNow - temp).count();
        }
    }


    private:
        bool isReadBefore;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastReadTime;
};