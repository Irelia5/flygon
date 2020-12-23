#include "flygon/base/Thread.h"
#include "flygon/base/CurrentThread.h"
#include "flygon/base/Exception.h"
#include <assert.h>
#include <sys/prctl.h>

namespace flygon
{

namespace detail
{

void afterFork()
{
    flygon::CurrentThread::t_cachedTid = 0;
    flygon::CurrentThread::t_threadName = "main";
    CurrentThread::tid();
}

class ThreadNameInitializer
{
public:
    ThreadNameInitializer()
    {
        flygon::CurrentThread::t_threadName = "main";
        CurrentThread::tid();
        pthread_atfork(NULL, NULL, &afterFork);
    }
};

ThreadNameInitializer init;

struct ThreadData
{
    typedef flygon::Thread::ThreadFunc ThreadFunc;

    ThreadFunc func_;
    std::string name_;
    pid_t* tid_;
    CountDownLatch* latch_;

    ThreadData(ThreadFunc func,
               const std::string& name,
               pid_t* tid,
               CountDownLatch* latch)
        : func_(func)
        , name_(name)
        , tid_(tid)
        , latch_(latch)
    { }

    void runInThread()
    {
        *tid_ = flygon::CurrentThread::tid();
        tid_ = NULL;
        latch_->countDown();
        latch_ = NULL;

        flygon::CurrentThread::t_threadName = name_.empty() ? "flygon Thread" : name_.c_str();
        ::prctl(PR_SET_NAME, flygon::CurrentThread::t_threadName);

        try
        {
            func_();
            flygon::CurrentThread::t_threadName = "finished";
        }
        catch (const Exception& ex)
        {
            flygon::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
            abort();
        }
        catch (const std::exception& ex)
        {
            flygon::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            abort();
        }
        catch (...)
        {
            flygon::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
            throw; // rethrow
        }

    }
};



void* startThread(void *obj)
{
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}

} //namesapce detail

AtomicInt32 Thread::numCreated_;

Thread::Thread(ThreadFunc func, const std::string& name)
    : started_(false)
    , joined_(false)
    , pthreadId_(0)
    , tid_(0)
    , func_(std::move(func))
    , name_(name)
    , latch_(1)
{
    setDefaultName();
}

Thread::~Thread()
{
    if (started_ && !joined_)
    {
        pthread_detach(pthreadId_);
    }
}

void Thread::setDefaultName()
{
    int num = numCreated_.incrementAndGet();
    if (name_.empty()) 
    {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread%d", num);
        name_ = buf;
    }
}

void Thread::start()
{
    assert(!started_);
    started_ = true;
    detail::ThreadData* data = new detail::ThreadData(func_, name_, &tid_, &latch_);
    if (pthread_create(&pthreadId_, NULL, &detail::startThread, data)) 
    {
        delete data;
        /// LOG
    } 
    else 
    {
        latch_.wait();
        assert(tid_ > 0);
    }
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, NULL);
}

} // namesapce flygon