#include "flygon/base/Mutex.h"
#include "flygon/base/Thread.h"
#include <stdio.h>
#include <vector>
#include <memory>

flygon::MutexLock g_mutex;
std::vector<int> g_vec;
const int kCount = 10*1000*1000;

void threadFunc()
{
    for (int i = 0; i < kCount; ++i)
    {
        flygon::MutexLockGuard lock(g_mutex);
        g_vec.push_back(i);
    }
}


int foo() __attribute__ ((noinline));
int g_count = 0;
int foo()
{
    flygon::MutexLockGuard lock(g_mutex);
    if (!g_mutex.isLockedByThisThread())
    {
        printf("FAIL\n");
        return -1;
    }
    g_count++;
    return 0;
}

int main()
{
    MCHECK(foo());
    if (g_count != 1) 
    {
        printf("MCHECK calls twices\n");
        abort();
    }

    const int kMaxThreads = 8;
    g_vec.reserve(kMaxThreads* kCount);

    threadFunc();
    printf("single thread with lock \n");

    for (int nthreads = 1; nthreads < kMaxThreads; ++nthreads)
    {
        std::vector<std::unique_ptr<flygon::Thread>> threads;
        g_vec.clear();
        for (int i = 0; i < nthreads; ++i)
        {
            threads.emplace_back(new flygon::Thread(&threadFunc));
            threads.back()->start();
        }

        for (int i = 0; i < nthreads; ++i)
        {
            threads[i]->join();
        }
        printf("%d thread(s) with lock, vec_size %lu \n", nthreads, g_vec.size());
    }



    return 0;
}