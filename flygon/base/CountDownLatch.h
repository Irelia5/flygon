#ifndef FLYGON_BASE_COUNTDOWNLATCH_H
#define FLYGON_BASE_COUNTDOWNLATCH_H

#include "flygon/base/noncopyable.h"
#include "flygon/base/Mutex.h"
#include "flygon/base/Condition.h"

namespace flygon
{

class CountDownLatch : noncopyable
{
public:
    explicit CountDownLatch(int count);

    void wait();

    void countDown();

    int getCount() const;

private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
};

} //namespace flygon

#endif //FLYGON_BASE_COUNTDOWNLATCH_H
