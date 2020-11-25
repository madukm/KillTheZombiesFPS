#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include <linux/futex.h>
#include <sys/time.h>

#ifdef DEBUG
#include <cstdio>
#endif
/*
class Semaphore
{
    std::mutex semaphore_lock;
    int sem_count;
    const int max;

    public:

    Semaphore(int count) : sem_count(count), max(count)
    {}

    void up()
    {
        semaphore_lock.lock();
        
        if (sem_count < max)
            sem_count++;
        
        semaphore_lock.unlock();
    }

    void down()
    {
        while(1) 
        {
            semaphore_lock.lock();

            // After getting the lock
            if (sem_count <= 0)
            {
                semaphore_lock.unlock();
                sleep(10);
                continue;
            }
            else
            {
                sem_count--;
                semaphore_lock.unlock();
                break;
            }
        }
    }
};*/

#include <mutex>
#include <condition_variable>
        
class Semaphore {
public:
    Semaphore (int _count)
    : count(_count) 
    {
    }
    
    inline void up() {
        std::unique_lock<std::mutex> lock(mtx);

        printf("UP!\n");

        count++;
        //notify the waiting thread
        cv.notify_one();
    }
    inline void down() {
        std::unique_lock<std::mutex> lock(mtx);

        printf("DOWN!\n");

        while(count == 0) {
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
