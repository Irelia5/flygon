#ifndef FLYGON_BASE_THREAD_H
#define FLYGON_BASE_THREAD_H

#include "flygon/base/noncopyable.h"
#include "flygon/base/Atomic.h"
#include "flygon/base/CountDownLatch.h"
#include <functional>
#include <pthread.h>
namespace flygon
{
//TODO: COUNT DOWN and tid??

class Thread : noncopyable
{
public:
    typedef std::function<void(void)> ThreadFunc;

    explicit Thread(ThreadFunc, const std::string& name = std::string());
    ~Thread();

    void start();
    int join();

    bool started() const { return started_; }
    const std::string& name() const { return name_; }
    pid_t tid() const { return tid_; }

    static int numCreated() { return numCreated_.get(); }

private:
    void setDefaultName();

    bool        started_;
    bool        joined_;
    pthread_t   pthreadId_;
    pid_t       tid_;
    ThreadFunc  func_;
    std::string name_;
    CountDownLatch latch_;

    static AtomicInt32 numCreated_;
};


} // namespace flygon

#endif //FLYGON_BASE_THREAD_H