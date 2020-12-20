#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include <mutex>
#include <condition_variable>
        
class Semaphore
{
    public:
    Semaphore (int _count) : count(_count) {}
    Semaphore () : count(1) {}
   
    inline void up()
    {
        std::unique_lock<std::mutex> lock(mtx);
        count++;
        //notify the waiting thread
        cv.notify_one();
    }

    inline void down()
    {
        std::unique_lock<std::mutex> lock(mtx);
        while(count == 0)
        {
            //wait on the mutex until notify is called
            cv.wait(lock);
        }
        count--;
    }

    private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
};

#endif
