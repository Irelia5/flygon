#ifndef FLYGON_BASE_CONDITION_H
#define FLYGON_BASE_CONDITION_H

#include "flygon/base/noncopyable.h"
#include "flygon/base/Mutex.h"
#include <pthread.h>

namespace flygon
{

class Condition : noncopyable
{
public:
    explicit Condition(MutexLock& mutex)
        : mutex_(mutex)
    {
        MCHECK(pthread_cond_init(&pcond_, NULL));
    }

    void wait()
    {
        MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));
    }

    void notify()
    {
        MCHECK(pthread_cond_signal(&pcond_));
    }

    void notifyAll()
    {
        MCHECK(pthread_cond_broadcast(&pcond_));
    }

private:
    MutexLock& mutex_;
    pthread_cond_t  pcond_;
};

} //namespace flygon





#endif //FLYGON_BASE_CONDITION_H