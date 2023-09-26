#pragma once

#include <chrono>

namespace GL_ENGINE
{
    class Timestep
    {
        public:
        Timestep() : lastReadTime(std::chrono::high_resolution_clock::now())
        {

        }

        template<typename T>
        float getDelta()
        {
            auto temp = lastReadTime;
            auto timeNow = std::chrono::high_resolution_clock::now();
            return std::chrono::duration<float, T>(timeNow - temp).count();
        }

        void notifyUpdate()
        {
            lastReadTime = std::chrono::high_resolution_clock::now();
        }



        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> lastReadTime;
    };
}
