#pragma once

#include <thread>
#include <queue>
#include <functional>

class ThreadedWorkQueue
{
    public:

        ThreadedWorkQueue() : working(true),
                              workThread(std::bind(ThreadedWorkQueue::poll, this))
        {

        }

        void poll()
        {
            while(working)
            {
                if (!workQueue.empty())
                {
                    (workQueue.front())();
                    workQueue.pop();
                }
            }
        }

        ~ThreadedWorkQueue()
        {
            working = false;
            workThread.join();
        }

    private:
        bool working;
        std::thread workThread;
        std::queue<std::function<void()>> workQueue;
};